#pragma once

#include "stdafx.h"
#include <mongoc.h>

class CMongoDatabase
{
	//���캯��
public:
	//���캯��
	CMongoDatabase(const char *uristr);
	//��������
	virtual ~CMongoDatabase();

	//���ܺ���
public:
	//�������
	bool InsertDoc(const char*db, const char*tb, const bson_t *doc);
	
	//��ѯ����
	bool SearchDoc(const char*db, const char*tb, const bson_t *cond, mongoc_cursor_t *&cursor, unsigned int skip, unsigned int limit, unsigned int batch_size, const bson_t *fields);
	
	//�������²���
	bool UpdateOnlyDoc(const char*db, const char*tb, const bson_t *cond, const bson_t *updatedoc);

	//���²������
	bool UpsertInsertDoc(const char*db, const char*tb, const bson_t *cond, const bson_t *updatedoc);

	//ɾ����������
	bool DeleteSingelDoc(const char*db, const char*tb, const bson_t *cond);

	//ɾ�����в���
	bool DeleteTotalDoc(const char*db, const char*tb, const bson_t *cond);

	//��������
	bool Count(const char*db, const char*tb, const bson_t *cond, long long &count);

	//�����
public:
	//ִ������
	bool Command(const char*db, const char*coll, const bson_t *command, bson_t &reply);

	//ת������
public:
	//˫�ֽ�ת���ֽ�
	void UnicodeToUtf8(wchar_t* wszString, char *utf);
	//���ֽ�ת˫�ֽ�
	void Utf8ToUnicode(char *utf, wchar_t* wszString);

	//���Խӿ�
public:
	//���Բ���
	bool TestInsertDoc();
	//���Բ���
	bool TestSearchDoc();
	//����ɾ��
	bool TestDeleteDoc();
	//���Ը���
	bool testUpdateDoc();

	//��Ա����
private:
	//����ָ��
	mongoc_client_t	*	m_pMongocClient;
};