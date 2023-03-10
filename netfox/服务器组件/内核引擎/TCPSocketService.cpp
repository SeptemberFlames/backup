#include "Stdafx.h"
#include "TCPSocketService.h"
#include "TraceServiceManager.h"

//////////////////////////////////////////////////////////////////////////

//消息定义
#define WM_SOCKET_NOTIFY			WM_USER+100						//消息标识
#define WM_SERVICE_REQUEST			WM_USER+101						//服务请求

//////////////////////////////////////////////////////////////////////////
//动作定义

#define REQUEST_CONNECT				1								//请求连接
#define REQUEST_SEND_DATA			2								//请求发送
#define REQUEST_SEND_DATA_EX		3								//请求发送
#define	REQUEST_CLOSE_SOCKET		4								//请求关闭

//连接请求
struct tagConnectRequest
{
	WORD							wPort;							//连接端口
	DWORD							dwServerIP;						//连接地址 
};

//发送请求
struct  tagSendDataRequest
{
	WORD							wMainCmdID;						//主命令码
	WORD							wSubCmdID;						//子命令码
};

//发送请求
struct tagSendDataExRequest
{
	WORD							wMainCmdID;						//主命令码
	WORD							wSubCmdID;						//子命令码
	WORD							wDataSize;						//数据大小
	BYTE							cbSendBuffer[SOCKET_TCP_PACKET];//发送缓冲
};

//////////////////////////////////////////////////////////////////////////

//构造函数
CTCPSocketServiceThread::CTCPSocketServiceThread()
{
	//内核变量
	m_hWnd = NULL;
	m_hSocket = INVALID_SOCKET;
	m_cbTCPSocketStatus = SOCKET_STATUS_IDLE;

	//接收变量
	m_wBufferSize = 0;
	ZeroMemory(m_cbDataBuffer, sizeof(m_cbDataBuffer));

	//缓冲变量
	m_dwBufferData = 0L;
	m_dwBufferSize = 0L;
	m_bNeedBuffer = false;
	m_pcbDataBuffer = NULL;

	//计数变量
	m_dwSendTickCount = 0;
	m_dwRecvTickCount = 0;

	return;
}

//析构函数
CTCPSocketServiceThread::~CTCPSocketServiceThread()
{
	//关闭连接
	if (m_hSocket != INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}

	//删除缓冲
	SafeDeleteArray(m_pcbDataBuffer);

	return;
}

//停止线程
bool CTCPSocketServiceThread::ConcludeThread(DWORD dwWaitSeconds)
{
	//退出消息
	if (IsRuning() == true)
	{
		PostThreadMessage(WM_QUIT, 0, 0);
	}

	return __super::ConcludeThread(dwWaitSeconds);
}

//投递请求
bool CTCPSocketServiceThread::PostThreadRequest(WORD wIdentifier, VOID * const pBuffer, WORD wDataSize)
{
	//投递请求
	CWHDataLocker DataLocker(m_CriticalSection);
	m_DataQueue.InsertData(wIdentifier, pBuffer, wDataSize);

	//发送消息
	ASSERT(m_hWnd != NULL);
	if (m_hWnd != NULL) PostMessage(m_hWnd, WM_SERVICE_REQUEST, wDataSize, GetCurrentThreadId());

	return true;
}

//开始事件
bool CTCPSocketServiceThread::OnEventThreadStrat()
{
	//变量定义
	WNDCLASS WndClass;
	ZeroMemory(&WndClass, sizeof(WndClass));

	//设置变量
	WndClass.lpfnWndProc = DefWindowProc;
	WndClass.hInstance = AfxGetInstanceHandle();
	WndClass.lpszClassName = TEXT("TCPSocketStatusServiceThread");

	//注册窗口
	RegisterClass(&WndClass);

	//创建窗口
	HWND hParentWnd = GetDesktopWindow();
	m_hWnd = CreateWindow(WndClass.lpszClassName, NULL, WS_CHILD, 0, 0, 0, 0, hParentWnd, NULL, WndClass.hInstance, NULL);

	return true;
}

//停止事件
bool CTCPSocketServiceThread::OnEventThreadConclude()
{
	//关闭窗口
	if (m_hWnd != NULL)
	{
		DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}

	//关闭连接
	PerformCloseSocket(SHUT_REASON_INSIDE);

	//清理队列
	CWHDataLocker DataLocker(m_CriticalSection);
	m_DataQueue.RemoveData(false);

	return true;
}

//运行函数
bool CTCPSocketServiceThread::OnEventThreadRun()
{
	//获取消息
	MSG Message;
	if (GetMessage(&Message, NULL, 0, 0) == FALSE) return false;

	//消息处理
	switch (Message.message)
	{
		case WM_SOCKET_NOTIFY:			//网络消息
			{
				OnSocketNotify(Message.wParam, Message.lParam);
				return true;
			}
		case WM_SERVICE_REQUEST:		//服务请求
			{
				OnServiceRequest(Message.wParam, Message.lParam);
				return true;
			}
		default:						//默认消息
			{
				DefWindowProc(Message.hwnd, Message.message, Message.wParam, Message.lParam);
				return true;
			}
	}

	return false;
}

//执行连接
DWORD CTCPSocketServiceThread::PerformConnect(DWORD dwServerIP, WORD wPort)
{
	//校验参数
	ASSERT(m_hSocket == INVALID_SOCKET);
	ASSERT(m_cbTCPSocketStatus == SOCKET_STATUS_IDLE);
	ASSERT((dwServerIP != INADDR_NONE) && (dwServerIP != 0));

	try
	{
		//校验状态
		if (m_hSocket != INVALID_SOCKET) throw CONNECT_EXCEPTION;
		if (m_cbTCPSocketStatus != SOCKET_STATUS_IDLE) throw CONNECT_EXCEPTION;
		if ((dwServerIP == INADDR_NONE) || (dwServerIP == 0)) throw CONNECT_EXCEPTION;

		//设置参数
		m_wBufferSize = 0;
		m_dwSendTickCount = GetTickCount() / 1000L;
		m_dwRecvTickCount = GetTickCount() / 1000L;

		//建立 SOCKET
		m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_hSocket == INVALID_SOCKET) throw CONNECT_EXCEPTION;

		//变量定义
		SOCKADDR_IN SocketAddr;
		ZeroMemory(&SocketAddr, sizeof(SocketAddr));

		//设置变量
		SocketAddr.sin_family = AF_INET;
		SocketAddr.sin_port = htons(wPort);
		SocketAddr.sin_addr.S_un.S_addr = dwServerIP;

		//绑定窗口
		WSAAsyncSelect(m_hSocket, m_hWnd, WM_SOCKET_NOTIFY, FD_READ | FD_CONNECT | FD_CLOSE | FD_WRITE);

		//连接服务器
		INT nErrorCode = connect(m_hSocket, (SOCKADDR *)&SocketAddr, sizeof(SocketAddr));
		if ((nErrorCode == SOCKET_ERROR) && (WSAGetLastError() != WSAEWOULDBLOCK)) throw CONNECT_EXCEPTION;

		//设置变量
		m_cbTCPSocketStatus = SOCKET_STATUS_WAIT;

		return CONNECT_SUCCESS;
	}
	catch (...)
	{
		//关闭连接
		PerformCloseSocket(SHUT_REASON_INSIDE);
	}

	return CONNECT_EXCEPTION;
}

//发送函数
DWORD CTCPSocketServiceThread::PerformSendData(WORD wMainCmdID, WORD wSubCmdID)
{
	//校验状态
	if (m_hSocket == INVALID_SOCKET) return false;
	if (m_cbTCPSocketStatus != SOCKET_STATUS_CONNECT) return false;

	//设置数据
	if (m_TCPSocketData.InitSocketData(wMainCmdID, wSubCmdID, NULL, 0) == false)
	{
		return false;
	}

	//发送数据
	if (SendDataBuffer(m_TCPSocketData.m_cbDataBuffer, m_TCPSocketData.m_wBufferSize) == false)
	{
		return false;
	}

	return true;
}

//发送函数
DWORD CTCPSocketServiceThread::PerformSendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//校验大小
	ASSERT(wDataSize <= SOCKET_TCP_PACKET);
	if (wDataSize > SOCKET_TCP_PACKET) return 0L;

	//校验状态
	if (m_hSocket == INVALID_SOCKET) return 0L;
	if (m_cbTCPSocketStatus != SOCKET_STATUS_CONNECT) return 0L;

	//设置数据
	if (m_TCPSocketData.InitSocketData(wMainCmdID, wSubCmdID, pData, wDataSize) == false)
	{
		return false;
	}

	//发送数据
	if (SendDataBuffer(m_TCPSocketData.m_cbDataBuffer, m_TCPSocketData.m_wBufferSize) == false)
	{
		return false;
	}

	return true;
}

//关闭连接
VOID CTCPSocketServiceThread::PerformCloseSocket(BYTE cbShutReason)
{
	//内核变量
	m_dwBufferSize = 0L;
	m_dwBufferData = 0L;
	m_bNeedBuffer = false;
	m_cbTCPSocketStatus = SOCKET_STATUS_IDLE;

	//计数变量
	m_dwSendTickCount = 0;
	m_dwRecvTickCount = 0;

	//关闭判断
	if (m_hSocket != INVALID_SOCKET)
	{
		//关闭连接
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;

		//关闭通知
		if (cbShutReason != SHUT_REASON_INSIDE)
		{
			CTCPSocketService * pTCPSocketStatusService = CONTAINING_RECORD(this, CTCPSocketService, m_TCPSocketServiceThread);
			pTCPSocketStatusService->OnSocketShut(cbShutReason);
		}
	}

	return;
}

//缓冲数据
VOID CTCPSocketServiceThread::AmortizeBuffer(VOID * pData, WORD wDataSize)
{
	//申请缓冲
	if ((m_dwBufferData + wDataSize) > m_dwBufferSize)
	{
		//变量定义
		LPBYTE pcbDataBuffer = NULL;
		LPBYTE pcbDeleteBuffer = m_pcbDataBuffer;

		//计算大小
		DWORD dwNeedSize = m_dwBufferData + wDataSize;
		DWORD dwApplySize = __max(dwNeedSize, m_dwBufferSize * 2L);

		//申请缓冲
		try
		{
			pcbDataBuffer = new BYTE[dwApplySize];
		}
		catch (...) {}

		//失败判断
		if (pcbDataBuffer == NULL)
		{
			PerformCloseSocket(SHUT_REASON_EXCEPTION);
			return;
		}

		//设置变量
		m_dwBufferSize = dwApplySize;
		m_pcbDataBuffer = pcbDataBuffer;
		CopyMemory(m_pcbDataBuffer, pcbDeleteBuffer, m_dwBufferData);

		//删除缓冲
		SafeDeleteArray(pcbDeleteBuffer);
	}

	//设置变量
	m_bNeedBuffer = true;
	m_dwBufferData += wDataSize;
	CopyMemory(m_pcbDataBuffer + m_dwBufferData - wDataSize, pData, wDataSize);

	return;
}

//发送数据
WORD CTCPSocketServiceThread::SendDataBuffer(VOID * pBuffer, WORD wSendSize)
{
	//变量定义
	WORD wTotalCount = 0;

	//设置变量
	m_dwSendTickCount = GetTickCount() / 1000L;

	//发送数据
	while ((m_bNeedBuffer == false) && (wTotalCount < wSendSize))
	{
		//发送数据
		INT nSendCount = send(m_hSocket, (char *)pBuffer + wTotalCount, wSendSize - wTotalCount, 0);

		//错误判断
		if (nSendCount == SOCKET_ERROR)
		{
			//缓冲判断
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				AmortizeBuffer((LPBYTE)pBuffer + wTotalCount, wSendSize - wTotalCount);
				return wSendSize;
			}

			//关闭连接
			PerformCloseSocket(SHUT_REASON_EXCEPTION);

			return 0L;
		}
		else
		{
			//设置变量
			wTotalCount += nSendCount;
		}
	}

	//缓冲数据
	if (wTotalCount < wSendSize)
	{
		AmortizeBuffer((LPBYTE)pBuffer + wTotalCount, wSendSize - wTotalCount);
	}

	return wSendSize;
}

//请求消息
LRESULT CTCPSocketServiceThread::OnServiceRequest(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	tagDataHead DataHead;
	CWHDataLocker DataLocker(m_CriticalSection);

	//提取数据
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	if (m_DataQueue.DistillData(DataHead, cbBuffer, sizeof(cbBuffer)) == false) return 0;

	//数据处理
	switch (DataHead.wIdentifier)
	{
		case REQUEST_CONNECT:			//连接请求
			{
				//校验数据
				ASSERT(DataHead.wDataSize == sizeof(tagConnectRequest));
				tagConnectRequest * pConnectRequest = (tagConnectRequest *)cbBuffer;

				//数据处理
				if (PerformConnect(pConnectRequest->dwServerIP, pConnectRequest->wPort) != CONNECT_SUCCESS)
				{
					//事件通知
					CTCPSocketService * pTCPSocketStatusService = CONTAINING_RECORD(this, CTCPSocketService, m_TCPSocketServiceThread);
					pTCPSocketStatusService->OnSocketLink(WSAEADDRNOTAVAIL);
				}

				return 1;
			}
		case REQUEST_SEND_DATA:			//发送请求
			{
				//校验数据
				ASSERT(DataHead.wDataSize == sizeof(tagSendDataRequest));
				tagSendDataRequest * pSendDataRequest = (tagSendDataRequest *)cbBuffer;

				//数据处理
				PerformSendData(pSendDataRequest->wMainCmdID, pSendDataRequest->wSubCmdID);

				return 1;
			}
		case REQUEST_SEND_DATA_EX:	//发送请求
			{
				//校验数据
				tagSendDataExRequest * pSendDataExRequest = (tagSendDataExRequest *)cbBuffer;
				ASSERT(DataHead.wDataSize >= (sizeof(tagSendDataExRequest) - sizeof(pSendDataExRequest->cbSendBuffer)));
				ASSERT(DataHead.wDataSize == (pSendDataExRequest->wDataSize + sizeof(tagSendDataExRequest) - sizeof(pSendDataExRequest->cbSendBuffer)));

				//数据处理
				PerformSendData(pSendDataExRequest->wMainCmdID, pSendDataExRequest->wSubCmdID, pSendDataExRequest->cbSendBuffer, pSendDataExRequest->wDataSize);

				return 1;
			}
		case REQUEST_CLOSE_SOCKET:	//关闭请求
			{
				//关闭连接
				PerformCloseSocket(SHUT_REASON_NORMAL);

				return 1;
			}
	}

	return 0;
}

//网络消息
LRESULT CTCPSocketServiceThread::OnSocketNotify(WPARAM wParam, LPARAM lParam)
{
	switch (WSAGETSELECTEVENT(lParam))
	{
		case FD_READ:		//数据读取
			{
				return OnSocketNotifyRead(wParam, lParam);
			}
		case FD_WRITE:		//网络发送
			{
				return OnSocketNotifyWrite(wParam, lParam);
			}
		case FD_CLOSE:		//网络关闭
			{
				return OnSocketNotifyClose(wParam, lParam);
			}
		case FD_CONNECT:	//网络连接
			{
				return OnSocketNotifyConnect(wParam, lParam);
			}
	}

	return 0;
}


//网络读取
LRESULT CTCPSocketServiceThread::OnSocketNotifyRead(WPARAM wParam, LPARAM lParam)
{
	//读取数据
	INT nResultCode = recv(m_hSocket, (char *)m_cbDataBuffer + m_wBufferSize, sizeof(m_cbDataBuffer) - m_wBufferSize, 0);

	//关闭判断
	if (nResultCode == SOCKET_ERROR)
	{
		PerformCloseSocket(SHUT_REASON_EXCEPTION);
		return 1L;
	}

	//设置变量
	m_wBufferSize += nResultCode;
	m_dwRecvTickCount = GetTickCount() / 1000L;

	//数据处理
	while ((m_wBufferSize >= sizeof(TCP_Head)) && (m_hSocket != INVALID_SOCKET))
	{
		//变量定义
		TCP_Info * pInfo = (TCP_Info *)m_cbDataBuffer;

		//长度校验
		if ((pInfo->wPacketSize < sizeof(TCP_Info)) || (pInfo->wPacketSize > SOCKET_TCP_BUFFER))
		{
			PerformCloseSocket(SHUT_REASON_EXCEPTION);
			return 1L;
		}

		//完整判断
		if (m_wBufferSize < pInfo->wPacketSize) return 1L;

		//变量定义
		tagTCPData TCPData;
		WORD wBufferDataSize = pInfo->wPacketSize;

		//设置数据
		if (m_TCPSocketData.SetBufferData(m_cbDataBuffer, wBufferDataSize, TCPData) == false)
		{
			PerformCloseSocket(SHUT_REASON_EXCEPTION);
			return 1L;
		}

		//移动数据
		m_wBufferSize -= wBufferDataSize;
		MoveMemory(m_cbDataBuffer, m_cbDataBuffer + wBufferDataSize, m_wBufferSize);

		//网络检测
		if ((TCPData.Command.wMainCmdID == MDM_KN_COMMAND) && (TCPData.Command.wSubCmdID == SUB_KN_DETECT_SOCKET))
		{
			PerformSendData(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET);
		}

		//常规判断
		if (TCPData.Command.wMainCmdID != MDM_KN_COMMAND)
		{
			try
			{
				//处理数据
				CTCPSocketService * pTCPSocketStatusService = CONTAINING_RECORD(this, CTCPSocketService, m_TCPSocketServiceThread);
				if (pTCPSocketStatusService->OnSocketRead(TCPData.Command, TCPData.pDataBuffer, TCPData.wDataSize) == false)
				{
					throw TEXT("网络数据包处理失败");
				}
			}
			catch (...)
			{
				//关闭连接
				PerformCloseSocket(SHUT_REASON_EXCEPTION);
				return 1L;
			}
		}
	};

	return 1L;
}

//网络发送
LRESULT CTCPSocketServiceThread::OnSocketNotifyWrite(WPARAM wParam, LPARAM lParam)
{
	//缓冲判断
	if ((m_bNeedBuffer == true) && (m_dwBufferData > 0L))
	{
		//变量定义
		DWORD dwTotalCount = 0;
		DWORD dwPacketSize = 4096;

		//设置变量
		m_dwSendTickCount = GetTickCount() / 1000L;

		//发送数据
		while (dwTotalCount < m_dwBufferData)
		{
			//发送数据
			WORD wSendSize = (WORD)__min(dwPacketSize, m_dwBufferData - dwTotalCount);
			INT nSendCount = send(m_hSocket, (char*)m_pcbDataBuffer + dwTotalCount, wSendSize, 0);

			//错误判断
			if (nSendCount == SOCKET_ERROR)
			{
				//缓冲判断
				if (WSAGetLastError() == WSAEWOULDBLOCK)
				{
					//设置变量
					m_bNeedBuffer = false;
					m_dwBufferData -= dwTotalCount;

					//移动内存
					if (m_dwBufferData > 0L)
					{
						m_bNeedBuffer = true;
						MoveMemory(m_pcbDataBuffer, m_pcbDataBuffer + dwTotalCount, m_dwBufferData);
					}

					return 1L;
				}

				//关闭连接
				PerformCloseSocket(SHUT_REASON_EXCEPTION);

				return 1L;
			}

			//设置变量
			dwTotalCount += nSendCount;
		}

		//设置变量
		m_dwBufferData = 0L;
		m_bNeedBuffer = false;
	}

	return 1L;
}

//网络关闭
LRESULT CTCPSocketServiceThread::OnSocketNotifyClose(WPARAM wParam, LPARAM lParam)
{
	//关闭连接
	PerformCloseSocket(SHUT_REASON_REMOTE);

	return 1L;
}

//网络连接
LRESULT CTCPSocketServiceThread::OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam)
{
	//获取错误
	INT nErrorCode = WSAGETSELECTERROR(lParam);

	//事件通知
	CTCPSocketService * pTCPSocketStatusService = CONTAINING_RECORD(this, CTCPSocketService, m_TCPSocketServiceThread);
	pTCPSocketStatusService->OnSocketLink(nErrorCode);

	if (nErrorCode != 0)
	{
		PerformCloseSocket(SHUT_REASON_INSIDE);
		return 0;
	}

	//设置状态
	m_cbTCPSocketStatus = SOCKET_STATUS_CONNECT;


	return 1L;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CTCPSocketService::CTCPSocketService()
{
	//内核变量
	m_wServiceID = 0;
	m_bService = false;
	m_pITCPSocketEvent = NULL;

	return;
}

//构造函数
CTCPSocketService::~CTCPSocketService()
{
	//停止服务
	ConcludeService();

	return;
}

//接口查询
VOID * CTCPSocketService::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IServiceModule, Guid, dwQueryVer);
	QUERYINTERFACE(ITCPSocketService, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITCPSocketService, Guid, dwQueryVer);
	return NULL;
}

//启动服务
bool CTCPSocketService::StartService()
{
	//运行判断
	ASSERT((m_bService == false) && (m_pITCPSocketEvent != NULL));
	if ((m_bService == true) || (m_pITCPSocketEvent == NULL)) return false;

	//服务线程
	if (m_TCPSocketServiceThread.StartThread() == false) return false;

	//设置变量
	m_bService = true;

	return true;
}

//停止服务
bool CTCPSocketService::ConcludeService()
{
	//设置变量
	m_bService = false;

	//停止线程
	m_TCPSocketServiceThread.ConcludeThread(INFINITE);

	return true;
}

//配置函数
bool CTCPSocketService::SetServiceID(WORD wServiceID)
{
	//校验状态
	ASSERT(m_bService == false);
	if (m_bService == true) return false;

	//设置变量
	m_wServiceID = wServiceID;

	return true;
}

//设置接口
bool CTCPSocketService::SetTCPSocketEvent(IUnknownEx * pIUnknownEx)
{
	//状态校验
	ASSERT(m_bService == false);
	if (m_bService == true) return false;

	//查询接口
	m_pITCPSocketEvent = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, ITCPSocketEvent);

	//错误判断
	if (m_pITCPSocketEvent == NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//关闭连接
bool CTCPSocketService::CloseSocket()
{
	//状态校验
	ASSERT(m_bService == true);
	if (m_bService == false) return false;

	//投递请求
	return m_TCPSocketServiceThread.PostThreadRequest(REQUEST_CLOSE_SOCKET, NULL, 0);
}

//连接地址
bool CTCPSocketService::Connect(DWORD dwServerIP, WORD wPort)
{
	//校验状态
	ASSERT(m_bService == true);
	if (m_bService == false) return false;

	//构造数据
	tagConnectRequest ConnectRequest;
	ZeroMemory(&ConnectRequest, sizeof(ConnectRequest));

	//设置变量
	ConnectRequest.wPort = wPort;
	ConnectRequest.dwServerIP = htonl(dwServerIP);

	//投递请求
	return m_TCPSocketServiceThread.PostThreadRequest(REQUEST_CONNECT, &ConnectRequest, sizeof(ConnectRequest));
}

//连接地址
bool CTCPSocketService::Connect(LPCTSTR szServerIP, WORD wPort)
{
	//校验状态
	ASSERT(m_bService == true);
	if (m_bService == false) return false;

	//构造数据
	tagConnectRequest ConnectRequest;
	ZeroMemory(&ConnectRequest, sizeof(ConnectRequest));

	//设置变量
	ConnectRequest.wPort = wPort;
	ConnectRequest.dwServerIP = TranslateAddress(szServerIP);

	//投递请求
	return m_TCPSocketServiceThread.PostThreadRequest(REQUEST_CONNECT, &ConnectRequest, sizeof(ConnectRequest));
}

//发送函数
bool CTCPSocketService::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	//状态校验
	ASSERT(m_bService == true);
	if (m_bService == false) return false;

	//发送互斥
	CWHDataLocker DataLocker(m_SendCriticalSection);

	//构造数据
	tagSendDataRequest SendDataRequest;
	ZeroMemory(&SendDataRequest, sizeof(SendDataRequest));

	//设置变量
	SendDataRequest.wSubCmdID = wSubCmdID;
	SendDataRequest.wMainCmdID = wMainCmdID;

	//投递请求
	return m_TCPSocketServiceThread.PostThreadRequest(REQUEST_SEND_DATA, &SendDataRequest, sizeof(SendDataRequest));
}

//发送函数
bool CTCPSocketService::SendData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//状态校验
	ASSERT(m_bService == true);
	if (m_bService == false) return false;

	//发送互斥
	CWHDataLocker DataLocker(m_SendCriticalSection);

	//范围检测
	if (wDataSize < 0 || wDataSize > SOCKET_TCP_PACKET)
	{
		CString subString;
		subString.Format(TEXT("网络引擎发送数据大小越界, 主命令:%d, 从命令:%d, 大小:%d"), wMainCmdID, wSubCmdID, wDataSize);
		CTraceService::TraceString(subString, TraceLevel_Exception);
		return false;
	}

	//构造数据
	tagSendDataExRequest SendDataRequestEx;
	ZeroMemory(&SendDataRequestEx, sizeof(SendDataRequestEx));

	//设置变量
	SendDataRequestEx.wDataSize = wDataSize;
	SendDataRequestEx.wSubCmdID = wSubCmdID;
	SendDataRequestEx.wMainCmdID = wMainCmdID;

	//附加数据
	if (wDataSize > 0)
	{
		ASSERT(pData != NULL);
		CopyMemory(SendDataRequestEx.cbSendBuffer, pData, wDataSize);
	}

	//投递请求
	WORD wSendSize = sizeof(SendDataRequestEx) - sizeof(SendDataRequestEx.cbSendBuffer) + wDataSize;
	return m_TCPSocketServiceThread.PostThreadRequest(REQUEST_SEND_DATA_EX, &SendDataRequestEx, wSendSize);
}

//连接消息
bool CTCPSocketService::OnSocketLink(INT nErrorCode)
{
	//投递消息
	ASSERT(m_pITCPSocketEvent != NULL);
	return m_pITCPSocketEvent->OnEventTCPSocketLink(m_wServiceID, nErrorCode);
}

//关闭消息
bool CTCPSocketService::OnSocketShut(BYTE cbShutReason)
{
	//投递消息
	ASSERT(m_pITCPSocketEvent != NULL);
	return m_pITCPSocketEvent->OnEventTCPSocketShut(m_wServiceID, cbShutReason);
}

//读取消息
bool CTCPSocketService::OnSocketRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//投递消息
	ASSERT(m_pITCPSocketEvent != NULL);
	return m_pITCPSocketEvent->OnEventTCPSocketRead(m_wServiceID, Command, pData, wDataSize);
}

//地址解释
DWORD CTCPSocketService::TranslateAddress(LPCTSTR szServerIP)
{
	//转换地址
	CT2CA ServerAddr(szServerIP);
	DWORD dwServerIP = inet_addr(ServerAddr);

	//域名解释
	if (dwServerIP == INADDR_NONE)
	{
		LPHOSTENT lpHost = gethostbyname(ServerAddr);
		if (lpHost == NULL) return INADDR_NONE;
		dwServerIP = ((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}

	return dwServerIP;
}

//////////////////////////////////////////////////////////////////////////

//组件创建函数
DECLARE_CREATE_MODULE(TCPSocketService);

//////////////////////////////////////////////////////////////////////////
