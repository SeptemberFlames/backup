#include "Stdafx.h"
#include "SocketData.h"
#include "TraceService.h"

//////////////////////////////////////////////////////////////////////////

//构造函数
CTCPSocketData::CTCPSocketData()
{
	//设置变量
	m_wBufferSize = 0;
	ZeroMemory(m_cbDataBuffer, sizeof(m_cbDataBuffer));

	return;
}

//析构函数
CTCPSocketData::~CTCPSocketData()
{
}

//设置数据
bool CTCPSocketData::InitSocketData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize, ULONGLONG ulCmdNo, DWORD dwPort)
{
	//校验大小
	ASSERT(wDataSize <= SOCKET_TCP_BUFFER);
	if (wDataSize > SOCKET_TCP_BUFFER) return false;

	//变量定义
	TCP_Head * pHead = (TCP_Head *)m_cbDataBuffer;

	//设置命令
	pHead->CommandInfo.wSubCmdID = wSubCmdID;
	pHead->CommandInfo.wMainCmdID = wMainCmdID;

	//设置包头
	pHead->TCPInfo.wDataKind = 0;
	pHead->TCPInfo.wPacketSize = sizeof(TCP_Head) + wDataSize;
	pHead->TCPInfo.dwConnectPort = dwPort;

	//设置数据
	if (wDataSize > 0)
	{
		m_wBufferSize = sizeof(TCP_Head) + wDataSize;
		CopyMemory(m_cbDataBuffer + sizeof(TCP_Head), pData, wDataSize);
	}
	else
	{
		m_wBufferSize = sizeof(TCP_Head);
	}

	//映射数据
	if (MappedBuffer() == false)
	{
		ASSERT(FALSE);
		return false;
	}

	//特殊处理
	if (wMainCmdID == MDM_KN_COMMAND && wSubCmdID == SUB_KN_NETWORK_SHUT)
	{
		//服务完成
		pHead->TCPInfo.wMsgType = MSG_TYPE_SERVER_FINISH;
	}
	else
	{
		//正常消息
		pHead->TCPInfo.wMsgType = MSG_TYPE_NORMAL_DATA;
	}

	pHead->TCPInfo.wEnPacketSize = htons(m_wBufferSize - 2);
	pHead->TCPInfo.ulCmdNo = ulCmdNo;

	return true;
}

//设置数据
bool CTCPSocketData::SetBufferData(LPBYTE pcbBuffer, WORD wBufferSize, tagTCPData & TCPData)
{
	//数据校验
	ASSERT(wBufferSize > sizeof(TCP_Info) && (wBufferSize <= SOCKET_TCP_BUFFER));
	if (wBufferSize <= sizeof(TCP_Info) || (wBufferSize > SOCKET_TCP_BUFFER)) return false;

	//设置数据
	m_wBufferSize = wBufferSize;
	CopyMemory(m_cbDataBuffer, pcbBuffer, wBufferSize);

	//校验数据
	ASSERT(((TCP_Info *)m_cbDataBuffer)->wPacketSize == wBufferSize);
	if (((TCP_Info *)m_cbDataBuffer)->wPacketSize != wBufferSize) return false;

	//映射数据
	if (UnMappedBuffer() == false)
	{
		ASSERT(FALSE);
		return false;
	}

	//变量定义
	TCP_Head * pHead = (TCP_Head *)m_cbDataBuffer;

	//设置数据
	TCPData.wMsgType = pHead->TCPInfo.wMsgType;
	TCPData.Command = pHead->CommandInfo;
	TCPData.wDataSize = m_wBufferSize - sizeof(TCP_Head);
	TCPData.pDataBuffer = m_cbDataBuffer + sizeof(TCP_Head);

	return true;
}

bool CTCPSocketData::ResetBufferData()
{
	ZeroMemory(m_cbDataBuffer, sizeof(m_cbDataBuffer));
	m_wBufferSize = 0;
	return true;
}

//映射数据
bool CTCPSocketData::MappedBuffer()
{
	//变量定义
	TCP_Info * pInfo = (TCP_Info *)m_cbDataBuffer;

	//字节映射
	for (WORD i = sizeof(TCP_Info); i < m_wBufferSize; i++)
	{
		m_cbDataBuffer[i] = g_SendByteMap[m_cbDataBuffer[i]];
	}

	//设置包头
	pInfo->wDataKind |= DK_MAPPED;

	return true;
}

//映射数据
bool CTCPSocketData::UnMappedBuffer()
{
	//校验参数
	ASSERT(m_wBufferSize >= sizeof(TCP_Head));
	ASSERT(((TCP_Head *)m_cbDataBuffer)->TCPInfo.wPacketSize == m_wBufferSize);

	//校验数据
	if (m_wBufferSize < sizeof(TCP_Head)) return false;
	if (((TCP_Head *)m_cbDataBuffer)->TCPInfo.wPacketSize != m_wBufferSize) return false;

	//变量定义
	TCP_Info * pInfo = (TCP_Info *)m_cbDataBuffer;

	//映射数据
	if ((pInfo->wDataKind&DK_MAPPED) != 0)
	{
		//校验字节
		for (INT i = sizeof(TCP_Info); i < m_wBufferSize; i++)
		{
			m_cbDataBuffer[i] = g_RecvByteMap[m_cbDataBuffer[i]];
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CUDPSocketData::CUDPSocketData()
{
	//设置变量
	m_wBufferSize = 0;
	ZeroMemory(m_cbDataBuffer, sizeof(m_cbDataBuffer));

	return;
}

//析构函数
CUDPSocketData::~CUDPSocketData()
{
}

//设置数据
bool CUDPSocketData::InitSocketData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//校验大小
	ASSERT(wDataSize <= SOCKET_UDP_BUFFER);
	if (wDataSize > SOCKET_UDP_BUFFER) return false;

	//变量定义
	UDP_Head * pHead = (UDP_Head *)m_cbDataBuffer;

	//设置命令
	pHead->CommandInfo.wSubCmdID = wSubCmdID;
	pHead->CommandInfo.wMainCmdID = wMainCmdID;

	//设置包头
	pHead->UDPInfo.cbDataKind = 0;
	pHead->UDPInfo.cbCheckCode = 0;
	pHead->UDPInfo.wPacketSize = sizeof(UDP_Head) + wDataSize;

	//设置数据
	if (wDataSize > 0)
	{
		m_wBufferSize = sizeof(UDP_Head) + wDataSize;
		CopyMemory(m_cbDataBuffer + sizeof(UDP_Head), pData, wDataSize);
	}
	else
	{
		m_wBufferSize = sizeof(UDP_Head);
	}

	return true;
}

//设置数据
bool CUDPSocketData::SetBufferData(LPBYTE pcbBuffer, WORD wBufferSize, tagUDPData & UDPData)
{
	//数据校验
	ASSERT(wBufferSize > sizeof(UDP_Info) && (wBufferSize <= SOCKET_UDP_BUFFER));
	if (wBufferSize <= sizeof(UDP_Info) || (wBufferSize > SOCKET_UDP_BUFFER)) return false;

	//设置数据
	m_wBufferSize = wBufferSize;
	CopyMemory(m_cbDataBuffer, pcbBuffer, wBufferSize);

	//校验数据
	ASSERT(((UDP_Info *)m_cbDataBuffer)->wPacketSize == wBufferSize);
	if (((UDP_Info *)m_cbDataBuffer)->wPacketSize != wBufferSize) return false;

	//变量定义
	UDP_Head * pHead = (UDP_Head *)m_cbDataBuffer;

	//设置数据
	UDPData.wMsgType = pHead->UDPInfo.wMsgType;
	UDPData.Command = pHead->CommandInfo;
	UDPData.wDataSize = m_wBufferSize - sizeof(UDP_Head);
	UDPData.pDataBuffer = m_cbDataBuffer + sizeof(UDP_Head);

	return true;
}

//重置数据
bool CUDPSocketData::ResetBufferData()
{
	ZeroMemory(m_cbDataBuffer, sizeof(m_cbDataBuffer));
	m_wBufferSize = 0;
	return true;
}

//////////////////////////////////////////////////////////////////////////