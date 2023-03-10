
#ifndef WH_SMS_SERVICE_HEAD_FILE
#define WH_SMS_SERVICE_HEAD_FILE

#pragma once

#include "KernelEngineHead.h"

#include <vector>
#include <thread>

class KERNEL_ENGINE_CLASS CSmsService
{
public:
	CSmsService();
	~CSmsService();

	//发送入口
public:
	//发送入口
	void SendVoiceMessage(DWORD dwChannel, CString szMobile, DWORD dwPhoneCode);

	//发送函数
public:
	//发送函数
	void OnSendMessage();

	//功能函数
public:
	//发送请求
	int http_post(char *page, char *poststr);
	//创建链接
	int socked_connect(char *arg);
	//字符转换
	std::wstring Utf82Unicode(const string& utf8string);

	//成员变量
public:
	DWORD						m_basefd;
	DWORD						m_dwChannel;
	DWORD						m_dwPhoneCode;

	CString						m_szSmsConfig;
	CString						m_szMobile;

	//重要参数
	TCHAR						m_szAppID[256];
	TCHAR						m_szAppKey[256];
	TCHAR						m_szHostName[256];
	TCHAR						m_szSendSmsUrl[256];
	TCHAR						m_szSendSmsContent[256];

	//发送内容
	TCHAR						m_szMessage[256];

	//短信配置
protected:
	CString						m_szSmsConfigFileName;
};

//////////////////////////////////////////////////////////////////////////////////

#endif