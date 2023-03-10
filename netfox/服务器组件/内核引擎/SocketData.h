#ifndef SOCKET_DATA_HEAD_FILE
#define SOCKET_DATA_HEAD_FILE

#pragma once

#include "KernelEngineHead.h"

//////////////////////////////////////////////////////////////////////////

//数据信息
struct tagTCPData
{
	//消息类型
	WORD						wMsgType;							//消息类型

	//命令信息
	TCP_Command					Command;							//网络命令

	//数据信息
	WORD						wDataSize;							//数据大小
	VOID *						pDataBuffer;						//数据指针
};

//数据信息
struct tagUDPData
{
	//消息类型
	WORD						wMsgType;							//消息类型

	//命令信息
	UDP_Command					Command;							//网络命令

	//数据信息
	WORD						wDataSize;							//数据大小
	VOID *						pDataBuffer;						//数据指针
};

//////////////////////////////////////////////////////////////////////////

//数据处理
class CTCPSocketData
{
	//变量定义
public:
	WORD						m_wBufferSize;						//数据长度
	BYTE						m_cbDataBuffer[SOCKET_TCP_BUFFER];	//数据缓存

	//函数定义
public:
	//构造函数
	CTCPSocketData();
	//析构函数
	virtual ~CTCPSocketData();

	//数据设置
public:
	//设置数据
	bool InitSocketData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, ULONGLONG ulCmdNo = 0, DWORD dwPort = 0);
	//设置数据
	bool SetBufferData(LPBYTE pcbBuffer, WORD wBufferSize, tagTCPData & TCPData);
	//重置数据
	bool ResetBufferData();

	//映射数据
private:
	//映射数据
	bool MappedBuffer();
	//映射数据
	bool UnMappedBuffer();
};

//////////////////////////////////////////////////////////////////////////

//数据处理
class CUDPSocketData
{
	//变量定义
public:
	WORD						m_wBufferSize;						//数据长度
	BYTE						m_cbDataBuffer[SOCKET_UDP_BUFFER];	//数据缓存

	//函数定义
public:
	//构造函数
	CUDPSocketData();
	//析构函数
	virtual ~CUDPSocketData();

	//数据设置
public:
	//设置数据
	bool InitSocketData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//设置数据
	bool SetBufferData(LPBYTE pcbBuffer, WORD wBufferSize, tagUDPData & UDPData);
	//重置数据
	bool ResetBufferData();
};

//////////////////////////////////////////////////////////////////////////

#endif