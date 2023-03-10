﻿#ifndef TIMER_ENGINE_HEAD_FILE
#define TIMER_ENGINE_HEAD_FILE

#pragma once

#include "KernelEngineHead.h"

//类说明
class CTimerEngine;

//////////////////////////////////////////////////////////////////////////

//定时器线程
class CTimerThread : public CWHThread
{
	//变量定义
protected:
	DWORD								m_dwTimerSpace;					//时间间隔
	DWORD								m_dwLastTickCount;				//处理时间

	//接口变量
protected:
	CTimerEngine *						m_pTimerEngine;					//定时器引擎

	//函数定义
public:
	//构造函数
	CTimerThread(void);
	//析构函数
	virtual ~CTimerThread(void);

	//功能函数
public:
	//配置函数
	bool InitThread(CTimerEngine * pTimerEngine, DWORD dwTimeSpace);

	//重载函数
private:
	//运行函数
	virtual bool OnEventThreadRun();
};

//////////////////////////////////////////////////////////////////////////

//定时器子项
struct tagTimerItem
{
	DWORD								dwElapse;						//倒数时间
	DWORD								wTimerID;						//时间标识
	DWORD								dwTimeLeave;					//剩余时间
	DWORD								dwRepeatTimes;					//重复次数
	WPARAM								wBindParameter;					//绑定参数
};

//////////////////////////////////////////////////////////////////////////

//数组说明
typedef CWHArray<tagTimerItem *> CTimerItemArray;						//子项数组

//定时器引擎
class CTimerEngine : public ITimerEngine
{
	friend class CTimerThread;
	friend class CDataBase;

	//状态变量
protected:
	bool								m_bService;						//运行标志
	DWORD								m_dwTimePass;					//经过时间
	DWORD								m_dwTimeLeave;					//倒计时间

	//配置定义
protected:
	DWORD								m_dwTimerSpace;					//时间间隔
	ITimerEngineEvent *					m_pITimerEngineEvent;			//事件接口

	//组件变量
protected:
	CTimerThread						m_TimerThread;					//线程对象
	CTimerItemArray						m_TimerItemFree;				//空闲数组
	CTimerItemArray						m_TimerItemActive;				//活动数组
	CCriticalSection					m_TimerItemSection; 			//锁定对象

	//函数定义
public:
	//构造函数
	CTimerEngine(void);
	//析构函数
	virtual ~CTimerEngine(void);

	//基础接口
public:
	//释放对象
	virtual VOID  Release() { delete this; }
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
	//设置接口
	virtual bool SetTimerEngineEvent(IUnknownEx * pIUnknownEx);

	//接口函数
public:
	//设置定时器
	virtual bool SetTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter);
	//删除定时器
	virtual bool KillTimer(DWORD dwTimerID);
	//删除定时器
	virtual bool KillAllTimer();

	//内部函数
private:
	//定时器通知
	void OnTimerThreadSink();
};

//////////////////////////////////////////////////////////////////////////

#endif