#include "Stdafx.h"
#include "SmsService.h"
#include "TraceService.h"
#include "ServerPushLog.h"

#define LISTENQ		1024
#define MAXPARAM	2048
#define MAXLINE		4096
#define MAXSUB		2000

//#include <io.h>
#include <windows.h>

#pragma comment(lib, "wsock32.lib")

CSmsService::CSmsService()
{
	//清空变量
	ZeroMemory(m_szAppID, CountArray(m_szAppID));
	ZeroMemory(m_szAppKey, CountArray(m_szAppKey));
	ZeroMemory(m_szHostName, CountArray(m_szHostName));
	ZeroMemory(m_szSendSmsUrl, CountArray(m_szSendSmsUrl));
	ZeroMemory(m_szSendSmsContent, CountArray(m_szSendSmsContent));
	ZeroMemory(m_szMessage, CountArray(m_szMessage));
}

CSmsService::~CSmsService()
{
	::closesocket(m_basefd);
}

int CSmsService::http_post(char *page, char *poststr)
{
	//前置声明
	USES_CONVERSION;

	//变量准备
	char sendline[MAXLINE + 1] = { 0 };
	char recvline[MAXLINE + 1] = { 0 };
	int iRecv = 0;

	//格式参数
	_snprintf_s(sendline, MAXSUB,
		"POST %s HTTP/1.0\r\n"
		"Host: %s\r\n"
		"Content-type: application/x-www-form-urlencoded\r\n"
		"Content-length: %d\r\n\r\n"
		"%s", page, W2A(m_szHostName), strlen(poststr), poststr);

	//发送数据
	send(m_basefd, sendline, strlen((sendline)), 0);

	//接收结果
	while ((iRecv = recv(m_basefd, recvline, MAXLINE, 0)) > 0)
	{
		recvline[iRecv] = '\0';
		std::wstring Strunicode = Utf82Unicode(recvline);
		CString mMFCrecvline = Strunicode.c_str();

		TCHAR szFormatBuff[MAX_LOGER_LEN] = TEXT("");
		_sntprintf_s(szFormatBuff, _TRUNCATE, TEXT("recvline:%s"), mMFCrecvline.GetBuffer(0));
		CPushLog(TEXT("SmsService"), LOG_ERROR, szFormatBuff);
	}
	return iRecv;
}

//创建链接
int CSmsService::socked_connect(char *arg)
{
	//参数准备
	struct sockaddr_in their_addr = { 0 };
	char buf[1024] = { 0 };
	char rbuf[1024] = { 0 };
	char pass[128] = { 0 };
	struct hostent *host = NULL;

	//初始连接
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		return -1;
	}

	//域名解析
	if ((host = gethostbyname(arg)) == NULL)
	{
		return -1;
	}

	//连接参数
	memset(&their_addr, 0, sizeof(their_addr));
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(80);
	their_addr.sin_addr = *((struct in_addr *)host->h_addr);

	//开始连接
	if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) < 0)
	{
		::closesocket(sockfd);
		return  -1;
	}
	return sockfd;
}

//发送入口
void CSmsService::SendVoiceMessage(DWORD dwChannel, CString szMobile, DWORD dwPhoneCode)
{
	//保存参数
	m_szMobile = szMobile;
	m_dwChannel = dwChannel;
	m_dwPhoneCode = dwPhoneCode;

	//线程处理
	std::thread t(&CSmsService::OnSendMessage, this);
	t.detach();
}

//发送函数
void CSmsService::OnSendMessage()
{
	//前置声明
	USES_CONVERSION;

	//短信配置
	TCHAR szPath[MAX_PATH] = TEXT("");
	GetModuleFileName(AfxGetInstanceHandle(), szPath, sizeof(szPath));
	m_szSmsConfigFileName = szPath;
	int nSmsConfigIndex = m_szSmsConfigFileName.ReverseFind(TEXT('\\'));
	m_szSmsConfigFileName = m_szSmsConfigFileName.Left(nSmsConfigIndex);
	m_szSmsConfigFileName += TEXT("\\SmsConfig.ini");

	//读取主键
	WORD wChannelPlatform = CWHChannel::CalcChannelPlatfrom(m_dwChannel);
	m_szSmsConfig.Format(TEXT("SmsConfig_%d"), wChannelPlatform);

	//读取参数
	GetPrivateProfileString(m_szSmsConfig, TEXT("api_id"), TEXT(""), m_szAppID, 256, m_szSmsConfigFileName);
	GetPrivateProfileString(m_szSmsConfig, TEXT("api_key"), TEXT(""), m_szAppKey, 256, m_szSmsConfigFileName);
	GetPrivateProfileString(m_szSmsConfig, TEXT("host_name"), TEXT(""), m_szHostName, 256, m_szSmsConfigFileName);
	GetPrivateProfileString(m_szSmsConfig, TEXT("send_sms_url"), TEXT(""), m_szSendSmsUrl, 256, m_szSmsConfigFileName);
	GetPrivateProfileString(m_szSmsConfig, TEXT("send_sms_content"), TEXT(""), m_szSendSmsContent, 256, m_szSmsConfigFileName);

	//参数检测
	if (!wcsicmp(m_szAppID, TEXT("")))
	{
		CString subString;
		subString.Format(TEXT("短信AppID错误, 渠道平台:%d"), wChannelPlatform);
		CTraceService::TraceString(subString, TraceLevel_Exception);
	}
	else if (!wcsicmp(m_szAppKey, TEXT("")))
	{
		CString subString;
		subString.Format(TEXT("短信AppKey错误, 渠道平台:%d"), wChannelPlatform);
		CTraceService::TraceString(subString, TraceLevel_Exception);
	}
	else if (!wcsicmp(m_szHostName, TEXT("")))
	{
		CString subString;
		subString.Format(TEXT("短信HostName错误, 渠道平台:%d"), wChannelPlatform);
		CTraceService::TraceString(subString, TraceLevel_Exception);
	}
	else if (!wcsicmp(m_szSendSmsUrl, TEXT("")))
	{
		CString subString;
		subString.Format(TEXT("短信SendSmsUrl错误, 渠道平台:%d"), wChannelPlatform);
		CTraceService::TraceString(subString, TraceLevel_Exception);
	}
	else if (!wcsicmp(m_szSendSmsContent, TEXT("")))
	{
		CString subString;
		subString.Format(TEXT("短信SendSmsContent错误, 渠道平台:%d"), wChannelPlatform);
		CTraceService::TraceString(subString, TraceLevel_Exception);
	}
	else
	{
		//格式内容
		TCHAR szMessage[256] = { 0 };
		wsprintf(szMessage, m_szSendSmsContent, m_dwPhoneCode);

		//建立连接
		m_basefd = socked_connect(W2A(m_szHostName));

		//错误检测
		if (m_basefd == -1)
		{
			delete this;
			return;
		}

		//转换参数
		char* pMobile = T2A(m_szMobile);
		char* pMessage = T2A(szMessage);
		char* pAppID = T2A(m_szAppID);
		char* pAppKey = T2A(m_szAppKey);
		char* pSendSmsUrl = T2A(m_szSendSmsUrl);

		//格式参数
		char params[MAXPARAM];
		sprintf_s(params, MAXPARAM, "account=%s&password=%s&mobile=%s&content=%s", pAppID, pAppKey, pMobile, pMessage);
		http_post(pSendSmsUrl, params);

		//释放内容
		m_szMobile.ReleaseBuffer();

		//关闭连接
		closesocket(m_basefd);
	}

	//释放指针
	delete this;
}

//字符转换
std::wstring CSmsService::Utf82Unicode(const string& utf8string)
{
	int widesize = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (widesize == 0)
	{
		throw std::exception("Error in conversion.");
	}

	std::vector<wchar_t> resultstring(widesize);

	int convresult = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, &resultstring[0], widesize);

	if (convresult != widesize)
	{
		throw std::exception("La falla!");
	}

	return std::wstring(&resultstring[0]);
}