#ifndef BUTTON_RPC_SERVER_SINK_HEAD_FILE
#define BUTTON_RPC_SERVER_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "ServerUserManager.h"
#include <time.h> 
#include <vector>
#include <queue>
using namespace std;

//////////////////////////////////////////////////////////////////////////////////

//调度钩子
class CRpcServerSink : public IRpcServerEngineSink
{
	//函数定义
public:
	//构造函数
	CRpcServerSink();
	//析构函数
	virtual ~CRpcServerSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//系统事件
public:
	//启动事件
	virtual bool OnDataBaseEngineStart(IUnknownEx * pIUnknownEx);
	//停止事件
	virtual bool OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx);

	//内核事件
public:
	//请求事件
	virtual bool OnDataBaseEngineRequest(VOID * pData, WORD wDataSize);

	//线程函数
public:
	void OnThreadFunction();


	//回调函数
public:
	void foo_1();

	//变量定义
public:
	CInitParameter *				m_pInitParameter;					//配置参数

	//状态变量
public:
	BOOL							m_bRuning;							//运行标识

	//引擎组件
public:
	ITimerEngine *					m_pITimerEngine;					//时间引擎
	IDataBaseEngine *				m_pIDataBaseEngine;					//数据引擎
	ITCPSocketService *				m_pITCPSocketService;				//协调服务
	IRpcServerEngine *				m_pIRpcServerEngine;				//调度引擎
	
	//用户管理
public:
	CServerUserManager				m_ServerUserManager;				//用户管理

	//安全队列
private:
	std::thread*					m_workerThreads;					//工作线程
	CThreadSafeQueue<StreamBuffer>  m_messageQueue;						//安全队里
};

//////////////////////////////////////////////////////////////////////////////////

#endif