#ifndef TCP_NETWORK_ENGINE_HEAD_FILE
#define TCP_NETWORK_ENGINE_HEAD_FILE

#pragma once

#include "SocketData.h"
#include "KernelEngineHead.h"
#include "AsynchronismEngine.h"
#include <unordered_map>
using namespace std;

//接收极限
#define RECV_SOCKET_TCP_BUFFER	SOCKET_TCP_BUFFER * 8

//////////////////////////////////////////////////////////////////////////
//枚举定义

//操作类型
enum enOperationType
{
	enOperationType_Send,		//发送类型
	enOperationType_Recv,		//接收类型
};

//////////////////////////////////////////////////////////////////////////

//类说明
class COverLappedRecv;
class COverLappedSend;
class CTCPNetworkSocket;
class CTCPNetworkEngine;

//数组定义
typedef class CWHArray<COverLappedSend *>	COverLappedSendArray;
typedef class CWHArray<COverLappedRecv *>	COverLappedRecvArray;

//////////////////////////////////////////////////////////////////////////
//接口定义

//网络接口
interface ITCPNetworkSocketSink
{
	//绑定事件
	virtual bool OnEventSocketBind(CTCPNetworkSocket * pTCPNetworkSocket) = NULL;
	//关闭事件
	virtual bool OnEventSocketShut(CTCPNetworkSocket * pTCPNetworkSocket) = NULL;
	//读取事件
	virtual bool OnEventSocketRead(TCP_Command Command, VOID * pData, WORD wDataSize, CTCPNetworkSocket * CTCPNetworkSocket) = NULL;
};

//////////////////////////////////////////////////////////////////////////

//重叠结构
class COverLappedItem
{
	//变量定义
public:
	WSABUF							m_WSABuffer;						//数据指针
	OVERLAPPED						m_OverLapped;						//重叠结构
	const enOperationType			m_OperationType;					//操作类型

	//函数定义
public:
	//构造函数
	COverLappedItem(enOperationType OperationType);
	//析构函数
	virtual ~COverLappedItem();

	//信息函数
public:
	//获取类型
	enOperationType GetOperationType() { return m_OperationType; }
};

//////////////////////////////////////////////////////////////////////////

//发送结构
class COverLappedSend : public COverLappedItem
{
	//数据变量
public:
	BYTE							m_cbBuffer[SOCKET_TCP_BUFFER];			//数据缓冲

	//函数定义
public:
	//构造函数
	COverLappedSend();
	//析构函数
	virtual ~COverLappedSend();
};

//////////////////////////////////////////////////////////////////////////

//接收结构
class COverLappedRecv : public COverLappedItem
{
	//函数定义
public:
	//构造函数
	COverLappedRecv();
	//析构函数
	virtual ~COverLappedRecv();
};

//////////////////////////////////////////////////////////////////////////

//网络子项
class CTCPNetworkSocket
{
	//友元声明
	friend class CTCPNetworkEngine;

	//连接属性
public:
	CHAR								m_szClientPort[33];							//唯一标识
	CHAR								m_szClientAddr[47];							//客户地址
	DWORD								m_dwClientAddr;								//连接地址
	DWORD								m_dwActiveTime;								//激活时间

	//内核变量
public:
	WORD								m_wIndex;									//连接索引
	WORD								m_wRountID;									//循环索引
	int									m_wSurvivalTime;							//生成时间
	SOCKET								m_hSocketHandle;							//连接句柄
	ITCPNetworkSocketSink *				m_pITCPNetworkSocketSink;					//回调接口
	DWORD                               m_dwListenPort;								//监听端口

	//客户推出
public:
	BOOL								m_bClientQuit;								//客户推出

	//序号变量
public:
	ULONGLONG                           m_ulSendIndex;								//发送索引
	ULONGLONG							m_ulRecvIndex;								//接收索引

	//组件变量
public:
	CTCPSocketData						m_TCPSocketData;							//网络数据
	CCriticalSection					m_NetworkSocketSection;						//锁定对象

	//状态变量
public:
	bool								m_bSendIng;									//发送标志
	bool								m_bRecvIng;									//接受标志
	bool								m_bShutDown;								//中断标志
	bool								m_bAllowBatch;								//接收群发

	//本地端口
public:
	DWORD								m_dwLocalPort;								//本地端口

	//区分标记
public:
	bool								m_bFirstRecvData;							//首次接收
	WORD								m_wConnectType;								//连接类型

	//接受变量
public:
	DWORD								m_wTcpBufferSize;							//接受长度
	BYTE								m_cbTcpDataBuffer[RECV_SOCKET_TCP_BUFFER];	//接受缓冲

	//网络变量
public:
	DWORD								m_wWebBufferSize;							//接受长度
	BYTE								m_cbWebDataBuffer[RECV_SOCKET_TCP_BUFFER];	//接受缓冲

	int									BitSHA(string origin, char *MessageDigest);
	int									WebsocketHandshake(string cbTcharTempRecvDataBuff, char *sendBuf);
	int									EncodeWebSocketData(BYTE* SrcBuff, int SrcLen, BYTE* DesBuff);
	int									DncodeWebSocketData(BYTE * recBytes, int recByteLength, int &deleteLenth, BYTE * payload_data);

	//重叠变量
public:
	COverLappedRecv						m_OverLappedRecv;							//重叠结构
	COverLappedSendArray				m_OverLappedSendActive;						//重叠结构
	COverLappedSendArray				m_OverLappedSendBuffer;						//重叠结构

	//函数定义
public:
	//构造函数
	CTCPNetworkSocket(WORD wIndex, ITCPNetworkSocketSink * pITCPNetworkSocketSink);
	//析构函数
	virtual ~CTCPNetworkSocket();

	//标识函数
public:
	//获取索引
	inline WORD GetIndex() { return m_wIndex; }
	//获取计数
	inline WORD GetRountID() { return m_wRountID; }
	//获取标识
	inline DWORD GetIdentifierID() { return MAKELONG(m_wIndex, m_wRountID); }

	//客户退出
public:
	//客户推出
	BOOL GetClientQuit() { return m_bClientQuit; }

	//属性函数
public:
	//获取地址
	inline DWORD GetClientAddr() { return m_dwClientAddr; }
	//激活时间
	inline DWORD GetActiveTime() { return m_dwActiveTime; }

	//状态函数
public:
	//群发允许
	inline bool IsAllowBatch() { return m_bAllowBatch; }
	//判断连接
	inline bool IsValidSocket() { return (m_hSocketHandle != INVALID_SOCKET); }

	//管理函数
public:
	//恢复数据
	VOID ResumeData();
	//绑定对象
	VOID AttachData(SOCKET hSocket, DWORD dwListenPort, DWORD dwClientAddr);
	//绑定标识
	VOID AttachClientPort(CHAR * szClientPort, CHAR * szClientAddr);

	//发送函数
public:
	//发送函数
	bool SendData(WORD wMainCmdID, WORD wSubCmdID, WORD wRountID);
	//发送函数
	bool SendData(VOID * pData, WORD wDataSize, WORD wMainCmdID, WORD wSubCmdID, WORD wRountID);

	//控制函数
public:
	//接收操作
	bool RecvData();
	//关闭连接
	bool CloseSocket(WORD wRountID);
	//关闭接收
	bool ShutDownSocket(WORD wRountID);

	//重置收发
public:
	//重置收发
	bool ResetSendRecv();

	//状态管理
public:
	//允许群发
	bool AllowBatchSend(WORD wRountID, bool bAllowBatch);

	//通知接口
public:
	//发送完成
	bool OnSendCompleted(COverLappedSend * pOverLappedSend, DWORD dwThancferred);
	//接收完成
	bool OnRecvCompleted(COverLappedRecv * pOverLappedRecv, DWORD dwThancferred);
	//关闭完成
	bool OnCloseCompleted();

	//辅助函数
private:
	//发送判断
	bool SendVerdict(WORD wRountID);
	//发送数据
	bool SendDataBuffer(VOID * pBuffer, WORD wSendSize);
	//获取缓存
	COverLappedSend * GetSendOverLapped(WORD wPacketSize);
};

//////////////////////////////////////////////////////////////////////////

//读写线程类
class CTCPNetworkThreadReadWrite : public CWHThread
{
	//友元定义
	friend class CTCPNetworkEngine;

	//变量定义
protected:
	HANDLE									m_hCompletionPort;				//完成端口

	//函数定义
public:
	//构造函数
	CTCPNetworkThreadReadWrite();
	//析构函数
	virtual ~CTCPNetworkThreadReadWrite();

	//重载函数
private:
	//运行函数
	virtual bool OnEventThreadRun();
};

//////////////////////////////////////////////////////////////////////////

//应答线程对象
class CTCPNetworkThreadAccept : public CWHThread
{
	//友元定义
	friend class CTCPNetworkEngine;

	//变量定义
protected:
	SOCKET									m_hServerSocket;				//监听连接
	HANDLE									m_hCompletionPort;				//完成端口
	CTCPNetworkEngine *						m_pTCPNetworkEngine;			//网络引擎

	//函数定义
public:
	//构造函数
	CTCPNetworkThreadAccept();
	//析构函数
	virtual ~CTCPNetworkThreadAccept();

	//重载函数
private:
	//运行函数
	virtual bool OnEventThreadRun();
};

//////////////////////////////////////////////////////////////////////////

//检测线程类
class CTCPNetworkThreadDetect : public CWHThread
{
	//友元定义
	friend class CTCPNetworkEngine;

	//变量定义
protected:
	DWORD									m_dwPileTime;					//积累时间
	DWORD									m_dwDetectTime;					//检测时间
	CTCPNetworkEngine *						m_pTCPNetworkEngine;			//网络引擎

	//函数定义
public:
	//构造函数
	CTCPNetworkThreadDetect();
	//析构函数
	virtual ~CTCPNetworkThreadDetect();

	//重载函数
private:
	//运行函数
	virtual bool OnEventThreadRun();
};

//////////////////////////////////////////////////////////////////////////

//类说明
typedef CWHArray<CTCPNetworkSocket *> CTCPNetworkItemPtrArray;
typedef CWHArray<CTCPNetworkThreadReadWrite *> CTCPNetworkThreadRWPtrArray;
typedef std::unordered_map<WORD, CTCPNetworkSocket *> CTCPNetworkItemPtrMap;

//网络引擎
class CTCPNetworkEngine : public ITCPNetworkEngine, public IAsynchronismEngineSink, public ITCPNetworkSocketSink
{
	friend class CTCPNetworkThreadDetect;
	friend class CTCPNetworkThreadAccept;
	friend class CTCPNetworkThreadReadWrite;

	//状态变量
protected:
	bool								m_bService;							//服务标志
	DWORD								m_wCurrentPort;						//当前端口

	//配置变量
protected:
	DWORD								m_wMaxConnect;						//最大连接
	DWORD								m_wServicePort;						//监听端口

	//内核变量
protected:
	SOCKET								m_hServerSocket;					//连接句柄
	HANDLE								m_hCompletionPort;					//完成端口
	ITCPNetworkEngineEvent *			m_pITCPNetworkEngineEvent;			//事件接口

	//回调互斥
protected:
	CCriticalSection					m_TCPNetworkEngineSection;			//回调互斥

	//子项变量
protected:
	CCriticalSection					m_NetworkItemSection;				//子项锁定
	CTCPNetworkItemPtrMap				m_NetworkItemActiveMap;				//活跃连接
	CTCPNetworkItemPtrMap				m_NetworkItemFreeMap;				//空闲连接

	//组件变量
protected:
	CAsynchronismEngine					m_AsynchronismEngine;				//异步对象
	CTCPNetworkThreadDetect				m_SocketDetectThread;				//检测线程
	CTCPNetworkThreadAccept				m_SocketAcceptThread;				//应答线程
	CTCPNetworkThreadRWPtrArray			m_SocketRWThreadArray;				//读写线程

	//函数定义
public:
	//构造函数
	CTCPNetworkEngine();
	//析构函数
	virtual ~CTCPNetworkEngine();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//服务接口
public:
	//启动服务
	virtual bool StartService();
	//停止服务
	virtual bool ConcludeService();

	//信息接口
public:
	//配置端口
	virtual DWORD GetServicePort() { return m_wServicePort; }
	//当前端口
	virtual DWORD GetCurrentPort() { return m_wCurrentPort; }

	//配置接口
public:
	//设置接口
	virtual bool SetTCPNetworkEngineEvent(IUnknownEx * pIUnknownEx);
	//设置参数
	virtual bool SetServiceParameter(WORD wServicePort, WORD wMaxConnect, LPCTSTR pszCompilation);

	//发送接口
public:
	//发送函数
	virtual bool SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID);
	//发送函数
	virtual bool SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//批量发送
	virtual bool SendDataBatch(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, BYTE cbBatchMask);

	//控制接口
public:
	//关闭连接
	virtual bool CloseSocket(DWORD dwSocketID);
	//设置关闭
	virtual bool ShutDownSocket(DWORD dwSocketID);
	//允许群发
	virtual bool AllowBatchSend(DWORD dwSocketID, bool bAllowBatch, BYTE cbBatchMask);

	//异步接口
public:
	//启动时间
	virtual bool OnAsynchronismEngineStart() { return true; }
	//停止事件
	virtual bool OnAsynchronismEngineConclude() { return true; }
	//异步数据
	virtual bool OnAsynchronismEngineData(WORD wIdentifier, VOID * pData, WORD wDataSize);

	//内部通知
public:
	//绑定事件
	virtual bool OnEventSocketBind(CTCPNetworkSocket * pTCPNetworkSocket);
	//关闭事件
	virtual bool OnEventSocketShut(CTCPNetworkSocket * pTCPNetworkSocket);
	//读取事件
	virtual bool OnEventSocketRead(TCP_Command Command, VOID * pData, WORD wDataSize, CTCPNetworkSocket * pTCPNetworkSocket);

	//辅助函数
private:
	//检测连接
	bool DetectSocket();
	//创建网络
	bool CreateSocket(DWORD wServicePort);

	//对象管理
protected:
	//激活空闲对象
	CTCPNetworkSocket * ActiveNetworkItem();
	//获取对象
	CTCPNetworkSocket * GetNetworkItem(WORD wIndex);
	//释放连接对象
	bool FreeNetworkItem(CTCPNetworkSocket * pTCPNetworkSocket);
};

//////////////////////////////////////////////////////////////////////////

#endif
