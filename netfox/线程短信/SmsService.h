
#ifndef WH_SMS_SERVICE_HEAD_FILE
#define WH_SMS_SERVICE_HEAD_FILE

#pragma once

#include "KernelEngineHead.h"

#include "ThreadPool.h"
#include <vector>
#include <thread>

class KERNEL_ENGINE_CLASS CSmsService
{
public:
	CSmsService();
	~CSmsService();

	//服务入口
public:
	//开始服务
	bool OnServiceStart();
	//停止服务
	bool OnServiceConclude();

	//发送入口
public:
	//发送入口
	void SendVoiceMessage(DWORD dwChannel, CString szMobile, DWORD dwPhoneCode);

	//发送函数
private:
	//发送函数
	bool OnSendMessage(DWORD dwChannel, CString szMobile, DWORD dwPhoneCode);

	//功能函数
private:
	//发送请求
	int http_post(int socket, char * hostName, char *page, char *postStr);
	//创建链接
	int socked_connect(char *arg);
	//字符转换
	std::wstring UtfToUnicode(const string& utf8string);

	//短信配置
private:
	CString						m_szSmsConfigFileName;

	//工作线程
private:
	CThreadPool					m_threadPool;
};

//////////////////////////////////////////////////////////////////////////////////

#endif