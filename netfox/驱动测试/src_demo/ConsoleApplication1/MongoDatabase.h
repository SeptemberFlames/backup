#pragma once

#include "stdafx.h"
#include <mongoc.h>

class CMongoDatabase
{
	//构造函数
public:
	//构造函数
	CMongoDatabase(const char *uristr);
	//析构函数
	virtual ~CMongoDatabase();

	//功能函数
public:
	//插入操作
	bool InsertDoc(const char*db, const char*tb, const bson_t *doc);
	
	//查询操作
	bool SearchDoc(const char*db, const char*tb, const bson_t *cond, mongoc_cursor_t *&cursor, unsigned int skip, unsigned int limit, unsigned int batch_size, const bson_t *fields);
	
	//单纯更新操作
	bool UpdateOnlyDoc(const char*db, const char*tb, const bson_t *cond, const bson_t *updatedoc);

	//更新插入操作
	bool UpsertInsertDoc(const char*db, const char*tb, const bson_t *cond, const bson_t *updatedoc);

	//删除单个操作
	bool DeleteSingelDoc(const char*db, const char*tb, const bson_t *cond);

	//删除所有操作
	bool DeleteTotalDoc(const char*db, const char*tb, const bson_t *cond);

	//计数请求
	bool Count(const char*db, const char*tb, const bson_t *cond, long long &count);

	//命令函数
public:
	//执行命令
	bool Command(const char*db, const char*coll, const bson_t *command, bson_t &reply);

	//转换函数
public:
	//双字节转单字节
	void UnicodeToUtf8(wchar_t* wszString, char *utf);
	//单字节转双字节
	void Utf8ToUnicode(char *utf, wchar_t* wszString);

	//测试接口
public:
	//测试插入
	bool TestInsertDoc();
	//测试查找
	bool TestSearchDoc();
	//测试删除
	bool TestDeleteDoc();
	//测试更新
	bool testUpdateDoc();

	//成员变量
private:
	//操作指针
	mongoc_client_t	*	m_pMongocClient;
};