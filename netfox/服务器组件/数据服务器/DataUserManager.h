#ifndef DATA_USER_MANAGER_HEAD_FILE
#define DATA_USER_MANAGER_HEAD_FILE

#pragma once

#include "stdafx.h"
#include "AfxTempl.h"
#include <vector>
#include <map>

#include "DataBasePacket.h"
#include "SpinMutex.h"

//////////////////////////////////////////////////////////////////////////////////

//执行结构
struct UserSql
{
	DWORD							dataBase;							//数据库名
	CString							sql;								//执行语句
};

typedef map<CString, DWORD>	CKeyMap;									//索引映射
typedef map<DWORD, UserSql>	CUserSql;									//用户语句
typedef map<DWORD, tagCacheUserInfo> CUserMap;							//用户映射
typedef map<DWORD, CSpinMutex *> CSpinMutexMap;							//用户同步

//用户管理类
class CDataUserManager
{
	//函数定义
public:
	//构造函数
	CDataUserManager();
	//析构函数
	virtual ~CDataUserManager();

	//用户数组
public:
	CKeyMap							m_UserKeyId;						//索引映射
	CUserMap						m_UserInfoMap;						//用户映射

	//数据接口
public:
	IDataBaseEngine *				m_pIDataBaseEngine;					//数据引擎

	//设置接口
public:
	void SetDataBaseEngine(IDataBaseEngine * pIDataBaseEngine);
	
	//定时函数
public:
	//删除离线
	void DelOfflineUser(DWORD dwUserID);

	//功能函数
public:
	//查找用户
	DWORD GetUserID(CString szUserKey);
	//删除用户
	DWORD DelUserID(CString szUserKey);

	//日志函数
public:
	//推送日志
	void PushUserSql(enCacheDataBase database, enCacheSqlType sqlType, CString sql);

	//功能函数
public:
	//查找用户
	tagCacheUserInfo GetUserInfo(DWORD dwUserID);
	//增加用户
	bool AddUserInfo(DWORD dwUserID, tagCacheUserInfo userInfo, enCacheDataBase database = enCacheDataBase::DefaultDB, enCacheSqlType sqlType = enCacheSqlType::Default, CString sql = TEXT(""));
	//删除用户
	bool DelUserInfo(DWORD dwUserID, enCacheDataBase database = enCacheDataBase::DefaultDB, enCacheSqlType sqlType = enCacheSqlType::Default, CString sql = TEXT(""));
	//修改用户
	bool ModifyUserInfo(DWORD dwUserID, std::string userJson, enCacheDataBase database = enCacheDataBase::DefaultDB, enCacheSqlType sqlType = enCacheSqlType::Default, CString sql = TEXT(""));
};

//////////////////////////////////////////////////////////////////////////////////

#endif