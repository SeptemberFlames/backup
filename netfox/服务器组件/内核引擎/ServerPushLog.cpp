
#include "StdAfx.h"
#include "TraceService.h"
#include "ServerPushLog.h"

//消息定义
#define Wm_hSocket_NOTIFY		WM_USER+100							//消息标识
#define WM_SERVICE_REQUEST		WM_USER+101							//服务请求

//动作定义
#define REQUEST_CONNECT			1									//请求连接
#define REQUEST_SEND_LOG		2									//请求发送
#define	REQUEST_CLOSE_SOCKET	3									//请求关闭

//检查间隔
#define CHECK_INTERVAL			10									//检查间隔

//连接请求
struct tagConnectRequest
{
	WORD						wPort;								//连接端口
	DWORD						dwServerIP;							//连接地址 
};

//发送请求
struct tagSendLogRequest
{
	TCHAR						szLogPathName[MAX_PATH];			//日志路径
	LONGLONG					llLogIndex;							//日志索引
	LOG_Level					Level;								//日志等级
	TCHAR						szFormat[MAX_LOGER_LEN];			//日志内容
};

//初始成员
CServerPushLogThread * CServerPushLog::m_ServerPushLogThread = NULL;
tagTableLogIndex * CServerPushLog::m_TableLogIndex = NULL;
CServerPushLog *  CServerPushLog::m_ServerLog = NULL;
LONGLONG CServerPushLog::m_llServerLogIndex = 0;
LONGLONG CServerPushLog::m_llKernelLogIndex = 0;
LONGLONG CServerPushLog::m_llScoreLogIndex = 0;
LONGLONG CServerPushLog::m_llSqlLogIndex = 0;

//////////////////////////////////////////////////////////////////////////
//构造函数
CServerPushLogThread::CServerPushLogThread()
{
	//缓冲变量
	m_dwBufferData = 0L;
	m_dwBufferSize = 0L;
	m_bNeedBuffer = false;
	m_pcbDataBuffer = NULL;

	//内核变量
	m_hWnd = NULL;
	m_hSocket = INVALID_SOCKET;
	m_cbTCPSocketStatus = SOCKET_STATUS_IDLE;

	//初始环境
	WSADATA wsaData;
	WORD socketVersion = MAKEWORD(2, 2);
	if (WSAStartup(socketVersion, &wsaData) != 0) return;

	//获取路径
	TCHAR szPath[MAX_PATH] = TEXT("");
	GetModuleFileName(AfxGetInstanceHandle(), szPath, sizeof(szPath));

	//信息配置
	CString szLogOutputConfigFileName = szPath;
	INT DownLoadnIndex = szLogOutputConfigFileName.ReverseFind(TEXT('\\'));
	szLogOutputConfigFileName = szLogOutputConfigFileName.Left(DownLoadnIndex);
	szLogOutputConfigFileName += TEXT("\\LogOutputConfig.ini");

	//读取配置
	GetPrivateProfileString(TEXT("OutPutConfig"), TEXT("OutputAddress"), TEXT("127.0.0.1"), m_LogOutputAddress, 32, szLogOutputConfigFileName);
	m_LogOutputPort = (USHORT)GetPrivateProfileInt(TEXT("OutPutConfig"), TEXT("OutputPort"), 0, szLogOutputConfigFileName);

	return;
}

//析构函数
CServerPushLogThread::~CServerPushLogThread()
{

}
//功能函数
void CServerPushLogThread::PushLogData(TCHAR szLogPathName[MAX_PATH], LONGLONG llLogIndex, LOG_Level Level, TCHAR szFormat[MAX_LOGER_LEN])
{
	//构造数据
	tagSendLogRequest SendLogRequest;
	ZeroMemory(&SendLogRequest, sizeof(SendLogRequest));

	//设置变量
	lstrcpyn(SendLogRequest.szLogPathName, szLogPathName, CountArray(SendLogRequest.szLogPathName));
	lstrcpyn(SendLogRequest.szFormat, szFormat, CountArray(SendLogRequest.szFormat));
	SendLogRequest.llLogIndex = llLogIndex;
	SendLogRequest.Level = Level;

	//投递请求
	PostThreadRequest(REQUEST_SEND_LOG, &SendLogRequest, sizeof(SendLogRequest));
}

//开始事件
bool CServerPushLogThread::OnEventThreadStrat()
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

	//构造数据
	tagConnectRequest ConnectRequest;
	ZeroMemory(&ConnectRequest, sizeof(ConnectRequest));

	//设置变量
	ConnectRequest.wPort = m_LogOutputPort;
	ConnectRequest.dwServerIP = TranslateAddress(m_LogOutputAddress);

	//投递请求
	PostThreadRequest(REQUEST_CONNECT, &ConnectRequest, sizeof(ConnectRequest));
	return true;
}

//停止事件
bool CServerPushLogThread::OnEventThreadConclude()
{
	//关闭窗口
	if (m_hWnd != NULL)
	{
		DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}

	//关闭连接
	PerformCloseSocket(SHUT_REASON_NORMAL);

	//清理队列
	CWHDataLocker DataLocker(m_ServerPushLogSection);
	m_DataQueue.RemoveData(false);

	return true;
}

//运行函数
bool CServerPushLogThread::OnEventThreadRun()
{
	//获取消息
	MSG Message;
	if (GetMessage(&Message, NULL, 0, 0) == FALSE) return true;

	//消息处理
	switch (Message.message)
	{
		case Wm_hSocket_NOTIFY:			//网络消息
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
}

//网络消息
LRESULT CServerPushLogThread::OnSocketNotify(WPARAM wParam, LPARAM lParam)
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

//请求消息
LRESULT CServerPushLogThread::OnServiceRequest(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	tagDataHead DataHead;
	CWHDataLocker DataLocker(m_ServerPushLogSection);

	//提取数据
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	if (m_DataQueue.DistillData(DataHead, cbBuffer, sizeof(cbBuffer)) == false) return 0;

	//数据处理
	switch (DataHead.wIdentifier)
	{
		case REQUEST_CONNECT:			//连接请求
			{
				//校验数据
				tagConnectRequest * pConnectRequest = (tagConnectRequest *)cbBuffer;

				//尝试连接
				if (PerformConnect(pConnectRequest->dwServerIP, pConnectRequest->wPort) != CONNECT_SUCCESS)
				{
					PerformCloseSocket(SHUT_REASON_EXCEPTION);
				}

				return 1;
			}
		case REQUEST_SEND_LOG:			//发送请求
			{
				//数据处理
				PerformSendData(cbBuffer, sizeof(tagSendLogRequest));

				return 1;
			}
		case REQUEST_CLOSE_SOCKET:		//关闭请求
			{
				//关闭连接
				PerformCloseSocket(SHUT_REASON_NORMAL);
				return 1;
			}
	}

	return 0;
}

//投递请求
bool CServerPushLogThread::PostThreadRequest(WORD wIdentifier, VOID * const pBuffer, WORD wDataSize)
{
	//投递请求
	CWHDataLocker DataLocker(m_ServerPushLogSection);
	m_DataQueue.InsertData(wIdentifier, pBuffer, wDataSize);

	//发送消息
	ASSERT(m_hWnd != NULL);
	if (m_hWnd != NULL) PostMessage(m_hWnd, WM_SERVICE_REQUEST, wDataSize, GetCurrentThreadId());

	return true;
}


//网络读取
LRESULT CServerPushLogThread::OnSocketNotifyRead(WPARAM wParam, LPARAM lParam)
{
	return 1L;
}

//网络发送
LRESULT CServerPushLogThread::OnSocketNotifyWrite(WPARAM wParam, LPARAM lParam)
{
	//缓冲判断
	if ((m_bNeedBuffer == true) && (m_dwBufferData > 0L))
	{
		//变量定义
		DWORD dwTotalCount = 0;
		DWORD dwPacketSize = 4096;


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

		return 1L;
	}

	return 1L;
}

//网络关闭
LRESULT CServerPushLogThread::OnSocketNotifyClose(WPARAM wParam, LPARAM lParam)
{
	//关闭连接
	PerformCloseSocket(SHUT_REASON_REMOTE);

	return 1L;
}

//网络连接
LRESULT CServerPushLogThread::OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam)
{
	//获取错误
	INT nErrorCode = WSAGETSELECTERROR(lParam);
	if (nErrorCode != 0)
	{
		PerformCloseSocket(SHUT_REASON_EXCEPTION);
		return 0;
	}

	//连接成功
	//CTraceService::TraceString(TEXT("连接日志服务器成功"), TraceLevel_Normal);

	//设置状态
	m_cbTCPSocketStatus = SOCKET_STATUS_CONNECT;

	return 1L;
}

//执行连接
DWORD CServerPushLogThread::PerformConnect(DWORD dwServerIP, WORD wPort)
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
		WSAAsyncSelect(m_hSocket, m_hWnd, Wm_hSocket_NOTIFY, FD_READ | FD_CONNECT | FD_CLOSE | FD_WRITE);

		//连接服务器
		INT nErrorCode = connect(m_hSocket, (SOCKADDR *)&SocketAddr, sizeof(SocketAddr));
		if ((nErrorCode == SOCKET_ERROR) && (WSAGetLastError() != WSAEWOULDBLOCK))
		{
			return CONNECT_FAILURE;
		}

		//设置变量
		m_cbTCPSocketStatus = SOCKET_STATUS_WAIT;

		return CONNECT_SUCCESS;
	}
	catch (...)
	{
		//关闭连接
		PerformCloseSocket(SHUT_REASON_EXCEPTION);

		return CONNECT_FAILURE;
	}

	return CONNECT_FAILURE;
}

//发送函数
DWORD CServerPushLogThread::PerformSendData(VOID * pData, WORD wDataSize)
{
	//数据转换
	tagSendLogRequest * SendLogRequest = (tagSendLogRequest *)pData;

	//获取时间
	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);

	//格式时间
	TCHAR szTimeBuffer[64] = { 0 };
	_sntprintf_s(szTimeBuffer, _TRUNCATE, TEXT("%04d-%02d-%02d %02d:%02d:%02d:%04d "),
		SystemTime.wYear,
		SystemTime.wMonth,
		SystemTime.wDay,
		SystemTime.wHour,
		SystemTime.wMinute,
		SystemTime.wSecond,
		SystemTime.wMilliseconds
	);

	//提示消息
	TCHAR szLogLevel[RECV_BUFFER_LEN] = { 0 };
	switch (SendLogRequest->Level)
	{
		case LOG_DEBUG:
			_sntprintf_s(szLogLevel, _TRUNCATE, TEXT("[%s LOG_DEBUG:-->%I64d]:%s %s\n"), szTimeBuffer, SendLogRequest->llLogIndex, " ", SendLogRequest->szFormat);
			break;
		case LOG_ERROR:
			_sntprintf_s(szLogLevel, _TRUNCATE, TEXT("[%s LOG_ERROR:-->%I64d]:%s %s\n"), szTimeBuffer, SendLogRequest->llLogIndex, " ", SendLogRequest->szFormat);
			break;
		case LOG_WARN:
			_sntprintf_s(szLogLevel, _TRUNCATE, TEXT("[%s LOG_WARN :-->%I64d]:%s %s\n"), szTimeBuffer, SendLogRequest->llLogIndex, " ", SendLogRequest->szFormat);
			break;
		default:
			break;
	}

	//缓存数据
	char pDataBuffer[SEND_BUFFER_LEN + 3] = { 0 };
	char pFileName[RECV_BUFFER_LEN + 1] = { 0 };
	char pContent[RECV_BUFFER_LEN + 1] = { 0 };

	int strFileNameLen = WideCharToMultiByte(CP_UTF8, 0, SendLogRequest->szLogPathName, -1, NULL, 0, NULL, NULL) - 1;
	if (strFileNameLen > RECV_BUFFER_LEN) strFileNameLen = RECV_BUFFER_LEN;
	WideCharToMultiByte(CP_UTF8, 0, SendLogRequest->szLogPathName, -1, pFileName, strFileNameLen, NULL, NULL);
	pFileName[strFileNameLen] = '\0';

	int strContentLen = WideCharToMultiByte(CP_UTF8, 0, szLogLevel, -1, NULL, 0, NULL, NULL) - 1;
	if (strContentLen > RECV_BUFFER_LEN) strContentLen = RECV_BUFFER_LEN;
	WideCharToMultiByte(CP_UTF8, 0, szLogLevel, -1, pContent, strContentLen, NULL, NULL);
	pContent[strContentLen] = '\0';

	//拷贝缓存
	*(short*)pDataBuffer = (short)strFileNameLen;
	memcpy(pDataBuffer + 2, pFileName, strFileNameLen);
	memcpy(pDataBuffer + 2 + strFileNameLen, pContent, strContentLen);

	//数据长度
	int SendLength = 2 + strFileNameLen + strContentLen;
	pDataBuffer[SendLength] = '\0';

	//准备发送
	if (SendLength > SOCKET_TCP_PACKET) return 0L;

	//校验状态
	if (m_hSocket == INVALID_SOCKET) return 0L;
	if (m_cbTCPSocketStatus != SOCKET_STATUS_CONNECT) return 0L;

	//充足缓冲
	CHAR send_buf[SOCKET_TCP_PACKET] = { 0 };
	WORD send_size = 0;

	//处理缓冲
	*(short*)send_buf = htons(SendLength);
	memcpy(send_buf + 2, pDataBuffer, SendLength);
	send_size = SendLength + 2;

	//发送数据
	if (SendDataBuffer(send_buf, send_size) == false)
	{
		return 0L;
	}

	return 1L;
}

//关闭连接
VOID CServerPushLogThread::PerformCloseSocket(BYTE cbShutReason)
{
	//内核变量
	m_dwBufferSize = 0L;
	m_dwBufferData = 0L;
	m_bNeedBuffer = false;

	//关闭网络
	closesocket(m_hSocket);
	m_hSocket = INVALID_SOCKET;
	m_cbTCPSocketStatus = SOCKET_STATUS_IDLE;

	//重新连接
	if (cbShutReason != SHUT_REASON_NORMAL)
	{
		//构造数据
		tagConnectRequest ConnectRequest;
		ZeroMemory(&ConnectRequest, sizeof(ConnectRequest));

		//设置变量
		ConnectRequest.wPort = m_LogOutputPort;
		ConnectRequest.dwServerIP = TranslateAddress(m_LogOutputAddress);

		//投递请求
		PostThreadRequest(REQUEST_CONNECT, &ConnectRequest, sizeof(ConnectRequest));
	}

	return;
}


//缓冲数据
VOID CServerPushLogThread::AmortizeBuffer(VOID * pData, WORD wDataSize)
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
WORD CServerPushLogThread::SendDataBuffer(VOID * pBuffer, WORD wSendSize)
{
	//变量定义
	WORD wTotalCount = 0;

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

//地址解释
DWORD CServerPushLogThread::TranslateAddress(LPCTSTR szServerIP)
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

/////////////////////////////////////
//构造函数
CServerPushLog::CServerPushLog()
{
	//重置参数
	m_sLogOutputPort = 0;
	m_serverPort = 0;

	//获取路径
	TCHAR szPath[MAX_PATH] = TEXT("");
	GetModuleFileName(AfxGetInstanceHandle(), szPath, sizeof(szPath));

	//信息配置
	CString szLogOutputConfigFileName = szPath;
	int DownLoadnIndex = szLogOutputConfigFileName.ReverseFind(TEXT('\\'));
	szLogOutputConfigFileName = szLogOutputConfigFileName.Left(DownLoadnIndex);
	szLogOutputConfigFileName += TEXT("\\LogOutputConfig.ini");

	//日志配置
	TCHAR sLogOutputAddress[32] = TEXT("");
	GetPrivateProfileString(TEXT("OutPutConfig"), TEXT("OutputAddress"), TEXT("127.0.0.1"), sLogOutputAddress, 32, szLogOutputConfigFileName);
	CT2CA sLogOutputAddress1(sLogOutputAddress);
	strcpy_s(m_sLogOutputAddress, sLogOutputAddress1);
	m_sLogOutputPort = (USHORT)GetPrivateProfileInt(TEXT("OutPutConfig"), TEXT("OutputPort"), 0, szLogOutputConfigFileName);

	//平台配置
	CString szPlafromName = TEXT("");
	szPlafromName = szPath;
	int nConvertIndex = szPlafromName.ReverseFind(TEXT('\\'));
	szPlafromName = szPlafromName.Left(nConvertIndex);
	szPlafromName += TEXT("\\Platfrom.ini");

	//读取名称
	TCHAR szPlatfrom[PLATFROM_NAME_LEN] = TEXT("");
	GetPrivateProfileString(TEXT("Platfrom"), TEXT("PlatfromName"), TEXT(""), szPlatfrom, PLATFROM_NAME_LEN, szPlafromName);

	//保存名称
	lstrcpyn(m_szPlatfromName, szPlatfrom, CountArray(m_szPlatfromName));

	//网络模块
	WORD socketVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(socketVersion, &wsaData) != 0) return;

	//异步线程
	m_ServerPushLogThread = new CServerPushLogThread();
	if (m_ServerPushLogThread == NULL) return;

	//桌子日志
	m_TableLogIndex = new tagTableLogIndex;
	if (m_TableLogIndex == NULL) return;

}

//析构函数
CServerPushLog::~CServerPushLog()
{
	ConcludeService();
}

//启动服务
bool CServerPushLog::StartService()
{
	//运行判断
	if (m_ServerPushLogThread->IsRuning() == true)return false;

	//启动线程
	if (m_ServerPushLogThread->StartThread() == false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//停止服务
bool CServerPushLog::ConcludeService()
{
	//停止线程
	if (m_ServerPushLogThread->IsRuning())
	{
		m_ServerPushLogThread->ConcludeThread(INFINITE);
	}
	delete m_ServerPushLogThread;
	m_ServerPushLogThread = NULL;
	delete m_TableLogIndex;
	m_TableLogIndex = NULL;
	delete m_ServerLog;
	m_ServerLog = NULL;
	return true;
}

//共享指针
CServerPushLog* CServerPushLog::SharedLog()
{
	if (m_ServerLog == NULL)
	{
		//创建对象
		m_ServerLog = new CServerPushLog();
		if (m_ServerLog == NULL) return NULL;

		//启动服务
		m_ServerLog->StartService();
	}
	return m_ServerLog;
}

//通信端口
void CServerPushLog::SetServerPort(WORD port)
{
	m_ServerLog->m_serverPort = port;
}

//文件路径
void CServerPushLog::SetServerLogName(TCHAR * szLogPathName)
{
	lstrcpyn(m_ServerLog->m_szLogPathName, szLogPathName, CountArray(m_ServerLog->m_szLogPathName));
}

//服务日志
void CServerPushLog::PushServerLog(TCHAR Category[MAX_PATH], LOG_Level Level, TCHAR pszFormat[MAX_LOGER_LEN])
{
	try
	{
		//过滤端口
		if (m_ServerLog->m_sLogOutputPort == 0)
		{
			return;
		}

		//获取时间
		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);

		//日志路径
		TCHAR szLogPathName[MAX_PATH] = { 0 };
		_sntprintf_s(szLogPathName, _TRUNCATE, TEXT("/%s/%s/%d_Log/%04d_%02d_%02d/%s/%02d.log"),
			m_ServerLog->m_szPlatfromName,
			m_ServerLog->m_szLogPathName,
			m_ServerLog->m_serverPort,
			SystemTime.wYear,
			SystemTime.wMonth,
			SystemTime.wDay,
			Category,
			SystemTime.wHour
		);

		//日志索引
		m_llServerLogIndex++;

		//日志队列
		if (m_ServerPushLogThread != NULL) m_ServerPushLogThread->PushLogData(
			szLogPathName,
			m_llServerLogIndex,
			Level,
			pszFormat
		);
	}
	catch (...)
	{
		CTraceService::TraceString(TEXT("写入服务日志异常"), TraceLevel_Exception);
	}
}

//内核日志
void CServerPushLog::PushKernelLog(TCHAR Category[MAX_PATH], LOG_Level Level, TCHAR pszFormat[MAX_LOGER_LEN])
{
	try
	{
		//过滤端口
		if (m_ServerLog->m_sLogOutputPort == 0)
		{
			return;
		}

		//获取时间
		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);

		//日志路径
		TCHAR szLogPathName[MAX_PATH] = { 0 };
		_sntprintf_s(szLogPathName, _TRUNCATE, TEXT("/%s/%s/%d_Log/%04d_%02d_%02d/%s/%02d.log"),
			m_ServerLog->m_szPlatfromName,
			m_ServerLog->m_szLogPathName,
			m_ServerLog->m_serverPort,
			SystemTime.wYear,
			SystemTime.wMonth,
			SystemTime.wDay,
			Category,
			SystemTime.wHour
		);

		//日志索引
		m_llKernelLogIndex++;

		//日志队列
		if (m_ServerPushLogThread != NULL) m_ServerPushLogThread->PushLogData(
			szLogPathName,
			m_llKernelLogIndex,
			Level,
			pszFormat
		);
	}
	catch (...)
	{
		CTraceService::TraceString(TEXT("写入内核日志异常"), TraceLevel_Exception);
	}
}

//游戏日志
void CServerPushLog::PushGameLog(TCHAR GameLogPath[MAX_PATH], LOG_Level Level, TCHAR pszFormat[MAX_LOGER_LEN], WORD wTableID)
{
	try
	{
		//过滤端口
		if (m_ServerLog->m_sLogOutputPort == 0)
		{
			return;
		}

		//获取时间
		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);

		//日志路径
		TCHAR szLogPathName[MAX_PATH] = { 0 };
		_sntprintf_s(szLogPathName, _TRUNCATE, TEXT("/%s/%s"), m_ServerLog->m_szPlatfromName, GameLogPath);

		//日志序号
		LONGLONG llTableLogIndex = 0;

		//查找索引
		tagTableLogIndex::iterator iter = m_TableLogIndex->find(wTableID);
		if (iter == m_TableLogIndex->end())
		{
			llTableLogIndex = 1;
			m_TableLogIndex->insert(tagTableLogIndex::value_type(wTableID, llTableLogIndex));
		}
		else
		{
			//获取序号
			llTableLogIndex = iter->second;
			//保存序号
			iter->second = llTableLogIndex + 1;
		}

		//日志队列
		if (m_ServerPushLogThread != NULL) m_ServerPushLogThread->PushLogData(
			szLogPathName,
			llTableLogIndex,
			Level,
			pszFormat
		);
	}
	catch (...)
	{
		CTraceService::TraceString(TEXT("写入游戏日志异常"), TraceLevel_Exception);
	}
}

//金币日志
void CServerPushLog::PushScoreLog(TCHAR GameLogPath[MAX_PATH], LOG_Level Level, DWORD dwGameID, TCHAR pszFormat[MAX_LOGER_LEN])
{
	try
	{
		//过滤端口
		if (m_ServerLog->m_sLogOutputPort == 0)
		{
			return;
		}

		//获取时间
		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);

		//日志路径
		TCHAR szLogPathName[MAX_PATH] = { 0 };
		_sntprintf_s(szLogPathName, _TRUNCATE, TEXT("/%s/%s/%04d_%02d_%02d/%d.log"),
			m_ServerLog->m_szPlatfromName,
			GameLogPath,
			SystemTime.wYear,
			SystemTime.wMonth,
			SystemTime.wDay,
			dwGameID
		);

		//日志索引
		m_llScoreLogIndex++;

		//日志队列
		if (m_ServerPushLogThread != NULL) m_ServerPushLogThread->PushLogData(
			szLogPathName,
			m_llScoreLogIndex,
			Level,
			pszFormat
		);
	}
	catch (...)
	{
		CTraceService::TraceString(TEXT("写入金币日志异常"), TraceLevel_Exception);
	}
}
