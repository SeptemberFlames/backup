#include "Stdafx.h"
#include "TCPNetworkEngine.h"
#include "TraceServiceManager.h"
#include "TraceService.h"
#include "ServerPushLog.h" 
#include "SHA1ACT.h"
#include "mybase64.h"

//////////////////////////////////////////////////////////////////////////
//宏定义

//端口定义
#define PORT_AUTO_MIN						3000						//最小端口
#define PORT_AUTO_MAX						9000						//最大端口

//系数定义
#define DEAD_QUOTIETY						0							//死亡系数
#define DANGER_QUOTIETY						2							//危险系数
#define SAFETY_QUOTIETY						4							//安全系数

//动作定义
#define ASYNCHRONISM_SEND_DATA				1							//发送标识
#define ASYNCHRONISM_SEND_BATCH				2							//群体发送
#define ASYNCHRONISM_SHUT_DOWN				3							//安全关闭
#define ASYNCHRONISM_ALLOW_BATCH			4							//允许群发 
#define ASYNCHRONISM_CLOSE_SOCKET			5							//关闭连接
#define ASYNCHRONISM_DETECT_SOCKET			6							//检测连接

//检测时间
#define DETECT_SOCKET_TIME					10000						//检测时间

CString		pushDataBaseFileName;										//数据库推送日志路径
CString		excDataBaseFileName;										//数据库执行日志路径
CString		serverFileName;												//远程日志路径

//////////////////////////////////////////////////////////////////////////
//结构定义

//发送请求
struct tagSendDataRequest
{
	//索引变量
	WORD							wIndex;								//连接索引
	WORD							wRountID;							//循环索引

	//数据信息
	WORD							wDataSize;							//数据大小
	WORD							wSubCmdID;							//子命令码
	WORD							wMainCmdID;							//主命令码
};

//群发请求
struct tagBatchSendRequest
{
	WORD							wDataSize;							//数据大小
	WORD							wSubCmdID;							//子命令码
	WORD							wMainCmdID;							//主命令码
};

//允许群发
struct tagAllowBatchSend
{
	WORD							wIndex;								//连接索引
	WORD							wRountID;							//循环索引
	BYTE							cbAllowBatch;						//允许标志
};

//关闭连接
struct tagCloseSocket
{
	WORD							wIndex;								//连接索引
	WORD							wRountID;							//循环索引
};

//安全关闭
struct tagShutDownSocket
{
	WORD							wIndex;								//连接索引
	WORD							wRountID;							//循环索引
};

//////////////////////////////////////////////////////////////////////////

//构造函数
COverLappedItem::COverLappedItem(enOperationType OperationType) : m_OperationType(OperationType)
{
	//设置变量
	ZeroMemory(&m_WSABuffer, sizeof(m_WSABuffer));
	ZeroMemory(&m_OverLapped, sizeof(m_OverLapped));

	return;
}

//析构函数
COverLappedItem::~COverLappedItem()
{
}

//////////////////////////////////////////////////////////////////////////

//构造函数
COverLappedSend::COverLappedSend() : COverLappedItem(enOperationType_Send)
{
	m_WSABuffer.len = 0;
	m_WSABuffer.buf = (char *)m_cbBuffer;
}

//析构函数
COverLappedSend::~COverLappedSend()
{
}

//////////////////////////////////////////////////////////////////////////

//构造函数
COverLappedRecv::COverLappedRecv() : COverLappedItem(enOperationType_Recv)
{
	m_WSABuffer.len = 0;
	m_WSABuffer.buf = NULL;
}

//析构函数
COverLappedRecv::~COverLappedRecv()
{
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CTCPNetworkSocket::CTCPNetworkSocket(WORD wIndex, ITCPNetworkSocketSink * pITCPNetworkSocketSink)
{
	//关键变量
	m_wIndex = wIndex;
	m_pITCPNetworkSocketSink = pITCPNetworkSocketSink;
	m_TCPSocketData.ResetBufferData();

	//本地端口
	m_dwLocalPort = 0;

	//连接属性
	m_dwClientAddr = 0L;
	m_dwActiveTime = 0L;

	//核心变量
	m_wRountID = 1;
	m_wSurvivalTime = DEAD_QUOTIETY;
	m_hSocketHandle = INVALID_SOCKET;

	//索引变量
	m_ulSendIndex = 0;
	m_ulRecvIndex = 0;

	//监听端口
	m_dwListenPort = 0;

	//状态变量
	m_bSendIng = false;
	m_bRecvIng = false;
	m_bShutDown = false;
	m_bAllowBatch = false;

	//接收变量
	m_wTcpBufferSize = 0;
	m_wWebBufferSize = 0;

	//区分标记
	m_bFirstRecvData = false;
	m_wConnectType = 0;

	//客户退出
	m_bClientQuit = FALSE;

	//特殊变量
	ZeroMemory(m_cbWebDataBuffer, sizeof(m_cbWebDataBuffer));
	ZeroMemory(m_cbTcpDataBuffer, sizeof(m_cbTcpDataBuffer));

	//连接属性
	ZeroMemory(m_szClientPort, sizeof(m_szClientPort));
	ZeroMemory(m_szClientAddr, sizeof(m_szClientAddr));

	//删除数组
	m_OverLappedSendBuffer.RemoveAll();
	m_OverLappedSendActive.RemoveAll();

	return;
}

//析构函数
CTCPNetworkSocket::~CTCPNetworkSocket()
{
	//删除空闲
	for (INT_PTR i = 0; i < m_OverLappedSendBuffer.GetCount(); i++)
	{
		delete m_OverLappedSendBuffer[i];
	}

	//删除活动
	for (INT_PTR i = 0; i < m_OverLappedSendActive.GetCount(); i++)
	{
		delete m_OverLappedSendActive[i];
	}

	//删除数组
	m_OverLappedSendBuffer.RemoveAll();
	m_OverLappedSendActive.RemoveAll();

	return;
}

//恢复数据,上层接口自身对象锁已加锁
VOID CTCPNetworkSocket::ResumeData()
{
	//核心变量
	m_hSocketHandle = INVALID_SOCKET;
	m_wSurvivalTime = DEAD_QUOTIETY;

	//本地端口
	m_dwLocalPort = 0;

	//连接属性
	m_dwClientAddr = 0L;
	m_dwActiveTime = 0L;

	//状态变量
	m_bSendIng = false;
	m_bRecvIng = false;
	m_bShutDown = false;
	m_bAllowBatch = false;

	//网络数据
	m_TCPSocketData.ResetBufferData();

	//循环索引
	m_wRountID = __max(1, m_wRountID + 1);

	//索引变量
	m_ulSendIndex = 0;
	m_ulRecvIndex = 0;

	//监听端口
	m_dwListenPort = 0;

	//接收变量
	m_wTcpBufferSize = 0;
	m_wWebBufferSize = 0;

	//区分标记
	m_bFirstRecvData = false;
	m_wConnectType = 0;

	//客户退出
	m_bClientQuit = FALSE;

	//特殊变量
	ZeroMemory(m_cbWebDataBuffer, sizeof(m_cbWebDataBuffer));
	ZeroMemory(m_cbTcpDataBuffer, sizeof(m_cbTcpDataBuffer));

	//连接信息
	ZeroMemory(m_szClientPort, sizeof(m_szClientPort));
	ZeroMemory(m_szClientAddr, sizeof(m_szClientAddr));

	//复用对象
	m_OverLappedSendBuffer.Append(m_OverLappedSendActive);
	//清理活跃
	m_OverLappedSendActive.RemoveAll();

	return;
}

//绑定数据
VOID CTCPNetworkSocket::AttachData(SOCKET hSocket, DWORD dwListenPort, DWORD dwClientAddr)
{
	//没有延迟
	int nodelay = 1;
	setsockopt(hSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&nodelay, sizeof(nodelay));

	//参数准备
	struct sockaddr addr;
	int addr_len = sizeof(addr);

	//本地端口
	if (getpeername(hSocket, &addr, &addr_len) == 0)
	{
		if (addr.sa_family == AF_INET)
		{
			m_dwLocalPort = ntohs(((sockaddr_in*)&addr)->sin_port);
		}
	}

	//状态变量
	m_bSendIng = false;
	m_bRecvIng = false;
	m_bShutDown = false;
	m_bAllowBatch = false;

	//索引变量
	m_ulSendIndex = 0;
	m_ulRecvIndex = 0;

	//监听端口
	m_dwListenPort = dwListenPort;

	//设置变量
	m_hSocketHandle = hSocket;
	m_dwClientAddr = dwClientAddr;
	m_wSurvivalTime = SAFETY_QUOTIETY;
	m_dwActiveTime = (DWORD)time(NULL);

	//日志提示
	TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
	_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("AttachData绑定数据, 本地端口:%d, 索引:%d"), m_dwLocalPort, GetIdentifierID());
	CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

	return;
}

//绑定标志
VOID CTCPNetworkSocket::AttachClientPort(CHAR * szClientPort, CHAR *szClientAddr)
{
	//大小判断
	if (strlen(szClientPort) != 0 && strlen(szClientAddr) != 0)
	{
		//唯一标识
		CopyMemory(m_szClientPort, szClientPort, strlen(szClientPort));
		m_szClientPort[strlen(szClientPort) + 1] = '\0';

		//客户地址
		CopyMemory(m_szClientAddr, szClientAddr, strlen(szClientAddr));
		m_szClientAddr[strlen(szClientAddr) + 1] = '\0';

		//连接地址
		m_dwClientAddr = inet_addr(m_szClientAddr);
		if (m_dwClientAddr == INADDR_NONE) return;

		//日志提示
		TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
		_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("AttachClientPort绑定标志, 唯一标志:%S, 客户地址:%S, 索引:%d"), m_szClientPort, m_szClientAddr, GetIdentifierID());
		CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);
	}

	//发送通知
	m_pITCPNetworkSocketSink->OnEventSocketBind(this);
}

//发送函数
bool CTCPNetworkSocket::SendData(WORD wMainCmdID, WORD wSubCmdID, WORD wRountID)
{
	return SendData(NULL, 0, wMainCmdID, wSubCmdID, wRountID);
}

//发送函数
bool CTCPNetworkSocket::SendData(VOID * pData, WORD wDataSize, WORD wMainCmdID, WORD wSubCmdID, WORD wRountID)
{
	//校验大小
	if (wDataSize > SOCKET_TCP_PACKET)
	{
		TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
		_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("SendData发送大小判断错误, 唯一标志:%S, 本地端口:%d, 主命令:%d, 从命令:%d, 大小:%d, 索引:%d"), m_szClientPort, m_dwLocalPort, wMainCmdID, wSubCmdID, wDataSize, GetIdentifierID());
		CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);
		return false;
	}

	//发送判断
	if (SendVerdict(wRountID) == false)
	{
		return false;
	}

	//首包判断
	if (m_bFirstRecvData == false)
	{
		//日志记录
		TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
		_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("SendData发送数据检测, 首包还未收到, 跳过数据发送, 唯一标志:%S, 本地端口:%d, 主命令:%d, 从命令:%d, 大小:%d, 索引:%d"), m_szClientPort, m_dwLocalPort, wMainCmdID, wSubCmdID, wDataSize, GetIdentifierID());
		CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);
		return true;
	}

	//序号加加
	m_ulSendIndex++;

	//设置数据
	if (m_TCPSocketData.InitSocketData(wMainCmdID, wSubCmdID, pData, wDataSize, m_ulSendIndex, m_dwListenPort) == false)
	{
		//日志记录
		TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
		_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("SendData设置数据错误, 唯一标志:%S, 本地端口:%d, 主命令:%d, 从命令:%d, 大小:%d, 索引:%d"), m_szClientPort, m_dwLocalPort, wMainCmdID, wSubCmdID, wDataSize, GetIdentifierID());
		CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);
		//序号减减
		m_ulSendIndex--;
		return false;
	}

	//连接类型
	if (m_wConnectType == 0)
	{
		//输出缓冲
		BYTE cbWebDataSendBuffer[SOCKET_TCP_BUFFER] = { 0 };
		int dwWebDataSendBufferLen = 0;

		//拼装数据
		dwWebDataSendBufferLen = EncodeWebSocketData(m_TCPSocketData.m_cbDataBuffer, m_TCPSocketData.m_wBufferSize, cbWebDataSendBuffer);

		//发送数据
		if (SendDataBuffer(cbWebDataSendBuffer, dwWebDataSendBufferLen) == false)
		{
			//日志记录
			TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
			_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("SendData发送WEB数据失败, 关闭连接, 唯一标志:%S, 本地端口:%d, 主命令:%d, 从命令:%d, 大小:%d, 发送索引:%I64d, 索引:%d"), m_szClientPort, m_dwLocalPort, wMainCmdID, wSubCmdID, wDataSize, m_ulSendIndex, GetIdentifierID());
			CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);
			//关闭连接
			CloseSocket(m_wRountID);
			return false;
		}
		else
		{
			/*
			//日志记录
			TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
			_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("SendData发送WEB数据成功, 唯一标志:%S, 本地端口:%d, 主命令:%d, 从命令:%d, 大小:%d, 发送索引:%I64d, 索引:%d"), m_szClientPort, m_dwLocalPort, wMainCmdID, wSubCmdID, wDataSize, m_ulSendIndex, GetIdentifierID());
			CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);
			*/
		}
	}
	else
	{
		//发送数据
		if (SendDataBuffer(m_TCPSocketData.m_cbDataBuffer, m_TCPSocketData.m_wBufferSize) == false)
		{
			//日志记录
			TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
			_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("SendData发送TCP数据失败, 关闭连接, 唯一标志:%S, 本地端口:%d, 主命令:%d, 从命令:%d, 大小:%d, 发送索引:%I64d, 索引:%d"), m_szClientPort, m_dwLocalPort, wMainCmdID, wSubCmdID, wDataSize, m_ulSendIndex, GetIdentifierID());
			CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

			//关闭连接
			CloseSocket(m_wRountID);
			return false;
		}
		else
		{
			/*
			//日志记录
			TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
			_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("SendData发送TCP数据成功, 唯一标志:%S, 本地端口:%d, 主命令:%d, 从命令:%d, 大小:%d, 发送索引:%I64d, 索引:%d"), m_szClientPort, m_dwLocalPort, wMainCmdID, wSubCmdID, wDataSize, m_ulSendIndex, GetIdentifierID());
			CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);
			*/
		}
	}

	return true;
}

//投递接收
bool CTCPNetworkSocket::RecvData()
{
	//中止判断
	if (m_bShutDown)
	{
		//日志记录
		TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
		_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("连接标志为中断, 投递接收请求, 不再接收数据, 本地端口:%d, 唯一标识:%S, 索引:%d"), m_dwLocalPort, m_szClientPort, GetIdentifierID());
		CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

		//接收标志
		m_bRecvIng = false;

		return true;
	}

	//接收数据
	DWORD dwThancferred = 0, dwFlags = 0;
	INT nResultCode = WSARecv(m_hSocketHandle, &m_OverLappedRecv.m_WSABuffer, 1, &dwThancferred, &dwFlags, &m_OverLappedRecv.m_OverLapped, NULL);

	//结果处理
	if ((nResultCode == SOCKET_ERROR) && (WSAGetLastError() != WSA_IO_PENDING))
	{
		//日志记录
		TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
		_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("完成端口投递接收请求失败, 关闭连接, 本地端口:%d, 唯一标识:%S, 索引:%d"), m_dwLocalPort, m_szClientPort, GetIdentifierID());
		CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

		//关闭连接
		CloseSocket(m_wRountID);
		return false;
	}

	//设置变量
	m_bRecvIng = true;

	return true;
}

//关闭连接(反复调用的接口,需要检测状态,状态不对,说明已经处理过)
bool CTCPNetworkSocket::CloseSocket(WORD wRountID)
{
	//状态判断
	if (m_wRountID != wRountID) return false;

	//日志记录
	TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
	_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("CTCPNetworkSocket::CloseSocket关闭连接, 接收标志:%d, 发送标志:%d, 唯一标识:%S, 本地端口:%d, 索引:%d"), m_bRecvIng, m_bSendIng, m_szClientPort, m_dwLocalPort, GetIdentifierID());
	CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

	//强制关闭
	if (m_hSocketHandle != INVALID_SOCKET)
	{
		//关闭连接
		closesocket(m_hSocketHandle);
		//重置句柄
		m_hSocketHandle = INVALID_SOCKET;
	}

	//判断关闭
	if (m_bRecvIng == false && m_bSendIng == false)
	{
		//通知一次
		OnCloseCompleted();
	}

	return true;
}

//关闭接收(反复调用的接口,需要检测状态,状态不对,说明已经处理过)
bool CTCPNetworkSocket::ShutDownSocket(WORD wRountID)
{
	//状态判断
	if (m_hSocketHandle == INVALID_SOCKET || m_wRountID != wRountID) return false;

	//日志记录
	TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
	_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("CTCPNetworkSocket::ShutDownSocket关闭接收, 接收标志:%d, 发送标志:%d, 唯一标识:%S, 本地端口:%d, 索引:%d"), m_bRecvIng, m_bSendIng, m_szClientPort, m_dwLocalPort, GetIdentifierID());
	CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

	//设置关闭
	m_bShutDown = true;

	return true;
}

//重置收发
bool CTCPNetworkSocket::ResetSendRecv()
{
	m_bSendIng = false;
	m_bRecvIng = false;

	return true;
}

//允许群发
bool CTCPNetworkSocket::AllowBatchSend(WORD wRountID, bool bAllowBatch)
{
	//状态判断
	if (m_wRountID != wRountID) return false;
	if (m_hSocketHandle == INVALID_SOCKET) return false;

	//设置变量
	m_bAllowBatch = bAllowBatch;

	return true;
}

//发送完成
bool CTCPNetworkSocket::OnSendCompleted(COverLappedSend * pOverLappedSend, DWORD dwThancferred)
{
	//校验变量
	ASSERT(m_bSendIng == true);
	ASSERT(m_OverLappedSendActive.GetCount() > 0);
	ASSERT(pOverLappedSend == m_OverLappedSendActive[0]);

	//数据检测
	if (pOverLappedSend == m_OverLappedSendActive[0])
	{
		//释放结构
		m_OverLappedSendActive.RemoveAt(0);
		m_OverLappedSendBuffer.Add(pOverLappedSend);
	}
	else
	{
		//日志记录
		TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
		_sntprintf_s(szFormatBuff, CountArray(szFormatBuff), TEXT("完成端口发送数据完成, 出现野指针, 关闭连接, 本地端口:%d, 唯一标识:%S"), m_dwLocalPort, m_szClientPort);
		CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

		//关闭连接
		CloseSocket(m_wRountID);
		return false;
	}

	//设置变量
	m_bSendIng = false;

	//判断句柄
	if (m_hSocketHandle == INVALID_SOCKET)
	{
		//日志记录
		TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
		_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("完成端口发送数据无效句柄, 关闭连接, 本地端口:%d, 唯一标识:%S"), m_dwLocalPort, m_szClientPort);
		CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

		//关闭连接
		CloseSocket(m_wRountID);
		return true;
	}

	//设置变量
	if (dwThancferred != 0)
	{
		m_wSurvivalTime = SAFETY_QUOTIETY;
	}

	//继续发送
	if (m_OverLappedSendActive.GetCount() > 0)
	{
		//获取数据
		pOverLappedSend = m_OverLappedSendActive[0];

		//发送数据
		DWORD dwThancferred = 0;

		if (SOCKET_ERROR == WSASend(m_hSocketHandle, &pOverLappedSend->m_WSABuffer, 1, &dwThancferred, 0, &pOverLappedSend->m_OverLapped, NULL))
		{
			int iErrorCode = WSAGetLastError();
			if (iErrorCode != WSA_IO_PENDING)
			{
				//日志记录
				TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
				_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("完成端口发送数据失败, 关闭连接, 错误标识:%d, 本地端口:%d, 唯一标识:%S"), iErrorCode, m_dwLocalPort, m_szClientPort);
				CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

				//关闭连接
				CloseSocket(m_wRountID);
				return false;
			}
		}

		//设置变量
		m_bSendIng = true;

		//中断标志
		if (m_bShutDown)
		{
			//日志记录
			TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
			_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("完成端口接着发送数据, 中断标志, 接收状态:%d, 发送状态:%d, 重叠个数:%d, 本地端口:%d, 唯一标识:%S"), m_bRecvIng, m_bSendIng, m_OverLappedSendActive.GetCount(), m_dwLocalPort, m_szClientPort);
			CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);
		}
	}

	//没有数据收发，直接关闭连接
	if (m_bRecvIng == false && m_bSendIng == false)
	{
		CloseSocket(m_wRountID);
	}

	return true;
}

//接收完成
bool CTCPNetworkSocket::OnRecvCompleted(COverLappedRecv * pOverLappedRecv, DWORD dwThancferred)
{
	//设置变量
	m_bRecvIng = false;

	//判断句柄
	if (m_hSocketHandle == INVALID_SOCKET)
	{
		//日志记录
		TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
		_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("完成端口接收数据检测到无效句柄, 关闭连接, 本地端口:%d, 唯一标识:%S, 索引:%d"), m_dwLocalPort, m_szClientPort, GetIdentifierID());
		CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

		//关闭连接
		CloseSocket(m_wRountID);
		return true;
	}

	//临时缓冲
	BYTE cbTempRecvDataBuff[SOCKET_TCP_BUFFER] = { 0 };
	int dwRealRecvLen = 0;

	//首次检查
	if (m_bFirstRecvData == false)
	{
		//首次接收
		dwRealRecvLen = SOCKET_TCP_BUFFER;
	}
	else
	{
		//接收类型
		if (m_wConnectType == 0)
		{
			//剩余长度
			int dwRestBufferLen = RECV_SOCKET_TCP_BUFFER - m_wWebBufferSize;
			//实际长度
			dwRealRecvLen = dwRestBufferLen > SOCKET_TCP_BUFFER ? SOCKET_TCP_BUFFER : dwRestBufferLen;
		}
		else
		{
			//剩余长度
			int dwRestBufferLen = RECV_SOCKET_TCP_BUFFER - m_wTcpBufferSize;
			//实际长度
			dwRealRecvLen = dwRestBufferLen > SOCKET_TCP_BUFFER ? SOCKET_TCP_BUFFER : dwRestBufferLen;
		}
	}

	//实际接收
	int dwTempRecvLength = recv(m_hSocketHandle, (char *)cbTempRecvDataBuff, dwRealRecvLen, 0);
	if (dwTempRecvLength < 0)
	{
		int iErrorCode = WSAGetLastError();
		if (iErrorCode == EINTR || iErrorCode == WSAEWOULDBLOCK || iErrorCode == EAGAIN)
		{
			return RecvData();
		}
		else
		{
			//日志记录
			TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
			_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("完成端口接收异常, 关闭连接, 错误标识:%d, 本地端口:%d, 唯一标识:%S, 索引:%d"), iErrorCode, m_dwLocalPort, m_szClientPort, GetIdentifierID());
			CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

			//关闭连接
			CloseSocket(m_wRountID);
			return true;
		}
	}
	else if (dwTempRecvLength == 0)
	{
		//日志记录
		TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
		_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("完成端口接收关闭, 关闭连接, 本地端口:%d, 唯一标识:%S, 索引:%d"), m_dwLocalPort, m_szClientPort, GetIdentifierID());
		CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

		//关闭连接
		CloseSocket(m_wRountID);
		return true;
	}
	else
	{
		//中断判断
		if (m_bShutDown)
		{
			//日志记录
			TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
			_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("连接标志为中断, 接收数据完成, 不再接收数据, 本地端口:%d, 唯一标识:%S, 索引:%d"), m_dwLocalPort, m_szClientPort, GetIdentifierID());
			CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

			//接收标记
			m_bRecvIng = false;

			return true;
		}

		//首次接收
		if (m_bFirstRecvData == false)
		{
			char cbTcharTempRecvDataBuff[SOCKET_TCP_BUFFER] = { 0 };
			memcpy(cbTcharTempRecvDataBuff, (char *)cbTempRecvDataBuff, dwTempRecvLength);
			if (string(cbTcharTempRecvDataBuff).find(("Sec-WebSocket-Key:")) != -1)
			{
				//应答回复
				char HandshakeBuff[SOCKET_TCP_BUFFER] = { 0 };
				WebsocketHandshake(string(cbTcharTempRecvDataBuff), HandshakeBuff);
				//发送数据 
				SendDataBuffer(HandshakeBuff, strlen(HandshakeBuff));
				//更改标志
				m_bFirstRecvData = true;
				//连接类型
				m_wConnectType = 0;
				//继续接收
				return RecvData();
			}
			else
			{
				//更改标志
				m_bFirstRecvData = true;
				//连接类型
				m_wConnectType = 1;
			}
		}

		//连接类型
		if (m_wConnectType == 0)
		{
			//网络缓冲
			CopyMemory(m_cbWebDataBuffer + m_wWebBufferSize, cbTempRecvDataBuff, dwTempRecvLength);
			m_wWebBufferSize += dwTempRecvLength;

			//设置变量
			m_wSurvivalTime = SAFETY_QUOTIETY;

			//处理数据
			try
			{
				//循环处理
				while (m_wWebBufferSize > 0)
				{
					//输出缓冲
					BYTE cbWebDataOutBuffer[SOCKET_TCP_BUFFER] = { 0 };
					int valWebDataOutBufferLen = 0;
					int delWebDataOutBufferLenth = 0;

					//处理数据
					valWebDataOutBufferLen = DncodeWebSocketData(m_cbWebDataBuffer, m_wWebBufferSize, delWebDataOutBufferLenth, cbWebDataOutBuffer);
					if (valWebDataOutBufferLen < 0)
					{
						if (valWebDataOutBufferLen == -1)
						{
							//继续接收
							return RecvData();
						}
						else
						{
							//日志记录
							TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
							_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("解析H5端口数据错误, 关闭连接, 本地端口:%d, 唯一标识:%S, 索引:%d"), m_dwLocalPort, m_szClientPort, GetIdentifierID());
							CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

							//关闭连接
							CloseSocket(m_wRountID);
							//提前返回
							return false;
						}
					}
					else if (valWebDataOutBufferLen == 0)
					{
						//日志记录
						TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
						_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("解析H5端口数据关闭, 关闭连接, 本地端口:%d, 唯一标识:%S, 索引:%d"), m_dwLocalPort, m_szClientPort, GetIdentifierID());
						CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

						//关闭连接
						CloseSocket(m_wRountID);
						//提前返回
						return false;
					}
					else
					{
						//缓冲数据
						m_wWebBufferSize -= delWebDataOutBufferLenth;
						MoveMemory(m_cbWebDataBuffer, m_cbWebDataBuffer + delWebDataOutBufferLenth, m_wWebBufferSize);

						//有效数据
						CopyMemory(m_cbTcpDataBuffer + m_wTcpBufferSize, cbWebDataOutBuffer, valWebDataOutBufferLen);
						m_wTcpBufferSize += valWebDataOutBufferLen;

						//有效数据
						while (m_wTcpBufferSize >= sizeof(TCP_Head))
						{
							//变量定义
							TCP_Info * pInfo = (TCP_Info *)m_cbTcpDataBuffer;

							//长度判断
							if (pInfo->wPacketSize > SOCKET_TCP_BUFFER)
							{
								throw TEXT("数据包长度太长");
							}

							//长度判断
							if (pInfo->wPacketSize < sizeof(TCP_Info))
							{
								throw TEXT("数据包长度太短");
							}

							//完成判断
							if (m_wTcpBufferSize < pInfo->wPacketSize) break;

							//包头大小
							WORD wBufferDataSize = pInfo->wPacketSize;

							//消息类型
							if (pInfo->wMsgType == MSG_TYPE_QUICK_CONNECT)
							{
								//是否处理
								if (strlen(m_szClientPort) == 0)
								{
									//准备参数
									CHAR szClientPort[33] = { 0 };
									CHAR szClientAddr[47] = { 0 };

									//唯一标识
									CopyMemory(szClientPort, m_cbTcpDataBuffer + sizeof(TCP_Head), strlen(szClientPort) - 1);
									szClientPort[strlen(szClientPort) + 1] = '\0';

									//唯一标识
									CopyMemory(szClientAddr, m_cbTcpDataBuffer + sizeof(TCP_Head) + 32, strlen(szClientAddr) - 1);
									szClientAddr[strlen(szClientAddr) + 1] = '\0';

									//标识地址
									AttachClientPort(szClientPort, szClientAddr);
								}

								//移动数据
								m_wTcpBufferSize -= wBufferDataSize;
								MoveMemory(m_cbTcpDataBuffer, m_cbTcpDataBuffer + wBufferDataSize, m_wTcpBufferSize);

							}
							else if (pInfo->wMsgType == MSG_TYPE_CLIENT_QUIT)
							{
								//保存标记
								m_bClientQuit = TRUE;

								//移动数据
								m_wTcpBufferSize -= wBufferDataSize;
								MoveMemory(m_cbTcpDataBuffer, m_cbTcpDataBuffer + wBufferDataSize, m_wTcpBufferSize);
							}
							else
							{
								//设置数据
								tagTCPData SocketData;
								if (m_TCPSocketData.SetBufferData(m_cbTcpDataBuffer, wBufferDataSize, SocketData) == false)
								{
									throw TEXT("数据包解压或者解密错误");
								}

								//移动数据
								m_wTcpBufferSize -= wBufferDataSize;
								MoveMemory(m_cbTcpDataBuffer, m_cbTcpDataBuffer + wBufferDataSize, m_wTcpBufferSize);

								//接收索引
								m_ulRecvIndex++;

								/*
								//日志记录
								TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
								_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("完成端口接收WEB数据, 关闭连接, 本地端口:%d, 唯一标识:%S, 主命令:%d, 从命令:%d, 接收序号:%I64d, 接收索引:%I64d, 索引:%d"), m_dwLocalPort, m_szClientPort, SocketData.Command.wMainCmdID, SocketData.Command.wSubCmdID, pInfo->ulCmdNo, m_ulRecvIndex, GetIdentifierID());
								CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);
								*/

								//数据处理
								if (SocketData.wMsgType == MSG_TYPE_NORMAL_DATA && SocketData.Command.wMainCmdID != MDM_KN_COMMAND && SocketData.Command.wMainCmdID != MDM_KN_COMMAND_LAJI && SocketData.Command.wMainCmdID != MDM_KN_COMMAND_LAJI2)
								{
									m_pITCPNetworkSocketSink->OnEventSocketRead(SocketData.Command, SocketData.pDataBuffer, SocketData.wDataSize, this);
								}
							}
						}
					}
				}
			}
			catch (LPCTSTR pszMessage)
			{
				//日志记录
				TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
				_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("完成端口接收数据捕获到异常, 关闭连接, 异常:%s, 本地端口:%d, 唯一标识:%S, 索引:%d"), pszMessage, m_dwLocalPort, m_szClientPort, GetIdentifierID());
				CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

				//关闭连接
				CloseSocket(m_wRountID);

				return false;
			}
			catch (...)
			{
				//日志记录
				TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
				_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("完成端口接收数据捕获到未知错误, 关闭连接, 本地端口:%d, 唯一标识:%S, 索引:%d"), m_dwLocalPort, m_szClientPort, this->GetIdentifierID());
				CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

				//关闭连接
				CloseSocket(m_wRountID);

				return false;
			}
		}
		else
		{
			//Tcp缓冲数据
			CopyMemory(m_cbTcpDataBuffer + m_wTcpBufferSize, cbTempRecvDataBuff, dwTempRecvLength);
			m_wTcpBufferSize += dwTempRecvLength;

			//设置变量
			m_wSurvivalTime = SAFETY_QUOTIETY;

			//处理数据
			try
			{
				while (m_wTcpBufferSize >= sizeof(TCP_Head))
				{
					//变量定义
					TCP_Info * pInfo = (TCP_Info *)m_cbTcpDataBuffer;

					//长度判断
					if (pInfo->wPacketSize > SOCKET_TCP_BUFFER)
					{
						throw TEXT("数据包长度太长");
					}

					//长度判断
					if (pInfo->wPacketSize < sizeof(TCP_Info))
					{
						throw TEXT("数据包长度太短");
					}

					//完成判断
					if (m_wTcpBufferSize < pInfo->wPacketSize) break;

					//包头大小
					WORD wBufferDataSize = pInfo->wPacketSize;

					//消息类型
					if (pInfo->wMsgType == MSG_TYPE_QUICK_CONNECT)
					{
						//是否处理
						if (strlen(m_szClientPort) == 0)
						{
							//准备参数
							CHAR szClientPort[33] = { 0 };
							CHAR szClientAddr[47] = { 0 };

							//唯一标识
							CopyMemory(szClientPort, m_cbTcpDataBuffer + sizeof(TCP_Head), sizeof(szClientPort) - 1);
							CopyMemory(szClientAddr, m_cbTcpDataBuffer + sizeof(TCP_Head) + 32, sizeof(szClientAddr) - 1);

							//标识地址
							AttachClientPort(szClientPort, szClientAddr);
						}

						//移动数据
						m_wTcpBufferSize -= wBufferDataSize;
						MoveMemory(m_cbTcpDataBuffer, m_cbTcpDataBuffer + wBufferDataSize, m_wTcpBufferSize);
					}
					else if (pInfo->wMsgType == MSG_TYPE_CLIENT_QUIT)
					{
						//保存标记
						m_bClientQuit = TRUE;

						//移动数据
						m_wTcpBufferSize -= wBufferDataSize;
						MoveMemory(m_cbTcpDataBuffer, m_cbTcpDataBuffer + wBufferDataSize, m_wTcpBufferSize);
					}
					else
					{
						//设置数据
						tagTCPData SocketData;
						if (m_TCPSocketData.SetBufferData(m_cbTcpDataBuffer, wBufferDataSize, SocketData) == false)
						{
							throw TEXT("数据包解压或者解密错误");
						}

						//移动数据
						m_wTcpBufferSize -= wBufferDataSize;
						MoveMemory(m_cbTcpDataBuffer, m_cbTcpDataBuffer + wBufferDataSize, m_wTcpBufferSize);

						//接收索引
						m_ulRecvIndex++;

						/*
						//日志记录
						TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
						_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("完成端口接收TCP数据, 本地端口:%d, 唯一标识:%S, 主命令:%d, 从命令:%d, 接收序号:%I64d, 接收索引:%I64d, 索引:%d"), m_dwLocalPort, m_szClientPort, SocketData.Command.wMainCmdID, SocketData.Command.wSubCmdID, pInfo->ulCmdNo, m_ulRecvIndex, GetIdentifierID());
						CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);
						*/

						//数据处理
						if (SocketData.wMsgType == MSG_TYPE_NORMAL_DATA && SocketData.Command.wMainCmdID != MDM_KN_COMMAND && SocketData.Command.wMainCmdID != MDM_KN_COMMAND_LAJI && SocketData.Command.wMainCmdID != MDM_KN_COMMAND_LAJI2)
						{
							m_pITCPNetworkSocketSink->OnEventSocketRead(SocketData.Command, SocketData.pDataBuffer, SocketData.wDataSize, this);
						}
					}
				}
			}
			catch (LPCTSTR pszMessage)
			{
				//日志记录
				TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
				_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("完成端口接收数据捕获到异常, 关闭连接, 异常:%s, 本地端口:%d, 唯一标识:%S, 索引:%d"), pszMessage, m_dwLocalPort, m_szClientPort, GetIdentifierID());
				CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

				//关闭连接
				CloseSocket(m_wRountID);

				return false;
			}
			catch (...)
			{
				//日志记录
				TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
				_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("完成端口接收数据捕获到未知错误, 关闭连接, 本地端口:%d, 唯一标识:%S, 索引:%d"), m_dwLocalPort, m_szClientPort, GetIdentifierID());
				CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

				//关闭连接
				CloseSocket(m_wRountID);

				return false;
			}
		}
		return RecvData();
	}
}

//关闭完成
bool CTCPNetworkSocket::OnCloseCompleted()
{
	//日志记录
	TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
	_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("关闭套结字, 关闭完成, 本地端口:%d, 唯一标识:%S, 接收索引:%I64d, 发送索引:%I64d, 索引:%d"), m_dwLocalPort, m_szClientPort, m_ulRecvIndex, m_ulSendIndex, GetIdentifierID());
	CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

	//关闭事件
	m_pITCPNetworkSocketSink->OnEventSocketShut(this);

	return true;
}

//发送判断
bool CTCPNetworkSocket::SendVerdict(WORD wRountID)
{
	if (m_hSocketHandle == INVALID_SOCKET)
	{
		return false;
	}

	if ((m_wRountID != wRountID) || (m_bShutDown == true))
	{
		return false;
	}

	return true;
}

//发送数据
bool CTCPNetworkSocket::SendDataBuffer(VOID * pBuffer, WORD wSendSize)
{
	//获取缓冲
	COverLappedSend * pOverLappedSend = GetSendOverLapped(wSendSize);

	//关闭判断
	if (pOverLappedSend == NULL)
	{
		return false;
	}

	//变量定义
	ULONG lSourceLen = pOverLappedSend->m_WSABuffer.len;

	//设置数据
	pOverLappedSend->m_WSABuffer.len += wSendSize;
	CopyMemory(pOverLappedSend->m_cbBuffer + lSourceLen, pBuffer, wSendSize);

	//发送数据
	if (m_bSendIng == false)
	{
		//发送数据
		DWORD dwThancferred = 0;

		if (SOCKET_ERROR == WSASend(m_hSocketHandle, &pOverLappedSend->m_WSABuffer, 1, &dwThancferred, 0, &pOverLappedSend->m_OverLapped, NULL))
		{
			int iErrorCode = WSAGetLastError();
			if (iErrorCode != WSA_IO_PENDING)
			{
				return false;
			}
		}

		//设置变量
		m_bSendIng = true;
	}

	return true;
}

//获取缓存
COverLappedSend * CTCPNetworkSocket::GetSendOverLapped(WORD wPacketSize)
{
	//重叠结构
	COverLappedSend * pOverLappedSend = NULL;

	//重用判断
	if (m_OverLappedSendActive.GetCount() > 1)
	{
		//对象数量
		INT_PTR nActiveCount = m_OverLappedSendActive.GetCount();
		//获取对象
		pOverLappedSend = m_OverLappedSendActive[nActiveCount - 1];
		//对象检测
		if (pOverLappedSend != NULL && sizeof(pOverLappedSend->m_cbBuffer) >= (pOverLappedSend->m_WSABuffer.len + wPacketSize + sizeof(DWORD) * 2))
		{
			//返回对象
			return pOverLappedSend;
		}
	}

	//空闲对象
	if (m_OverLappedSendBuffer.GetCount() > 0)
	{
		//对象数量
		INT_PTR nFreeCount = m_OverLappedSendBuffer.GetCount();
		//获取对象
		pOverLappedSend = m_OverLappedSendBuffer[nFreeCount - 1];
		//移除对象
		m_OverLappedSendBuffer.RemoveAt(nFreeCount - 1);
		//对象检测
		if (pOverLappedSend != NULL)
		{
			//设置变量
			pOverLappedSend->m_WSABuffer.len = 0;
			//保存激活
			m_OverLappedSendActive.Add(pOverLappedSend);
			//返回对象
			return pOverLappedSend;
		}
	}

	try
	{
		//创建对象
		pOverLappedSend = new COverLappedSend;
		if (pOverLappedSend == NULL)
		{
			CPushKernelLog(KernelEngineLog, LOG_ERROR, TEXT("创建重叠结构失败"));
			return NULL;
		}

		//设置变量
		pOverLappedSend->m_WSABuffer.len = 0;
		m_OverLappedSendActive.Add(pOverLappedSend);

		//返回对象
		return pOverLappedSend;
	}
	catch (...)
	{
		CPushKernelLog(KernelEngineLog, LOG_ERROR, TEXT("创建重叠结构异常"));
		ASSERT(FALSE);
	}


	return NULL;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CTCPNetworkThreadReadWrite::CTCPNetworkThreadReadWrite()
{
	//设置变量
	m_hCompletionPort = NULL;

	return;
}

//析构函数
CTCPNetworkThreadReadWrite::~CTCPNetworkThreadReadWrite()
{

}

//运行函数
bool CTCPNetworkThreadReadWrite::OnEventThreadRun()
{
	//校验参数
	ASSERT(m_hCompletionPort != NULL);

	//变量定义
	DWORD dwThancferred = 0;
	OVERLAPPED * pOverLapped = NULL;
	CTCPNetworkSocket * pTCPNetworkSocket = NULL;

	//完成端口
	BOOL bSuccess = GetQueuedCompletionStatus(m_hCompletionPort, &dwThancferred, (PULONG_PTR)&pTCPNetworkSocket, &pOverLapped, INFINITE);
	if (bSuccess == FALSE)
	{
		/*
		If a call to GetQueuedCompletionStatus fails because the completion port handle associated with it is closed
		while the call is outstanding, the function returns FALSE, *lpOverlapped will be NULL, and GetLastError will
		return ERROR_ABANDONED_WAIT_0.
		*/
		if (NULL == pOverLapped && GetLastError() == ERROR_ABANDONED_WAIT_0)
		{
			//继续运行
			return true;
		}
		else
		{
			/*
			If theGetQueuedCompletionStatus function succeeds, it dequeued a completion packet for a successful I/O operation
			from the completion port and has stored information in the variables pointed to by the following parameters:
			lpNumberOfBytes, lpCompletionKey, and lpOverlapped. Upon failure (the return value is FALSE), those same parameters
			can contain particular value combinations as follows:
			*/

			/*
			the first Situation:
			If *lpOverlapped is NULL, the function did not dequeue a completion packet from the completion port. In this case,
			the function does not store information in the variables pointed to by the lpNumberOfBytes and lpCompletionKey
			parameters, and their values are indeterminate.
			*/
			if (pOverLapped == NULL)
			{
				//继续运行
				return true;
			}

			/*
			the second Situation:
			If *lpOverlapped is not NULL and the function dequeues a completion packet for a failed I/O operation from the
			completion port, the function stores information about the failed operation in the variables pointed to by
			lpNumberOfBytes, lpCompletionKey, and lpOverlapped. To get extended error information, call GetLastError
			*/
			if (pOverLapped != NULL)
			{
				//获取错误
				if (WAIT_TIMEOUT == GetLastError())
				{
					//继续运行
					return true;
				}
				else
				{
					if (pTCPNetworkSocket != NULL)
					{
						//日志记录
						TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
						_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("CTCPNetworkThreadReadWrite::GetQueuedCompletionStatus错误, 关闭连接, 原因:%d, 索引:%d"), GetLastError(), pTCPNetworkSocket->GetIdentifierID());
						CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

						//对象加锁
						CWHDataLocker DataLocker(pTCPNetworkSocket->m_NetworkSocketSection);

						//重置收发
						pTCPNetworkSocket->ResetSendRecv();

						//关闭连接
						pTCPNetworkSocket->CloseSocket(pTCPNetworkSocket->GetRountID());
					}

					//继续运行
					return true;
				}
			}

			/*
			the Third Situation:
			If a socket handle associated with a completion port is closed, GetQueuedCompletionStatus returns ERROR_SUCCESS,
			with lpNumberOfBytes equal zero.
			*/
			if (dwThancferred == 0)
			{
				//日志记录
				TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
				_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("CTCPNetworkThreadReadWrite::GetQueuedCompletionStatus错误, socket已经被关闭, 完成线程继续执行, 线程ID:%d"), GetCurrentThreadId());
				CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

				//继续运行
				return true;
			}

			//继续运行
			return true;
		}
	}
	else
	{
		//退出判断
		if ((pTCPNetworkSocket == NULL) && (pOverLapped == NULL))
		{
			//日志记录
			TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
			_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("CTCPNetworkThreadReadWrite::GetQueuedCompletionStatus重叠对象为NULL, 完成线程退出, 线程ID:%d"), GetCurrentThreadId());
			CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

			return false;
		}

		//对象加锁
		CWHDataLocker DataLocker(pTCPNetworkSocket->m_NetworkSocketSection);

		//变量定义
		COverLappedItem * pSocketLapped = CONTAINING_RECORD(pOverLapped, COverLappedItem, m_OverLapped);

		//操作处理
		switch (pSocketLapped->GetOperationType())
		{
			case enOperationType_Send:	//数据发送
			{
				pTCPNetworkSocket->OnSendCompleted((COverLappedSend *)pSocketLapped, dwThancferred);
				break;
			}
			case enOperationType_Recv:	//数据读取
			{
				pTCPNetworkSocket->OnRecvCompleted((COverLappedRecv *)pSocketLapped, dwThancferred);
				break;
			}
			default:
				break;
		}

		return true;
	}
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CTCPNetworkThreadAccept::CTCPNetworkThreadAccept()
{
	//设置变量
	m_hCompletionPort = NULL;
	m_pTCPNetworkEngine = NULL;
	m_hServerSocket = INVALID_SOCKET;

	return;
}

//析构函数
CTCPNetworkThreadAccept::~CTCPNetworkThreadAccept()
{

}

//运行函数
bool CTCPNetworkThreadAccept::OnEventThreadRun()
{
	//校验参数
	ASSERT(m_hCompletionPort != NULL);
	ASSERT(m_pTCPNetworkEngine != NULL);

	//变量定义
	SOCKET hConnectSocket = INVALID_SOCKET;
	CTCPNetworkSocket * pTCPNetworkSocket = NULL;

	try
	{
		//监听连接
		SOCKADDR_IN SocketAddr;
		INT nBufferSize = sizeof(SocketAddr);

		//接受连接
		hConnectSocket = WSAAccept(m_hServerSocket, (SOCKADDR *)&SocketAddr, &nBufferSize, NULL, NULL);

		//退出判断
		if (hConnectSocket == INVALID_SOCKET)
		{
			int iLastError = WSAGetLastError();
			if (iLastError == WSAENOTSOCK || iLastError == WSAEINTR)
			{
				//监听关闭,退出线程
				return false;
			}
			else
			{
				//日志记录
				TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
				_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("CTCPNetworkThreadAccept::OnEventThreadRun分配socket失败,错误标识:%d"), iLastError);
				CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

				//其他错误,继续执行
				return true;
			}
		}

		//获取连接
		pTCPNetworkSocket = m_pTCPNetworkEngine->ActiveNetworkItem();

		//失败判断
		if (pTCPNetworkSocket == NULL)
		{
			//关闭连接
			closesocket(hConnectSocket);
			//继续运行
			return true;
		}

		//锁定对象
		CWHDataLocker DataLocker(pTCPNetworkSocket->m_NetworkSocketSection);

		//绑定对象
		pTCPNetworkSocket->AttachData(hConnectSocket, m_pTCPNetworkEngine->m_wCurrentPort, SocketAddr.sin_addr.S_un.S_addr);

		//创建端口
		CreateIoCompletionPort((HANDLE)hConnectSocket, m_hCompletionPort, (ULONG_PTR)pTCPNetworkSocket, 0);

		//发起接收
		pTCPNetworkSocket->RecvData();
	}
	catch (LPCTSTR pszMessage)
	{
		//清理对象
		if (pTCPNetworkSocket != NULL)
		{
			CPushKernelLog(KernelEngineLog, LOG_ERROR, TEXT("接受线程出现异常,并且内核对象不为空"));
		}
		else
		{
			//日志记录
			TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
			_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("接受线程接收到错误:%s"), pszMessage);
			CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);
		}

		//关闭连接
		if (hConnectSocket != INVALID_SOCKET)
		{
			closesocket(hConnectSocket);
		}
	}
	catch (...)
	{
		//清理对象
		if (pTCPNetworkSocket != NULL)
		{
			CPushKernelLog(KernelEngineLog, LOG_ERROR, TEXT("接受线程出现异常,并且内核对象不为空"));
		}
		else
		{
			CPushKernelLog(KernelEngineLog, LOG_ERROR, TEXT("接受线程接收到未知错误"));
		}

		//关闭连接
		if (hConnectSocket != INVALID_SOCKET)
		{
			closesocket(hConnectSocket);
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CTCPNetworkThreadDetect::CTCPNetworkThreadDetect()
{
	m_dwPileTime = 0L;
	m_dwDetectTime = DETECT_SOCKET_TIME;
	m_pTCPNetworkEngine = NULL;
}

//析构函数
CTCPNetworkThreadDetect::~CTCPNetworkThreadDetect()
{
}

//运行函数
bool CTCPNetworkThreadDetect::OnEventThreadRun()
{
	//校验参数
	ASSERT(m_pTCPNetworkEngine != NULL);

	//设置间隔
	Sleep(200);
	m_dwPileTime += 200L;

	/*
	//屏蔽检测
	if (m_dwPileTime >= m_dwDetectTime)
	{
		m_dwPileTime = 0L;
		m_pTCPNetworkEngine->DetectSocket();
	}
	*/

	return true;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CTCPNetworkEngine::CTCPNetworkEngine()
{
	//状态变量
	m_bService = false;
	m_wCurrentPort = 0;

	//配置变量
	m_wMaxConnect = MAX_CONTENT;
	m_wServicePort = PORT_AUTO_SELECT;

	//内核变量
	m_hCompletionPort = NULL;
	m_hServerSocket = INVALID_SOCKET;
	m_pITCPNetworkEngineEvent = NULL;

	//子项变量
	m_NetworkItemActiveMap.clear();
	m_NetworkItemFreeMap.clear();

	return;
}

//析构函数
CTCPNetworkEngine::~CTCPNetworkEngine()
{
}

//接口查询
VOID * CTCPNetworkEngine::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IServiceModule, Guid, dwQueryVer);
	QUERYINTERFACE(ITCPNetworkEngine, Guid, dwQueryVer);
	QUERYINTERFACE(IAsynchronismEngineSink, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITCPNetworkEngine, Guid, dwQueryVer);
	return NULL;
}

//启动服务
bool CTCPNetworkEngine::StartService()
{
	//状态校验
	ASSERT(m_bService == false);
	if (m_bService == true) return false;

	//校验参数
	ASSERT(m_wMaxConnect != 0);
	if (m_wMaxConnect == 0) return false;

	//系统信息
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);

	//完成端口
	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, SystemInfo.dwNumberOfProcessors);
	if (m_hCompletionPort == NULL) return false;


	serverFileName.Format(TEXT("kernelEngineLog_%d.txt"), m_wServicePort);
	excDataBaseFileName.Format(TEXT("excDataBaseLog.txt"));
	pushDataBaseFileName.Format(TEXT("PushExcDataBaseLog.txt"));

	//创建网络
	try
	{
		if ((m_wServicePort == PORT_AUTO_SELECT) || (m_wServicePort == 0))
		{
			//创建网络
			for (WORD wServicePort = PORT_AUTO_MIN; wServicePort < PORT_AUTO_MAX; wServicePort++)
			{
				//端口过滤
				if (wServicePort == PORT_CHAT) continue;
				if (wServicePort == PORT_TASK) continue;
				if (wServicePort == PORT_LOGON) continue;
				if (wServicePort == PORT_PERSIONCENTER) continue;
				if (wServicePort == PORT_UPDATE) continue;
				if (wServicePort == PORT_CENTER) continue;
				if (wServicePort == PORT_MESSAGE) continue;
				if (wServicePort == PORT_CHAT) continue;
				if (wServicePort == PORT_AGENTLIEBIAN) continue;
				if (wServicePort == PORT_CHARGELIEBIAN) continue;
				if (wServicePort == PORT_STATICSTICS) continue;

				//创建网络
				if (CreateSocket(wServicePort) == true)
				{
					//保存端口
					m_wCurrentPort = wServicePort;

					break;
				}
			}

			//失败判断
			if (m_wServicePort == PORT_AUTO_MAX)
			{
				g_TraceServiceManager.TraceString(TEXT("尝试了中队的端口号，监听操作仍然没有成功"), TraceLevel_Exception);
				return false;
			}
		}
		else
		{
			//创建网络
			if (CreateSocket(m_wServicePort) == false)
			{
				//构造提示
				TCHAR szString[512] = TEXT("");
				_sntprintf_s(szString, _TRUNCATE, TEXT("端口正被其他服务占用，监听%ld端口失败"), m_wServicePort);

				//输出信息
				g_TraceServiceManager.TraceString(szString, TraceLevel_Exception);

				return false;
			}

			//保存端口
			m_wCurrentPort = m_wServicePort;
		}
	}
	catch (LPCTSTR pszString)
	{
		//错误信息
		g_TraceServiceManager.TraceString(pszString, TraceLevel_Exception);

		return false;
	}

	//异步引擎
	if (m_AsynchronismEngine.SetAsynchronismSink(QUERY_ME_INTERFACE(IUnknownEx)) == false)
	{
		ASSERT(FALSE);
		return false;
	}

	//读写线程
	for (DWORD i = 0; i < SystemInfo.dwNumberOfProcessors; i++)
	{
		//变量定义
		CTCPNetworkThreadReadWrite * pNetworkRSThread = NULL;

		//创建对象
		try
		{
			//创建对象
			pNetworkRSThread = new CTCPNetworkThreadReadWrite();
			pNetworkRSThread->m_hCompletionPort = m_hCompletionPort;

			//插入数组
			m_SocketRWThreadArray.Add(pNetworkRSThread);
		}
		catch (...)
		{
			//错误断言
			ASSERT(FALSE);

			//删除对象
			SafeDelete(pNetworkRSThread);

			return false;
		}
	}

	//检测线程
	m_SocketDetectThread.m_dwPileTime = 0L;
	m_SocketDetectThread.m_dwDetectTime = DETECT_SOCKET_TIME;
	m_SocketDetectThread.m_pTCPNetworkEngine = this;

	//应答线程
	m_SocketAcceptThread.m_pTCPNetworkEngine = this;
	m_SocketAcceptThread.m_hServerSocket = m_hServerSocket;
	m_SocketAcceptThread.m_hCompletionPort = m_hCompletionPort;

	//读写线程
	for (INT_PTR i = 0; i < m_SocketRWThreadArray.GetCount(); i++)
	{
		if (m_SocketRWThreadArray[i]->StartThread() == false)
		{
			return false;
		}
	}

	//辅助线程
	if (m_SocketDetectThread.StartThread() == false) return false;
	if (m_SocketAcceptThread.StartThread() == false) return false;

	//异步引擎
	if (m_AsynchronismEngine.StartService() == false) return false;

	//设置变量
	m_bService = true;

	//释放对象
	m_NetworkItemActiveMap.clear();
	m_NetworkItemFreeMap.clear();

	return true;
}

//停止服务
bool CTCPNetworkEngine::ConcludeService()
{
	//设置变量
	m_bService = false;

	//关闭网络
	if (m_hServerSocket != INVALID_SOCKET)
	{
		closesocket(m_hServerSocket);
		m_hServerSocket = INVALID_SOCKET;
	}

	//异步引擎
	m_AsynchronismEngine.ConcludeService();

	//关闭线程
	m_SocketDetectThread.ConcludeThread(INFINITE);
	m_SocketAcceptThread.ConcludeThread(INFINITE);

	//读写线程
	INT_PTR nCount = m_SocketRWThreadArray.GetCount();
	if (m_hCompletionPort != NULL)
	{
		for (INT_PTR i = 0; i < nCount; i++) PostQueuedCompletionStatus(m_hCompletionPort, 0, NULL, NULL);
	}

	//读写线程
	for (INT_PTR i = 0; i < nCount; i++)
	{
		CTCPNetworkThreadReadWrite * pSocketThread = m_SocketRWThreadArray[i];
		ASSERT(pSocketThread != NULL);
		pSocketThread->ConcludeThread(INFINITE);
		SafeDelete(pSocketThread);
	}
	m_SocketRWThreadArray.RemoveAll();

	//完成端口
	if (m_hCompletionPort != NULL)
	{
		CloseHandle(m_hCompletionPort);
		m_hCompletionPort = NULL;
	}

	//互斥加锁
	CWHDataLocker ItemLocker(m_NetworkItemSection);

	//循环处理
	for (CTCPNetworkItemPtrMap::iterator iter = m_NetworkItemActiveMap.begin(); iter != m_NetworkItemActiveMap.end(); iter++)
	{
		//获取数据
		CTCPNetworkSocket * pTCPNetworkSocket = iter->second;
		if (pTCPNetworkSocket == NULL) continue;

		//关闭连接
		pTCPNetworkSocket->CloseSocket(pTCPNetworkSocket->GetRountID());

		//保存空闲
		m_NetworkItemFreeMap[pTCPNetworkSocket->GetIndex()] = pTCPNetworkSocket;
	}

	//重置数据
	m_NetworkItemActiveMap.clear();

	return true;
}

//设置参数
bool CTCPNetworkEngine::SetServiceParameter(WORD wServicePort, WORD wMaxConnect, LPCTSTR pszCompilation)
{
	//校验状态
	ASSERT(m_bService == false);
	if (m_bService == true) return false;

	m_wMaxConnect = wMaxConnect;
	m_wServicePort = wServicePort;
	m_wCurrentPort = wServicePort;

	//日志端口
	CServerPushLog::SharedLog()->SetServerPort(m_wCurrentPort);

	//连接上限
	CString subGameLog;
	subGameLog.Format(TEXT("内核连接上限:%d"), m_wMaxConnect);
	CTraceService::TraceString(subGameLog, TraceLevel_Normal);

	return true;
}

//设置接口
bool CTCPNetworkEngine::SetTCPNetworkEngineEvent(IUnknownEx * pIUnknownEx)
{
	//校验状态
	ASSERT(m_bService == false);
	if (m_bService == true) return false;

	//查询接口
	m_pITCPNetworkEngineEvent = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, ITCPNetworkEngineEvent);

	//错误判断
	if (m_pITCPNetworkEngineEvent == NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//发送函数
bool CTCPNetworkEngine::SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID)
{
	//变量定义
	tagSendDataRequest SendDataRequest;
	ZeroMemory(&SendDataRequest, sizeof(SendDataRequest));

	//构造函数
	SendDataRequest.wDataSize = 0;
	SendDataRequest.wSubCmdID = wSubCmdID;
	SendDataRequest.wMainCmdID = wMainCmdID;
	SendDataRequest.wIndex = SOCKET_INDEX(dwSocketID);
	SendDataRequest.wRountID = SOCKET_ROUNTID(dwSocketID);

	//发送请求
	return m_AsynchronismEngine.PostAsynchronismData(ASYNCHRONISM_SEND_DATA, &SendDataRequest, sizeof(SendDataRequest));
}

//发送函数
bool CTCPNetworkEngine::SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//校验大小
	ASSERT(wDataSize <= SOCKET_TCP_PACKET);
	if (wDataSize > SOCKET_TCP_PACKET) return false;

	//变量定义
	tagDataBuffer DataBuffer[2];
	tagSendDataRequest SendDataRequest;

	//设置变量
	ZeroMemory(&DataBuffer, sizeof(DataBuffer));
	ZeroMemory(&SendDataRequest, sizeof(SendDataRequest));

	//构造函数
	SendDataRequest.wDataSize = wDataSize;
	SendDataRequest.wSubCmdID = wSubCmdID;
	SendDataRequest.wMainCmdID = wMainCmdID;
	SendDataRequest.wIndex = SOCKET_INDEX(dwSocketID);
	SendDataRequest.wRountID = SOCKET_ROUNTID(dwSocketID);

	//请求包头
	DataBuffer[0].pDataBuffer = &SendDataRequest;
	DataBuffer[0].wDataSize = sizeof(SendDataRequest);

	//附加数据
	if (wDataSize > 0)
	{
		ASSERT(pData != NULL);
		DataBuffer[1].pDataBuffer = pData;
		DataBuffer[1].wDataSize = wDataSize;
	}

	//发送请求
	return m_AsynchronismEngine.PostAsynchronismData(ASYNCHRONISM_SEND_DATA, DataBuffer, CountArray(DataBuffer));
}

//批量发送
bool CTCPNetworkEngine::SendDataBatch(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, BYTE cbBatchMask)
{
	//校验数据
	ASSERT(wDataSize <= SOCKET_TCP_PACKET);
	if (wDataSize > SOCKET_TCP_PACKET) return false;

	//变量定义
	tagDataBuffer DataBuffer[2];
	tagBatchSendRequest BatchSendRequest;

	//设置变量
	ZeroMemory(&DataBuffer, sizeof(DataBuffer));
	ZeroMemory(&BatchSendRequest, sizeof(BatchSendRequest));

	//构造函数
	BatchSendRequest.wDataSize = wDataSize;
	BatchSendRequest.wSubCmdID = wSubCmdID;
	BatchSendRequest.wMainCmdID = wMainCmdID;

	//请求包头
	DataBuffer[0].pDataBuffer = &BatchSendRequest;
	DataBuffer[0].wDataSize = sizeof(BatchSendRequest);

	//附加数据
	if (wDataSize > 0)
	{
		ASSERT(pData != NULL);
		DataBuffer[1].pDataBuffer = pData;
		DataBuffer[1].wDataSize = wDataSize;
	}

	//发送请求
	return m_AsynchronismEngine.PostAsynchronismData(ASYNCHRONISM_SEND_BATCH, DataBuffer, CountArray(DataBuffer));
}

//关闭连接
bool CTCPNetworkEngine::CloseSocket(DWORD dwSocketID)
{
	//变量定义
	tagCloseSocket tCloseSocket;
	ZeroMemory(&tCloseSocket, sizeof(tCloseSocket));

	//构造数据
	tCloseSocket.wIndex = SOCKET_INDEX(dwSocketID);
	tCloseSocket.wRountID = SOCKET_ROUNTID(dwSocketID);

	//发送请求
	return m_AsynchronismEngine.PostAsynchronismData(ASYNCHRONISM_CLOSE_SOCKET, &tCloseSocket, sizeof(tCloseSocket));
}

//设置关闭
bool CTCPNetworkEngine::ShutDownSocket(DWORD dwSocketID)
{
	//通知关闭
	SendData(dwSocketID, MDM_KN_COMMAND, SUB_KN_NETWORK_SHUT, NULL, 0);

	//变量定义
	tagShutDownSocket tShutDownSocket;
	ZeroMemory(&tShutDownSocket, sizeof(tShutDownSocket));

	//构造数据
	tShutDownSocket.wIndex = SOCKET_INDEX(dwSocketID);
	tShutDownSocket.wRountID = SOCKET_ROUNTID(dwSocketID);

	//发送请求
	return m_AsynchronismEngine.PostAsynchronismData(ASYNCHRONISM_SHUT_DOWN, &tShutDownSocket, sizeof(tShutDownSocket));
}

//允许群发
bool CTCPNetworkEngine::AllowBatchSend(DWORD dwSocketID, bool bAllowBatch, BYTE cbBatchMask)
{
	//变量定义
	tagAllowBatchSend tAllowBatchSend;
	ZeroMemory(&tAllowBatchSend, sizeof(tAllowBatchSend));

	//构造数据
	tAllowBatchSend.cbAllowBatch = bAllowBatch;
	tAllowBatchSend.wIndex = SOCKET_INDEX(dwSocketID);
	tAllowBatchSend.wRountID = SOCKET_ROUNTID(dwSocketID);

	//发送请求
	return m_AsynchronismEngine.PostAsynchronismData(ASYNCHRONISM_ALLOW_BATCH, &tAllowBatchSend, sizeof(tAllowBatchSend));
}

//异步数据
bool CTCPNetworkEngine::OnAsynchronismEngineData(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	switch (wIdentifier)
	{
		case ASYNCHRONISM_SEND_DATA:		//发送请求
		{
			//变量定义
			tagSendDataRequest * pSendDataRequest = (tagSendDataRequest *)pData;

			//获取对象
			CTCPNetworkSocket * pTCPNetworkSocket = GetNetworkItem(pSendDataRequest->wIndex);
			if (pTCPNetworkSocket == NULL) return false;

			//对象加锁
			CWHDataLocker NetworkItemLocker(pTCPNetworkSocket->m_NetworkSocketSection);

			//发送数据
			pTCPNetworkSocket->SendData(pSendDataRequest + 1, pSendDataRequest->wDataSize, pSendDataRequest->wMainCmdID, pSendDataRequest->wSubCmdID, pSendDataRequest->wRountID);

			return true;
		}
		case ASYNCHRONISM_SEND_BATCH:		//群发请求
		{
			//校验数据
			tagBatchSendRequest * pBatchSendRequest = (tagBatchSendRequest *)pData;

			//互斥加锁
			CWHDataLocker ItemLocker(m_NetworkItemSection);

			//获取活动
			CTCPNetworkItemPtrMap TempNetworkItemPtrMap;
			TempNetworkItemPtrMap.insert(m_NetworkItemActiveMap.begin(), m_NetworkItemActiveMap.end());

			//互斥解锁
			ItemLocker.UnLock();

			//群发数据
			for (CTCPNetworkItemPtrMap::iterator iter = TempNetworkItemPtrMap.begin(); iter != TempNetworkItemPtrMap.end(); iter++)
			{
				//获取数据
				CTCPNetworkSocket * pTCPNetworkSocket = iter->second;
				if (pTCPNetworkSocket == NULL) continue;

				//对象加锁
				CWHDataLocker NetworkItemLocker(pTCPNetworkSocket->m_NetworkSocketSection);

				//有效判断
				if (pTCPNetworkSocket->IsValidSocket() == false) continue;

				//发送数据
				if (pTCPNetworkSocket->IsAllowBatch() == true)
				{
					pTCPNetworkSocket->SendData(pBatchSendRequest + 1, pBatchSendRequest->wDataSize, pBatchSendRequest->wMainCmdID, pBatchSendRequest->wSubCmdID, pTCPNetworkSocket->GetRountID());
				}
			}

			return true;
		}
		case ASYNCHRONISM_SHUT_DOWN:		//安全关闭
		{
			//校验数据
			ASSERT(wDataSize == sizeof(tagShutDownSocket));
			tagShutDownSocket * pShutDownRequest = (tagShutDownSocket *)pData;

			//获取对象
			CTCPNetworkSocket * pTCPNetworkSocket = GetNetworkItem(pShutDownRequest->wIndex);
			if (pTCPNetworkSocket == NULL) return false;

			//对象加锁
			CWHDataLocker NetworkItemLocker(pTCPNetworkSocket->m_NetworkSocketSection);

			//安全关闭
			pTCPNetworkSocket->ShutDownSocket(pShutDownRequest->wRountID);

			return true;
		}
		case ASYNCHRONISM_ALLOW_BATCH:		//允许群发
		{
			//校验数据
			ASSERT(wDataSize == sizeof(tagAllowBatchSend));
			tagAllowBatchSend * pAllowBatchSend = (tagAllowBatchSend *)pData;

			//获取对象
			CTCPNetworkSocket * pTCPNetworkSocket = GetNetworkItem(pAllowBatchSend->wIndex);
			if (pTCPNetworkSocket == NULL) return false;

			//对象加锁
			CWHDataLocker NetworkItemLocker(pTCPNetworkSocket->m_NetworkSocketSection);

			//允许群发
			pTCPNetworkSocket->AllowBatchSend(pAllowBatchSend->wRountID, pAllowBatchSend->cbAllowBatch ? true : false);

			return true;
		}
		case ASYNCHRONISM_CLOSE_SOCKET:		//关闭连接
		{
			//校验数据
			ASSERT(wDataSize == sizeof(tagCloseSocket));
			tagCloseSocket * pCloseSocket = (tagCloseSocket *)pData;

			//获取对象
			CTCPNetworkSocket * pTCPNetworkSocket = GetNetworkItem(pCloseSocket->wIndex);
			if (pTCPNetworkSocket == NULL) return false;

			//对象加锁
			CWHDataLocker NetworkItemLocker(pTCPNetworkSocket->m_NetworkSocketSection);

			//关闭连接
			pTCPNetworkSocket->CloseSocket(pCloseSocket->wRountID);

			return true;
		}
		case ASYNCHRONISM_DETECT_SOCKET:	//检测连接
		{
			//互斥加锁
			CWHDataLocker ItemLocker(m_NetworkItemSection);

			//获取活动
			CTCPNetworkItemPtrMap TempNetworkItemPtrMap;
			TempNetworkItemPtrMap.insert(m_NetworkItemActiveMap.begin(), m_NetworkItemActiveMap.end());

			//互斥解锁
			ItemLocker.UnLock();

			//群发数据
			for (CTCPNetworkItemPtrMap::iterator iter = TempNetworkItemPtrMap.begin(); iter != TempNetworkItemPtrMap.end(); iter++)
			{
				//获取数据
				CTCPNetworkSocket * pTCPNetworkSocket = iter->second;
				if (pTCPNetworkSocket == NULL) continue;

				//对象加锁
				CWHDataLocker NetworkItemLocker(pTCPNetworkSocket->m_NetworkSocketSection);

				//有效判断
				if (pTCPNetworkSocket->IsValidSocket() == false) continue;

				//连接判断
				if (pTCPNetworkSocket->m_wSurvivalTime <= DANGER_QUOTIETY && pTCPNetworkSocket->m_wSurvivalTime > DEAD_QUOTIETY)
				{
					pTCPNetworkSocket->m_wSurvivalTime--;
					pTCPNetworkSocket->SendData(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET, pTCPNetworkSocket->GetRountID());
				}
				else if (pTCPNetworkSocket->m_wSurvivalTime <= DEAD_QUOTIETY)
				{
					//日志记录
					TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
					_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("检测到死亡连接, 关闭连接, 本地端口:%d, 唯一标识:%S"), pTCPNetworkSocket->m_dwLocalPort, pTCPNetworkSocket->m_szClientPort);
					CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

					//关闭连接
					pTCPNetworkSocket->CloseSocket(pTCPNetworkSocket->GetRountID());
					continue;
				}
				else
				{
					pTCPNetworkSocket->m_wSurvivalTime--;
				}

				switch (pTCPNetworkSocket->m_wSurvivalTime)
				{
					case DEAD_QUOTIETY:		//死亡连接
					{
						//日志记录	
						TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
						_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("检测到死亡连接, 关闭连接, 本地端口:%d, 唯一标识:%S"), pTCPNetworkSocket->m_dwLocalPort, pTCPNetworkSocket->m_szClientPort);
						CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

						//关闭连接
						pTCPNetworkSocket->CloseSocket(pTCPNetworkSocket->GetRountID());
						break;
					}
					case DANGER_QUOTIETY:	//危险系数
					{
						pTCPNetworkSocket->m_wSurvivalTime--;
						pTCPNetworkSocket->SendData(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET, pTCPNetworkSocket->GetRountID());
						break;
					}
					default:				//默认处理
					{
						pTCPNetworkSocket->m_wSurvivalTime--;
						pTCPNetworkSocket->SendData(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET, pTCPNetworkSocket->GetRountID());
						break;
					}
				}
			}
			return true;
		}
		default:
			break;
	}

	//校验数据
	ASSERT(FALSE);

	return false;
}

//绑定事件
bool CTCPNetworkEngine::OnEventSocketBind(CTCPNetworkSocket * pTCPNetworkSocket)
{
	//锁定队列
	CWHDataLocker ItemLocker(m_TCPNetworkEngineSection);

	//校验数据
	ASSERT(pTCPNetworkSocket != NULL);
	ASSERT(m_pITCPNetworkEngineEvent != NULL);

	//投递消息
	DWORD dwSocketID = pTCPNetworkSocket->GetIdentifierID();
	DWORD dwClientAddr = pTCPNetworkSocket->GetClientAddr();
	m_pITCPNetworkEngineEvent->OnEventTCPNetworkBind(dwSocketID, dwClientAddr);

	return true;
}

//关闭事件
bool CTCPNetworkEngine::OnEventSocketShut(CTCPNetworkSocket * pTCPNetworkSocket)
{
	try
	{
		//锁定队列
		CWHDataLocker ItemLocker(m_TCPNetworkEngineSection);

		//投递数据
		DWORD dwSocketID = pTCPNetworkSocket->GetIdentifierID();
		DWORD dwClientAddr = pTCPNetworkSocket->GetClientAddr();
		DWORD dwActiveTime = pTCPNetworkSocket->GetActiveTime();
		BOOL bClientQuit = pTCPNetworkSocket->GetClientQuit();

		//通知关闭
		m_pITCPNetworkEngineEvent->OnEventTCPNetworkShut(dwSocketID, dwClientAddr, dwActiveTime, bClientQuit);

		//释放连接
		FreeNetworkItem(pTCPNetworkSocket);
	}
	catch (...)
	{

	}

	return true;
}

//读取事件
bool CTCPNetworkEngine::OnEventSocketRead(TCP_Command Command, VOID * pData, WORD wDataSize, CTCPNetworkSocket * pTCPNetworkSocket)
{
	//锁定队列
	CWHDataLocker ItemLocker(m_TCPNetworkEngineSection);

	//校验数据
	ASSERT(pTCPNetworkSocket != NULL);
	ASSERT(m_pITCPNetworkEngineEvent != NULL);

	//投递消息
	DWORD dwSocketID = pTCPNetworkSocket->GetIdentifierID();

	//网络处理
	m_pITCPNetworkEngineEvent->OnEventTCPNetworkRead(dwSocketID, Command, pData, wDataSize);

	return true;
}

//检测连接
bool CTCPNetworkEngine::DetectSocket()
{
	//发送请求
	return m_AsynchronismEngine.PostAsynchronismData(ASYNCHRONISM_DETECT_SOCKET, (LPVOID)(NULL), 0);
}

//创建网络
bool CTCPNetworkEngine::CreateSocket(DWORD wServicePort)
{
	//变量定义
	SOCKADDR_IN SocketAddr;
	ZeroMemory(&SocketAddr, sizeof(SocketAddr));

	//设置变量
	SocketAddr.sin_family = AF_INET;
	SocketAddr.sin_addr.s_addr = INADDR_ANY;
	SocketAddr.sin_port = htons(wServicePort);
	m_hServerSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	//错误判断
	if (m_hServerSocket == INVALID_SOCKET) throw TEXT("系统资源不足或者 TCP/IP协议没有安装，网络启动失败");

	//绑定连接
	if (::bind(m_hServerSocket, (SOCKADDR*)&SocketAddr, sizeof(SocketAddr)) == SOCKET_ERROR)
	{
		//关闭句柄
		closesocket(m_hServerSocket);
		m_hServerSocket = INVALID_SOCKET;

		return false;
	}

	//监听端口
	if (::listen(m_hServerSocket, 512) == SOCKET_ERROR)
	{
		//关闭句柄
		closesocket(m_hServerSocket);
		m_hServerSocket = INVALID_SOCKET;

		return false;
	}

	return true;
}

//获取对象
CTCPNetworkSocket * CTCPNetworkEngine::ActiveNetworkItem()
{
	//锁定队列
	CWHDataLocker ItemLocker(m_NetworkItemSection);

	//获取对象
	CTCPNetworkSocket * pTCPNetworkSocket = NULL;

	//优先空闲
	if (m_NetworkItemFreeMap.size() > 0)
	{
		//获取首位
		CTCPNetworkItemPtrMap::iterator iter = m_NetworkItemFreeMap.begin();
		if (iter != m_NetworkItemFreeMap.end())
		{
			//保存对象
			pTCPNetworkSocket = iter->second;

			//删除空闲
			m_NetworkItemFreeMap.erase(iter);

			//保存活跃
			m_NetworkItemActiveMap.insert(CTCPNetworkItemPtrMap::value_type(pTCPNetworkSocket->GetIndex(), pTCPNetworkSocket));
		}
	}

	//创建对象
	if (pTCPNetworkSocket == NULL)
	{
		//上限判断
		WORD wActiveCount = (WORD)m_NetworkItemActiveMap.size();
		if (wActiveCount < m_wMaxConnect)
		{
			try
			{
				//创建对象
				pTCPNetworkSocket = new CTCPNetworkSocket(wActiveCount, this);
				if (pTCPNetworkSocket == NULL)
				{
					TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
					_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("分配内核对象分配失败, 最大数量:%d, 活跃数量:%d"), m_wMaxConnect, wActiveCount);
					CTraceService::TraceString(szFormatBuff, TraceLevel_Exception);
					CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);
					return NULL;
				}

				//活跃数组
				m_NetworkItemActiveMap.insert(CTCPNetworkItemPtrMap::value_type(pTCPNetworkSocket->GetIndex(), pTCPNetworkSocket));
			}
			catch (...)
			{
				TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
				_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("分配内核对象分配异常, 最大数量:%d, 活跃数量:%d"), m_wMaxConnect, wActiveCount);
				CTraceService::TraceString(szFormatBuff, TraceLevel_Exception);
				CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);
				return NULL;
			}
		}
		else
		{
			TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
			_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("分配内核对象达到限制, 最大数量:%d, 活跃数量:%d"), m_wMaxConnect, wActiveCount);
			CTraceService::TraceString(szFormatBuff, TraceLevel_Exception);
			CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);
			return NULL;
		}
	}

	//日志记录	
	TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
	_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("分配内核对象调试信息, 最大数量:%d, 活跃数量:%d, 空闲数量:%d"), m_wMaxConnect, m_NetworkItemActiveMap.size(), m_NetworkItemFreeMap.size());
	CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

	return pTCPNetworkSocket;
}

//获取对象
CTCPNetworkSocket * CTCPNetworkEngine::GetNetworkItem(WORD wIndex)
{
	//锁定对象
	CWHDataLocker ItemLocker(m_NetworkItemSection);

	//变量对象
	CTCPNetworkSocket * pTCPNetworkSocket = NULL;

	//获取对象
	CTCPNetworkItemPtrMap::iterator iter = m_NetworkItemActiveMap.find(wIndex);
	if (iter != m_NetworkItemActiveMap.end())
	{
		pTCPNetworkSocket = iter->second;
	}

	return pTCPNetworkSocket;
}

//统一释放
bool CTCPNetworkEngine::FreeNetworkItem(CTCPNetworkSocket * pTCPNetworkSocket)
{
	//校验参数
	ASSERT(pTCPNetworkSocket != NULL);
	if (pTCPNetworkSocket == NULL) return false;

	//锁定对象
	CWHDataLocker ItemLocker(m_NetworkItemSection);

	//获取对象
	CTCPNetworkItemPtrMap::iterator iter = m_NetworkItemActiveMap.find(pTCPNetworkSocket->GetIndex());
	if (iter != m_NetworkItemActiveMap.end())
	{
		//删除活跃
		m_NetworkItemActiveMap.erase(iter);

		//恢复数据
		pTCPNetworkSocket->ResumeData();

		//增加空闲
		m_NetworkItemFreeMap.insert(CTCPNetworkItemPtrMap::value_type(pTCPNetworkSocket->GetIndex(), pTCPNetworkSocket));

		//日志记录	
		TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
		_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("释放网络对象成功, 释放索引:%d, 活跃数量:%d, 空闲数量:%d, 索引:%d"), pTCPNetworkSocket->GetIndex(), m_NetworkItemActiveMap.size(), m_NetworkItemFreeMap.size(), pTCPNetworkSocket->GetIdentifierID());
		CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

		return true;
	}


	//释放失败
	TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
	_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("释放网络对象失败, 释放索引:%d, 活跃数量:%d, 空闲数量:%d, 索引:%d"), pTCPNetworkSocket->GetIndex(), m_NetworkItemActiveMap.size(), m_NetworkItemFreeMap.size(), pTCPNetworkSocket->GetIdentifierID());
	CPushKernelLog(KernelEngineLog, LOG_ERROR, szFormatBuff);

	return false;
}


//SHA编码
int CTCPNetworkSocket::BitSHA(string origin, char *MessageDigest)
{
	char inputstr[1024];
	memset(inputstr, 0, sizeof(inputstr));

	memcpy(inputstr, origin.c_str(), origin.length());

	SHA1Context sha;
	SHA1Reset(&sha);
	SHA1Input(&sha, inputstr, strlen(inputstr));
	SHA1Result(&sha);

	for (int i = 0; i < 1024; i++)
	{
		MessageDigest[i] = (BYTE)((sha.Message_Digest[i >> 2] >> ((3 - (i & 3)) * 8)) & 255);
	}

	return 0;
}

//握手应答
int CTCPNetworkSocket::WebsocketHandshake(string cbTcharTempRecvDataBuff, char *HandshakeBuff)
{
	int start = 0;
	int end = 0;

	start = cbTcharTempRecvDataBuff.find("Key: ");
	start += 5;

	cbTcharTempRecvDataBuff = cbTcharTempRecvDataBuff.substr(start, 24);
	cbTcharTempRecvDataBuff += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

	unsigned long len = 41;

	char ShaResult[1024] = { 0 };
	char Base64Result[1024] = { 0 };

	BitSHA(cbTcharTempRecvDataBuff, ShaResult);

	gEncBase64(ShaResult, 20, Base64Result, &len);

	sprintf_s(HandshakeBuff, _TRUNCATE, "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: %s\r\n\r\n", Base64Result);

	return 0;
}

//拼装数据
int CTCPNetworkSocket::EncodeWebSocketData(BYTE* SrcBuff, int SrcLen, BYTE* DesBuff)
{
	int DestBuffLen = 0;
	if (SrcBuff == NULL)
	{
		return DestBuffLen;
	}

	if (SrcLen <= 125)
	{
		DesBuff[0] = (BYTE)0x82;		//0x81代表字符串，ox82代表二进制
		DesBuff[1] = (BYTE)SrcLen;
		CopyMemory(DesBuff + 2, (LPCWSTR)SrcBuff, SrcLen);
		DestBuffLen = SrcLen + 2;
	}
	else if (SrcLen > 125 && SrcLen <= 65535)
	{
		DesBuff[0] = (BYTE)0x82;		//0x81代表字符串，ox82代表二进制
		DesBuff[1] = (BYTE)126;
		DesBuff[2] = (byte)(SrcLen >> 8);
		DesBuff[3] = (byte)(SrcLen & 0xFF);
		CopyMemory(DesBuff + 4, (LPCWSTR)SrcBuff, SrcLen);
		DestBuffLen = SrcLen + 4;
	}
	else
	{
		DesBuff[0] = (BYTE)0x82;		//0x81代表字符串，ox82代表二进制
		DesBuff[1] = (BYTE)127;

		DesBuff[2] = 0;
		DesBuff[3] = 0;
		DesBuff[4] = 0;
		DesBuff[5] = 0;
		DesBuff[6] = (byte)(SrcLen >> 24);
		DesBuff[7] = (byte)(SrcLen >> 16);
		DesBuff[8] = (byte)(SrcLen >> 8);
		DesBuff[9] = (byte)(SrcLen & 0xFF);

		CopyMemory(DesBuff + 10, (LPCWSTR)SrcBuff, SrcLen);

		DestBuffLen = SrcLen + 2 + 8;
	}

	return DestBuffLen;
}

/*
协议说明:
包长度为126：2个字节的基本长度 + 2个字节有效数据长度 + 4个字节的掩码 + 有效数据
包长度为127：2个字节的基本长度 + 8个字节有效数据长度 + 4个字节的掩码 + 有效数据
其 他 情 况：2个字节的基本长度 + 有效数据
返 回 结 果：-2发生错误 -1继续接收 0关闭 其他代表真实数据
*/
//拆分数据
int CTCPNetworkSocket::DncodeWebSocketData(BYTE * recBytes, int recByteLength, int &deleteLenth, BYTE * payload_data)
{
	//非法长度
	if (recByteLength < 2)
	{
		return -2;
	}

	//fin位: 为1表示已接收完整报文, 为0表示继续监听后续报文
	bool fin = (recBytes[0] & 0x80) == 0x80;
	if (!fin)
	{
		return -1;
	}

	//掩码检测
	bool mask_flag = (recBytes[1] & 0x80) == 0x80;
	if (!mask_flag)
	{
		return -2;
	}

	//判断关闭
	uint8_t opcode = static_cast<uint8_t>(recBytes[0] & 0x0f);
	if (opcode == 0x08)
	{
		return 0;
	}

	//声明变量
	BYTE masks[4];						//掩码长度
	int payLoadLen;					//数据长度
	int dataHeadLen;				//协议头长

	//变量赋值
	payLoadLen = recBytes[1] & 0x7F;
	dataHeadLen = 0;
	memset(masks, 0, 4);

	if (payLoadLen == 126)
	{
		//拷贝掩码
		memcpy(masks, recBytes + 4, 4);
		//协议头长
		dataHeadLen = 4 + 4;
		//数据长度
		unsigned short s = 0;
		memcpy(&s, (const BYTE*)&recBytes[2], 2);
		payLoadLen = ntohs(s);
		//拷贝数据
		memcpy(payload_data, recBytes + 4 + 4, payLoadLen);

	}
	else if (payLoadLen == 127)
	{
		//拷贝掩码
		memcpy(masks, recBytes + 10, 4);
		//协议头长
		dataHeadLen = 10 + 4;
		//数据长度
		BYTE  uInt64Bytes[8] = { 0 };
		for (int i = 0; i < 8; i++)
		{
			uInt64Bytes[i] = recBytes[9 - i];
		}
		long long len = *(long long *)uInt64Bytes;
		//拷贝数据
		for (long long i = 0; i < len; i++)
		{
			payload_data[i] = recBytes[i + 14];
		}
	}
	else
	{
		//拷贝掩码
		memcpy(masks, recBytes + 2, 4);
		//协议头长
		dataHeadLen = 2 + 4;
		//拷贝数据
		memcpy(payload_data, recBytes + 6, payLoadLen);
	}

	//还原数据
	for (int i = 0; i < payLoadLen; i++)
	{
		payload_data[i] = (BYTE)(payload_data[i] ^ masks[i % 4]);
	}

	//删除长度
	deleteLenth = payLoadLen + dataHeadLen;

	//数据长度
	return payLoadLen;
}


//////////////////////////////////////////////////////////////////////////

//组件创建函数
DECLARE_CREATE_MODULE(TCPNetworkEngine);

//////////////////////////////////////////////////////////////////////////
