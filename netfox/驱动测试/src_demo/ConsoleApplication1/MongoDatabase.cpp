#include "stdafx.h"
#include "MongoDatabase.h"

//���캯��
CMongoDatabase::CMongoDatabase(const char *uristr)
{
	//��ʼ������
	m_pMongocClient = NULL;

	//��ʼ���ӿ�
	mongoc_init();

	//��������ָ��
	m_pMongocClient = mongoc_client_new(uristr); /*���ߵ�db */
	if (m_pMongocClient == NULL)
	{
		fprintf(stderr, "Failed to parse URI.\n");
		return;
	}
	mongoc_client_set_error_api(m_pMongocClient, 2);
}

//��������
CMongoDatabase::~CMongoDatabase()
{
	m_pMongocClient = NULL;
}

/*
(1)��������
bson_oid_t oid;
bson_t *doc;
doc = bson_new ();
bson_oid_init (&oid, NULL);
BSON_APPEND_OID (doc, "_id", &oid);//�ĵ�id
BSON_APPEND_UTF8 (doc, "hello", "world");//hello�ֶ�
��2���������
InsertDoc("mydb", "mycoll", doc)
��3����������
bson_destroy (doc);
*/


//��������
bool CMongoDatabase::InsertDoc(const char*db, const char*tb, const bson_t *doc)
{
	bson_error_t error;
	mongoc_collection_t *collection = mongoc_client_get_collection(m_pMongocClient, db, tb);
	if (collection == NULL)
	{
		return false;
	}

	//MONGOC_INSERT_NONE��ָ���������
	if (!mongoc_collection_insert(collection, MONGOC_INSERT_NONE, doc, NULL, &error))
	{
		char *docStr = bson_as_json(doc, NULL);

		/*
		LOG4CPLUS_ERROR_FMT(GetLogger(),
			"mongoc_collection_insert failed,cond:%s,error: %s\n",
			docStr, error.message);
		*/
		bson_free(docStr);
		mongoc_collection_destroy(collection);
		return false;
	}
	mongoc_collection_destroy(collection);
	return true;
}


/*
(1)��ѯ����
bson_t *cond;
cond = bson_new ();
mongoc_cursor_t *cursor(NULL);

(2)��ѯ,�����α�cursor
SearchDoc("mydb","mycoll",cond,cursor)

(3)��ȡ��ѯ���
const bson_t *doc;
char *str;
while (mongoc_cursor_next (cursor, &doc)) {
str = bson_as_json (doc, NULL);
printf ("%s\n", str);
bson_free (str);
}

(4) ����������
bson_destroy (cond);
mongoc_cursor_destroy (cursor);
*/

//��ѯ����
bool CMongoDatabase::SearchDoc(const char*db, const char*tb, const bson_t *cond, mongoc_cursor_t *&cursor, unsigned int skip, unsigned int limit, unsigned int batch_size, const bson_t *fields)
{
	mongoc_collection_t *collection = mongoc_client_get_collection(m_pMongocClient, db, tb);
	if (collection == NULL)
	{
		return false;
	}

	//MONGOC_QUERY_NONE���ṩ��ѯ���
	cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, skip, limit, batch_size, cond, fields, NULL);
	if (cursor == NULL)
	{
		char *condStr = bson_as_json(cond, NULL);
		/*
		LOG4CPLUS_ERROR_FMT(GetLogger(),
			"SearchDocs mongoc_collection_find failed,cond:%s,error: %s\n",
			condStr, error.message);
		*/
		bson_free(condStr);
		mongoc_collection_destroy(collection);
		return false;
	}
	mongoc_collection_destroy(collection);
	return true;
}


/*
(1)���������͸�������
db.a.update({"_id" : ObjectId("55ef549236fe322f9490e17b")},{"$set":{"key":"new_value","updated":true}})
bson_t *cond = BCON_NEW ("_id", BCON_OID (&oid));//����Ϊid "_id" : ObjectId("55ef549236fe322f9490e17b")
bson_t *updatedoc = BCON_NEW ("$set", "{",
"key", BCON_UTF8 ("new_value"),
"updated", BCON_BOOL (true),
"}");//{"$set"}

(2)�����ĵ�
UpdateDoc("mydb","mycoll",cond, updatedoc)

(3)��������
if (query)
bson_destroy (query);
if (update)
bson_destroy (update);
*/

//�������²���
bool CMongoDatabase::UpdateOnlyDoc(const char*db, const char*tb, const bson_t *cond, const bson_t *updatedoc)
{
	bson_error_t error;
	mongoc_collection_t *collection = mongoc_client_get_collection(m_pMongocClient, db, tb);
	if (collection == NULL)
	{
		return false;
	}

	//MONGOC_UPDATE_MULTI_UPDATE�������з��Ͻ����
	if (!mongoc_collection_update(collection, MONGOC_UPDATE_MULTI_UPDATE, cond, updatedoc, NULL, &error))
	{
		char *condStr = bson_as_json(cond, NULL);
		/*
		LOG4CPLUS_ERROR_FMT(GetLogger(),
			"UpdateDoc mongoc_collection_update failed,cond:%s,error: %s\n",
			condStr, error.message);
		*/
		bson_free(condStr);
		mongoc_collection_destroy(collection);
		return false;
	}
	mongoc_collection_destroy(collection);
	return true;
}


/*
(1)���������͸�����������
bson_t *cond = BCON_NEW ("_id", BCON_OID (&oid));//����Ϊid "_id" : ObjectId("55ef549236fe322f9490e17b")
bson_t *updatedoc = BCON_NEW ("$set", "{",
"key", BCON_UTF8 ("new_value"),
"updated", BCON_BOOL (true),
"}");

(2)�����ĵ�
UpsertDoc("mydb","mycoll",cond,updatedoc)

(3)��������
if (cond)
bson_destroy (cond);
if (updatedoc)
bson_destroy (updatedoc);
*/

//���²������
bool CMongoDatabase::UpsertInsertDoc(const char*db, const char*tb, const bson_t *cond, const bson_t *updatedoc)
{
	bson_error_t error;
	mongoc_collection_t *collection = mongoc_client_get_collection(m_pMongocClient, db, tb);
	if (collection == NULL)
	{
		return false;
	}

	//MONGOC_UPDATE_UPSERT���û�оͲ���,���߾͸���
	if (!mongoc_collection_update(collection, MONGOC_UPDATE_UPSERT, cond, updatedoc, NULL, &error))
	{
		char *condStr = bson_as_json(cond, NULL);
		/*
		LOG4CPLUS_ERROR_FMT(GetLogger(),
			"UpsertDoc mongoc_collection_update failed,cond:%s,error: %s\n",
			condStr, error.message);
		*/
		bson_free(condStr);
		mongoc_collection_destroy(collection);
		return false;
	}
	mongoc_collection_destroy(collection);
	return true;
}



/*
(1)��������
bson_t *cond = bson_new ();
BSON_APPEND_OID (doc, "_id", &oid);

(2)ɾ������
DeleteSingelDoc("mydb","mycoll",cond)

(3)��������
bson_destroy (cond);
*/

//ɾ����һ�������������ĵ�
bool CMongoDatabase::DeleteSingelDoc(const char*db, const char*tb, const bson_t *cond)
{
	bson_error_t error;
	mongoc_collection_t *collection = mongoc_client_get_collection(m_pMongocClient, db, tb);
	if (collection == NULL)
	{
		return false;
	}

	//MONGOC_REMOVE_SINGLE_REMOVEɾ�������ĵ�
	if (!mongoc_collection_remove(collection, MONGOC_REMOVE_SINGLE_REMOVE, cond, NULL, &error))
	{
		char *condStr = bson_as_json(cond, NULL);
		/*
		LOG4CPLUS_ERROR_FMT(GetLogger(), "Delete failed,cond:%s,error: %s\n",
			condStr, error.message);
		*/
		bson_free(condStr);
		mongoc_collection_destroy(collection);
		return false;
	}
	mongoc_collection_destroy(collection);
	return true;
}

/*
(1)��������
bson_t *cond = bson_new ();
BSON_APPEND_OID (doc, "_id", &oid);

(2)ɾ������
DeleteTotalDoc("mydb","mycoll",cond)

(3)��������
bson_destroy (cond);
*/

//ɾ�����з����������ĵ�
bool CMongoDatabase::DeleteTotalDoc(const char*db, const char*tb, const bson_t *cond)
{
	bson_error_t error;
	mongoc_collection_t *collection = mongoc_client_get_collection(m_pMongocClient, db, tb);
	if (collection == NULL)
	{
		return false;
	}

	//MONGOC_REMOVE_NONE��ָ��ɾ������
	if (!mongoc_collection_remove(collection, MONGOC_REMOVE_NONE, cond, NULL, &error))
	{
		char *condStr = bson_as_json(cond, NULL);
		/*
		LOG4CPLUS_ERROR_FMT(GetLogger(), "Delete failed,cond:%s,error: %s\n",
			condStr, error.message);
		*/
		bson_free(condStr);
		mongoc_collection_destroy(collection);
		return false;
	}
	mongoc_collection_destroy(collection);
	return true;
}



/*
(1)��������
bson_t *cond = bson_new_from_json ((const uint8_t *)"{\"hello\" : \"world\"}", -1, &error);//�����������

(2)ִ�м�������
long long count(0);
Count("mydb", "mycoll",cond,count)
*/

//�������󣬶Է����������ĵ������м������ڷ�ҳ��ʾʱ���øò���
bool CMongoDatabase::Count(const char*db, const char*tb, const bson_t *cond, long long &count)
{
	bson_error_t error;
	mongoc_collection_t *collection = mongoc_client_get_collection(m_pMongocClient, db, tb); //ָ�����ݿ�ͼ���
	if (collection == NULL)
	{
		return false;
	}

	//MONGOC_QUERY_NONE��ָ����ѯ����
	count = mongoc_collection_count(collection, MONGOC_QUERY_NONE, cond, 0, 0, NULL, &error);
	if (count < 0)
	{
		char *condStr = bson_as_json(cond, NULL);
		/*
		LOG4CPLUS_ERROR_FMT(GetLogger(),
			"mongoc_collection_count failed,cond:%s,error: %s\n",
			condStr, error.message);
		*/
		bson_free(condStr);
		mongoc_collection_destroy(collection);
		return false;
	}
	mongoc_collection_destroy(collection);
	return true;
}

/*
(1)������������
bson_t *command;
command = BCON_NEW ("collStats", BCON_UTF8 ("mycoll"));

(2)ִ������
bson_t reply;//ִ�������
Command("mydb", "mycoll",command)

(3)��ȡ���ؽ��
char *str = bson_as_json (&reply, NULL);//���ִ�������Ľ��
printf ("%s\n", str);
bson_free (str);

(4)��������
bson_destroy (command);
bson_destroy (&reply);
*/

//ִ��ϵͳ����
bool CMongoDatabase::Command(const char*db, const char*coll, const bson_t *command, bson_t &reply)
{
	bson_error_t error;
	mongoc_collection_t *collection = mongoc_client_get_collection(m_pMongocClient, db, coll);
	if (collection == NULL)
	{
		return false;
	}

	if (!mongoc_collection_command_simple(collection, command, NULL, &reply, &error))
	{
		char *commandStr = bson_as_json(command, NULL);
		/*
		LOG4CPLUS_ERROR_FMT(GetLogger(), "failed to run command:%s,error: %s\n",
			commandStr, error.message);
		*/

		bson_free(commandStr);
		mongoc_collection_destroy(collection);
		return false;
	}
	mongoc_collection_destroy(collection);
	return true;
}

//˫�ֽ�ת���ֽ�
void CMongoDatabase::UnicodeToUtf8(wchar_t* wszString, char *utf)
{
	int ansiiLen = ::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
	::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), utf, ansiiLen, NULL, NULL);
	utf[ansiiLen] = '\0';
}

//���ֽ�ת˫�ֽ�
void CMongoDatabase::Utf8ToUnicode(char *utf, wchar_t* wszString)
{
	int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, utf, -1, nullptr, 0);
	MultiByteToWideChar(CP_UTF8, 0, utf, -1, wszString, unicodeLen);
	wszString[unicodeLen] = '\0';
}


//���Բ���
bool CMongoDatabase::TestInsertDoc()
{
	/*
	//BCON_NEW��������, �����������������
	bson_t *new_doc = BCON_NEW("hello", BCON_UTF8("world")); //hello�ֶ�ֵΪworld
	InsertDoc("db", "test", new_doc);
	bson_destroy(new_doc);
	*/

	//BCON_NEW����������, �����������������
	bson_t *multi_doc;
	multi_doc = BCON_NEW("name", BCON_UTF8("Babe"),
		"statistics", "{",
		"batting_average", BCON_DOUBLE(.342),
		"hits", BCON_INT32(2873),
		"home_runs", BCON_INT32(714),
		"rbi", BCON_INT32(2213),
		"}",
		"nicknames", BCON_UTF8("Ruth")
		);
	InsertDoc("db", "test", multi_doc);
	bson_destroy(multi_doc);

	return true;
}

//���Բ���
bool CMongoDatabase::TestSearchDoc()
{
	bson_t *cond = BCON_NEW("name", BCON_UTF8("Babe"));
	mongoc_cursor_t *cursor(NULL);
	const bson_t *doc = NULL;
	char *str = NULL;

	if (SearchDoc("db", "test", cond, cursor, 0, 0, 0, NULL) != false)
	{
		while (mongoc_cursor_next(cursor, &doc)) {
			str = bson_as_json(doc, NULL);
			printf("%s\n", str);

			Json::Reader reader;
			Json::Value value;

			if (reader.parse(str, value))
			{
				std::string out = value["name"].asString();
				printf("read name:%s\n", out.c_str());
			}

			bson_free(str);
		}
	}
	mongoc_cursor_destroy(cursor);
	bson_destroy(cond);

	return true;
}

//����ɾ��
bool CMongoDatabase::TestDeleteDoc()
{
	//$and��$or���Ҫ������, �ؼ��ֺ���Ĳ���ƴ�������������
	bson_t *delete_update = BCON_NEW("$or", "[",
		"{", "name", BCON_UTF8("Babe"), "}",
		"{", "nicknames", BCON_UTF8("Ruth"), "}",
		"]");

	DeleteTotalDoc("db", "test", delete_update);

	return true;
}

//���Ը���
bool CMongoDatabase::testUpdateDoc()
{
	//ִ������
	bson_t *cond = BCON_NEW("name", BCON_UTF8("Babe"));

	//��������
	bson_t *updatedoc = BCON_NEW("$set", "{",
		"nicknames", BCON_UTF8("Ruth"),
		"}");

	UpdateOnlyDoc("db", "test", cond, updatedoc);

	bson_destroy(cond);
	bson_destroy(updatedoc);

	return true;
}