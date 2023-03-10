#ifndef DATA_BASE_ENGINE_HEAD_FILE
#define DATA_BASE_ENGINE_HEAD_FILE

#pragma once

#include "TimerEngine.h"
#include "KernelEngineHead.h"
#include "AsynchronismEngine.h"

//////////////////////////////////////////////////////////////////////////

//类说明
class CDataBaseThread;
class CDataBaseEngine;

//////////////////////////////////////////////////////////////////////////

//处理线程
class CDataBaseThread : public CWHThread
{
	//友元定义
	friend class CDataBaseEngine;

	//数据变量
protected:
	CWHDataQueue					m_DataQueue;						//数据队列 
	CCriticalSection				m_DataBaseThreadSection;			//锁定对象

	//运行变量
protected:
	HANDLE							m_hCompletionPort;					//完成端口
	IDataBaseEngineSink *			m_pIDataBaseEngineSink;				//钩子接口

	//指针变量
protected:
	CDataBaseEngine *				m_pDataBaseEngine;					//数据引擎
	CDataBaseThread *				m_pNextDataBaseThread;				//对象指针

	//辅助变量
private:
	BYTE							m_cbBuffer[SOCKET_TCP_BUFFER];		//接收缓冲

	//函数定义
public:
	//构造函数
	CDataBaseThread();
	//析构函数
	virtual ~CDataBaseThread();

	//重载函数
protected:
	//运行函数
	virtual bool OnEventThreadRun();
	//开始事件
	virtual bool OnEventThreadStrat();
	//终止事件
	virtual bool OnEventThreadConclude();

	//数据接口
protected:
	//异步数据
	bool OnDataBaseEngineData(WORD wIdentifier, VOID * pData, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////

//数据库引擎
class CDataBaseEngine : public IDataBaseEngine, public ITimerEngineEvent
{
	//友元定义
	friend class CDataBaseThread;

	//状态变量
protected:
	bool							m_bService;							//服务标志
	bool							m_bDeferData;						//延时标志

	//组件变量
protected:
	CWHDataQueue					m_DataQueue;						//数据队列
	CTimerEngine					m_TimerEngine;						//时间引擎
	CCriticalSection				m_DataBaseEngineSection;			//锁定对象

	//指针变量
protected:
	CDataBaseThread *				m_pHeadThreadIdle;					//线程指针
	CDataBaseThread *				m_pHeadThreadActive;				//线程指针

	//指针数组
protected:
	std::vector<CDataBaseThread *>	m_pDataBaseThreadAry;				//指针数组
	WORD							m_wDataBaseThreadCount;				//指针数量

	//辅助变量
private:
	BYTE							m_cbBuffer[SOCKET_TCP_BUFFER];		//接收缓冲

	//函数定义
public:
	//构造函数
	CDataBaseEngine();
	//析构函数
	virtual ~CDataBaseEngine();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//服务接口
public:
	//启动服务
	virtual bool StartService();
	//停止服务
	virtual bool ConcludeService();

	//信息接口
public:
	//引擎负荷
	virtual bool GetBurthenInfo(tagBurthenInfo & BurthenInfo);

	//配置接口
public:
	//配置模块
	virtual bool SetDataBaseEngineSink(IUnknownEx * pIUnknownEx);
	//配置模块
	virtual bool SetDataBaseEngineSink(IUnknownEx * pIUnknownEx[], WORD wSinkCount);

	//请求控制
public:
	//控制事件
	virtual bool PostDataBaseControl(WORD wControlID, VOID * pData, WORD wDataSize);
	//请求事件
	virtual bool PostDataBaseRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//请求事件
	virtual bool PostDataBaseRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize, LONGLONG llConnectIndex);

	//时间接口
public:
	//时间事件
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter);

	//辅助函数
protected:
	//线程结果
	VOID OnThreadResult(CDataBaseThread * pDataBaseThread, bool bSuccess);
	//投递数据
	VOID OnDataBaseEngineData(WORD wIdentifier, VOID * pBuffer, WORD wDataSize);
	//投递数据
	VOID OnDataBaseEngineData(WORD wIdentifier, VOID * pBuffer, WORD wDataSize, LONGLONG llConnectIndex);
	//投递数据
	VOID OnDataBaseEngineData(WORD wIdentifier, tagDataBuffer DataBuffer[], WORD wDataCount);
	//投递数据
	VOID OnDataBaseEngineData(WORD wIdentifier, tagDataBuffer DataBuffer[], WORD wDataCount, LONGLONG llConnectIndex);
};

//////////////////////////////////////////////////////////////////////////

#endif