#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "RpcServerSink.h"
#include <fstream>
#include <assert.h>

#define IDI_SAVE_USER_DATA		2									//存储数据

//构造函数
CRpcServerSink::CRpcServerSink()
{
	//初始变量
	m_workerThreads = NULL;
	m_bRuning = false;
}

//析构函数
CRpcServerSink::~CRpcServerSink()
{

}

VOID * CRpcServerSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IRpcServerEngineSink, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IRpcServerEngineSink, Guid, dwQueryVer);
	return NULL;
}

//启动事件
bool CRpcServerSink::OnDataBaseEngineStart(IUnknownEx * pIUnknownEx)
{
	//运行标志
	m_bRuning = true;

	//调度线程
	int concurrency = std::thread::hardware_concurrency();
	if (m_workerThreads == NULL)
	{
		m_workerThreads = new std::thread[concurrency];
		for (int i = 0; i < concurrency; i++)
		{
			m_workerThreads[i] = std::thread(&CRpcServerSink::OnThreadFunction, this);
		}
	}

	//设置时间
	ASSERT(m_pITimerEngine != NULL);
	m_pITimerEngine->SetTimer(IDI_SAVE_USER_DATA, TIME_CELL, TIMES_INFINITY, 0);

	return true;
}

//停止事件
bool CRpcServerSink::OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx)
{
	//运行标志
	m_bRuning = FALSE;

	//组件变量
	m_pITimerEngine = NULL;
	m_pIDataBaseEngine = NULL;
	m_pITCPSocketService = NULL;

	return true;
}

//请求事件
bool CRpcServerSink::OnDataBaseEngineRequest(VOID * pData, WORD wDataSize)
{
	//数据入队
	StreamBuffer iodev((char*)pData, wDataSize);
	m_messageQueue.push(iodev);
	return true;
}

//线程函数
void CRpcServerSink::OnThreadFunction()
{
	while (m_bRuning) 
	{
		if (m_pIRpcServerEngine == NULL)
		{
			//CTraceService::TraceString(TEXT("等待远程调度引擎初始化"), TraceLevel_Exception);
			Sleep(50);
			continue;
		}

		//数据出队
		StreamBuffer iodev = m_messageQueue.wait_and_pop();

		//数据解析
		Serializer ds(iodev);
		std::string funname;
		ds >> funname;

		m_pIRpcServerEngine->call_and_replay(funname, ds.current(), ds.size() - funname.size());
	}
}

//测试函数
void CRpcServerSink::foo_1()
{
	int aa = 0;
}
