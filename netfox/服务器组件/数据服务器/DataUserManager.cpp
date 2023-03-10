#include "StdAfx.h"
#include "DataUserManager.h"

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDataUserManager::CDataUserManager()
{
	return;
}

//析构函数
CDataUserManager::~CDataUserManager()
{
	return;
}

//数据接口
void CDataUserManager::SetDataBaseEngine(IDataBaseEngine * pIDataBaseEngine)
{
	m_pIDataBaseEngine = pIDataBaseEngine;
}

//删除离线
void CDataUserManager::DelOfflineUser(DWORD dwUserID)
{
	//查找数据
	tagCacheUserInfo userInfo = GetUserInfo(dwUserID);

	//清理数据
	DelUserID(userInfo.szUserKey);
	DelUserInfo(dwUserID);
}


//查找用户
DWORD CDataUserManager::GetUserID(CString szUserKey)
{
	CKeyMap::iterator keyIter = m_UserKeyId.find(szUserKey);
	if (keyIter != m_UserKeyId.end()) {
		return keyIter->second;
	}
	return 0;
}

//删除用户
DWORD CDataUserManager::DelUserID(CString szUserKey)
{
	CKeyMap::iterator keyIter = m_UserKeyId.find(szUserKey);
	if (keyIter != m_UserKeyId.end()) {
		CUserMap::iterator userIter = m_UserInfoMap.find(keyIter->second);
		if (userIter != m_UserInfoMap.end()) {
			m_UserInfoMap.erase(userIter);
		}
		m_UserKeyId.erase(keyIter);
	}
	return 0;
}

//推送日志
void CDataUserManager::PushUserSql(enCacheDataBase database, enCacheSqlType sqlType, CString sql)
{
	if (database != enCacheDataBase::DefaultDB && sqlType != enCacheSqlType::Default && sql != "") {
		DBR_GR_SaveUserData saveUserData;

		saveUserData.dwDataBase = database;
		saveUserData.dwType = sqlType;
		swprintf_s(saveUserData.szSql, TEXT("%s"), sql.GetBuffer(0));

		if (m_pIDataBaseEngine) {
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_SAVE_USER_DATA, 0, &saveUserData, sizeof(saveUserData));
		}
	}
}

//查找用户
tagCacheUserInfo CDataUserManager::GetUserInfo(DWORD dwUserID)
{
	CUserMap::iterator keyIter = m_UserInfoMap.find(dwUserID);
	if (keyIter == m_UserInfoMap.end()) {
		tagCacheUserInfo cacheUserInfo;
		return cacheUserInfo;
	}
	else {
		return m_UserInfoMap[dwUserID];
	}
}

//增加用户
bool CDataUserManager::AddUserInfo(DWORD dwUserID, tagCacheUserInfo userInfo, enCacheDataBase database, enCacheSqlType sqlType, CString sql)
{
	//保存数据
	m_UserKeyId[userInfo.szUserKey] = userInfo.dwUserID;
	m_UserInfoMap[userInfo.dwUserID] = userInfo;

	//保存语句
	PushUserSql(database, sqlType, sql);
	return true;
}

//删除用户
bool CDataUserManager::DelUserInfo(DWORD dwUserID, enCacheDataBase database, enCacheSqlType sqlType, CString sql)
{
	CUserMap::iterator userIter = m_UserInfoMap.find(dwUserID);
	if (userIter != m_UserInfoMap.end()) {
		m_UserInfoMap.erase(userIter);
	}

	//保存语句
	PushUserSql(database, sqlType, sql);
	return true;
}

//修改用户
bool CDataUserManager::ModifyUserInfo(DWORD dwUserID, std::string userJson, enCacheDataBase database, enCacheSqlType sqlType, CString sql)
{
	tagCacheUserInfo userInfo = GetUserInfo(dwUserID);
	if (userInfo.dwUserID != dwUserID) {
		return false;
	}

	TinyJson json;
	json.ReadJson(userJson);

	xarray data = json.Get<xarray>("data");
	for (int i = 0; i < data.Count(); i++) {
		data.Enter(i);
		DWORD key = data.Get<DWORD>("key");
		switch (key)
		{
		case enCacheUserKey::dwGameID: 
			//直接赋值
			userInfo.dwGameID = data.Get<DWORD>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::wFaceID:
			//直接赋值
			userInfo.wFaceID = data.Get<WORD>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::cbGender:
			//直接赋值
			userInfo.cbGender = data.Get<BYTE>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::dwCustomID:
			//直接赋值
			userInfo.dwCustomID = data.Get<DWORD>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::cbMemberOrder:
			//直接赋值
			userInfo.cbMemberOrder = data.Get<BYTE>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::cbMasterOrder:
			//直接赋值
			userInfo.cbMasterOrder = data.Get<BYTE>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::dwUserRight:
			//直接赋值
			userInfo.dwUserRight = data.Get<DWORD>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::dwMasterRight:
			//直接赋值
			userInfo.dwMasterRight = data.Get<DWORD>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::szAccounts:
			//直接赋值
			userInfo.szAccounts = CWHCommon::Utf8ToWChar(data.Get<std::string>("value").c_str());
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::szLogonPass:
			//直接赋值
			userInfo.szLogonPass = CWHCommon::Utf8ToWChar(data.Get<std::string>("value").c_str());
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::szInsurePass:
			//直接赋值
			userInfo.szInsurePass = CWHCommon::Utf8ToWChar(data.Get<std::string>("value").c_str());
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::szNickName:
			//直接赋值
			userInfo.szNickName = CWHCommon::Utf8ToWChar(data.Get<std::string>("value").c_str());
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::szRegisterMobile:
			//直接赋值
			userInfo.szRegisterMobile = CWHCommon::Utf8ToWChar(data.Get<std::string>("value").c_str());
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::szUnderWrite:
			//直接赋值
			userInfo.szUnderWrite = CWHCommon::Utf8ToWChar(data.Get<std::string>("value").c_str());
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::szFaceUrl:
			//直接赋值
			userInfo.szFaceUrl = CWHCommon::Utf8ToWChar(data.Get<std::string>("value").c_str());
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::dwGroupID:
			//直接赋值
			userInfo.dwGroupID = data.Get<DWORD>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::szGroupName:
			//直接赋值
			userInfo.szGroupName = CWHCommon::Utf8ToWChar(data.Get<std::string>("value").c_str());
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::lScore:
			//差量赋值
			userInfo.lScore += data.Get<SCORE>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::lRevenue:
			//差量赋值
			userInfo.lRevenue += data.Get<SCORE>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::lRoomCards:
			//差量赋值
			userInfo.lRoomCards += data.Get<SCORE>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::dwWinCount:
			//差量赋值
			userInfo.dwWinCount += data.Get<DWORD>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::dwLostCount:
			//差量赋值
			userInfo.dwLostCount += data.Get<DWORD>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::dwDrawCount:
			//差量赋值
			userInfo.dwDrawCount += data.Get<DWORD>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::dwFleeCount:
			//差量赋值
			userInfo.dwFleeCount += data.Get<DWORD>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::dwPlayTimeCount:
			//差量赋值
			userInfo.dwPlayTimeCount += data.Get<DWORD>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::dwGameLogonTimes:
			//差量赋值
			userInfo.dwGameLogonTimes += data.Get<DWORD>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::dwRegType:
			//直接赋值
			userInfo.dwRegType = data.Get<DWORD>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::dwChannel:
			//直接赋值
			userInfo.dwChannel = data.Get<DWORD>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::dwFirstCharge:
			//直接赋值
			userInfo.dwFirstCharge = data.Get<DWORD>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::dwIsChangePWD:
			//直接赋值
			userInfo.dwIsChangePWD = data.Get<DWORD>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::wNullity:
			//直接赋值
			userInfo.wNullity = data.Get<WORD>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::wStunDown:
			//直接赋值
			userInfo.wStunDown = data.Get<WORD>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::cbMoorMachine:
			//直接赋值
			userInfo.cbMoorMachine = data.Get<BYTE>("value");
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::szLastLogonMachine:
			//直接赋值
			userInfo.szLastLogonMachine = CWHCommon::Utf8ToWChar(data.Get<std::string>("value").c_str());
			AddUserInfo(dwUserID, userInfo);
			break;
		case enCacheUserKey::szRegisterMachine:
			//直接赋值
			userInfo.szRegisterMachine = CWHCommon::Utf8ToWChar(data.Get<std::string>("value").c_str());
			AddUserInfo(dwUserID, userInfo);
			break;
		default:
			break;
		}
	}

	//保存语句
	PushUserSql(database, sqlType, sql);
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
