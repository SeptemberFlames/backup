#ifndef ASYNCHRONISM_ENGINE_HEAD_FILE
#define ASYNCHRONISM_ENGINE_HEAD_FILE

#pragma once

#include "KernelEngineHead.h"

//////////////////////////////////////////////////////////////////////////

//数据线程
class CAsynchronismThread : public CWHThread
{
	//友元定义
	friend class CAsynchronismEngine;

	//运行变量
protected:
	HANDLE							m_hCompletionPort;					//完成端口
	IAsynchronismEngineSink *		m_pIAsynchronismEngineSink;			//回调接口

	//辅助变量
private:
	BYTE							m_cbBuffer[SOCKET_TCP_BUFFER];		//接收缓冲 

	//函数定义
public:
	//构造函数
	CAsynchronismThread();
	//析构函数
	virtual ~CAsynchronismThread();

	//重载函数
protected:
	//运行函数
	virtual bool OnEventThreadRun();
	//开始事件
	virtual bool OnEventThreadStrat();
	//终止事件
	virtual bool OnEventThreadConclude();
};

//////////////////////////////////////////////////////////////////////////

//异步引擎
class CAsynchronismEngine : public IAsynchronismEngine
{
	//友元定义
	friend class CAsynchronismThread;

	//组件变量
protected:
	CWHDataQueue					m_DataQueue;					//数据队列
	CCriticalSection				m_AsynchronismSection;			//锁定对象
	CAsynchronismThread				m_AsynchronismThread;			//异步线程

	//函数定义
public:
	//构造函数
	CAsynchronismEngine();
	//析构函数
	virtual ~CAsynchronismEngine();

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

	//配置接口
public:
	//队列负荷
	virtual bool GetBurthenInfo(tagBurthenInfo & BurthenInfo);
	//设置模块
	virtual bool SetAsynchronismSink(IUnknownEx * pIUnknownEx);

	//投递接口
public:
	//投递数据
	virtual bool PostAsynchronismData(WORD wIdentifier, VOID * pData, WORD wDataSize);
	//投递数据
	virtual bool PostAsynchronismData(WORD wIdentifier, tagDataBuffer DataBuffer[], WORD wDataCount);
};

//////////////////////////////////////////////////////////////////////////

#endif