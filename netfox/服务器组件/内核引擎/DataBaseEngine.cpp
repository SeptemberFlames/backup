#include "Stdafx.h"
#include "DataBaseEngine.h"
#include "TraceServiceManager.h"
#include "TraceService.h"
#include "ServerPushLog.h"
//////////////////////////////////////////////////////////////////////////

//时间定义
#define IDI_CHECK_ENGINE_SINK			1						//引擎检测

//////////////////////////////////////////////////////////////////////////

//构造函数
CDataBaseThread::CDataBaseThread()
{
	//运行变量
	m_hCompletionPort = NULL;
	m_pIDataBaseEngineSink = NULL;

	//指针变量
	m_pDataBaseEngine = NULL;
	m_pNextDataBaseThread = NULL;

	//辅助变量
	ZeroMemory(m_cbBuffer, sizeof(m_cbBuffer));

	return;
}

//析构函数
CDataBaseThread::~CDataBaseThread()
{
}

//运行函数
bool CDataBaseThread::OnEventThreadRun()
{
	//校验参数
	ASSERT(m_hCompletionPort != NULL);
	ASSERT(m_pIDataBaseEngineSink != NULL);

	//变量定义
	DWORD dwThancferred = 0;
	OVERLAPPED * pOverLapped = NULL;
	CDataBaseEngine * pDataBaseEngine = NULL;

	//完成端口
	if (GetQueuedCompletionStatus(m_hCompletionPort, &dwThancferred, (PULONG_PTR)&pDataBaseEngine, &pOverLapped, INFINITE))
	{
		//退出判断
		if (pDataBaseEngine == NULL)
		{
			CPushKernelLog(KernelEngineLog, LOG_ERROR, TEXT("完成端口pDataBaseEngine == NULL"));
			return false;
		}

		//队列锁定
		CWHDataLocker DataLocker(m_DataBaseThreadSection);

		//提取数据
		tagDataHead DataHead;
		m_DataQueue.DistillData(DataHead, m_cbBuffer, sizeof(m_cbBuffer));

		//队列解锁
		DataLocker.UnLock();

		//数据处理
		bool bSuccess = false;

		try
		{
			bSuccess = OnDataBaseEngineData(DataHead.wIdentifier, m_cbBuffer, DataHead.wDataSize);
		}
		catch (...)
		{
			CPushKernelLog(KernelEngineLog, LOG_ERROR, TEXT("完成端口数据库执行异常"));
		}

		//处理结果
		pDataBaseEngine->OnThreadResult(this, bSuccess);

		return true;
	}

	return false;
}

//开始事件
bool CDataBaseThread::OnEventThreadStrat()
{
	//完成端口
	ASSERT(m_hCompletionPort == NULL);
	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

	//事件通知
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(m_pDataBaseEngine, IUnknownEx) != NULL);
	bool bSuccess = m_pIDataBaseEngineSink->OnDataBaseEngineStart(QUERY_OBJECT_PTR_INTERFACE(m_pDataBaseEngine, IUnknownEx));

	return bSuccess;
}

//停止事件
bool CDataBaseThread::OnEventThreadConclude()
{
	//关闭端口
	if (m_hCompletionPort != NULL)
	{
		CloseHandle(m_hCompletionPort);
		m_hCompletionPort = NULL;
	}

	//事件通知
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(m_pDataBaseEngine, IUnknownEx) != NULL);
	bool bSuccess = m_pIDataBaseEngineSink->OnDataBaseEngineConclude(QUERY_OBJECT_PTR_INTERFACE(m_pDataBaseEngine, IUnknownEx));

	return bSuccess;
}

//异步数据
bool CDataBaseThread::OnDataBaseEngineData(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	switch (wIdentifier)
	{
		case EVENT_TIMER:		//时间事件
			{
				//校验参数
				ASSERT(wDataSize == sizeof(NTY_TimerEvent));
				if (wDataSize != sizeof(NTY_TimerEvent)) return false;

				//变量定义
				NTY_TimerEvent * pTimerEvent = (NTY_TimerEvent *)pData;

				//处理数据
				try
				{
					ASSERT(m_pIDataBaseEngineSink != NULL);
					return m_pIDataBaseEngineSink->OnDataBaseEngineTimer(pTimerEvent->dwTimerID, pTimerEvent->dwBindParameter);
				}
				catch (...)
				{
					ASSERT(FALSE);
					return false;
				}

				return true;
			}
		case EVENT_CONTROL:			//控制事件
			{
				//校验参数
				ASSERT(wDataSize >= sizeof(NTY_ControlEvent));
				if (wDataSize < sizeof(NTY_ControlEvent)) return false;

				//变量定义
				NTY_ControlEvent * pControlEvent = (NTY_ControlEvent *)pData;

				//处理数据
				try
				{
					ASSERT(m_pIDataBaseEngineSink != NULL);
					m_pIDataBaseEngineSink->OnDataBaseEngineControl(pControlEvent->wControlID, pControlEvent + 1, wDataSize - sizeof(NTY_ControlEvent));
				}
				catch (...)
				{
					ASSERT(FALSE);
					return false;
				}

				return true;
			}
		case EVENT_DATABASE:		//数据事件
			{
				//校验参数
				ASSERT(wDataSize >= sizeof(NTY_DataBaseEvent));
				if (wDataSize < sizeof(NTY_DataBaseEvent)) return false;

				//变量定义
				NTY_DataBaseEvent * pDataBaseEvent = (NTY_DataBaseEvent *)pData;

				//处理数据
				try
				{
					bool DataBaseRet = m_pIDataBaseEngineSink->OnDataBaseEngineRequest(pDataBaseEvent->wRequestID, pDataBaseEvent->dwContextID, pDataBaseEvent + 1, wDataSize - sizeof(NTY_DataBaseEvent));
					if (DataBaseRet == false)
					{
						//失败日志
						CString subGameLog;
						subGameLog.Format(TEXT("执行数据库请求失败, 标志:%d"), pDataBaseEvent->wRequestID);
						CTraceService::TraceString(subGameLog, TraceLevel_Exception);
					}
					return DataBaseRet;
				}
				catch (...)
				{
					//异常日志
					CString subGameLog;
					subGameLog.Format(TEXT("执行数据库请求异常, 标志:%d"), pDataBaseEvent->wRequestID);
					CTraceService::TraceString(subGameLog, TraceLevel_Exception);
					return false;
				}

				return true;
			}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CDataBaseEngine::CDataBaseEngine()
{
	//状态变量
	m_bService = false;
	m_bDeferData = false;

	//设置变量
	m_pHeadThreadIdle = NULL;
	m_pHeadThreadActive = NULL;

	//辅助变量
	ZeroMemory(m_cbBuffer, sizeof(m_cbBuffer));

	//设置接口
	m_TimerEngine.SetTimerEngineEvent(QUERY_ME_INTERFACE(ITimerEngineEvent));

	return;
}

//析构函数
CDataBaseEngine::~CDataBaseEngine()
{
	//终止服务
	ConcludeService();

	return;
}

//接口查询
VOID * CDataBaseEngine::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IServiceModule, Guid, dwQueryVer);
	QUERYINTERFACE(IDataBaseEngine, Guid, dwQueryVer);
	QUERYINTERFACE(ITimerEngineEvent, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngine, Guid, dwQueryVer);
	return NULL;
}

//启动服务
bool CDataBaseEngine::StartService()
{
	//校验状态
	ASSERT((m_pHeadThreadIdle != NULL) && (m_pHeadThreadActive == NULL));
	if ((m_pHeadThreadIdle == NULL) || (m_pHeadThreadActive != NULL)) return false;

	//状态变量
	m_bService = true;
	m_bDeferData = false;

	//变量定义
	CDataBaseThread * pDataBaseThread = m_pHeadThreadIdle;

	//启动线程
	while (pDataBaseThread != NULL)
	{
		//启动线程
		if (pDataBaseThread->StartThread() == false)
		{
			ASSERT(FALSE);
			return false;
		}

		//设置指针
		pDataBaseThread = pDataBaseThread->m_pNextDataBaseThread;
	}

	//启动引擎
	if (m_TimerEngine.StartService() == false) return false;

	//设置时间
	m_TimerEngine.SetTimer(IDI_CHECK_ENGINE_SINK, 60000L, TIMES_INFINITY, 0L);

	return true;
}

//停止服务
bool CDataBaseEngine::ConcludeService()
{
	//状态变量
	m_bService = false;
	m_bDeferData = false;

	//停止组件
	m_TimerEngine.ConcludeService();

	//删除对象
	while (m_pHeadThreadIdle != NULL)
	{
		//获取对象
		CDataBaseThread * pDataBaseThread = m_pHeadThreadIdle;
		m_pHeadThreadIdle = pDataBaseThread->m_pNextDataBaseThread;

		//投递关闭
		PostQueuedCompletionStatus(pDataBaseThread->m_hCompletionPort, 0, 0, NULL);

		//停止线程
		pDataBaseThread->ConcludeThread(INFINITE);

		//删除对象
		SafeDelete(pDataBaseThread);
	}

	//设置变量
	m_pHeadThreadIdle = NULL;
	m_pHeadThreadActive = NULL;

	return true;
}

//引擎负荷
bool CDataBaseEngine::GetBurthenInfo(tagBurthenInfo & BurthenInfo)
{
	//队列锁定
	CWHDataLocker DataLocker(m_DataBaseEngineSection);

	//获取负荷
	m_DataQueue.GetBurthenInfo(BurthenInfo);

	return true;
}

//配置模块
bool CDataBaseEngine::SetDataBaseEngineSink(IUnknownEx * pIUnknownEx)
{
	//校验状态
	ASSERT((m_pHeadThreadIdle == NULL) && (m_pHeadThreadActive == NULL));
	if ((m_pHeadThreadIdle != NULL) || (m_pHeadThreadActive != NULL)) return false;

	//查询接口
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IDataBaseEngineSink) != NULL);
	IDataBaseEngineSink * pIDataBaseEngineSink = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IDataBaseEngineSink);

	//错误判断
	if (pIDataBaseEngineSink == NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	//创建对象
	try
	{
		//创建对象
		CDataBaseThread * pDataBaseThread = new CDataBaseThread;
		if (pDataBaseThread == NULL) throw TEXT("创建 CDataBaseThread 对象失败");

		//设置对象
		pDataBaseThread->m_pDataBaseEngine = this;
		pDataBaseThread->m_pNextDataBaseThread = m_pHeadThreadIdle;
		pDataBaseThread->m_pIDataBaseEngineSink = pIDataBaseEngineSink;

		//设置链表
		m_pHeadThreadIdle = pDataBaseThread;

		return true;
	}
	catch (...)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//配置模块
bool CDataBaseEngine::SetDataBaseEngineSink(IUnknownEx * pIUnknownEx[], WORD wSinkCount)
{
	//校验状态
	ASSERT((m_pHeadThreadIdle == NULL) && (m_pHeadThreadActive == NULL));
	if ((m_pHeadThreadIdle != NULL) || (m_pHeadThreadActive != NULL)) return false;

	//清空数据
	m_pDataBaseThreadAry.clear();
	m_wDataBaseThreadCount = 0;

	//设置接口
	for (WORD i = 0; i < wSinkCount; i++)
	{
		//查询接口
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx[i], IDataBaseEngineSink) != NULL);
		IDataBaseEngineSink * pIDataBaseEngineSink = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx[i], IDataBaseEngineSink);

		//错误判断
		if (pIDataBaseEngineSink == NULL)
		{
			ASSERT(FALSE);
			return false;
		}

		//创建对象
		try
		{
			//创建对象
			CDataBaseThread * pDataBaseThread = new CDataBaseThread;
			if (pDataBaseThread == NULL) throw TEXT("创建 CDataBaseThread 对象失败");

			//设置对象
			pDataBaseThread->m_pDataBaseEngine = this;
			pDataBaseThread->m_pNextDataBaseThread = m_pHeadThreadIdle;
			pDataBaseThread->m_pIDataBaseEngineSink = pIDataBaseEngineSink;

			//设置链表
			m_pHeadThreadIdle = pDataBaseThread;

			//保存指针
			m_pDataBaseThreadAry.push_back(pDataBaseThread);
			m_wDataBaseThreadCount++;
		}
		catch (...)
		{
			ASSERT(FALSE);
			return false;
		}
	}

	return true;
}

//控制事件
bool CDataBaseEngine::PostDataBaseControl(WORD wControlID, VOID * pData, WORD wDataSize)
{
	//校验状态
	ASSERT(m_bService == true);
	if (m_bService == false) return false;

	//构造数据
	NTY_ControlEvent ContolEvent;
	ContolEvent.wControlID = wControlID;

	//投递数据
	OnDataBaseEngineData(EVENT_CONTROL, &ContolEvent, sizeof(ContolEvent));

	return true;
}

//处理事件
bool CDataBaseEngine::PostDataBaseRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//校验状态
	if (m_bService == false) return false;

	//校验参数
	ASSERT(wDataSize <= SOCKET_TCP_PACKET);
	if (wDataSize > SOCKET_TCP_PACKET) return false;

	//构造数据
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
		OnDataBaseEngineData(EVENT_DATABASE, DataBuffer, CountArray(DataBuffer));
	}
	else
	{
		//投递数据
		OnDataBaseEngineData(EVENT_DATABASE, &DataBaseEvent, sizeof(DataBaseEvent));
	}

	return true;
}

//处理事件
bool CDataBaseEngine::PostDataBaseRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize, LONGLONG llConnectIndex)
{
	//校验状态
	if (m_bService == false) return false;

	//校验参数
	ASSERT(wDataSize <= SOCKET_TCP_PACKET);
	if (wDataSize > SOCKET_TCP_PACKET) return false;

	//构造数据
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
		OnDataBaseEngineData(EVENT_DATABASE, DataBuffer, CountArray(DataBuffer), llConnectIndex);
	}
	else
	{
		//投递数据
		OnDataBaseEngineData(EVENT_DATABASE, &DataBaseEvent, sizeof(DataBaseEvent), llConnectIndex);
	}

	return true;
}

//时间事件
bool CDataBaseEngine::OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{
	//校验状态
	ASSERT(m_bService == true);
	if (m_bService == false) return false;

	//构造数据
	NTY_TimerEvent TimerEvent;
	TimerEvent.dwTimerID = dwTimerID;
	TimerEvent.dwBindParameter = dwBindParameter;

	//投递数据
	OnDataBaseEngineData(EVENT_TIMER, &TimerEvent, sizeof(TimerEvent));

	return true;
}

//线程结果
VOID CDataBaseEngine::OnThreadResult(CDataBaseThread * pDataBaseThread, bool bSuccess)
{
	//校验状态
	ASSERT(m_bService == true);
	if (m_bService == false) return;

	//锁定变量
	CWHDataLocker DataLocker(m_DataBaseEngineSection);

	//继续处理
	if (m_DataQueue.GetDataPacketCount() == 0L)
	{
		//变量定义
		CDataBaseThread * pLastThread = NULL;
		CDataBaseThread * pIdleThread = m_pHeadThreadIdle;
		CDataBaseThread * pCurrentThread = m_pHeadThreadActive;

		//释放对象
		while (pCurrentThread != NULL)
		{
			//对象判断
			if (pDataBaseThread == pCurrentThread)
			{
				//活动链表
				if (pLastThread == NULL)
				{
					m_pHeadThreadActive = pCurrentThread->m_pNextDataBaseThread;
				}
				else
				{
					pLastThread->m_pNextDataBaseThread = pCurrentThread->m_pNextDataBaseThread;
				}

				//空闲链表
				m_pHeadThreadIdle = pCurrentThread;
				pCurrentThread->m_pNextDataBaseThread = pIdleThread;

				break;
			}
			else
			{
				//校验状态
				ASSERT(pCurrentThread->m_pNextDataBaseThread != NULL);

				//设置变量
				pLastThread = pCurrentThread;
				pCurrentThread = pCurrentThread->m_pNextDataBaseThread;
			}
		}
	}
	else
	{
		//提取数据
		tagDataHead DataHead;
		m_DataQueue.DistillData(DataHead, m_cbBuffer, sizeof(m_cbBuffer));

		//投递数据
		CWHDataLocker DataLocker(pDataBaseThread->m_DataBaseThreadSection);
		pDataBaseThread->m_DataQueue.InsertData(DataHead.wIdentifier, m_cbBuffer, DataHead.wDataSize);

		//发送通知
		PostQueuedCompletionStatus(pDataBaseThread->m_hCompletionPort, 0, (ULONG_PTR)this, NULL);
	}

	return;
}

//投递数据
VOID CDataBaseEngine::OnDataBaseEngineData(WORD wIdentifier, VOID * pBuffer, WORD wDataSize)
{
	//校验状态
	ASSERT(m_bService == true);
	if (m_bService == false) return;

	//锁定变量
	CWHDataLocker DataLocker(m_DataBaseEngineSection);

	//获取线程
	if (m_pHeadThreadIdle != NULL)
	{
		//获取对象
		CDataBaseThread * pCurrentThread = m_pHeadThreadIdle;
		CDataBaseThread * pHeadThreadActive = m_pHeadThreadActive;

		//活动链表
		m_pHeadThreadActive = pCurrentThread;
		pCurrentThread->m_pNextDataBaseThread = pHeadThreadActive;

		//空闲链表
		m_pHeadThreadIdle = m_pHeadThreadIdle->m_pNextDataBaseThread;

		//投递数据
		CWHDataLocker DataLocker(pCurrentThread->m_DataBaseThreadSection);
		pCurrentThread->m_DataQueue.InsertData(wIdentifier, pBuffer, wDataSize);

		//发送通知
		PostQueuedCompletionStatus(pCurrentThread->m_hCompletionPort, 0, (ULONG_PTR)this, NULL);
	}
	else
	{
		//投递数据
		m_DataQueue.InsertData(wIdentifier, pBuffer, wDataSize);
	}

	return;
}

//投递数据
VOID CDataBaseEngine::OnDataBaseEngineData(WORD wIdentifier, VOID * pBuffer, WORD wDataSize, LONGLONG llConnectIndex)
{
	//校验状态
	ASSERT(m_bService == true);
	if (m_bService == false) return;

	//锁定变量
	CWHDataLocker DataLocker(m_DataBaseEngineSection);

	//引擎索引
	WORD wEngineIndex = llConnectIndex % m_wDataBaseThreadCount;

	//获取对象
	CDataBaseThread * pCurrentThread = m_pDataBaseThreadAry[wEngineIndex];
	if (pCurrentThread != NULL) 
	{
		//投递数据
		CWHDataLocker DataLocker(pCurrentThread->m_DataBaseThreadSection);
		pCurrentThread->m_DataQueue.InsertData(wIdentifier, pBuffer, wDataSize);

		//发送通知
		PostQueuedCompletionStatus(pCurrentThread->m_hCompletionPort, 0, (ULONG_PTR)this, NULL);
	}
	return;
}

//投递数据
VOID CDataBaseEngine::OnDataBaseEngineData(WORD wIdentifier, tagDataBuffer DataBuffer[], WORD wDataCount)
{
	//校验状态
	ASSERT(m_bService == true);
	if (m_bService == false) return;

	//锁定变量
	CWHDataLocker DataLocker(m_DataBaseEngineSection);

	//获取线程
	if (m_pHeadThreadIdle != NULL)
	{
		//获取对象
		CDataBaseThread * pCurrentThread = m_pHeadThreadIdle;
		CDataBaseThread * pHeadThreadActive = m_pHeadThreadActive;

		//活动链表
		m_pHeadThreadActive = pCurrentThread;
		pCurrentThread->m_pNextDataBaseThread = pHeadThreadActive;

		//空闲链表
		m_pHeadThreadIdle = m_pHeadThreadIdle->m_pNextDataBaseThread;

		//投递数据
		CWHDataLocker DataLocker(pCurrentThread->m_DataBaseThreadSection);
		pCurrentThread->m_DataQueue.InsertData(wIdentifier, DataBuffer, wDataCount);

		//发送通知
		PostQueuedCompletionStatus(pCurrentThread->m_hCompletionPort, 0, (ULONG_PTR)this, NULL);
	}
	else
	{
		//投递数据
		m_DataQueue.InsertData(wIdentifier, DataBuffer, wDataCount);
	}

	return;
}

//投递数据
VOID CDataBaseEngine::OnDataBaseEngineData(WORD wIdentifier, tagDataBuffer DataBuffer[], WORD wDataCount, LONGLONG llConnectIndex)
{
	//校验状态
	ASSERT(m_bService == true);
	if (m_bService == false) return;

	//锁定变量
	CWHDataLocker DataLocker(m_DataBaseEngineSection);

	//引擎索引
	WORD wEngineIndex = llConnectIndex % m_wDataBaseThreadCount;

	//获取对象
	CDataBaseThread * pCurrentThread = m_pDataBaseThreadAry[wEngineIndex];
	if (pCurrentThread != NULL)
	{
		//投递数据
		CWHDataLocker DataLocker(pCurrentThread->m_DataBaseThreadSection);
		pCurrentThread->m_DataQueue.InsertData(wIdentifier, DataBuffer, wDataCount);

		//发送通知
		PostQueuedCompletionStatus(pCurrentThread->m_hCompletionPort, 0, (ULONG_PTR)this, NULL);
	}
	return;
}

//////////////////////////////////////////////////////////////////////////

//组件创建函数
DECLARE_CREATE_MODULE(DataBaseEngine);

//////////////////////////////////////////////////////////////////////////