#include "StdAfx.h"
#include "AttemperEngine.h"
#include "TraceServiceManager.h"
#include "TraceService.h"
#include "ServerPushLog.h"
//////////////////////////////////////////////



//构造函数
CAttemperEngine::CAttemperEngine()
{
	//接口变量
	m_pITCPNetworkEngine = NULL;
	m_pIAttemperEngineSink = NULL;

	return;
}

//析构函数
CAttemperEngine::~CAttemperEngine(void)
{
	//停止服务
	ConcludeService();

	return;
}

//接口查询
VOID * CAttemperEngine::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAttemperEngine, Guid, dwQueryVer);
	QUERYINTERFACE(ITCPSocketEvent, Guid, dwQueryVer);
	QUERYINTERFACE(ITimerEngineEvent, Guid, dwQueryVer);
	QUERYINTERFACE(IDataBaseEngineEvent, Guid, dwQueryVer);
	QUERYINTERFACE(ITCPNetworkEngineEvent, Guid, dwQueryVer);
	QUERYINTERFACE(IAsynchronismEngineSink, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngine, Guid, dwQueryVer);
	return NULL;
}

//启动服务
bool CAttemperEngine::StartService()
{
	//效验参数
	ASSERT((m_pITCPNetworkEngine != NULL) && (m_pIAttemperEngineSink != NULL));
	if ((m_pITCPNetworkEngine == NULL) || (m_pIAttemperEngineSink == NULL))return false;

	//注册
	if (m_AsynchronismEngine.SetAsynchronismSink(QUERY_ME_INTERFACE(IUnknownEx)) == false)
	{
		ASSERT(FALSE);
		return false;
	}

	//异步引擎
	if (m_AsynchronismEngine.StartService() == false) return false;

	return true;
}

//停止服务
bool  CAttemperEngine::ConcludeService()
{
	//停止请求队列
	m_AsynchronismEngine.ConcludeService();

	return true;
}

//网络接口
bool  CAttemperEngine::SetNetworkEngine(IUnknownEx * pIUnknownEx)
{
	//设置接口
	if (pIUnknownEx != NULL)
	{
		//查询接口
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, ITCPNetworkEngine) != NULL);
		m_pITCPNetworkEngine = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, ITCPNetworkEngine);

		//成功判断
		if (m_pITCPNetworkEngine == NULL) return false;
	}
	else m_pITCPNetworkEngine = NULL;

	return true;
}

//回调接口
bool CAttemperEngine::SetAttemperEngineSink(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IAttemperEngineSink) != NULL);
	m_pIAttemperEngineSink = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IAttemperEngineSink);

	//结果判断
	if (m_pIAttemperEngineSink == NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//自定事件
bool  CAttemperEngine::OnEventCustom(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	//验证参数
	ASSERT((wRequestID&EVENT_MASK_CUSTOM) == wRequestID);
	if ((wRequestID&EVENT_MASK_CUSTOM) != wRequestID) return false;

	//投递数据
	return m_AsynchronismEngine.PostAsynchronismData(wRequestID, pData, wDataSize);
}

//控制消息
bool CAttemperEngine::OnEventControl(WORD wControlID, VOID * pData, WORD wDataSize)
{
	//缓冲判断
	ASSERT(wDataSize <= SOCKET_TCP_PACKET);
	if (wDataSize > SOCKET_TCP_PACKET) return false;

	//缓冲锁定
	CWHDataLocker lock(m_AttemperEngineSection);

	//数据包头
	NTY_ControlEvent ControlEvent;
	ControlEvent.wControlID = wControlID;

	//投递数据
	if (wDataSize > 0)
	{
		//变量定义
		tagDataBuffer DataBuffer[2];
		ZeroMemory(&DataBuffer, sizeof(DataBuffer));

		//数据包头
		DataBuffer[0].pDataBuffer = &ControlEvent;
		DataBuffer[0].wDataSize = sizeof(ControlEvent);

		//附加数据
		DataBuffer[1].pDataBuffer = pData;
		DataBuffer[1].wDataSize = wDataSize;

		//投递数据
		return m_AsynchronismEngine.PostAsynchronismData(EVENT_CONTROL, DataBuffer, CountArray(DataBuffer));
	}
	else
	{
		//投递数据
		return m_AsynchronismEngine.PostAsynchronismData(EVENT_CONTROL, &ControlEvent, sizeof(ControlEvent));
	}

	return false;
}

//定时器事件
bool  CAttemperEngine::OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{
	//缓冲锁定
	CWHDataLocker lock(m_AttemperEngineSection);
	//数据包头
	NTY_TimerEvent TimerEvent;
	ZeroMemory(&TimerEvent, sizeof(TimerEvent));

	//构造
	TimerEvent.dwTimerID = dwTimerID;
	TimerEvent.dwBindParameter = dwBindParameter;

	//投递数据
	return m_AsynchronismEngine.PostAsynchronismData(EVENT_TIMER, &TimerEvent, sizeof(TimerEvent));
}

//数据库结果
bool  CAttemperEngine::OnEventDataBaseResult(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//验证
	ASSERT(wDataSize <= SOCKET_TCP_PACKET);
	if (wDataSize > SOCKET_TCP_PACKET) return false;

	//缓冲锁定
	CWHDataLocker lock(m_AttemperEngineSection);

	//数据包头
	NTY_DataBaseEvent DataBaseEvent;
	DataBaseEvent.wRequestID = wRequestID;
	DataBaseEvent.dwContextID = dwContextID;

	//投递数据
	if (wDataSize > 0)
	{
		//变量定义
		tagDataBuffer DataBuffer[2];
		ZeroMemory(&DataBuffer, sizeof(DataBuffer));

		//数据包头
		DataBuffer[0].pDataBuffer = &DataBaseEvent;
		DataBuffer[0].wDataSize = sizeof(DataBaseEvent);

		//附加数据
		DataBuffer[1].pDataBuffer = pData;
		DataBuffer[1].wDataSize = wDataSize;

		//投递数据
		return m_AsynchronismEngine.PostAsynchronismData(EVENT_DATABASE, DataBuffer, CountArray(DataBuffer));
	}
	else
	{
		//投递数据
		return m_AsynchronismEngine.PostAsynchronismData(EVENT_DATABASE, &DataBaseEvent, sizeof(DataBaseEvent));
	}

	return false;
}

//连接事件
bool CAttemperEngine::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)
{
	//缓冲锁定
	CWHDataLocker lock(m_AttemperEngineSection);
	//数据包头
	NTY_TCPSocketLinkEvent TCPSocketLinkEvent;
	ZeroMemory(&TCPSocketLinkEvent, sizeof(TCPSocketLinkEvent));

	//构造函数
	TCPSocketLinkEvent.wServiceID = wServiceID;
	TCPSocketLinkEvent.nErrorCode = nErrorCode;

	//投递数据
	return m_AsynchronismEngine.PostAsynchronismData(EVENT_TCP_SOCKET_LINK, &TCPSocketLinkEvent, sizeof(TCPSocketLinkEvent));
}

//关闭事件
bool CAttemperEngine::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	//缓冲锁定
	CWHDataLocker lock(m_AttemperEngineSection);
	//数据包头
	NTY_TCPSocketShutEvent TCPSocketShutEvent;
	ZeroMemory(&TCPSocketShutEvent, sizeof(TCPSocketShutEvent));

	//构造函数
	TCPSocketShutEvent.wServiceID = wServiceID;
	TCPSocketShutEvent.cbShutReason = cbShutReason;

	//投递数据
	return m_AsynchronismEngine.PostAsynchronismData(EVENT_TCP_SOCKET_SHUT, &TCPSocketShutEvent, sizeof(TCPSocketShutEvent));
}

//读取事件
bool  CAttemperEngine::OnEventTCPSocketRead(WORD dwSocketID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//验证参数
	ASSERT(wDataSize <= SOCKET_TCP_PACKET);
	if (wDataSize > SOCKET_TCP_PACKET) return false;

	//数据包头
	NTY_TCPSocketReadEvent TCPSocketReadEvent;
	ZeroMemory(&TCPSocketReadEvent, sizeof(TCPSocketReadEvent));

	//数据包头
	TCPSocketReadEvent.Command = Command;
	TCPSocketReadEvent.wDataSize = wDataSize;
	TCPSocketReadEvent.wServiceID = dwSocketID;

	//投递数据
	if (wDataSize > 0)
	{
		//变量定义
		tagDataBuffer DataBuffer[2];
		ZeroMemory(&DataBuffer, sizeof(DataBuffer));

		//数据包头
		DataBuffer[0].pDataBuffer = &TCPSocketReadEvent;
		DataBuffer[0].wDataSize = sizeof(TCPSocketReadEvent);

		//附加数据
		DataBuffer[1].pDataBuffer = pData;
		DataBuffer[1].wDataSize = wDataSize;

		//投递数据
		return m_AsynchronismEngine.PostAsynchronismData(EVENT_TCP_SOCKET_READ, DataBuffer, CountArray(DataBuffer));
	}
	else
	{
		//投递数据
		return m_AsynchronismEngine.PostAsynchronismData(EVENT_TCP_SOCKET_READ, &TCPSocketReadEvent, sizeof(TCPSocketReadEvent));
	}

	return false;
}

//应答事件
bool CAttemperEngine::OnEventTCPNetworkBind(DWORD dwSocketID, DWORD dwClientAddr)
{
	//缓冲锁定
	CWHDataLocker lock(m_AttemperEngineSection);

	//数据包头
	NTY_TCPNetworkAcceptEvent TCPNetworkAcceptEvent;
	ZeroMemory(&TCPNetworkAcceptEvent, sizeof(TCPNetworkAcceptEvent));

	//构造函数
	TCPNetworkAcceptEvent.dwSocketID = dwSocketID;
	TCPNetworkAcceptEvent.dwClientAddr = dwClientAddr;

	//投递数据
	return m_AsynchronismEngine.PostAsynchronismData(EVENT_TCP_NETWORK_ACCEPT, &TCPNetworkAcceptEvent, sizeof(TCPNetworkAcceptEvent));
}

//关闭事件
bool CAttemperEngine::OnEventTCPNetworkShut(DWORD dwSocketID, DWORD dwClientAddr, DWORD dwActiveTime, BOOL bClientQuit)
{
	//缓冲锁定
	CWHDataLocker lock(m_AttemperEngineSection);

	//数据包头
	NTY_TCPNetworkShutEvent TCPNetworkShutEvent;
	ZeroMemory(&TCPNetworkShutEvent, sizeof(TCPNetworkShutEvent));

	//构造函数
	TCPNetworkShutEvent.dwSocketID = dwSocketID;
	TCPNetworkShutEvent.dwClientAddr = dwClientAddr;
	TCPNetworkShutEvent.dwActiveTime = dwActiveTime;
	TCPNetworkShutEvent.bClientQuit = bClientQuit;

	//投递数据
	return m_AsynchronismEngine.PostAsynchronismData(EVENT_TCP_NETWORK_SHUT, &TCPNetworkShutEvent, sizeof(TCPNetworkShutEvent));
}

//读取事件
bool  CAttemperEngine::OnEventTCPNetworkRead(DWORD dwSocketID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//验证参数
	ASSERT(wDataSize <= SOCKET_TCP_PACKET);
	if (wDataSize > SOCKET_TCP_PACKET) return false;

	//数据包头
	NTY_TCPNetworkReadEvent TCPNetworkReadEvent;
	ZeroMemory(&TCPNetworkReadEvent, sizeof(TCPNetworkReadEvent));

	//数据包头
	TCPNetworkReadEvent.Command = Command;
	TCPNetworkReadEvent.wDataSize = wDataSize;
	TCPNetworkReadEvent.dwSocketID = dwSocketID;

	//投递数据
	if (wDataSize > 0)
	{
		//变量定义
		tagDataBuffer DataBuffer[2];
		ZeroMemory(&DataBuffer, sizeof(DataBuffer));

		//数据包头
		DataBuffer[0].pDataBuffer = &TCPNetworkReadEvent;
		DataBuffer[0].wDataSize = sizeof(TCPNetworkReadEvent);

		//附加数据
		DataBuffer[1].pDataBuffer = pData;
		DataBuffer[1].wDataSize = wDataSize;

		//投递数据
		return m_AsynchronismEngine.PostAsynchronismData(EVENT_TCP_NETWORK_READ, DataBuffer, CountArray(DataBuffer));
	}
	else
	{
		//投递数据
		return m_AsynchronismEngine.PostAsynchronismData(EVENT_TCP_NETWORK_READ, &TCPNetworkReadEvent, sizeof(TCPNetworkReadEvent));
	}

	return false;
}

//启动事件
bool CAttemperEngine::OnAsynchronismEngineStart()
{
	//效验参数
	ASSERT(m_pIAttemperEngineSink != NULL);
	if (m_pIAttemperEngineSink == NULL)return false;

	//注册
	if (m_pIAttemperEngineSink->OnAttemperEngineStart(QUERY_ME_INTERFACE(IUnknownEx)) == false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//停止事件
bool CAttemperEngine::OnAsynchronismEngineConclude()
{
	//效验参数
	ASSERT(m_pIAttemperEngineSink != NULL);
	if (m_pIAttemperEngineSink == NULL)return false;

	//停止通知
	if (m_pIAttemperEngineSink->OnAttemperEngineConclude(QUERY_ME_INTERFACE(IUnknownEx)) == false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//异步数据
bool CAttemperEngine::OnAsynchronismEngineData(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(m_pITCPNetworkEngine != NULL);
	ASSERT(m_pIAttemperEngineSink != NULL);

	//内核事件
	switch (wIdentifier)
	{
		case EVENT_TIMER:				//时间事件
		{
			//大小断言
			ASSERT(wDataSize == sizeof(NTY_TimerEvent));
			if (wDataSize != sizeof(NTY_TimerEvent)) return false;

			//处理消息
			NTY_TimerEvent * pTimerEvent = (NTY_TimerEvent *)pData;
			m_pIAttemperEngineSink->OnEventTimer(pTimerEvent->dwTimerID, pTimerEvent->dwBindParameter);

			return true;
		}
		case EVENT_CONTROL:				//控制消息
		{
			//大小断言
			ASSERT(wDataSize >= sizeof(NTY_ControlEvent));
			if (wDataSize < sizeof(NTY_ControlEvent)) return false;

			//处理消息
			NTY_ControlEvent * pControlEvent = (NTY_ControlEvent *)pData;
			m_pIAttemperEngineSink->OnEventControl(pControlEvent->wControlID, pControlEvent + 1, wDataSize - sizeof(NTY_ControlEvent));
			return true;
		}
		case EVENT_DATABASE:			//数据库事件
		{
			//大小断言
			if (wDataSize < sizeof(NTY_DataBaseEvent)) return false;

			//类型强转
			NTY_DataBaseEvent * pDataBaseEvent = (NTY_DataBaseEvent *)pData;

			//处理请求
			m_pIAttemperEngineSink->OnEventDataBase(pDataBaseEvent->wRequestID, pDataBaseEvent->dwContextID, pDataBaseEvent + 1, wDataSize - sizeof(NTY_DataBaseEvent));

			return true;
		}
		case EVENT_TCP_SOCKET_READ:		//读取事件
		{
			//变量定义
			NTY_TCPSocketReadEvent *pReadEvent = (NTY_TCPSocketReadEvent *)pData;

			//大小断言
			ASSERT(wDataSize >= sizeof(NTY_TCPSocketReadEvent));
			ASSERT(wDataSize == (sizeof(NTY_TCPSocketReadEvent) + pReadEvent->wDataSize));

			//大小断言
			if (wDataSize < sizeof(NTY_TCPSocketReadEvent)) return false;
			if (wDataSize != (sizeof(NTY_TCPSocketReadEvent) + pReadEvent->wDataSize)) return false;

			//处理消息
			m_pIAttemperEngineSink->OnEventTCPSocketRead(pReadEvent->wServiceID, pReadEvent->Command, pReadEvent + 1, pReadEvent->wDataSize);

			return true;
		}
		case EVENT_TCP_SOCKET_SHUT:		//关闭事件
		{
			//大小断言
			ASSERT(wDataSize == sizeof(NTY_TCPSocketShutEvent));
			if (wDataSize != sizeof(NTY_TCPSocketShutEvent)) return false;

			//处理消息
			NTY_TCPSocketShutEvent *pShutEvent = (NTY_TCPSocketShutEvent *)pData;
			m_pIAttemperEngineSink->OnEventTCPSocketShut(pShutEvent->wServiceID, pShutEvent->cbShutReason);

			return true;
		}
		case EVENT_TCP_SOCKET_LINK:		//连接事件
		{
			//大小断言
			ASSERT(wDataSize == sizeof(NTY_TCPSocketLinkEvent));
			if (wDataSize != sizeof(NTY_TCPSocketLinkEvent)) return false;

			//处理消息
			NTY_TCPSocketLinkEvent *pLinkEvent = (NTY_TCPSocketLinkEvent *)pData;
			m_pIAttemperEngineSink->OnEventTCPSocketLink(pLinkEvent->wServiceID, pLinkEvent->nErrorCode);

			return true;
		}
		case EVENT_TCP_NETWORK_ACCEPT:		//应答事件
		{
			//大小断言
			ASSERT(wDataSize == sizeof(NTY_TCPNetworkAcceptEvent));
			if (wDataSize != sizeof(NTY_TCPNetworkAcceptEvent)) return false;

			//变量定义
			bool bSuccess = false;
			NTY_TCPNetworkAcceptEvent *pAcceptEvent = (NTY_TCPNetworkAcceptEvent *)pData;

			//处理消息
			try
			{
				bSuccess = m_pIAttemperEngineSink->OnEventTCPNetworkBind(pAcceptEvent->dwClientAddr, pAcceptEvent->dwSocketID);
			}
			catch (...) {}
			if (bSuccess == false)
			{
				m_pITCPNetworkEngine->CloseSocket(pAcceptEvent->dwSocketID);
			}

			return true;
		}
		case EVENT_TCP_NETWORK_READ:		//读取事件
		{
			//效验大小
			NTY_TCPNetworkReadEvent *pReadEvent = (NTY_TCPNetworkReadEvent *)pData;

			//大小效验
			if (wDataSize < sizeof(NTY_TCPNetworkReadEvent)) return false;

			//大小效验
			if (wDataSize != (sizeof(NTY_TCPNetworkReadEvent) + pReadEvent->wDataSize)) return false;

			//处理消息
			bool bSuccess = false;
			try
			{
				bSuccess = m_pIAttemperEngineSink->OnEventTCPNetworkRead(pReadEvent->Command, pReadEvent + 1, pReadEvent->wDataSize, pReadEvent->dwSocketID);
			}
			catch (...) {}
			if (bSuccess == false)
			{
				m_pITCPNetworkEngine->CloseSocket(pReadEvent->dwSocketID);
			}
			return true;
		}
		case EVENT_TCP_NETWORK_SHUT:		//关闭事件
		{
			//大小断言
			ASSERT(wDataSize == sizeof(NTY_TCPNetworkShutEvent));
			if (wDataSize != sizeof(NTY_TCPNetworkShutEvent)) return false;

			//处理消息
			NTY_TCPNetworkShutEvent *pShutEvent = (NTY_TCPNetworkShutEvent *)pData;
			m_pIAttemperEngineSink->OnEventTCPNetworkShut(pShutEvent->dwClientAddr, pShutEvent->dwActiveTime, pShutEvent->dwSocketID, pShutEvent->bClientQuit);

			return true;
		}
	}

	//其他消息
	return  m_pIAttemperEngineSink->OnEventAttemperData(wIdentifier, pData, wDataSize);
}

/////////////////////////////////////////////

//组建创建函数
DECLARE_CREATE_MODULE(AttemperEngine);

/////////////////////////////////////////////////////
