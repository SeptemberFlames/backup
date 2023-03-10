#include "StdAfx.h"
#include "AsynchronismEngine.h"
#include "TraceServiceManager.h"
#include "ServerPushLog.h"
//////////////////////////////////////////////////////////////////////

//构造函数
CAsynchronismThread::CAsynchronismThread()
{
	//运行变量
	m_hCompletionPort = NULL;
	m_pIAsynchronismEngineSink = NULL;

	//辅助变量
	ZeroMemory(m_cbBuffer, sizeof(m_cbBuffer));

	return;
}

//析构函数
CAsynchronismThread::~CAsynchronismThread()
{

}

//运行函数
bool CAsynchronismThread::OnEventThreadRun()
{
	//校验
	ASSERT(m_hCompletionPort != NULL);
	ASSERT(m_pIAsynchronismEngineSink != NULL);

	//变量定义
	DWORD dwThancferred = 0;
	OVERLAPPED * pOverLapped = NULL;
	CAsynchronismEngine * pAsynchronismEngine = NULL;

	//完成端口
	if (GetQueuedCompletionStatus(m_hCompletionPort, &dwThancferred, (PULONG_PTR)&pAsynchronismEngine, &pOverLapped, INFINITE))
	{
		//推出判断
		if (pAsynchronismEngine == NULL)return false;

		//队列算定
		CWHDataLocker DataLocker(pAsynchronismEngine->m_AsynchronismSection);

		//提取数据
		tagDataHead DataHead;
		pAsynchronismEngine->m_DataQueue.DistillData(DataHead, m_cbBuffer, sizeof(m_cbBuffer));

		//队列解锁
		DataLocker.UnLock();

		//数据处理
		try
		{
			m_pIAsynchronismEngineSink->OnAsynchronismEngineData(DataHead.wIdentifier, m_cbBuffer, DataHead.wDataSize);
		}
		catch (...)
		{

		}

		return true;
	}

	return false;
}

//开始事件
bool CAsynchronismThread::OnEventThreadStrat()
{
	//完成端口
	ASSERT(m_hCompletionPort == NULL);
	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

	//事件通知
	ASSERT(m_pIAsynchronismEngineSink != NULL);
	bool bSuccess = m_pIAsynchronismEngineSink->OnAsynchronismEngineStart();

	return bSuccess;
}

//停止事件
bool CAsynchronismThread::OnEventThreadConclude()
{
	//关闭端口
	if (m_hCompletionPort != NULL)
	{
		CloseHandle(m_hCompletionPort);
		m_hCompletionPort = NULL;
	}

	//事件通知
	ASSERT(m_pIAsynchronismEngineSink != NULL);
	bool bSuccess = m_pIAsynchronismEngineSink->OnAsynchronismEngineConclude();

	return bSuccess;
}

/////////////////////////////////////

//构造函数
CAsynchronismEngine::CAsynchronismEngine()
{
}

//析构函数
CAsynchronismEngine::~CAsynchronismEngine()
{
	//终止服务
	ConcludeService();

	return;
}

//接口查询
void * CAsynchronismEngine::QueryInterface(REFGUID  Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IServiceModule, Guid, dwQueryVer);
	QUERYINTERFACE(IAsynchronismEngine, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAsynchronismEngine, Guid, dwQueryVer);
	return NULL;
}

//启动服务
bool CAsynchronismEngine::StartService()
{
	//运行判断
	ASSERT(m_AsynchronismThread.IsRuning() == false);
	if (m_AsynchronismThread.IsRuning() == true)return false;

	//启动线程
	if (m_AsynchronismThread.StartThread() == false)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//停止服务
bool CAsynchronismEngine::ConcludeService()
{
	//投递推出
	HANDLE hCompletionPort = m_AsynchronismThread.m_hCompletionPort;
	if (hCompletionPort != NULL) PostQueuedCompletionStatus(hCompletionPort, 0, NULL, NULL);

	//停止线程
	if (m_AsynchronismThread.IsRuning() == true)m_AsynchronismThread.ConcludeThread(INFINITE);

	//队列锁定
	CWHDataLocker DataLocker(m_AsynchronismSection);

	//设置队列
	m_DataQueue.RemoveData(false);

	return true;
}

//队列负荷
bool CAsynchronismEngine::GetBurthenInfo(tagBurthenInfo & BurthenInfo)
{
	//队列锁定
	CWHDataLocker DataLocker(m_AsynchronismSection);

	//获取负荷
	m_DataQueue.GetBurthenInfo(BurthenInfo);

	return true;
}

//设置模块
bool CAsynchronismEngine::SetAsynchronismSink(IUnknownEx * pIUnknownEx)
{
	//运行判断
	ASSERT(m_AsynchronismThread.IsRuning() == false);

	//设置对象
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IAsynchronismEngineSink) != NULL);
	m_AsynchronismThread.m_pIAsynchronismEngineSink = QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx, IAsynchronismEngineSink);

	return true;
}

//异步数据
bool CAsynchronismEngine::PostAsynchronismData(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	//运行判断
	if (m_AsynchronismThread.IsRuning() == false) return false;

	//插入数据
	CWHDataLocker DataLocker(m_AsynchronismSection);
	if (m_DataQueue.InsertData(wIdentifier, pData, wDataSize) == false)
	{
		return false;
	}

	//投递消息
	PostQueuedCompletionStatus(m_AsynchronismThread.m_hCompletionPort, 0, (ULONG_PTR)this, NULL);

	return true;
}

//异步数据
bool CAsynchronismEngine::PostAsynchronismData(WORD wIdentifier, tagDataBuffer DataBuffer[], WORD wDataCount)
{
	//运行判断
	if (m_AsynchronismThread.IsRuning() == false)return false;

	//互斥加锁
	CWHDataLocker DataLocker(m_AsynchronismSection);

	//插入数据
	if (m_DataQueue.InsertData(wIdentifier, DataBuffer, wDataCount) == false)
	{
		return false;
	}

	//投递消息
	PostQueuedCompletionStatus(m_AsynchronismThread.m_hCompletionPort, 0, (ULONG_PTR)this, NULL);

	return true;
}

////////////////////////////////////////////////////////

//组建创建函数
DECLARE_CREATE_MODULE(AsynchronismEngine);

/////////////////////////////////////////////////////