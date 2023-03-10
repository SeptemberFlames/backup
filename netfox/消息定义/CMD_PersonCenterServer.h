#ifndef CMD_PersonCenter_HEAD_FILE
#define CMD_PersonCenter_HEAD_FILE

#pragma pack(1)

//个人信息
#define DTP_GP_UI_NICKNAME							1					//用户昵称
#define DTP_GP_UI_USER_NOTE							2					//用户说明
#define DTP_GP_UI_UNDER_WRITE						3					//个性签名
#define DTP_GP_UI_QQ								4					//Q Q 号码
#define DTP_GP_UI_EMAIL								5					//电子邮件
#define DTP_GP_UI_SEAT_PHONE						6					//固定电话
#define DTP_GP_UI_MOBILE_PHONE						7					//移动电话
#define DTP_GP_UI_COMPELLATION						8					//真实名字
#define DTP_GP_UI_DWELLING_PLACE					9					//联系地址
#define DTP_GP_UI_PASSPORTID    					10					//身份标识

//服务命令
#define MDM_GP_USER_SERVICE							3					//用户服务

//账号服务
#define SUB_GP_MODIFY_MACHINE						100					//绑定解绑机器
#define SUB_GP_MODIFY_LOGON_PASS					101					//修改登录密码
#define SUB_GP_MODIFY_INSURE_PASS					102					//修改银行密码
#define SUB_GP_PHONE_RESET_CODE						103					//重置请求验证
#define SUB_GP_PHONE_RESET_CODE_RESULT				104					//重置请求验证结果
#define SUB_GP_PHONE_RESET_PASSWORD					105					//重置登陆密码

//游客转正
#define SUB_GP_VISIT_TO_REGISTER					123                 //游客转正
#define SUB_GP_VISIT_TO_REGISTER_SUCCESS			124                 //转正成功
#define SUB_GP_VISIT_TO_REGISTER_FAILURE			125                 //转正失败

//个人资料
#define SUB_GP_USER_INDIVIDUAL						301					//个人资料
#define	SUB_GP_QUERY_INDIVIDUAL						302					//查询信息
#define SUB_GP_MODIFY_INDIVIDUAL					303					//修改资料

//游戏记录
#define SUB_GP_GAME_RECORD_LIST						550					//游戏记录列表
#define SUB_GP_GAME_RECORD_TOTAL					551					//游戏排行结果
#define SUB_GP_GAME_RECORD_CHILD					552					//游戏记录回放
#define SUB_GP_GAME_HELPROOM_RECORD					553					//代开记录查询
#define SUB_GP_GAME_HELPROOM_REPLY					554					//代开记录回复

#define SUB_GP_QUERY_INCOME_LIST					427					//查询每日赚金榜
#define SUB_GP_INCOME_LIST_REPLY					428					//每日赚金榜返回 

#define SUB_GP_BANK_REQBINDPHONE					415                 //请求绑定电话  
#define SUB_GP_BANK_REGISTERPHONE					416                 //注册手机号

#define SUB_GP_BANK_REGISTERSUCCESS					417                 //注册电话成功
#define SUB_GP_BANK_REGISTERFAILURE					418					//注册电话失败

#define SUB_GP_CHECKYKMOBLIE						419                 //检查游客手机号
#define SUB_GP_CHECKYKMOBLIEREPLY					420                 //检查手机号回复

#define SUB_GP_BINDWEIXI							421					//绑定微信

//自定义字段查询 公告
#define SUB_GP_QUERY_PUBLIC_NOTICE					500					//自定义字段查询
#define	SUB_GP_PUBLIC_NOTICE						501					//自定义数据查询

//财富排行榜
#define SUB_GP_QUERY_SCORE_RANK_LIST				600					//查询财富排行榜
#define SUB_GP_SCORE_RANK_LIST_REPLY				601					//财富排行榜返回
#define SUB_GP_SCORE_RANK_LIST_REPLY_END			602					//财富排行榜结束

//红包排行榜
#define SUB_GP_QUERY_HONG_BA0_CASH_RANK_LIST		700					//查询红包排行榜
#define SUB_GP_HONG_BA0_CASH_RANK_LIST_REPLY		701					//红包排行榜返回
#define SUB_GP_HONG_BA0_CASH_RANK_LIST_REPLY_END	702					//红包排行榜结束

//红包排行榜
#define SUB_GP_QUERY_FAN_LI_RANK_LIST				800					//查询返利排行榜
#define SUB_GP_FAN_LI_RANK_LIST_REPLY				801					//返利排行榜返回
#define SUB_GP_FAN_LI_RANK_LIST_REPLY_END			802					//返利排行榜结束

//充值排行榜
#define SUB_GP_QUERY_VIP_CHARGE_RANK_LIST			810					//查询充值排行榜
#define SUB_GP_VIP_CHARGE_RANK_LIST_REPLY			811					//充值排行榜返回
#define SUB_GP_VIP_CHARGE_RANK_LIST_REPLY_END		812					//充值排行榜结束

//个人资料
struct CMD_GP_UserIndividual
{
	//用户信息
	DWORD							dwUserID;							//用户 I D
	BYTE                            cbGender;                           //用户性别
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//手机号码
	DWORD							dwSpreaderID;						//推广信息
};


//查询信息
struct CMD_GP_QueryIndividual
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwClientAddr;						//客户地址
};

//修改资料
struct CMD_GP_ModifyIndividual
{
	//验证资料
	DWORD							dwUserID;							//用户 I D
	BYTE							cbGender;							//用户性别
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名
};

//////////////////////////////////////////////////////////////////////////////////
//游戏记录
struct DBR_GP_GetGameRecord_List
{
	DWORD							dwUserID;
};

//游戏记录
struct CMD_GP_GetGameTotalRecord
{
	DWORD							dwUserID;
	DWORD							dwRecordID;
};

//游戏记录
struct CMD_GP_GetGameChildRecord
{
	DWORD							dwUserID;
	DWORD							dwRecordID;
};


//登录失败
struct CMD_GP_LogonFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//绑定机器
struct CMD_GP_ModifyMachine
{
	BYTE							cbBind;								//绑定标志
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//请求验证
struct CMD_GP_PhoneResetCode
{
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR                           szMoblie[LEN_MOBILE_PHONE];			//手机号码
	DWORD							dwClientIP;							//客户地址
	DWORD							dwChannel;							//用户渠道
};

//请求验证结果
struct CMD_GP_PhoneResetCodeResult
{
	DWORD							dwResult;							//返回结果
	TCHAR							szDescribeString[128];				//描述消息
};

//手机重置密码
struct CMD_GP_PhoneResetPassword
{
	TCHAR                           szMoblie[LEN_MOBILE_PHONE];			//绑定手机
	TCHAR							szPassword[LEN_PASSWORD];			//用户密码
	DWORD                           dwKey;								//验证密码
	DWORD							dwChannel;							//用户渠道
};

//游客转正
struct CMD_GP_VisitToRegister
{
	DWORD                          dwUserID;
	DWORD                          dwClientIP;
	TCHAR						   szNewAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR						   szNewNickName[LEN_NICKNAME];         //玩家昵称
	TCHAR						   szNewPassword[LEN_MD5];				//登录密码
	TCHAR						   szNewMobilePhone[LEN_MOBILE_PHONE];	//手机号码
};

//游客转正成功
struct CMD_GP_VisitToRegisterSuccess
{
	TCHAR						   szNewAccounts[LEN_ACCOUNTS];			//登录帐号
	TCHAR						   szNewNickName[LEN_NICKNAME];         //玩家昵称
	TCHAR						   szNewPassword[LEN_MD5];				//登录密码
	TCHAR						   szNewMobilePhone[LEN_MOBILE_PHONE];  //手机号码
};

//游客转正失败
struct CMD_GP_VisitToRegisterFailure
{
	TCHAR							szDescribeString[256];				//提示消息
};

//自定义数据查询
struct CMD_GP_PublicNotice
{
	LONG							lResultCode;						//操作代码
	TCHAR							szDescribeString[256];				//成功消息
};

//用户游戏状态
struct CMD_GP_InGameSeverID
{
	DWORD							LockKindID;
	DWORD							LockServerID;
};

//绑定手机
struct CMD_GP_ReqBindPhone
{
	DWORD                           dwUserID;
	TCHAR                           szMoblie[LEN_MOBILE_PHONE];
	DWORD							dwChannel;
};

//检查游客手机
struct CMD_GP_CheckYKMoblie
{
	DWORD                           dwUserID;
};

//绑定电话
struct CMD_GP_RegisterBindPhone
{
	DWORD                           dwUserID;
	TCHAR                           szMoblie[LEN_MOBILE_PHONE];
	WORD                            dKey;
};

//银行失败
struct CMD_GP_UserInsureFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//修改登录密码
struct CMD_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//用户 I D
	DWORD                           dwClientIP;							//客户地址
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//修改银行密码
struct CMD_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//用户 I D
	DWORD                           dwIP;
	TCHAR							szDesPassword[LEN_PASSWORD];		//用户密码
	TCHAR							szScrPassword[LEN_PASSWORD];		//用户密码
};

//查询财富排行
struct CMD_GP_QueryScoreRankInfo
{
	DWORD							dwChannel;							//发送渠道
};

//财富排行信息
struct CMD_GP_ScoreRankInfo
{
	DWORD							dwGameID;							//玩家标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	WORD							cbGender;							//用户性别
	TCHAR							szFaceUrl[256];						//头像地址
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名
	SCORE							lScore;								//游戏积分
	DWORD							dwUserID;							//玩家标识
	WORD							wMemberOrder;						//会员等级
	DWORD							dwEid;								//聊天标识
};

//查询红包排行
struct CMD_GP_QueryHongbaoCashRankInfo
{
	DWORD							dwChannel;							//发送渠道
};

//红包排行信息
struct CMD_GP_HongBaoCashRankInfo
{
	DWORD							dwGameID;							//玩家标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	WORD							cbGender;							//用户性别
	TCHAR							szFaceUrl[256];						//头像地址
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名
	SCORE							lHongBaoCash;						//游戏红包
	DWORD							dwUserID;							//玩家标识
	WORD							wMemberOrder;						//会员等级
};

//查询返利排行
struct CMD_GP_QueryFanLiRankInfo
{
	DWORD							dwChannel;							//发送渠道
};

//红包返利信息
struct CMD_GP_FanLiRankInfo
{
	DWORD							dwGameID;							//玩家标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	SCORE							llScore;							//返利金币
};

//查询充值排行
struct CMD_GP_QueryVipChargeRankInfo
{
	DWORD							dwChannel;							//发送渠道
};

//红包返利信息
struct CMD_GP_VipChargeRankInfo
{
	DWORD							dwGameID;							//玩家标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//操作结果
#define SUB_GP_OPERATE_SUCCESS		900									//操作成功
#define SUB_GP_OPERATE_FAILURE		901									//操作失败
#define SUB_GP_MOBLIE_FAILURE       902                                 //绑定失败

//操作失败
struct CMD_GP_OperateFailure
{
	DWORD							lResultCode;						//错误代码
	DWORD							lPersonType;						//个人操作
	TCHAR							szDescribeString[128];				//描述消息
};

//操作成功
struct CMD_GP_OperateSuccess
{
	DWORD							lResultCode;						//操作代码
	DWORD							lPersonType;						//个人操作
	TCHAR							szDescribeString[128];				//成功消息
};

//////////////////////////////////////////////////////////////////////////////////
//远程服务

#define MDM_GP_REMOTE_SERVICE		4									//远程服务

//查找服务
#define SUB_GP_C_SEARCH_DATABASE	100									//数据查找
#define SUB_GP_C_SEARCH_CORRESPOND	101									//协调查找

//查找服务
#define SUB_GP_S_SEARCH_DATABASE	200									//数据查找
#define SUB_GP_S_SEARCH_CORRESPOND	201									//协调查找

//////////////////////////////////////////////////////////////////////////////////

//协调查找
struct CMD_GP_C_SearchCorrespond
{
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

//协调查找
struct CMD_GP_S_SearchCorrespond
{
	WORD							wUserCount;							//用户数目
	tagUserRemoteInfo				UserRemoteInfo[16];					//用户信息
};


//////////////////////////////////////////////////////////////////////////////////


//贵宾奖励
#define	MDM_GP_VIP_REWARD_SYSTEM					7					//代理裂变系统

#define SUB_GP_VIP_REWARD_SYSTEM_RWARD				100					//查询/领取奖励
#define SUB_GP_VIP_REWARD_SYSTEM_RWARD_LOG			101					//领取奖励日志
#define SUB_GP_VIP_REWARD_SYSTEM_RWARD_FINISH		102					//领取奖励日志完成

//查询/领取奖励
struct CMD_GP_C_Vip_System_Reward
{
	DWORD							dwUserID;							//玩家标识
	DWORD							dwIsLinQU;							//操作类型: 0查询,1领取
};

struct CMD_GP_S_Vip_System_Reward
{
	DWORD							dwIsLinQU;							//操作类型: 0查询,1领取
	SCORE							llScore;							//奖励积分
	TCHAR							szDescribeString[128];				//描述消息
};

//领取奖励日志
struct CMD_GP_C_Vip_System_Reward_Log
{
	DWORD							dwUserID;							//玩家标识
};

struct CMD_GP_S_Vip_System_Reward_Log
{
	SCORE							llScore;							//奖励积分
	TCHAR							szLinquDate[64];					//领取时间
};

//////////////////////////////////////////////////////////////////////////////////

//微信每日分享
#define MDM_MB_WEIXIN_PERDAY_SHARE		900								//微信每日分享
#define SUB_MB_WEIXIN_PERDAY_SHARE		100								//微信每日分享

struct  CMD_MB_WeiXinPerDayShare
{
	DWORD							dwChannel;							//用户渠道
	DWORD							dwUserID;							//玩家标志
	DWORD							dwClientAddr;						//操作地址
};

struct CMD_MB_WeiXinPerDayShareResult
{
	DWORD							dwResult;							//分享结果
	TCHAR							szDescribeString[128];				//描述消息
	SCORE							llShareRewardScore;					//奖励分数
};

#define MDM_MB_WEIXIN_FRIEND_SHARE		1000							//微信好友分享
#define SUB_MB_WEIXIN_FRIEND_SHARE		100								//微信好友分享

struct CMD_MB_WeiXinFriendShare
{
	DWORD							dwChannel;							//用户渠道
	DWORD							dwUserID;							//玩家标志
	DWORD							dwClientAddr;						//操作地址
};

struct CMD_MB_WeiXinFriendShareResult
{
	DWORD							dwResult;							//分享结果
	TCHAR							szDescribeString[128];				//描述消息
	SCORE							llShareRewardCash;					//奖励分数
};

#pragma pack()

#endif