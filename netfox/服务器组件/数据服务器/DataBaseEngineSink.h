#ifndef DATABASE_ENGINE_SINK_HEAD_FILE
#define DATABASE_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "DataBasePacket.h"

//////////////////////////////////////////////////////////////////////////////////

//连接结构
struct stuDataBaseSetting
{
	TCHAR szDBAddr[128];
	WORD wPort;
	TCHAR szUser[128];
	TCHAR szPassword[128];
};

//数据库类
class CDataBaseEngineSink : public IDataBaseEngineSink
{
	//友元定义
	friend class CServiceUnits;

	//主库连接
protected:
	CDataBaseAide					m_AccountsDBAide;					//用户数据库
	CDataBaseHelper					m_AccountsDBModule;					//用户数据库

	CDataBaseAide					m_TreasureDBAide;					//主库游戏币数据
	CDataBaseHelper					m_TreasureDBModule;					//主库游戏币数据

	CDataBaseAide					m_PlatformDBAide;					//主库平台数据
	CDataBaseHelper					m_PlatformDBModule;					//主库平台数据

	//从库连接
protected:
	CDataBaseAide					m_GameDBSalveAide;					//从库游戏数据
	CDataBaseHelper					m_GameDBSlaveModule;				//从库游戏数据

	CDataBaseAide					m_TreasureDBSlaveAide;				//从库游戏币数据
	CDataBaseHelper					m_TreasureDBSlaveModule;			//从库游戏币数据

	//组件变量
protected:
	CInitParameter *				m_pInitParameter;					//配置参数
	tagDataBaseParameter *			m_pDataBaseParameter;				//连接信息
	IDataBaseEngineEvent *			m_pIDataBaseEngineEvent;			//数据事件

	//函数定义
public:
	//构造函数
	CDataBaseEngineSink();
	//析构函数
	virtual ~CDataBaseEngineSink();

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
	//时间事件
	virtual bool OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//控制事件
	virtual bool OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize);
	//请求事件
	virtual bool OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);	

	//功能函数
public:
	//同步数据
	bool OnRequestSaveUserData(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD & dwUserID);
};

//////////////////////////////////////////////////////////////////////////////////

#endif