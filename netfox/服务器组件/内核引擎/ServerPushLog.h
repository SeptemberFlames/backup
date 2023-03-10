
#ifndef SERVER_LOG_HEAD_FILE
#define SERVER_LOG_HEAD_FILE

#pragma once

#include "KernelEngineHead.h"
#include <unordered_map>
#include <queue>

//发送日志
struct tagSendLog
{
	WORD						wDataSize;							//数据大小
	BYTE						cbSendBuffer[SEND_BUFFER_LEN + 3];	//发送缓冲
};

//类型定义
typedef std::queue<tagSendLog> tagSendLogQueue;						//发送队列
typedef std::unordered_map<WORD, LONGLONG> tagTableLogIndex;		//用户控制 

//数据线程
class CServerPushLogThread : public CWHThread
{
	//运行状态
private:
	BOOL						m_bRuning;							//运行状态
	HWND						m_hWnd;								//窗口句柄
	BYTE						m_cbTCPSocketStatus;				//网络状态
	CWHDataQueue				m_DataQueue;						//请求队列

	//锁定对象
public:
	CCriticalSection			m_ServerPushLogSection;				//锁定对象

	//网络定义
protected:
	SOCKET						m_hSocket;							//网络句柄
	USHORT						m_LogOutputPort;					//连接端口
	TCHAR						m_LogOutputAddress[16];				//连接地址

	//缓冲变量
protected:
	bool						m_bNeedBuffer;						//缓冲状态
	DWORD						m_dwBufferData;						//缓冲数据
	DWORD						m_dwBufferSize;						//缓冲大小
	LPBYTE						m_pcbDataBuffer;					//缓冲数据

	//函数定义
public:
	//构造函数
	CServerPushLogThread();
	//析构函数
	virtual ~CServerPushLogThread();

	//功能函数
public:
	//保存日志
	void PushLogData(TCHAR szLogPathName[MAX_PATH], LONGLONG llLogIndex, LOG_Level Level, TCHAR szFormat[MAX_LOGER_LEN]);

	//重载函数
public:
	//开始函数
	virtual bool OnEventThreadStrat();
	//停止事件
	virtual bool OnEventThreadConclude();
	//运行函数
	virtual bool OnEventThreadRun();

	//请求函数
public:
	//投递请求
	bool PostThreadRequest(WORD wIdentifier, VOID * const pBuffer, WORD wDataSize);

	//处理函数
private:
	//网络消息
	LRESULT OnSocketNotify(WPARAM wParam, LPARAM lParam);
	//请求消息
	LRESULT OnServiceRequest(WPARAM wParam, LPARAM lParam);

	//网络接口
public:
	//网络读取
	LRESULT OnSocketNotifyRead(WPARAM wParam, LPARAM lParam);
	//网络发送
	LRESULT OnSocketNotifyWrite(WPARAM wParam, LPARAM lParam);
	//网络关闭
	LRESULT OnSocketNotifyClose(WPARAM wParam, LPARAM lParam);
	//网络连接
	LRESULT OnSocketNotifyConnect(WPARAM wParam, LPARAM lParam);

	//控制函数
public:
	//关闭连接
	VOID PerformCloseSocket(BYTE cbShutReason);
	//连接服务器
	DWORD PerformConnect(DWORD dwServerIP, WORD wPort);
	//发送函数
	DWORD PerformSendData(VOID * pData, WORD wDataSize);

	//辅助函数
private:
	//缓冲数据
	VOID AmortizeBuffer(VOID * pData, WORD wDataSize);
	//发送数据
	WORD SendDataBuffer(VOID * pBuffer, WORD wSendSize);

	//内部函数
private:
	//地址解释
	DWORD TranslateAddress(LPCTSTR szServerIP);
};

//////////////////////////////////////////////////////////////////////////

class KERNEL_ENGINE_CLASS CServerPushLog : public CWHThread
{
	//友元定义
	friend class CServerPushLogThread;

public:
	CServerPushLog();
	~CServerPushLog();

	//组件变量
public:
	static CServerPushLogThread	*			m_ServerPushLogThread;

	//核心参数
public:
	USHORT									m_sLogOutputPort;
	char									m_sLogOutputAddress[16];
	SOCKET									m_udp_socket;
	sockaddr_in								m_addrSrv;


	//全局索引
protected:
	static CServerPushLog *					m_ServerLog;
	static LONGLONG							m_llServerLogIndex;
	static LONGLONG							m_llKernelLogIndex;
	static LONGLONG							m_llScoreLogIndex;
	static LONGLONG							m_llSqlLogIndex;

	//桌子索引
protected:
	static tagTableLogIndex *				m_TableLogIndex;

	//内部变量
public:
	INT										m_serverPort;						//程序端口
	TCHAR									m_szPlatfromName[MAX_PLATFROM_LEN];	//平台名称
	TCHAR									m_szLogPathName[MAX_PATH];			//日志路径

	//全局指针
public:
	//全局指针
	static CServerPushLog* SharedLog();

	//服务接口
public:
	//启动服务
	virtual bool StartService();
	//停止服务
	virtual bool ConcludeService();

	//功能接口
public:
	//设置端口
	static void SetServerPort(WORD port);
	//文件路径
	static void SetServerLogName(TCHAR * szLogPathName);
	//服务日志
	static void PushServerLog(TCHAR Category[MAX_PATH], LOG_Level Level, TCHAR pszFormat[MAX_LOGER_LEN]);
	//内核日志
	static void PushKernelLog(TCHAR Category[MAX_PATH], LOG_Level Level, TCHAR pszFormat[MAX_LOGER_LEN]);
	//游戏日志
	static void PushGameLog(TCHAR GameLogPath[MAX_PATH], LOG_Level Level, TCHAR pszFormat[MAX_LOGER_LEN], WORD wTableID = INVALID_TABLE);
	//金币日志
	static void PushScoreLog(TCHAR GameLogPath[MAX_PATH], LOG_Level Level, DWORD dwGameID, TCHAR pszFormat[MAX_LOGER_LEN]);
};

#endif