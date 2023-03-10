#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"
#include <fstream>
#include <assert.h>

//超时时间
#define IDI_DEL_OFFLINE_USER_TIME	60 * 60								//用户离线

//构造函数
CAttemperEngineSink::CAttemperEngineSink()
{
	//状态变量
	m_pInitParameter = NULL;
	m_pBindParameter = NULL;

	//组件变量
	m_pITimerEngine = NULL;
	m_pIDataBaseEngine = NULL;
	m_pITCPNetworkEngine = NULL;
	return;
}

//析构函数
CAttemperEngineSink::~CAttemperEngineSink()
{

}

//接口查询
VOID * CAttemperEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAttemperEngineSink, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink, Guid, dwQueryVer);
	return NULL;
}

//启动事件
bool CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	//绑定参数
	m_pBindParameter = new tagBindParameter[m_pInitParameter->m_wMaxConnect];
	ZeroMemory(m_pBindParameter, sizeof(tagBindParameter)*m_pInitParameter->m_wMaxConnect);
	
	//运行标志
	m_bRuning = TRUE;

	//设置引擎
	m_DataUserManager.SetDataBaseEngine(m_pIDataBaseEngine);

	//远程组件
	m_RpcServerEngine.StartService();

	//绑定用户
	m_RpcServerEngine.bind("UserOnline", &CAttemperEngineSink::UserOnline, this);
	m_RpcServerEngine.bind("UserOffline", &CAttemperEngineSink::UserOffline, this);

	m_RpcServerEngine.bind("GetUserID", &CDataUserManager::GetUserID, &m_DataUserManager);
	m_RpcServerEngine.bind("DelUserID", &CDataUserManager::DelUserID, &m_DataUserManager);

	m_RpcServerEngine.bind("AddUserInfo", &CDataUserManager::AddUserInfo, &m_DataUserManager);
	m_RpcServerEngine.bind("GetUserInfo", &CDataUserManager::GetUserInfo, &m_DataUserManager);
	m_RpcServerEngine.bind("DelUserInfo", &CDataUserManager::DelUserInfo, &m_DataUserManager);
	m_RpcServerEngine.bind("ModifyUserInfo", &CDataUserManager::ModifyUserInfo, &m_DataUserManager);

	//界面刷新
	CP_ControlResult ControlResult;
	ControlResult.cbSuccess = ER_SUCCESS;
	SendUIControlPacket(UI_CORRESPOND_RESULT, &ControlResult, sizeof(ControlResult));
	return true;
}

//停止事件
bool CAttemperEngineSink::OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)
{
	//运行标志
	m_bRuning = FALSE;

	//停止组件
	m_RpcServerEngine.ConcludeService();

	//组件变量
	m_pITimerEngine = NULL;
	m_pIDataBaseEngine = NULL;
	m_pITCPNetworkEngine = NULL;

	//删除数据
	SafeDeleteArray(m_pBindParameter);

	//关闭句柄
	closesocket(m_sListenSocket);

	return true;
}

//控制事件
bool CAttemperEngineSink::OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	switch (wIdentifier)
	{
	default:
		break;
	}

	return false;
}

//调度事件
bool CAttemperEngineSink::OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//时间事件
bool CAttemperEngineSink::OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{
	//定时类型
	if (dwTimerID == IDI_DEL_OFFLINE_USER_TIME)
	{
		//删除离线
		DWORD dwUserID = (DWORD)dwBindParameter;
		m_DataUserManager.DelOfflineUser(dwUserID);
	}

	return true;
}

//应答事件
bool CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID)
{
	//获取索引
	ASSERT(LOWORD(dwSocketID) < m_pInitParameter->m_wMaxConnect);
	if (LOWORD(dwSocketID) >= m_pInitParameter->m_wMaxConnect) return false;

	//变量定义
	WORD wBindIndex = LOWORD(dwSocketID);
	tagBindParameter * pBindParameter = (m_pBindParameter + wBindIndex);

	//设置变量
	pBindParameter->dwSocketID = dwSocketID;
	pBindParameter->dwClientAddr = dwClientAddr;
	pBindParameter->dwActiveTime = (DWORD)time(NULL);

	return true;
}

//关闭事件
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID, BOOL bClientQuit)
{
	//清除信息
	WORD wBindIndex = LOWORD(dwSocketID);
	ZeroMemory((m_pBindParameter + wBindIndex), sizeof(tagBindParameter));

	return false;
}

//读取事件
bool CAttemperEngineSink::OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	return false;
}

//读取事件
bool CAttemperEngineSink::OnEventTCPNetworkCopy(DWORD dwSrcSocketID, DWORD dwDstSocketID)
{
	return true;
}

//数据库事件
bool CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	default:
		break;
	}
	return false;
}

//关闭事件
bool CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	return true;
}

//连接事件
bool CAttemperEngineSink::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)
{
	return true;
}

//读取事件
bool CAttemperEngineSink::OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	return true;
}

//发送请求
bool CAttemperEngineSink::SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	//发送数据
	CServiceUnits * pServiceUnits = CServiceUnits::g_pServiceUnits;
	pServiceUnits->PostControlRequest(wRequestID, pData, wDataSize);

	return true;
}

//用户上线
void CAttemperEngineSink::UserOnline(DWORD dwUserID)
{
	m_pITimerEngine->KillTimer(dwUserID);
}

//用户下线
void CAttemperEngineSink::UserOffline(DWORD dwUserID)
{
	m_pITimerEngine->SetTimer(dwUserID, IDI_DEL_OFFLINE_USER_TIME * 1000, 1, dwUserID);
}

//////////////////////////////////////////////////////////////////////////////////
