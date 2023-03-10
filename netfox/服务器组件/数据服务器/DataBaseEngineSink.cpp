#include "StdAfx.h"
#include "ServiceUnits.h"
#include "DataBaseEngineSink.h"
#include<fstream>
#include <fstream>

//////////////////////////////////////////////////////////////////////////////////
using namespace std;
CCriticalSection g_lockBank;
//构造函数
CDataBaseEngineSink::CDataBaseEngineSink()
{
	//组件变量
	m_pInitParameter = NULL;
	m_pDataBaseParameter = NULL;
	m_pIDataBaseEngineEvent = NULL;

	return;
}

//析构函数
CDataBaseEngineSink::~CDataBaseEngineSink()
{

}

//接口查询
VOID * CDataBaseEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseEngineSink, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngineSink, Guid, dwQueryVer);
	return NULL;
}

//启动事件
bool CDataBaseEngineSink::OnDataBaseEngineStart(IUnknownEx * pIUnknownEx)
{
	//创建对象
	if ((m_AccountsDBModule.GetInterface() == NULL) && (m_AccountsDBModule.CreateInstance() == false))
	{
		ASSERT(FALSE);
		return false;
	}

	//创建对象
	if ((m_TreasureDBModule.GetInterface() == NULL) && (m_TreasureDBModule.CreateInstance() == false))
	{
		ASSERT(FALSE);
		return false;
	}

	//创建对象
	if ((m_PlatformDBModule.GetInterface() == NULL) && (m_PlatformDBModule.CreateInstance() == false))
	{
		ASSERT(FALSE);
		return false;
	}

	//创建对象
	if ((m_GameDBSlaveModule.GetInterface() == NULL) && (m_GameDBSlaveModule.CreateInstance() == false))
	{
		ASSERT(FALSE);
		return false;
	}

	//创建对象
	if ((m_TreasureDBSlaveModule.GetInterface() == NULL) && (m_TreasureDBSlaveModule.CreateInstance() == false))
	{
		ASSERT(FALSE);
		return false;
	}

	try
	{
		//主库链接
		{
			//连接信息
			tagDataBaseParameter * pAccountsDBParameter = &m_pInitParameter->m_AccountsDBParameter;
			tagDataBaseParameter * pTreasureDBParameter = &m_pInitParameter->m_TreasureDBParameter;
			tagDataBaseParameter * pPlatformDBParameter = &m_pInitParameter->m_PlatformDBParameter;

			//设置连接
			m_AccountsDBModule->SetConnectionInfo(pAccountsDBParameter->szDataBaseAddr, pAccountsDBParameter->wDataBasePort,
				pAccountsDBParameter->szDataBaseName, pAccountsDBParameter->szDataBaseUser, pAccountsDBParameter->szDataBasePass);
			m_TreasureDBModule->SetConnectionInfo(pTreasureDBParameter->szDataBaseAddr, pTreasureDBParameter->wDataBasePort,
				pTreasureDBParameter->szDataBaseName, pTreasureDBParameter->szDataBaseUser, pTreasureDBParameter->szDataBasePass);
			m_PlatformDBModule->SetConnectionInfo(pPlatformDBParameter->szDataBaseAddr, pPlatformDBParameter->wDataBasePort,
				pPlatformDBParameter->szDataBaseName, pPlatformDBParameter->szDataBaseUser, pPlatformDBParameter->szDataBasePass);

			//发起连接
			m_AccountsDBModule->OpenConnection();
			m_AccountsDBAide.SetDataBase(m_AccountsDBModule.GetInterface());

			//发起连接
			m_TreasureDBModule->OpenConnection();
			m_TreasureDBAide.SetDataBase(m_TreasureDBModule.GetInterface());

			//发起连接
			m_PlatformDBModule->OpenConnection();
			m_PlatformDBAide.SetDataBase(m_PlatformDBModule.GetInterface());
		}

		//从库连接
		{
			//从库配置信息
			int m_nSqlDataIpCount = 0;
			TCHAR szSqlDataSetting[256];
			stuDataBaseSetting	sutDataBaseSetting[10];
			ZeroMemory(sutDataBaseSetting, sizeof(sutDataBaseSetting));
			ZeroMemory(szSqlDataSetting, sizeof(szSqlDataSetting));
			TCHAR module_name[MAX_PATH];
			GetModuleFileName(NULL, module_name, MAX_PATH);
			CString strDir;
			strDir.Format(TEXT("%s"), module_name);
			int nSlipIndex = strDir.ReverseFind('\\');
			CString strWorkDir = strDir.Left(nSlipIndex);
			_stprintf_s(szSqlDataSetting, TEXT("%s\\SqlDataSetting.ini"), strWorkDir.GetBuffer(0));
			m_nSqlDataIpCount = GetPrivateProfileInt(TEXT("SqlDataSetting"), TEXT("SetCount"), 0, szSqlDataSetting);
			for (int i = 0; i < m_nSqlDataIpCount; ++i)
			{
				TCHAR szAppName[256] = { 0 };
				_stprintf_s(szAppName, TEXT("Set_%d"), i);
				GetPrivateProfileString(szAppName, TEXT("SetDBAddr"), 0, sutDataBaseSetting[i].szDBAddr, 256, szSqlDataSetting);
				sutDataBaseSetting[i].wPort = GetPrivateProfileInt(szAppName, TEXT("SetPort"), 1433, szSqlDataSetting);
				GetPrivateProfileString(szAppName, TEXT("SetUser"), 0, sutDataBaseSetting[i].szUser, 256, szSqlDataSetting);
				GetPrivateProfileString(szAppName, TEXT("SetPassword"), 0, sutDataBaseSetting[i].szPassword, 256, szSqlDataSetting);
			}

			m_TreasureDBSlaveModule->SetConnectionInfo(sutDataBaseSetting[1].szDBAddr, sutDataBaseSetting[1].wPort, TEXT("QPTreasureDB"), sutDataBaseSetting[1].szUser, sutDataBaseSetting[1].szPassword, false);
			m_GameDBSlaveModule->SetConnectionInfo(sutDataBaseSetting[0].szDBAddr, sutDataBaseSetting[1].wPort, TEXT("QPTreasureDB"), sutDataBaseSetting[0].szUser, sutDataBaseSetting[0].szPassword, false);

			m_GameDBSlaveModule->OpenConnection();
			m_GameDBSalveAide.SetDataBase(m_GameDBSlaveModule.GetInterface());

			m_TreasureDBSlaveModule->OpenConnection();
			m_TreasureDBSlaveAide.SetDataBase(m_TreasureDBSlaveModule.GetInterface());
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//错误信息
		LPCTSTR pszDescribe = pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe, TraceLevel_Exception);

		return false;
	}

	return true;
}

//停止事件
bool CDataBaseEngineSink::OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx)
{
	//设置对象
	m_AccountsDBAide.SetDataBase(NULL);
	m_TreasureDBAide.SetDataBase(NULL);
	m_PlatformDBAide.SetDataBase(NULL);

	//关闭连接
	if (m_AccountsDBModule.GetInterface() != NULL)
	{
		m_AccountsDBModule->CloseConnection();
		m_AccountsDBModule.CloseInstance();
	}

	//关闭连接
	if (m_TreasureDBModule.GetInterface() != NULL)
	{
		m_TreasureDBModule->CloseConnection();
		m_TreasureDBModule.CloseInstance();
	}

	//关闭连接
	if (m_PlatformDBModule.GetInterface() != NULL)
	{
		m_PlatformDBModule->CloseConnection();
		m_PlatformDBModule.CloseInstance();
	}

	//组件变量
	m_pInitParameter = NULL;
	m_pIDataBaseEngineEvent = NULL;

	return true;
}

//时间事件
bool CDataBaseEngineSink::OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{
	return false;
}

//控制事件
bool CDataBaseEngineSink::OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize)
{
	return false;
}

//请求事件
bool CDataBaseEngineSink::OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//变量定义
	DWORD dwUserID = 0L;

	switch (wRequestID)
	{
	case DBR_GP_SAVE_USER_DATA:	//同步数据
	{
		return OnRequestSaveUserData(wRequestID, pData, wDataSize, dwUserID);
	}
	default:
		break;
	}
	return false;
}

//同步数据
bool CDataBaseEngineSink::OnRequestSaveUserData(DWORD dwContextID, VOID * pData, WORD wDataSize, DWORD & dwUserID)
{
	DBR_GR_SaveUserData * pSaveUserData = (DBR_GR_SaveUserData *)pData;

	try
	{
		switch (pSaveUserData->dwDataBase)
		{
		case AccountDB:
		{
			m_AccountsDBModule->ExecuteSentence(pSaveUserData->szSql, true);
			return true;
		}
		case PlatformDB:
		{
			m_PlatformDBModule->ExecuteSentence(pSaveUserData->szSql, true);
			return true;
		}
		case TreasureDB:
		{
			m_TreasureDBModule->ExecuteSentence(pSaveUserData->szSql, true);
			return true;
		}
		default:
			break;
		}
	}
	catch (IDataBaseException * pIException)
	{
		//输出错误
		CTraceService::TraceString(pIException->GetExceptionDescribe(), TraceLevel_Exception);
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////////
