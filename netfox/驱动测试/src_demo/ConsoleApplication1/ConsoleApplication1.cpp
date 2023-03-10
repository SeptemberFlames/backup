// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <mongoc.h>
#include <stdio.h>
#include <stdlib.h>
#include <bson.h>
#include <bcon.h>
#include <time.h>
#include <iostream>

#include "MongoDatabase.h"

using namespace std;

/**********************************************************************/
void UnicodeToUTF8(char *utf, wchar_t* wszString)
{
	//预转换，得到所需空间的大小  
	int u8Len = ::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
	//同上，分配空间要给'\0'留个空间  
	//UTF8虽然是Unicode的压缩形式，但也是多字节字符串，所以可以以char的形式保存  
	//转换    //unicode版对应的strlen是wcslen  
	::WideCharToMultiByte(CP_UTF8, NULL, wszString, wcslen(wszString), utf, u8Len, NULL, NULL);
	//最后加上'\0'  
	utf[u8Len] = '\0';
}


#if 1
int _tmain(int argc, _TCHAR* argv[])
{
	CMongoDatabase test("mongodb://localhost:27017");
	//test.TestInsertDoc();
	test.TestSearchDoc();
	//test.TestDeleteDoc();

	
	/*
	mongoc_collection_t *collection;
	bson_error_t error;
	bson_t *doc; 
	const bson_t *doc_qurey;
	bson_t  child;
	const char *uristr = "mongodb://localhost:27017"; 
	const char *collection_name = "col";
	time_t     now = time(0);
	char       timebuf[256] = {};
	struct  tm  cur_time;
	
	char   *str;
	//bson_t *query;
	mongoc_cursor_t *cursor;
	bson_t query;
	bson_t child2, child3;

	//获取本地当前时间
	localtime_s(&cur_time, &now);
	//格式化时间
	strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", &cur_time);
	const char *optime = timebuf;

	//先调用mongoc_init()，libmongoc 的任何其他功能才能正确使用
	mongoc_init();

	//连线到db
	mongoc_client_t *client = mongoc_client_new(uristr); 
	if (NULL == client) {
		fprintf(stderr, "Failed to parse URI.\n");
		return EXIT_FAILURE;
	}
	mongoc_client_set_error_api(client, 2);
	
	//获取集合名
	collection = mongoc_client_get_collection(client, "test", collection_name);
	/******************写入操作例子如下:************************
	doc = bson_new();	
	BSON_APPEND_UTF8(doc, "Titile", "Test123 ");	
	BSON_APPEND_UTF8(doc, "Nowtime", optime);

	//开始一个子文档
	BSON_APPEND_DOCUMENT_BEGIN(doc, "subdoc", &child);
	BSON_APPEND_UTF8(&child, "subkey", "value");
	bson_append_document_end(doc, &child);

	double dval = 12.34;
	char *tm_sample = "2018:30";
	char buf_json[64] = { 0 };
	sprintf_s(buf_json, "[{\"sample_time\":\"%s\",\"sample_value\":\"%.2f\"}]", tm_sample, dval);
	bson_t *bt_arrayOfSample = bson_new_from_json((const uint8_t *)buf_json,
		strlen(buf_json), &error);
	BSON_APPEND_ARRAY(doc, "arrayOfSample", bt_arrayOfSample);

	size_t err_offset;
	if (!bson_validate(doc, BSON_VALIDATE_NONE, &err_offset)) {
		fprintf(stderr,
			"The document failed to validate at offset: %u\n",
			(unsigned)err_offset);
	}

	//将bson文档插入到集合
	if (!mongoc_collection_insert(collection, MONGOC_INSERT_NONE, doc, NULL, &error))
	{
		fprintf(stderr, "%s\n", error.message);
	}

	/**************************************批量写入操作例子如下:*************************************
	enum N { ndocs = 4 };
	bson_t *docs_bulk[ndocs];
	mongoc_bulk_operation_t *bulk =
		mongoc_collection_create_bulk_operation_with_opts(collection, NULL);
	docs_bulk[0] = BCON_NEW("x", BCON_DOUBLE(1.0), "tags", "[", "dog", "cat", "]");
	docs_bulk[1] = BCON_NEW("x", BCON_DOUBLE(2.0), "tags", "[", "cat", "]");
	docs_bulk[2] = BCON_NEW(
		"x", BCON_DOUBLE(2.0), "tags", "[", "mouse", "cat", "dog", "]");
	docs_bulk[3] = BCON_NEW("x", BCON_DOUBLE(3.0), "tags", "[", "]");

	for (int i = 0; i < ndocs; i++) {
		mongoc_bulk_operation_insert(bulk, docs_bulk[i]);
		bson_destroy(docs_bulk[i]);
		docs_bulk[i] = NULL;
	}

	bool ret = mongoc_bulk_operation_execute(bulk, NULL, &error);
	if (!ret) {
		fprintf(stderr, "Error inserting data: %s\n", error.message);
	}
	mongoc_bulk_operation_destroy(bulk);

	/***************************从JSON创建BSON例子如下:*****************************
	//JSON字符串可以通过bson_new_from_json创建单个BSON文档。
	//若要从一个JSON文档序列初始化BSON，使用bson_json_reader_t。
	
	char well_name[128] = { 0 };
	wchar_t* wszString = L"{\"hello\": \"test测试中文插入OK\"}";//
	UnicodeToUTF8(well_name, wszString);//unicode to UTF8  
	
	if (!bson_utf8_validate(well_name, strlen(well_name), true)) {
		printf("Validation failed.\n");
	}
	
	//const char *json = "{\"hello\": \"world\"}";      //"{\"hello\": \"world\"}"
	//bson_t *bson = bson_new_from_json((const uint8_t *)json, -1, &error);
	//if (!bson) {
	//	fprintf(stderr, "%s\n", error.message);
	//	return EXIT_FAILURE;
	//}

	//char *strtxt = bson_as_json(bson, NULL);
	//printf("%s\n", strtxt);
	/*************************BCON方式组织Json文件例子如下:**********************

	//打印这个BSON对象，以json字符串格式。
	//BCON是BSONC对象表示法的简称，是另一种以更接近预定的格式构建BSON文档的方式
	doc = BCON_NEW("name", BCON_UTF8("Babe Ruth"),
		"statistics", "{",
		"batting_average", BCON_DOUBLE(.342),
		"hits", BCON_INT32(2873),
		"home_runs", BCON_INT32(714),
		"rbi", BCON_INT32(2213),
		"}",
		"nicknames", "[",
		BCON_UTF8("the Sultan of Swat"),
		BCON_UTF8("the Bambino"),
		"]");

	str = bson_as_json(doc, NULL);
	printf("%s\n", str);
	/*************************查询文档例子如下:************************************************
	/*********************查询所有文档例子如下:***************************
	//// query是一个空的BSON文档，用于做查询说明符的时候匹配所有文档。
	//query = bson_new();
	//执行查询操作，返回一个匹配的文档的cursor(游标)。 使用BSON文档作为查询说明符;
	//如，{ "color" : "red" }将匹配所有文档中字段名为color且值为red的。空文档{}可以用于匹配的所有文件

	//cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
	//while (mongoc_cursor_next(cursor, &doc_qurey)) {
	//	str = bson_as_json(doc_qurey, NULL);
	//	printf("%s\n", str);
	//	bson_free(str);
	//}

	//bson_destroy(query);
	//mongoc_cursor_destroy(cursor);

	/*********************查询满足指定条件的文档例子如下:***************************
	//query在这里将添加一个键值对，查询的时候将只查询出匹配上的结果
	//bson_t *query_spc = bson_new();
	//BSON_APPEND_UTF8(query_spc, "Titile", "Hello world ");

	bson_t *query_spc = BCON_NEW("$query",
		"{",
		"likes",
		BCON_DOUBLE(666.0),
		"}");

	cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 0, 0, query_spc, NULL, NULL);
	const bson_t *doc_cur;
	char * str_tojson;
	while (mongoc_cursor_next(cursor, &doc_cur)) {
		str_tojson = bson_as_relaxed_extended_json(doc_cur, NULL);
		printf("%s\n", str_tojson);
		bson_free(str_tojson);
	}
	bson_destroy(query_spc);
	mongoc_cursor_destroy(cursor);
	/*********************条件查询语句01例子如下:***************************
	// id==4 or field1 <= 12
	bson_init(&query);
	bson_append_array_begin(&query, "$or", -1, &child);

	//0: 第一个or部分
	bson_append_document_begin(&child, "0", -1, &child2);
	BSON_APPEND_INT64(&child2, "id", 4);
	bson_append_document_end(&child, &child2);

	//1:第二个or部分
	bson_append_document_begin(&child, "1", -1, &child2);

	//field1 <= 12
	bson_append_document_begin(&child2, "field1", -1, &child3);
	BSON_APPEND_INT64(&child3, "$lte", 12);
	bson_append_document_end(&child2, &child3);
	bson_append_document_end(&child, &child2);
	bson_append_array_end(&query, &child);
	char * strtemp = bson_as_json(&query, NULL);
	printf("\n%s\n", strtemp);

	/*********************修改更新文档例子如下:***************************
	//测试：\x6D\x4B\x8B\xD5
	 query_spc = BCON_NEW("likes", BCON_DOUBLE(600.0));
	bson_t * update_bycond = BCON_NEW("$set", "{",
		"title", BCON_UTF8("test new values"),
		"updated", BCON_BOOL(true),
		"}");
	//执行update操作。这个操作将使用update的内容去替换之前插入到数据库中的doc的内容
	if (!mongoc_collection_update(collection, MONGOC_UPDATE_NONE, query_spc, update_bycond, NULL, &error)) {
		fprintf(stderr, "%s\n", error.message);
	}

	   bson_destroy(query_spc);
	bson_destroy(update_bycond);

	/*********************删除文档中的某个指定字段例子如下:********************
	query_spc = BCON_NEW("likes", BCON_DOUBLE(600.0));
	 update_bycond = BCON_NEW("$unset", "{",
		"title", BCON_BOOL(true),
		"}");
	//执行update操作。这个操作将使用update的内容去替换之前插入到数据库中的doc的内容
	if (!mongoc_collection_update(collection, MONGOC_UPDATE_NONE, query_spc, update_bycond, NULL, &error)) {
		fprintf(stderr, "%s\n", error.message);
	}

	bson_destroy(query_spc);
	bson_destroy(update_bycond);

	/*********************删除文档例子如下:***************************
	//doc = bson_new();
	//BSON_APPEND_DOUBLE(doc, "likes", 200.0);
	//if (!mongoc_collection_remove(collection, MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error)) {
	//	fprintf(stderr, "Delete failed: %s\n", error.message);
	//}

	/*********************统计满足匹配条件的文档数目例子如下:*********************
	// doc用于计数时候做匹配
	doc = bson_new();
	BSON_APPEND_DOUBLE(doc, "likes", 100.0);
	// 进行统计操作
	int64_t count = mongoc_collection_count(collection, MONGOC_QUERY_NONE, doc, 0, 0, NULL, &error);
	if (count < 0) {
		fprintf(stderr, "%s\n", error.message);
	}
	else {
		printf("%" PRId64 "\n", count);
	}

	
	/*******************************************************************
	//释放资源Release our handles and clean up libmongoc
	bson_destroy(doc);
	mongoc_collection_destroy(collection);
	mongoc_client_destroy(client);
	//退出之前调用mongoc_cleanup()。
	mongoc_cleanup();
	
	*/
	
	system("pause");

	return EXIT_SUCCESS;
}
#endif
