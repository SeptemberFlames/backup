#include "stdafx.h"
#include "LbsProxySession.h"

//���캯��
CLbsProxySession::CLbsProxySession()
{

}

//��������
CLbsProxySession::~CLbsProxySession()
{

}

//��ʼ������, �ڳ�ʼ��ʱ����������
bool CLbsProxySession::Init(const loss::CJsonObject& conf)
{
	/*
	if (boInit)
	{
		return true;
	}
	std::string dbip;
	int dbport;
	if (!conf.Get("dbip", dbip) || !conf.Get("dbport", dbport)
		|| !conf.Get("group_dbname", group_dbname)
		|| !conf.Get("group_tbname", group_tbname)
		|| !conf.Get("user_dbname", user_dbname)
		|| !conf.Get("user_tbname", user_tbname)
		|| !conf.Get("near_dis", near_dis))
	{
		return false;
	}
	snprintf(connectStr, sizeof(connectStr), "mongodb://%s:%d/", dbip.c_str(),
		dbport);
	mongoc_init();
	client = mongoc_client_new(connectStr); //"mongodb://localhost:27017/"
	if (!client)
	{
		return false;
	}
	boInit = true;
	*/
	return true;
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
InsertDoc("mydb", "mycoll",doc)
��3����������
bson_destroy (doc);
* */

/*
//��������
bool CLbsProxySession::InsertDoc(const char*db, const char*tb, const bson_t *doc)
{
	mongoc_collection_t *collection = mongoc_client_get_collection(client, db,
		tb); //���뵽���ݿ�db�ļ���coll��
	if (!mongoc_collection_insert(collection, MONGOC_INSERT_NONE, doc, NULL,
		&error))
	{
		char *docStr = bson_as_json(doc, NULL);
		LOG4CPLUS_ERROR_FMT(GetLogger(),
			"mongoc_collection_insert failed,cond:%s,error: %s\n",
			docStr, error.message);
		bson_free(docStr);
		mongoc_collection_destroy(collection);
		return false;
	}
	mongoc_collection_destroy(collection);
	return true;
}
*/

/*
(1)��ѯ����
bson_t *cond;
cond = bson_new ();
mongoc_cursor_t *cursor(NULL);
(2)��ѯ,�����α�cursor
SearchDocs("mydb","mycoll",cond,cursor)
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
* */

/*
//��ѯ����
bool CLbsProxySession::SearchDocs(const char*db, const char*tb,
	const bson_t *cond, mongoc_cursor_t *&cursor, unsigned int skip,
	unsigned int limit, unsigned int batch_size,
	const bson_t *fields)
{
	mongoc_collection_t *collection = mongoc_client_get_collection(client, db,
		tb);
	cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, skip, limit,
		batch_size, cond, fields, NULL); //��ȡ��ѯ�����ѯ��Ľ��
	if (!cursor)
	{
		char *condStr = bson_as_json(cond, NULL);
		LOG4CPLUS_ERROR_FMT(GetLogger(),
			"SearchDocs mongoc_collection_find failed,cond:%s,error: %s\n",
			condStr, error.message);
		bson_free(condStr);
		mongoc_collection_destroy(collection);
		return false;
	}
	mongoc_collection_destroy(collection);
	return true;
}
*/

/*
(1)���������͸�������,��db.a.update({"_id" : ObjectId("55ef549236fe322f9490e17b")},{"$set":{"key":"new_value","updated":true}})
bson_t *cond = BCON_NEW ("_id", BCON_OID (&oid));//����Ϊid "_id" : ObjectId("55ef549236fe322f9490e17b")
bson_t *updatedoc = BCON_NEW ("$set", "{",
"key", BCON_UTF8 ("new_value"),
"updated", BCON_BOOL (true),
"}");//{"$set"}
(2)�����ĵ�
UpdateDoc("mydb","mycoll",cond,updatedoc)
(3)��������
if (query)
bson_destroy (query);
if (update)
bson_destroy (update);
* */

/*
//��ѯ����
bool CLbsProxySession::UpdateDoc(const char*db, const char*coll,
	const bson_t *cond, const bson_t *updatedoc)
{
	mongoc_collection_t *collection = mongoc_client_get_collection(client, db,
		coll); //ָ�����ݿ�ͼ��ϣ�����
	if (!collection) //mongoc_client_get_collection����ʧ�ܻ�ִ��abort,ʵ���Ͽ��Բ����
	{
		LOG4CPLUS_ERROR_FMT(GetLogger(),
			"UpdateDoc mongoc_client_get_collection failed:%s\n",
			error.message);
		return false;
	}
	if (!mongoc_collection_update(collection, MONGOC_UPDATE_MULTI_UPDATE, cond,
		updatedoc, NULL, &error)) //MONGOC_UPDATE_MULTI_UPDATE�������з��Ͻ����
	{
		char *condStr = bson_as_json(cond, NULL);
		LOG4CPLUS_ERROR_FMT(GetLogger(),
			"UpdateDoc mongoc_collection_update failed,cond:%s,error: %s\n",
			condStr, error.message);
		bson_free(condStr);
		mongoc_collection_destroy(collection);
		return false;
	}
	mongoc_collection_destroy(collection);
	return true;
}
*/

/*
(1)���������͸����������û������룩
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
* */

/*
//û�и������ľͲ��룬����͸���
bool CLbsProxySession::UpsertDoc(const char*db, const char*coll,
	const bson_t *cond, const bson_t *updatedoc)
{
	mongoc_collection_t *collection = mongoc_client_get_collection(client, db,
		coll); //ָ�����ݿ�ͼ��ϣ�����
	if (!collection)
	{
		LOG4CPLUS_ERROR_FMT(GetLogger(),
			"UpdateDoc mongoc_client_get_collection failed:%s\n",
			error.message);
		return false;
	}
	if (!mongoc_collection_update(collection, MONGOC_UPDATE_UPSERT, cond,
		updatedoc, NULL, &error)) //MONGOC_UPDATE_UPSERT ����
	{
		char *condStr = bson_as_json(cond, NULL);
		LOG4CPLUS_ERROR_FMT(GetLogger(),
			"UpsertDoc mongoc_collection_update failed,cond:%s,error: %s\n",
			condStr, error.message);
		bson_free(condStr); //���õ���free
		mongoc_collection_destroy(collection);
		return false;
	}
	mongoc_collection_destroy(collection);
	return true;
}
*/


/*
(1)��������
bson_t *cond = bson_new ();
BSON_APPEND_OID (doc, "_id", &oid);
(2)ɾ������
DeleteDoc("mydb","mycoll",cond)
(3)��������
bson_destroy (cond);
* */

/*
//ɾ����һ�������������ĵ�
bool CLbsProxySession::DeleteDocOnce(const char*db, const char*coll,
	const bson_t *cond)
{
	mongoc_collection_t *collection = mongoc_client_get_collection(client, db,
		coll); //ָ�����ݿ�ͼ���
	if (!collection)
	{
		LOG4CPLUS_ERROR_FMT(GetLogger(),
			"DeleteDoc mongoc_client_get_collection failed");
		return false;
	}
	if (!mongoc_collection_remove(collection, MONGOC_REMOVE_SINGLE_REMOVE, cond,
		NULL, &error))
	{
		char *condStr = bson_as_json(cond, NULL);
		LOG4CPLUS_ERROR_FMT(GetLogger(), "Delete failed,cond:%s,error: %s\n",
			condStr, error.message);
		bson_free(condStr);
		mongoc_collection_destroy(collection);
		return false;
	}
	mongoc_collection_destroy(collection);
	return true;
}
*/

//ɾ�����з����������ĵ�
/*
bool CLbsProxySession::DeleteDocAll(const char*db, const char*coll,
	const bson_t *cond)
{
	mongoc_collection_t *collection = mongoc_client_get_collection(client, db,
		coll); //ָ�����ݿ�ͼ���
	if (!collection)
	{
		LOG4CPLUS_ERROR_FMT(GetLogger(),
			"DeleteDoc mongoc_client_get_collection failed");
		return false;
	}
	if (!mongoc_collection_remove(collection, MONGOC_REMOVE_NONE, cond,
		NULL, &error))
	{
		char *condStr = bson_as_json(cond, NULL);
		LOG4CPLUS_ERROR_FMT(GetLogger(), "Delete failed,cond:%s,error: %s\n",
			condStr, error.message);
		bson_free(condStr);
		mongoc_collection_destroy(collection);
		return false;
	}
	mongoc_collection_destroy(collection);
	return true;
}
*/

/*
(1)��������
bson_t *cond = bson_new_from_json ((const uint8_t *)"{\"hello\" : \"world\"}", -1, &error);//�����������
(2)ִ�м�������
long long count(0);
Count("mydb", "mycoll",cond,count)
* */

/*
//�������󣬶Է����������ĵ������м������ڷ�ҳ��ʾʱ���øò���
bool CLbsProxySession::Count(const char*db, const char*coll, const bson_t *cond,
	long long &count)
{
	mongoc_collection_t *collection = mongoc_client_get_collection(client, db,
		coll); //ָ�����ݿ�ͼ���
	if (!collection)
	{
		LOG4CPLUS_ERROR_FMT(GetLogger(),
			"Count mongoc_client_get_collection failed");
		return false;
	}
	count = mongoc_collection_count(collection, MONGOC_QUERY_NONE, cond, 0, 0,
		NULL, &error);
	if (count < 0)
	{
		char *condStr = bson_as_json(cond, NULL);
		LOG4CPLUS_ERROR_FMT(GetLogger(),
			"mongoc_collection_count failed,cond:%s,error: %s\n",
			condStr, error.message);
		bson_free(condStr);
		mongoc_collection_destroy(collection);
		return false;
	}
	mongoc_collection_destroy(collection);
	return true;
}
*/

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
* */

/*
//mongodb��һϵ�е����е�����������������������Ϣ�����
bool CLbsProxySession::Command(const char*db, const char*coll,
	const bson_t *command, bson_t &reply)
{
	mongoc_collection_t *collection = mongoc_client_get_collection(client, db,
		coll);
	if (!collection)
	{
		LOG4CPLUS_ERROR_FMT(GetLogger(),
			"Count mongoc_client_get_collection failed");
		return false;
	}
	if (!mongoc_collection_command_simple(collection, command, NULL, &reply,
		&error))
	{
		char *commandStr = bson_as_json(command, NULL);
		LOG4CPLUS_ERROR_FMT(GetLogger(), "failed to run command:%s,error: %s\n",
			commandStr, error.message);
		bson_free(commandStr);
		mongoc_collection_destroy(collection);
		return false;
	}
	mongoc_collection_destroy(collection);
	return true;
}
*/

/*
//��������
int CLbsProxySession::InsertDoc(const char*db, const char*tb,
	const loss::CJsonObject &insertdObj)
{
	int retCode(ERR_OK);
	LOG4CPLUS_DEBUG_FMT(GetLogger(), "insertdObj json(%s)",
		insertdObj.ToString().c_str());
	//������ĵ�
	bson_t * insertddoc = GetBsonDoc(insertdObj, error);
	DebugOutPutBson(GetLogger(), insertddoc, "InsertDoc insertddoc");
	if (!insertddoc)
	{
		LOG4CPLUS_WARN_FMT(GetLogger(), "insertdObj convert to bson failed(%s)",
			insertdObj.ToString().c_str());
		retCode = ERR_REQ_MISS_PARAM;
		return retCode;
	}
	//������£��������
	if (!InsertDoc(db, tb, insertddoc))
	{
		SAFE_DESTROY_BSON(insertddoc);
		retCode = ERR_OPERATOR_MONGO_ERROR;
		return retCode;
	}
	SAFE_DESTROY_BSON(insertddoc);
	return retCode;
}
*/

/*
//��ѯ����
int CLbsProxySession::SearchDocs(const char*db, const char*tb,
	const loss::CJsonObject &condObj,
	std::vector<loss::CJsonObject> &replyObjs,
	const loss::CJsonObject &fieldsObj, unsigned int skip,
	unsigned int limit, unsigned int batch_size)
{
	int retCode(ERR_OK);
	LOG4CPLUS_DEBUG_FMT(GetLogger(), "condObj json(%s)",
		condObj.ToString().c_str());
	//��ѯ������
	bson_t * condDoc = GetBsonDoc(condObj, error);
	DebugOutPutBson(GetLogger(), condDoc, "SearchDocs condDoc");
	if (!condDoc)
	{
		LOG4CPLUS_WARN_FMT(GetLogger(), "condObj convert to bson failed(%s)",
			condObj.ToString().c_str());
		retCode = ERR_REQ_MISS_PARAM;
		return retCode;
	}
	//��ѯ��fields
	bson_t * fieldsDoc(NULL);
	if (!fieldsObj.ToString().empty())
	{
		fieldsDoc = GetBsonDoc(fieldsObj, error);
		if (!fieldsDoc)
		{
			LOG4CPLUS_ERROR_FMT(GetLogger(),
				"SearchDocs get fieldsDoc failed,error: %s\n",
				error.message);
			retCode = ERR_REQ_MISS_PARAM;
			return retCode;
		}
	}
	DebugOutPutBson(GetLogger(), condDoc, "SearchDocs condDoc");
	if (fieldsDoc)
	{
		DebugOutPutBson(GetLogger(), fieldsDoc, "SearchDocs fieldsDoc");
	}
	else
	{
		LOG4CPLUS_DEBUG_FMT(GetLogger(), "SearchDocs all fields");
	}
	mongoc_cursor_t *cursor(NULL);
	if (!SearchDocs(db, tb, condDoc, cursor, skip, limit, batch_size,
		fieldsDoc))
	{
		LOG4CPLUS_WARN_FMT(GetLogger(), "SearchDocs failed");
		SAFE_DESTROY_BSON(condDoc);
		SAFE_DESTROY_BSON(fieldsDoc);
		retCode = ERR_OPERATOR_MONGO_ERROR;
		return retCode;
	}
	replyObjs.clear();
	loss::CJsonObject tmpReplyObj;
	//�������
	const bson_t *doc;
	char *str;
	while (mongoc_cursor_next(cursor, &doc))
	{
		str = bson_as_json(doc, NULL);
		tmpReplyObj.Clear();
		tmpReplyObj.Parse(str);
		replyObjs.push_back(tmpReplyObj);
		LOG4CPLUS_DEBUG_FMT(GetLogger(), "reply:%s\n", str);
		bson_free(str);
	}
	mongoc_cursor_destroy(cursor);
	SAFE_DESTROY_BSON(condDoc);
	SAFE_DESTROY_BSON(fieldsDoc);
	return retCode;
}
*/

/*
//��������,������£����򲻸���Ҳ������
int CLbsProxySession::UpdateDoc(const char*db, const char*tb,
	const loss::CJsonObject &condObj,
	const loss::CJsonObject &updatedObj)
{
	int retCode(ERR_OK);
	LOG4CPLUS_DEBUG_FMT(GetLogger(), "condObj json(%s),updatedObj json(%s)",
		condObj.ToString().c_str(), updatedObj.ToString().c_str());
	//���µ�����
	bson_t * conddoc = GetBsonDoc(condObj, error);
	DebugOutPutBson(GetLogger(), conddoc, "UpdateDoc conddoc");
	if (!conddoc)
	{
		LOG4CPLUS_WARN_FMT(GetLogger(), "condObj convert to bson failed(%s)",
			condObj.ToString().c_str());
		retCode = ERR_REQ_MISS_PARAM;
		return retCode;
	}
	//���µ��ĵ�
	bson_t * updatedoc = GetBsonDoc(updatedObj, error);
	DebugOutPutBson(GetLogger(), updatedoc);
	if (!updatedoc)
	{
		LOG4CPLUS_WARN_FMT(GetLogger(), "updatedObj convert to bson failed(%s)",
			updatedObj.ToString().c_str());
		retCode = ERR_REQ_MISS_PARAM;
		return retCode;
	}
	//������£����򲻸���Ҳ������
	if (!UpdateDoc(db, tb, conddoc, updatedoc))
	{
		SAFE_DESTROY_BSON(conddoc);
		SAFE_DESTROY_BSON(updatedoc);
		retCode = ERR_OPERATOR_MONGO_ERROR;
		return retCode;
	}
	SAFE_DESTROY_BSON(conddoc);
	SAFE_DESTROY_BSON(updatedoc);
	return retCode;
}
*/

/*
//���²�������,������£��������
int CLbsProxySession::UpsertDoc(const char*db, const char* tb,
	const loss::CJsonObject &condObj,
	const loss::CJsonObject &updatedObj)
{
	int retCode(ERR_OK);
	LOG4CPLUS_DEBUG_FMT(GetLogger(), "condObj json(%s),updatedObj json(%s)",
		condObj.ToString().c_str(), updatedObj.ToString().c_str());
	//���µ�����
	bson_t * conddoc = GetBsonDoc(condObj, error);
	DebugOutPutBson(GetLogger(), conddoc, "UpsertDoc conddoc");
	if (!conddoc)
	{
		LOG4CPLUS_WARN_FMT(GetLogger(), "condObj convert to bson failed(%s)",
			condObj.ToString().c_str());
		retCode = ERR_REQ_MISS_PARAM;
		return retCode;
	}
	//���µ��ĵ�
	bson_t * updatedoc = GetBsonDoc(updatedObj, error);
	DebugOutPutBson(GetLogger(), updatedoc);
	if (!updatedoc)
	{
		LOG4CPLUS_WARN_FMT(GetLogger(), "updatedObj convert to bson failed(%s)",
			updatedObj.ToString().c_str());
		retCode = ERR_REQ_MISS_PARAM;
		return retCode;
	}
	//������£��������
	if (!UpsertDoc(db, tb, conddoc, updatedoc))
	{
		SAFE_DESTROY_BSON(conddoc);
		SAFE_DESTROY_BSON(updatedoc);
		retCode = ERR_OPERATOR_MONGO_ERROR;
		return retCode;
	}
	SAFE_DESTROY_BSON(conddoc);
	SAFE_DESTROY_BSON(updatedoc);
	return retCode;
}
*/

/*
//ɾ�������ĵ�����
int CLbsProxySession::DeleteDocOnce(const char*db, const char*tb,
	const loss::CJsonObject &condObj)
{
	int retCode(ERR_OK);
	LOG4CPLUS_DEBUG_FMT(GetLogger(), "condObj json(%s)",
		condObj.ToString().c_str());
	//ɾ��������
	bson_t * condDoc = GetBsonDoc(condObj, error);
	if (!condDoc)
	{
		LOG4CPLUS_WARN_FMT(GetLogger(), "condObj convert to bson failed(%s)",
			condObj.ToString().c_str());
		retCode = ERR_REQ_MISS_PARAM;
		return retCode;
	}
	DebugOutPutBson(GetLogger(), condDoc, "DeleteDocOnce conddoc");
	if (!DeleteDocOnce(db, tb, condDoc))
	{
		SAFE_DESTROY_BSON(condDoc);
		retCode = ERR_OPERATOR_MONGO_ERROR;
		return retCode;
	}
	SAFE_DESTROY_BSON(condDoc);
	return retCode;
}
*/

/*
//ɾ�������ĵ�����
int CLbsProxySession::DeleteDocAll(const char*db, const char*tb,
	const loss::CJsonObject &condObj)
{
	int retCode(ERR_OK);
	LOG4CPLUS_DEBUG_FMT(GetLogger(), "condObj json(%s),updatedObj json(%s)",
		condObj.ToString().c_str());
	//ɾ��������
	bson_t * condDoc = GetBsonDoc(condObj, error);
	if (!condDoc)
	{
		LOG4CPLUS_WARN_FMT(GetLogger(), "condObj convert to bson failed(%s)",
			condObj.ToString().c_str());
		retCode = ERR_REQ_MISS_PARAM;
		return retCode;
	}
	DebugOutPutBson(GetLogger(), condDoc, "DeleteDocAll conddoc");
	if (!DeleteDocAll(db, tb, condDoc))
	{
		SAFE_DESTROY_BSON(condDoc);
		retCode = ERR_OPERATOR_MONGO_ERROR;
		return retCode;
	}
	SAFE_DESTROY_BSON(condDoc);
	return retCode;
}
*/

/*
//��������
int CLbsProxySession::Count(const char*db, const char*tb,
	const loss::CJsonObject &condObj, long long &count)
{
	int retCode(ERR_OK);
	LOG4CPLUS_DEBUG_FMT(GetLogger(), "condObj json(%s)",
		condObj.ToString().c_str());
	//���������
	bson_t * condDoc = GetBsonDoc(condObj, error);
	if (!condDoc)
	{
		LOG4CPLUS_WARN_FMT(GetLogger(), "condObj convert to bson failed(%s)",
			condObj.ToString().c_str());
		retCode = ERR_REQ_MISS_PARAM;
		return retCode;
	}
	DebugOutPutBson(GetLogger(), condDoc);
	if (!Count(db, tb, condDoc, count))
	{
		SAFE_DESTROY_BSON(condDoc);
		retCode = ERR_OPERATOR_MONGO_ERROR;
		return retCode;
	}
	SAFE_DESTROY_BSON(condDoc);
	return retCode;
}
*/

/*
//��������
int CLbsProxySession::Command(const char*db, const char*tb,
	const loss::CJsonObject &commandObj,
	loss::CJsonObject &replyObj)
{
	int retCode(ERR_OK);
	LOG4CPLUS_DEBUG_FMT(GetLogger(), "commandObj json(%s),updatedObj json(%s)",
		commandObj.ToString().c_str());
	//ִ�е�����
	bson_t * commandDoc = GetBsonDoc(commandObj, error);
	if (!commandDoc)
	{
		LOG4CPLUS_WARN_FMT(GetLogger(), "commandObj convert to bson failed(%s)",
			commandObj.ToString().c_str());
		retCode = ERR_REQ_MISS_PARAM;
		return retCode;
	}
	DebugOutPutBson(GetLogger(), commandDoc, "Command commandDoc");
	bson_t reply;
	if (!Command(db, tb, commandDoc, reply))
	{
		SAFE_DESTROY_BSON(commandDoc);
		retCode = ERR_OPERATOR_MONGO_ERROR;
		return retCode;
	}
	char *bStr = bson_as_json(&reply, NULL);
	replyObj.Clear();
	replyObj.Parse(bStr);
	bson_free(bStr); //���õ���free
	SAFE_DESTROY_BSON(commandDoc);
	return retCode;
}
*/