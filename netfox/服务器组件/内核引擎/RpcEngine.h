#ifndef RPC_ENGINE_HEAD_HEAD_FILE
#define RPC_ENGINE_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//包含文件

#pragma once

#include "Serializer.hpp"

//////////////////////////////////////////////////////////////////////////////////

//数据库名
enum enCacheDataBase
{
	DefaultDB = 0,														//默认库名
	AccountDB,															//主账号库
	AccountDBSlave,														//从账号库
	TreasureDB,															//主金币库
	TreasureDBSlave,													//从金币库
	PlatformDB,															//主平台库
	PlatformDBSlave,													//从平台库
};

//语句类型
enum enCacheSqlType
{
	Default = 0,														//默认执行
	Sentence,															//语句执行
	Process,															//过程执行
};

//服务状态
enum enCacheUserKey
{
	//基本信息
	dwUserID = 0,														//用户标识
	dwGameID,															//游戏标识
	wFaceID,															//头像标识
	cbGender,															//用户性别
	dwCustomID,															//自定索引
	cbMemberOrder,														//会员等级
	cbMasterOrder,														//管理等级

	//用户权限
	dwUserRight,														//会员权限
	dwMasterRight,														//管理权限

	//登录信息
	szAccounts,															//登录帐号
	szLogonPass,														//登录密码
	szInsurePass,														//银行密码
	szNickName,															//用户昵称
	szRegisterMobile,													//手机号码
	szUnderWrite,														//个性签名
	szFaceUrl,															//网络头像

	//社团信息
	dwGroupID,															//社团标识
	szGroupName,														//社团名字

	//用户成绩
	lScore,																//用户金币
	lGrade,																//用户成绩
	lRevenue,															//用户税收
	lRoomCards,															//房卡数量

	//输赢信息
	dwWinCount,															//胜利盘数
	dwLostCount,														//失败盘数
	dwDrawCount,														//和局盘数
	dwFleeCount,														//逃跑盘数
	dwPlayTimeCount,													//游戏时长

	//扩展信息
	dwRegType,															//注册类型
	dwChannel,															//登录渠道
	dwGameLogonTimes,													//登录次数
	dwFirstCharge,														//首次充值
	dwIsChangePWD,														//修改密码

	//状态信息
	wNullity,															//帐号禁止
	wStunDown,															//帐号关闭
	cbMoorMachine,														//固定机器
	szLastLogonMachine,													//最后机器

	//注册机器
	szRegisterMachine,													//注册机器

	//游戏信息
	dwKindID,															//所在游戏
	dwServerID,															//所在房间
};

//缓存用户
struct tagCacheUserInfo
{
	//主键信息
	CString							szUserKey;							//用户主键

	//基本信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	WORD							wFaceID;							//头像标识
	BYTE							cbGender;							//用户性别
	DWORD							dwCustomID;							//自定索引
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级

	//用户权限
	DWORD                           dwUserRight;						//会员权限
	DWORD							dwMasterRight;						//管理权限

	//登录信息
	CString							szAccounts;							//登录帐号
	CString							szLogonPass;						//登录密码
	CString							szInsurePass;						//银行密码
	CString							szNickName;							//用户昵称
	CString							szRegisterMobile;					//手机号码
	CString							szUnderWrite;						//个性签名
	CString							szFaceUrl;							//网络头像

	//社团信息
	DWORD							dwGroupID;							//社团标识
	CString							szGroupName;						//社团名字

	//用户成绩
	SCORE							lScore;								//用户金币
	SCORE							lRevenue;							//用户税收
	SCORE							lRoomCards;							//房卡数量

	//额外信息
	DWORD							dwExperience;						//经验数值
	DWORD							dwUserMedal;						//用户奖牌
	LONG							lLoveLiness;						//用户魅力

	//输赢信息
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数
	DWORD							dwPlayTimeCount;					//游戏时长

	//扩展信息
	DWORD							dwGameLogonTimes;					//登录次数
	DWORD							dwRegType;							//注册类型
	DWORD							dwChannel;							//登录渠道
	DWORD							dwFirstCharge;						//首次充值
	DWORD							dwIsChangePWD;						//修改密码

	//状态信息
	WORD							wNullity;							//帐号禁止
	WORD							wStunDown;							//帐号关闭
	BYTE							cbMoorMachine;						//固定机器
	CString							szLastLogonMachine;					//最后机器

	//注册机器
	CString							szRegisterMachine;					//注册机器	

	//游戏信息
	DWORD							dwKindID;							//所在游戏
	DWORD							dwServerID;							//所在房间

	//无参构造
	tagCacheUserInfo()
	{
		szUserKey = TEXT("");

		dwUserID = 0;
		dwGameID = 0;
		wFaceID = 0;
		cbGender = 0;
		dwCustomID = 0;
		cbMemberOrder = 0;
		cbMasterOrder = 0;

		dwUserRight = 0;
		dwMasterRight = 0;

		szAccounts = TEXT("");
		szLogonPass = TEXT("");
		szInsurePass = TEXT("");
		szNickName = TEXT("");
		szRegisterMobile = TEXT("");
		szUnderWrite = TEXT("");
		szFaceUrl = TEXT("");

		dwGroupID = 0;
		szGroupName = TEXT("");

		lScore = 0;
		lRevenue = 0;
		lRoomCards = 0;

		dwWinCount = 0;
		dwLostCount = 0;
		dwDrawCount = 0;
		dwFleeCount = 0;
		dwPlayTimeCount = 0;

		dwGameLogonTimes = 0;
		dwRegType = 0;
		dwChannel = 0;
		dwFirstCharge = 0;
		dwIsChangePWD = 0;

		wNullity = 0;
		wStunDown = 0;
		cbMoorMachine = 0;
		szLastLogonMachine = TEXT("");

		szRegisterMachine = TEXT("");

		dwKindID = 0;
		dwServerID = 0;
	}

	//重载运算
	friend Serializer& operator >> (Serializer& in, tagCacheUserInfo& d) {
		in >> d.szUserKey >> d.dwUserID >> d.dwGameID >> d.wFaceID >> d.cbGender >> d.dwCustomID >> d.cbMemberOrder >> d.cbMasterOrder >> d.dwUserRight >> d.dwMasterRight
			>> d.szAccounts >> d.szLogonPass >> d.szInsurePass >> d.szNickName >> d.szRegisterMobile >> d.szUnderWrite >> d.szFaceUrl >> d.dwGroupID >> d.szGroupName
			>> d.lScore >> d.lRevenue >> d.lRoomCards >> d.dwWinCount >> d.dwLostCount >> d.dwDrawCount >> d.dwFleeCount >> d.dwPlayTimeCount >> d.dwGameLogonTimes
			>> d.dwRegType >> d.dwChannel >> d.dwFirstCharge >> d.dwIsChangePWD >> d.wNullity >> d.wStunDown >> d.cbMoorMachine
			>> d.szLastLogonMachine >> d.szRegisterMachine >> d.dwKindID >> d.dwServerID;
		return in;
	}
	friend Serializer& operator << (Serializer& out, tagCacheUserInfo d) {
		out << d.szUserKey << d.dwUserID << d.dwGameID << d.wFaceID << d.cbGender << d.dwCustomID << d.cbMemberOrder << d.cbMasterOrder << d.dwUserRight << d.dwMasterRight
			<< d.szAccounts << d.szLogonPass << d.szInsurePass << d.szNickName << d.szRegisterMobile << d.szUnderWrite << d.szFaceUrl << d.dwGroupID << d.szGroupName
			<< d.lScore << d.lRevenue << d.lRoomCards << d.dwWinCount << d.dwLostCount << d.dwDrawCount << d.dwFleeCount << d.dwPlayTimeCount << d.dwGameLogonTimes
			<< d.dwRegType << d.dwChannel << d.dwFirstCharge << d.dwIsChangePWD << d.wNullity << d.wStunDown << d.cbMoorMachine
			<< d.szLastLogonMachine << d.szRegisterMachine << d.dwKindID << d.dwServerID;
		return out;
	}
};

//////////////////////////////////////////////////////////////////////////////////

#endif