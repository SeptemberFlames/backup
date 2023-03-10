#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//设备类型
#define LOGON_TYPE_IOS						1
#define LOGON_TYPE_ANDROID					2
#define LOGON_TYPE_PC						3

//////////////////////////////////////////////////////////////////////////////////
//登陆命令
#define MDM_GP_LOGON						1										//广场登录

#define SUB_GP_LOGON_GAMEID					1										//I D 登录
#define SUB_GP_LOGON_ACCOUNTS				2										//帐号登录
#define SUB_GP_REGISTER_ACCOUNTS			3										//注册帐号
#define SUB_GP_REQUEST_PHONE_CODE			4										//请求验证
#define SUB_GP_EFFICACY_NICKNAME			5										//昵称验证
#define SUB_GP_SNED_EFFICACY				6										//发送验证
#define SUB_GP_LOCK_MOBILE					7										//手机绑定
#define SUB_GP_GETACCOUNTEFFICACY			8										//获取验证

#define SUB_GP_VISTORLOGON					50										//游客登录
#define SUB_GP_REISTER_TPYE					71										//注册类型
#define SUB_GP_WEIXIN_LOGON					72										//微信登录
#define SUB_GP_QQ_LOGON						74										//QQ登陆
#define SUB_GP_WX_LOGON						75										//微信登陆弹框
#define SUB_GP_TOURIST_LOGON				76										//游客登陆

//登录结果
#define SUB_GP_LOGON_SUCCESS				100										//登录成功
#define SUB_GP_LOGON_FAILURE				101										//登录失败
#define SUB_GP_LOGON_FINISH					102										//登录完成
#define SUB_GP_VALIDATE_MBCARD				103										//登录失败
#define SUB_GP_LOCK_MB_NOW					107										//绑定手机
#define SUB_GP_LOCK_MB_FAILURE				108										//绑定失败
#define SUB_GP_LOGON_LINK_WAN				111										//登录失败
#define SUB_GP_LOGON_LINK_DEC				112										//登录失败
#define SUB_GP_REQUEST_PHONE_CODE_RESULT	173										//申请验证结果

//帐号登录
struct CMD_GP_LogonAccounts
{
	//系统信息
	TCHAR									szMachineID[LEN_MACHINE_ID];			//机器序列
	DWORD									dwClientIP;								//客户地址

	//登录信息
	TCHAR									szPassword[LEN_MD5];					//登录密码
	TCHAR									szAccounts[LEN_ACCOUNTS];				//登录帐号
	DWORD									dwLogonType;							//登录类型
	DWORD									dwClinetVersion;						//客户端版本号
	WORD									dwChannel;								//渠道标识
};

//管理登录
struct CMD_GP_MasterLogonAccounts
{
	//系统信息
	TCHAR									szMachineID[LEN_MACHINE_ID];			//机器序列

	//登录信息
	TCHAR									szPassword[LEN_MD5];					//登录密码
	TCHAR									szAccounts[LEN_ACCOUNTS];				//登录帐号
};

//注册帐号
struct CMD_GP_RegisterAccounts
{
	TCHAR									szMachineID[LEN_MACHINE_ID];			//机器序列

	DWORD									dwClientIP;								//玩家IP

	TCHAR									szLogonPass[LEN_MD5];					//登录密码
	TCHAR									szAccounts[LEN_ACCOUNTS];				//登录帐号
	TCHAR									szNickName[LEN_NICKNAME];				//用户昵称

	int										cbRegisterType;							//注册类型
	WORD									dwChannel;								//注册渠道

	TCHAR									szMoblie[LEN_MOBILE_PHONE];				//手机号码
	DWORD									dwPhoneCode;							//注册验证

	DWORD									dwClinetVersion;						//客户端版本号
	DWORD									dwSpreaderID;							//推广标识
};

//请求验证
struct CMD_GP_RequestPhoneCode
{
	TCHAR									szMachineID[LEN_MACHINE_ID];			//机器序列
	TCHAR									szMoblie[LEN_MOBILE_PHONE];				//手机号码
	DWORD									dwClientIP;								//客户地址
	DWORD									dwChannel;								//用户渠道
};

//请求验证结果
struct CMD_GP_RequestPhoneCodeResult
{
	DWORD									dwResult;								//返回结果
	TCHAR									szDescribeString[128];					//描述消息
};

//游客登陆
struct GSP_GP_VisitorLogon
{
	TCHAR									szMachineID[LEN_MACHINE_ID];
};

//微信登录
struct CMD_GP_WeiXinLogon
{
	int										strLogFrom;								//登录类型
	int										dwClinetVersion;						//客户端版本号
	DWORD									dwClientIP;								//玩家IP
	DWORD									dwPlazaVersion;							//广场版本
	TCHAR									strOpenID[50];							//平台信息
	TCHAR									szMachineID[LEN_MACHINE_ID];			//机器序列
	TCHAR									strwxNickName[100];						//微信昵称
	WORD									dwChannel;
};

//游客登录
struct CMD_GP_TouristLogon
{
	WORD									dwChannel;
	DWORD									dwClientIP;								//玩家IP
	TCHAR									szMachineID[LEN_MACHINE_ID];			//机器序列
	DWORD									dwLogonType;							//登录类型
	DWORD									dwClinetVersion;						//客户端版本号
	DWORD									dwSpreaderID;							//推广标识
};

//登录成功
struct CMD_GP_LogonSuccess
{
	//属性资料
	WORD									wFaceID;								//头像标识
	DWORD									dwUserID;								//用户 I D
	DWORD									dwGameID;								//游戏 I D

	//用户成绩
	SCORE									lUserScore;								//用户金币
	SCORE									lUserInsure;							//用户银行
	SCORE									lRoomCards;								//房卡数量

	//用户信息
	BYTE									cbGender;								//用户性别
	BYTE									cbMoorMachine;							//锁定机器
	BYTE									cbMaxStart;								//工作线路
	TCHAR									szAccounts[LEN_ACCOUNTS];				//登录帐号
	TCHAR									szLogonPass[LEN_MD5];					//登录密码
	TCHAR									szRegisterMobile[LEN_MOBILE_PHONE];		//手机号码
	TCHAR									szNickName[LEN_ACCOUNTS];				//用户昵称

	//附加信息
	BYTE									cbMemberOrder;							//会员等级
	TCHAR									szFaceUrl[256];							//网络头像
	DWORD									dwTotalCharge;							//累计充值

	//连接信息
	WORD									wServerLinkCount;						//连接条数
	TCHAR									szServerLinkUrl[SERVER_LINK_COUNT][32];	//连接地址

	//游戏信息
	DWORD									dwLockKindID;							//游戏标识
	DWORD									dwLockServerID;							//房间标识

	//扩展信息
	WORD									wRegType;								//账号类型: 1-注册, 2-游客 3-腾讯, 4-微信
	DWORD									dwServerTime;							//服务时间
};

//登录失败
struct CMD_GP_LogonFailure
{
	DWORD									lResultCode;							//错误代码
	TCHAR									szDescribeString[128];					//描述消息
};

//登陆完成
struct CMD_GP_LogonFinish
{
	WORD									wIntermitTime;							//中断时间
	WORD									wOnLineCountTime;						//更新时间
};

//QQ登陆
struct WEB_QQ_LoginNew
{
	DWORD									dwClientAddr;							//连接地址
	TCHAR									szMachineID[LEN_MACHINE_ID];			//机器序列
	TCHAR									szOpenID[50];							//QQ授权用户唯一标识
	TCHAR									szUnionID[50];							//QQ授权用户平台标识
	TCHAR									szNickName[LEN_NICKNAME];				//昵称
	TCHAR									szHeadImageUrl[256];					//网络头像
	BYTE									cbGender;								//性别
	WORD									dwChannel;								//渠道号
	DWORD									dwClientIP;								//玩家IP
	DWORD									dwClinetVersion;						//客户端版本号
	DWORD									strLogFrom;								//登录类型
	DWORD									dwSpreaderID;							//推广标识
};

//微信登陆
struct WEB_WX_LoginNew
{
	TCHAR									szMachineID[LEN_MACHINE_ID];			//机器序列
	TCHAR									szOpenID[50];							//微信授权用户唯一标识
	TCHAR									szUnionID[50];							//微信授权用户平台标识
	TCHAR									szNickName[LEN_NICKNAME];				//昵称
	TCHAR									szHeadImageUrl[256];					//网络头像
	BYTE									cbGender;								//性别
	WORD									dwChannel;								//渠道号
	DWORD									dwClientIP;								//玩家IP
	int										dwClinetVersion;						//客户端版本号
	int										strLogFrom;								//登录类型
	DWORD									dwSpreaderID;							//推广标识
};

//////////////////////////////////////////////////////////////////////////////////

#define DTP_GP_GROUP_INFO					1										//社团信息
#define DTP_GP_MEMBER_INFO					2										//会员信息
#define	DTP_GP_UNDER_WRITE					3										//个性签名
#define DTP_GP_STATION_URL					4										//主页信息

//社团信息
struct DTP_GP_GroupInfo
{
	DWORD									dwGroupID;								//社团索引
	TCHAR									szGroupName[LEN_GROUP_NAME];			//社团名字
};

//会员信息
struct DTP_GP_MemberInfo
{
	BYTE									cbMemberOrder;							//会员等级
	SYSTEMTIME								MemberOverDate;							//到期时间
};

//////////////////////////////////////////////////////////////////////////////////
//列表命令

#define MDM_GP_SERVER_LIST					2										//列表信息

//获取命令
#define SUB_GP_GET_LIST						1										//获取列表
#define SUB_GP_GET_SERVER					2										//获取房间
#define SUB_GP_GET_ONLINE					3										//获取在线
#define SUB_GP_GET_COLLECTION				4										//获取收藏
#define SUB_GP_GET_KIND_ALL					5										//获取游戏
#define SUB_GP_GET_SERVER_ALL				6										//获取房间
#define	SUB_GP_GET_SERVERTIME 				7										//获取服务器时间

//列表信息
#define SUB_GP_LIST_TYPE					100										//类型列表
#define SUB_GP_LIST_KIND					101										//种类列表
#define SUB_GP_LIST_NODE					102										//节点列表
#define SUB_GP_LIST_PAGE					103										//定制列表
#define SUB_GP_LIST_SERVER					104										//房间列表
#define SUB_GP_VIDEO_OPTION					105										//视频配置
#define SUB_GP_GET_LIST_KIND				106										//获取类型
#define SUB_GP_GET_LIST_SERVER				107										//获取房间
#define SUB_GP_GET_SYSTEM_MESSAGE  		 	108										//获取公告
#define SUB_GP_GET_NEWSUM           		109										//获取公告数量
#define	SUB_GP_LIST_CONFIGURE				110										//系统配置

//完成信息
#define SUB_GP_LIST_FINISH					200										//发送完成
#define SUB_GP_SERVER_FINISH				201										//房间完成

//在线信息
#define SUB_GR_KINE_ONLINE					300										//类型在线
#define SUB_GR_SERVER_ONLINE				301										//房间在线

//////////////////////////////////////////////////////////////////////////////////

//获取在线
struct CMD_GP_GetOnline
{
	DWORD									dwChannel;								//登陆渠道
};

//游戏在线
struct CMD_GP_KindOnline
{
	WORD									wKindID;								//类型标识
	DWORD									dwOnLineCount;							//在线人数
};

//房间在线
struct CMD_GP_ServerOnline
{
	WORD									wServerID;								//房间标识
	DWORD									dwOnLineCount;							//在线人数
};
//////////////////////////////////////////////////////////////////////////////////
//服务命令

#define MDM_GP_USER_SERVICE					3										//用户服务

//账号服务
#define SUB_GP_MODIFY_MACHINE				100										//修改机器
#define SUB_GP_MODIFY_LOGON_PASS			101										//修改密码
#define SUB_GP_MODIFY_INSURE_PASS			102										//修改密码
#define SUB_GP_MODIFY_UNDER_WRITE			103										//修改签名

//控制服务
#define SUB_GP_USER_CONTROL_XMS_CON			104										//增加控制
#define SUB_GP_USER_CONTROL_XMS_DELCON		105										//删除控制
#define SUB_GP_USER_CONTROL_XMS_PM			106										//房间控制				

//控制相关
#define SUB_GP_USER_MANAGE_SETTING			108										//配置送收分消息
#define SUB_GP_USER_MANAGE_INFO				109										//获取当前配置信息
#define SUB_GP_USER_MANAGE_REVENUE			110										//修减玩家库存

//修改头像
#define SUB_GP_USER_FACE_INFO				200										//头像信息
#define SUB_GP_SYSTEM_FACE_INFO				201										//系统头像
#define SUB_GP_CUSTOM_FACE_INFO				202										//自定头像

//个人资料
#define SUB_GP_USER_INDIVIDUAL				301										//个人资料
#define	SUB_GP_QUERY_INDIVIDUAL				302										//查询信息
#define SUB_GP_MODIFY_INDIVIDUAL			303										//修改资料
#define SUB_GR_USER_PASSWORD_INFO			308										//返回结果

//银行服务
#define SUB_GP_USER_SAVE_SCORE				400										//存款操作
#define SUB_GP_USER_TAKE_SCORE				401										//取款操作
#define SUB_GP_USER_TRANSFER_SCORE			402										//转账操作
#define SUB_GP_USER_INSURE_INFO				403										//银行资料
#define SUB_GP_QUERY_INSURE_INFO			404										//查询银行
#define SUB_GP_USER_INSURE_SUCCESS			405										//银行成功
#define SUB_GP_USER_INSURE_FAILURE			406										//银行失败
#define SUB_GP_QUERY_USER_INFO_REQUEST		407										//查询用户
#define SUB_GP_QUERY_USER_INFO_RESULT		408										//用户信息
#define SUB_GP_USER_LOGONBANK				409										//登录成功
#define SUB_GP_LOGONBANK_SUCCESS			410										//登录失败
#define SUB_GP_LOGONBANK_FAILURE			411										//登录银行
#define SUB_GP_QUERY_BANK_DETAIL			412										//银行明细
#define SUB_GP_BANK_DETAIL_RESULT			413										//明细结果

//操作结果
#define SUB_GP_OPERATE_SUCCESS				900										//操作成功
#define SUB_GP_OPERATE_FAILURE				901										//操作失败
#define SUB_GP_USER_CONTROL_XMS				902										//发送信息 弹出控制框
#define SUB_GP_USER_CONTROL_XMS_ONE			903										//发送被控人数，服务器—》客户端
#define SUB_GP_USER_CONTROL_XMS_UPONE		904										//发送被控人数，服务器—》客户端
#define SUB_GP_USER_CONTROL_XMS_FINSH		905										//发送被控人数，服务器—》客户端
#define SUB_GP_USER_CONTROL_XMS_DELONE		906										//发送被控人数，服务器—》客户端
#define SUB_GP_ONLINE_USER					907
#define SUB_GP_ONLINE_USER_LEAVE			908
#define SUB_GP_ONLINE_USER_UPSCORE			909
#define SUB_GP_USER_CONTROL_UPXMS			910										//发送信息 弹出控制框
#define SUB_GP_ONLINE_USER_WRITE_SCORE		911
#define SUB_GP_USER_CONTROL_UPXMSSTOCK		912										//发送信息 弹出控制框
#define SUB_GP_USER_CONTROL_NOTSTOCK		913										//发送信息 弹出控制框
#define SUB_GP_ONLINE_USER_UPTODAYIN		914										//发送信息 弹出控制框
#define SUB_GP_GET_VIP_DAYLIST				915										//发送信息 弹出控制框
#define SUB_GP_GET_VIP_DAYLIST_FINSH		916										//发送信息 弹出控制框

////////////////////////////////////////////////////////////////////////////////////////
//wxr20150826 捕鱼专用
#define SUB_GP_MANAGE_MSG					917										//配置送收分消息
#define SUB_GP_MANAGE_REVENUE				918										//修减玩家库存消息
////////////////////////////////////////////////////////////////////////////////////////

//库存不足
struct CMD_GP_NotStock
{
	WORD									lResultCode;							//错误代码
	WORD									wServerID;
};

//修改密码
struct CMD_GP_ModifyLogonPass
{
	DWORD									dwUserID;								//用户 I D
	DWORD									dwIP;
	TCHAR									szDesPassword[LEN_PASSWORD];			//用户密码
	TCHAR									szScrPassword[LEN_PASSWORD];			//用户密码
};

//修改密码
struct CMD_GP_ModifyInsurePass
{
	DWORD									dwUserID;								//用户 I D
	DWORD									dwIP;
	TCHAR									szDesPassword[LEN_PASSWORD];			//用户密码
	TCHAR									szScrPassword[LEN_PASSWORD];			//用户密码
};

//修改签名
struct CMD_GP_ModifyUnderWrite
{
	DWORD									dwUserID;								//用户 I D
	TCHAR									szPassword[LEN_PASSWORD];				//用户密码
	TCHAR									szUnderWrite[LEN_UNDER_WRITE];			//个性签名
};

//////////////////////////////////////////////////////////////////////////////////

//用户头像
struct CMD_GP_UserFaceInfo
{
	WORD									wFaceID;								//头像标识
	DWORD									dwCustomID;								//自定标识
};

//修改头像
struct CMD_GP_SystemFaceInfo
{
	WORD									wFaceID;								//头像标识
	DWORD									dwUserID;								//用户 I D
	TCHAR									szPassword[LEN_PASSWORD];				//用户密码
	TCHAR									szMachineID[LEN_MACHINE_ID];			//机器序列
};

//修改头像
struct CMD_GP_CustomFaceInfo
{
	DWORD									dwUserID;								//用户 I D
	TCHAR									szPassword[LEN_PASSWORD];				//用户密码
	TCHAR									szMachineID[LEN_MACHINE_ID];			//机器序列
	DWORD									dwCustomFace[FACE_CX*FACE_CY];			//图片信息
};

//////////////////////////////////////////////////////////////////////////////////

//绑定机器
struct CMD_GP_ModifyMachine
{
	BYTE									cbBind;									//绑定标志
	DWORD									dwUserID;								//用户标识
	TCHAR									szPassword[LEN_PASSWORD];				//用户密码
	TCHAR									szMachineID[LEN_MACHINE_ID];			//机器序列
};

struct CMD_SysemMessage
{
	TCHAR									szSystemMessage[LEN_USER_CHAT];			//系统消息
	TCHAR									szMessageUrl[LEN_USER_CHAT];			//系统消息
	LONGLONG								tConcludeTime;							//结束时间
};

//查询货币
struct CMD_GP_QueryMonery
{
	DWORD									dwUserID;								//用户标志
	TCHAR									szLogonPass[LEN_MD5];					//登录密码
};

//查询货币结果
struct CMD_GP_QueryMoneyResult
{
	DWORD									dwResultCode;							//查询结果
	SCORE									lScore;									//身上金币
	DWORD									dwTotalMoney;							//累计充值
	SCORE									lRoomCards;								//房卡数量
};

//获取数目
struct CMD_GP_GetUserReturnScore
{
	DWORD									dwUserID;								//用户ID
	DWORD									dwCount;								//获取数目
};

struct CMD_GP_ReturnYSSocre
{
	DWORD									dwUserID;
	LONGLONG								lScore;
};


struct CMD_GR_ChangeReturnInfo
{
	bool									bFinsh;									//兑换结果
	LONGLONG								llReturncount;
};

//个人资料
struct CMD_GP_UserIndividual
{
	DWORD									dwUserID;								//用户 I D
	BYTE									Gender;									//性别
};

//查询信息
struct CMD_GP_QueryIndividual
{
	DWORD									dwPlazaVersion;
	DWORD									dwUserID;								//用户 I D
	TCHAR									szPassword[LEN_PASSWORD];				//用户密码
};

//修改资料
struct CMD_GP_ModifyIndividual
{
	BYTE									cbGender;								//用户性别
	DWORD									dwUserID;								//用户 I D
	TCHAR									szPassword[LEN_PASSWORD];				//用户密码
};

struct CMD_GP_UserInsureTran
{
	DWORD									dwTranGameID;
	LONGLONG								llTranScore;
	WORD									wServerID;
};

struct CMD_LineUser
{
	DWORD									dwUserID;
	DWORD									dwGameID;
	TCHAR									szNickName[LEN_ACCOUNTS];				//用户昵称

	SCORE									llCurScore;								//身上金币
	SCORE									llInsureScore;							//银行金币

	WORD									wKindId;								//游戏ID
	WORD									wServerId;								//房间ID

	DWORD									dwChannel;								//登录渠道
};

//财富榜
struct CMD_GP_Wealthlist 
{
	BYTE									cbGender;								//用户性别 1
	DWORD									dwID;									//用户排名 4
	DWORD									dwUserID;								//用户 I D 4（变成gameid）
	SCORE									szScore;								//用户金币 8
	TCHAR									szNickName[32];							//用户昵称 32*2
	TCHAR									szUnderWrite[LEN_UNDER_WRITE];			//个性签名32*2
};

//用户控制信息
struct CMD_ControlUserInfo
{
	DWORD									dwGameID;
	BYTE									byFlydds;

	LONGLONG								llCurChangVal;
	LONGLONG								llMaxVal;

	BYTE									byWinOrLost;
	BYTE									byJuCount;

	DWORD									dwUserID;
	WORD									wServerId;
	
	LONGLONG								llTodayInVal;
	LONGLONG								llAllChangVal;

	LONGLONG								llStartTime;
	LONGLONG								llEndTime;
};

//房间控制信息
struct CMD_ServerConInfo
{
	BYTE									byMobWinOrLost;
	BYTE									byMobdds;
	BYTE									byPcWinOrLost;
	BYTE									byPcdds;

	WORD									wKind;
	WORD									wServerId;

	LONGLONG								llServerStort;
	LONGLONG								llServerStortLv1;
	LONGLONG								llServerStortLv2;
	LONGLONG								llServerStortLv3;
	LONGLONG								llServerStortLv4;
	LONGLONG								llServerStortLv5;
	LONGLONG								llLeast;

	DWORD									dwUserID;
};

//个人控制结构图
struct CMD_C_WriteControlUser
{
	TCHAR									subID[10];								//人的游戏ID
	TCHAR									subFlag[4];								//控制类型(0,1,2),1输，2赢
	TCHAR									subOdds[4];								//控制几率
	TCHAR									subGold[16];							//金币上限
	TCHAR									subAllJu[4];							//局数

	WORD									wServerId;

	BYTE									bAllRoom;

	LONGLONG								llStartTime;
	LONGLONG								llEndTime;
};

//个人控制结构图
struct CMD_S_WriteControlUser
{
	TCHAR									subID[10];								//人的游戏ID
	TCHAR									subFlag[4];								//控制类型(0,1,2)
	TCHAR									subOdds[4];								//控制几率
	TCHAR									subGold[16];							//金币上限

	TCHAR									subChanGold[16];						//金币上限

	TCHAR									subTodayInGold[16];						//金币上限
	TCHAR									subAllChanGold[16];						//金币上限

	TCHAR									subAllJu[4];							//金币最后赢的总和

	WORD									wServerId;
		
	LONGLONG								llStartTime;
	LONGLONG								llEndTime;

	int										Count;									//保存的数据条数
};

//个人控制结构图
struct CMD_C_WriteControlServer
{
	TCHAR									MobOdds[4];								//手机控制概率
	TCHAR									Mobflag[4];								//手机控制类型
	TCHAR									PcOdds[4];								//pc控制概率
	TCHAR									Pcflag[4];								//pc控制类型

	WORD									wConType;
	WORD									wServerId;
};

//个人控制结构图
struct CMD_S_WriteControlServer
{
	TCHAR									MobOdds[4];								//手机控制概率
	TCHAR									Mobflag[4];								//手机控制类型
	TCHAR									PcOdds[4];								//pc控制概率
	TCHAR									Pcflag[4];								//pc控制类型

	WORD									wConType;
	WORD									wKind;
	WORD									wServerId;

	LONGLONG								llServerStort;
	LONGLONG								llServerStortLv1;
	LONGLONG								llServerStortLv2;
	LONGLONG								llServerStortLv3;
	LONGLONG								llServerStortLv4;
	LONGLONG								llServerStortLv5;
	LONGLONG								llleast;
	int										Count;
};

//个人控制结构图
struct CMD_S_UpdataControlServer
{
	TCHAR									MobOdds[4];								//手机控制概率
	TCHAR									Mobflag[4];								//手机控制类型
	TCHAR									PcOdds[4];								//pc控制概率
	TCHAR									Pcflag[4];								//pc控制类型

	WORD									wConType;
	WORD									wKind;
	WORD									wServerId;
	int										Count;									//保存的数据条数
};

struct CMD_C_ControlDelUser
{
	TCHAR									subID[10];								//人的游戏ID
	WORD									wServerId;
};
struct CMD_S_ControlDelUser
{
	TCHAR									subID[10];								//人的游戏ID
	WORD									wServerId;								//房间标识
	int										Count;									//保存的数据条数
};

//用户进入
struct CMD_S_PlayUserUpdataScore
{

	DWORD									dwGameID;								//游戏标识
	SCORE									llCurScore;								//身上金币
	SCORE									llInsureScore;							//银行金币
	SCORE									lHongBaoCash;							//红包金额
	SCORE									lTotalHongBaoCash;						//所有红包
	WORD									wServerID;
};

//用户进入
struct CMD_S_PlayUserWriteScore
{

	DWORD									dwGameID;								//游戏标识
	LONGLONG								llChanScore;
	BYTE									byWriteMode;
	WORD									wServerID;
};

//////////////////////////////////////////////////////////////////////////////////

#define DTP_GP_UI_NICKNAME					1										//用户昵称
#define DTP_GP_UI_USER_NOTE					2										//用户说明
#define DTP_GP_UI_UNDER_WRITE				3										//个性签名
#define DTP_GP_UI_QQ						4										//Q Q 号码
#define DTP_GP_UI_EMAIL						5										//电子邮件
#define DTP_GP_UI_SEAT_PHONE				6										//固定电话
#define DTP_GP_UI_MOBILE_PHONE				7										//移动电话
#define DTP_GP_UI_COMPELLATION				8										//真实名字
#define DTP_GP_UI_DWELLING_PLACE			9										//联系地址

//////////////////////////////////////////////////////////////////////////////////

//银行资料
struct CMD_GP_UserInsureInfo
{
	WORD									wRevenueTake;							//税收比例
	WORD									wRevenueTransfer;						//税收比例
	WORD									wServerID;								//房间标识
	SCORE									lUserScore;								//用户金币
	SCORE									lUserInsure;							//银行金币
	SCORE									lTransferPrerequisite;					//转账条件
	TCHAR									szLastLoginTime[64];
	BYTE									cbAccountsProtect;
};

//手机银行资料
struct CMD_GP_MBUserInsureInfo
{
	WORD									wRevenueTake;							//税收比例
	WORD									wRevenueTransfer;						//税收比例
	WORD									wServerID;								//房间标识
	SCORE									lUserScore;								//用户金币
	SCORE									lUserInsure;							//银行金币
	SCORE									lTransferPrerequisite;					//转账条件，转账金额不能少于这个值
	SCORE									lBankPrerequisite;						//银行存取条件，，存取金币数必须大于此数才可操作
	SCORE									lTransferRetention;						//银行转账条件，转账以后银行金额不能少于此数
};

//存入金币
struct CMD_GP_UserSaveScore
{
	DWORD									dwPlazaVersion;							//广场版本
	DWORD									dwUserID;								//用户 I D
	DWORD									dwIP;
	SCORE									lSaveScore;								//存入金币
	TCHAR									szMachineID[LEN_MACHINE_ID];			//机器序列
};

//手机存入存入金币
struct CMD_GP_MBUserSaveScore
{
	DWORD									dwUserID;								//用户 I D
	SCORE									lSaveScore;								//存入金币
	TCHAR									szMachineID[LEN_MACHINE_ID];			//机器序列
};

//提取金币
struct CMD_GP_UserTakeScore
{
	DWORD									dwPlazaVersion;							//广场版本
	DWORD									dwUserID;								//用户 I D
	DWORD									dwIP;
	SCORE									lTakeScore;								//提取金币
	TCHAR									szPassword[LEN_MD5];					//银行密码
	TCHAR									szMachineID[LEN_MACHINE_ID];			//机器序列
};

//手机提取金币
struct CMD_GP_MBUserTakeScore
{
	DWORD									dwUserID;								//用户 I D
	SCORE									lTakeScore;								//提取金币
	TCHAR									szPassword[LEN_MD5];					//银行密码
	TCHAR									szMachineID[LEN_MACHINE_ID];			//机器序列
};

//登录银行
struct CMD_GP_UserLogonBank
{
	DWORD									dwUserID;								//用户 I D
	TCHAR									szPassword[LEN_MD5];					//银行密码
	DWORD									dwCdkey;
};

//手机转账金币
struct CMD_GP_MBUserTransferScore
{
	DWORD									dwUserID;								//用户 I D
	BYTE									cbByNickName;							//昵称赠送
	SCORE									lTransferScore;							//转账金币
	TCHAR									szPassword[LEN_MD5];					//银行密码
	TCHAR									szNickName[LEN_NICKNAME];				//目标用户
	TCHAR									szMachineID[LEN_MACHINE_ID];			//机器序列
};

//银行成功
struct CMD_GP_UserInsureSuccess
{
	DWORD									dwUserID;								//用户 I D
	SCORE									lUserScore;								//用户金币
	SCORE									lUserInsure;							//银行金币
	TCHAR									szDescribeString[128];					//描述消息
	TCHAR									szDateTime[64];							//操作时间
};

//手机银行成功
struct CMD_GP_MBUserInsureSuccess
{
	SCORE									lUserScore;								//用户金币
	SCORE									lUserInsure;							//银行金币
	TCHAR									szDescribeString[128];					//描述消息
};

//银行失败
struct CMD_GP_UserInsureFailure
{
	DWORD									lResultCode;							//错误代码
	TCHAR									szDescribeString[128];					//描述消息
};

//提取结果
struct CMD_GP_UserTakeResult
{
	DWORD									dwUserID;								//用户 I D
	SCORE									lUserScore;								//用户金币
	SCORE									lUserInsure;							//银行金币
};

//查询银行
struct CMD_GP_QueryInsureInfo
{
	DWORD									dwUserID;								//用户 I D
	TCHAR									szPassword[LEN_MD5];					//银行密码
};

//查询明细
struct CMD_GP_QueryBankDetail
{
	DWORD									dwPlazaVersion;							//广场版本
	DWORD									dwUserID;								//用户 I D
	BYTE									cbTransferIn;							//明细类型 
};

//明细结果
struct CMD_GP_BankDetailResult
{
	BYTE									cbState;
	BYTE									cbTransferIn;
	TCHAR									szNickName[LEN_NICKNAME];
	DWORD									dwGameID;
	SCORE									lSwapScore;
	TCHAR									szDateTime[64];
};
//查询用户
struct CMD_GP_QueryUserInfoRequest
{
	BYTE									cbByNickName;							//昵称赠送
	TCHAR									szNickName[LEN_NICKNAME];				//目标用户
};

//转账金币
struct CMD_GP_UserTransferScore
{
	DWORD									dwPlazaVersion;							//广场版本
	DWORD									dwUserID;								//用户 I D
	DWORD									dwIP;
	DWORD									dwGameID;
	LONGLONG								lTransferScore;							//转账金币
	TCHAR									szPassword[LEN_MD5];					//银行密码
	TCHAR									szNickName[LEN_NICKNAME];				//目标用户
	TCHAR									szMachineID[LEN_MACHINE_ID];			//机器序列
	TCHAR									szCdkey[LEN_MD5];
};

//用户信息
struct CMD_GP_UserTransferUserInfo
{
	DWORD									dwTargetGameID;							//目标用户
	TCHAR									szNickName[LEN_NICKNAME];				//目标用户
};

//////////////////////////////////////////////////////////////////////////////////

//操作失败
struct CMD_GP_OperateFailure
{
	DWORD									lResultCode;							//错误代码
	TCHAR									szDescribeString[128];					//描述消息
};

//操作成功
struct CMD_GP_OperateSuccess
{
	DWORD									lResultCode;							//操作代码
	TCHAR									szDescribeString[128];					//成功消息
};

//////////////////////////////////////////////////////////////////////////////////
//远程服务

#define MDM_GP_REMOTE_SERVICE				4										//远程服务

//查找服务
#define SUB_GP_C_SEARCH_DATABASE			100										//数据查找
#define SUB_GP_C_SEARCH_CORRESPOND			101										//协调查找

//查找服务
#define SUB_GP_S_SEARCH_DATABASE			200										//数据查找
#define SUB_GP_S_SEARCH_CORRESPOND			201										//协调查找

//////////////////////////////////////////////////////////////////////////////////

//协调查找
struct CMD_GP_C_SearchCorrespond
{
	DWORD									dwGameID;								//游戏标识
	TCHAR									szNickName[LEN_NICKNAME];				//用户昵称
};

//协调查找
struct CMD_GP_S_SearchCorrespond
{
	WORD									wUserCount;								//用户数目
	tagUserRemoteInfo						UserRemoteInfo[16];						//用户信息
};

//////////////////////////////////////////////////////////////////////////////////

//注册帐号
struct CMD_MB_RegisterAccounts
{
	//系统信息
	WORD									wModuleID;								//模块标识
	DWORD									dwPlazaVersion;							//广场版本
	BYTE									cbDeviceType;							//设备类型

	//密码变量
	TCHAR									szLogonPass[LEN_MD5];					//登录密码
	TCHAR									szInsurePass[LEN_MD5];					//银行密码

	//注册信息
	WORD									wFaceID;								//头像标识
	BYTE									cbGender;								//用户性别
	TCHAR									szAccounts[LEN_ACCOUNTS];				//登录帐号
	TCHAR									szNickName[LEN_NICKNAME];				//用户昵称

	//连接信息
	TCHAR									szMachineID[LEN_MACHINE_ID];			//机器标识
	TCHAR									szMobilePhone[LEN_MOBILE_PHONE];		//电话号码
	int										iGameVer;								//系统版本
};

////////////////////////////////////////////////////////////////////////////////////////////////
//房间列表
#define MDM_GP_ROOMINFO_SERVER				8

#define SUB_GP_GET_ROOMINFO					1										//获取房间信息
#define SUB_GP_GET_ROOMIFNO_RESULT			2										//房间消息结果

//请求房间列表信息
struct CMD_GP_RoomInfo_Requst
{
	DWORD									dwUserID;								//用户userID
	DWORD									dwKindID;								//游戏KindID
	BYTE									cbPlatformType;							//平台标识
	DWORD									dwGameVersion;							//游戏版本
};

//消息服务
#define MDM_GP_MESSAGE						11

#define SUB_MS_C_USER_LOGON					1										//登陆消息
#define SUB_MS_C_FRIEND_LIST				2										//请求好友
#define SUB_MS_C_FRIEND_MESSAGE				3										//请求消息
#define SUB_MS_C_ADD_FRIEND					4										//添加好友
#define SUB_MS_C_DEL_FRIEND					5										//删除好友
#define SUB_MS_C_SEND_MESSAGE				6										//发送消息
#define SUB_MS_C_READ_MESSAGE				7										//阅读消息

#define SUB_MS_S_USER_LOGON					101										//登陆结果
#define SUB_MS_S_FRIEND_LIST				102										//应答好友
#define SUB_MS_S_FRIEND_MESSAGE				103										//应答消息
#define SUB_MS_S_ADD_FRIEND					104										//添加应答
#define SUB_MS_S_DEL_FRIEND					105										//删除应答
#define SUB_MS_S_SEND_MESSAGE				106										//发送应答
#define SUB_MS_S_NOTIFY_MESSAGE				107										//消息通知
#define SUB_MS_S_READ_MESSAGE				108										//阅读应答

#define SUB_MS_S_HORN_TRADER				201										//商人喇叭
#define SUB_MS_S_HORN_GAME					202										//游戏喇叭
#define SUB_MS_S_HORN_DEFEND				203										//维护喇叭

//登陆请求
struct CMD_MS_C_UserLogon
{
	DWORD									dwUserID;								//玩家标识
	TCHAR									szPassword[LEN_PASSWORD];				//用户密码
	DWORD									dwChannel;								//登录渠道
};

struct CMD_MS_S_UserLogon
{
	DWORD									lResultCode;							//错误代码
	TCHAR									szDescribeString[128];					//描述消息
};

//好友请求
struct CMD_MS_C_FriendList
{
	DWORD									dwFriendID;								//玩家标识
};

//好友应答
struct CMD_MS_S_FriendList
{
	DWORD									dwFriendID;								//好友标识
	WORD									wFriendFaceID;							//好友头像
	TCHAR									szFriendNickName[LEN_NICKNAME];			//好友昵称
	TCHAR									szFriendFaceUrl[256];					//网络头像
	TCHAR									szFriendUnderWrite[256];				//个性签名
};

//好友消息
struct CMD_MS_C_FriendMessage
{
	DWORD									dwFriendID;								//玩家标识
};

//消息应答
struct CMD_MS_S_FriendMessage
{
	DWORD									dwUserID;								//用户标识
	DWORD									dwFriendID;								//好友标识
	WORD									wType;									//消息类型
	TCHAR									szMessage[256];							//消息内容
	WORD									wIsRead;								//是否阅读
	TCHAR									szCollectDate[64];						//消息时间
};

//添加好友
struct CMD_MS_C_AddFriend
{
	DWORD									dwUserID;								//玩家标识
	DWORD									dwFriendID;								//好友标识
};

//添加应答
struct CMD_MS_S_AddFriend
{
	DWORD									lResultCode;							//错误代码
	TCHAR									szDescribeString[128];					//描述消息

	DWORD									dwFriendID;								//好友标识
	WORD									wFriendFaceID;							//好友头像
	TCHAR									szFriendNickName[LEN_NICKNAME];			//好友昵称
	TCHAR									szFriendFaceUrl[256];					//网络头像
	TCHAR									szFriendUnderWrite[256];				//个性签名
};

//删除好友
struct CMD_MS_C_DelFriend
{
	DWORD									dwUserID;								//玩家标识
	DWORD									dwFriendID;								//好友标识
};

//删除应答
struct CMD_MS_S_DelFriend
{
	DWORD									lResultCode;							//错误代码
	TCHAR									szDescribeString[128];					//描述消息

	DWORD									dwFriendID;								//好友标识
};

//发送消息
struct CMD_MS_C_SendMessage
{
	DWORD									dwUserID;								//玩家标识
	DWORD									dwFriendID;								//好友标识
	WORD									wType;									//消息类型
	TCHAR									szMessage[256];							//消息内容
};

//发送应答
struct CMD_MS_S_SendMessage
{
	DWORD									dwFriendID;								//好友标识
	WORD									wType;									//消息类型
	TCHAR									szMessage[256];							//消息内容
	WORD									wIsRead;								//是否阅读
	TCHAR									szCollectDate[64];						//消息时间
};

//消息通知
struct CMD_MS_S_NotifyMessage
{
	DWORD									dwUserID;								//玩家标识
	DWORD									dwFriendID;								//好友标识
	WORD									wType;									//消息类型
	TCHAR									szMessage[256];							//消息内容
	WORD									wIsRead;								//是否阅读
	TCHAR									szCollectDate[64];						//消息时间
};


//阅读消息
struct CMD_MS_C_ReadMessage
{
	DWORD									dwUserID;								//玩家标识
	DWORD									dwFriendID;								//好友标识
};

//阅读应答
struct CMD_MS_S_ReadMessage
{
	DWORD									dwUserID;								//玩家标识
	DWORD									dwFriendID;								//好友标识
};

//商人喇叭
struct CMD_MS_S_HornTrumpet
{
	DWORD									dwSendUserID;							//用户 I D
	TCHAR									szTrumpetContent[TRUMPET_MAX_CHAR];		//喇叭内容
};

//维护喇叭
struct CMD_MS_S_DefendTrumpet
{
	TCHAR									szTrumpetContent[TRUMPET_MAX_CHAR];		//喇叭内容
};

//游戏喇叭
struct CMD_MS_S_GameTrumpet
{
	TCHAR									szTrumpetContent[256];					//消息内容
};

//////////////////////////////////////////////////////////////////////////////////
//更新命令
#define MDM_GP_UPDATE						190

#define SUB_GP_C_CHECK_MODEL				1										//检测审核模式
#define SUB_GP_C_CHECK_PLAZAR_RES			2										//检测大厅资源
#define SUB_GP_C_CHECK_GAME_RES				3										//检测游戏资源

#define SUB_GP_S_CHECK_MODEL				101										//检测审核模式返回
#define SUB_GP_S_CHECK_PLAZAR_RES			102										//检测大厅资源返回
#define SUB_GP_S_CHECK_GAME_RES				103										//检测游戏资源返回

struct CMD_GP_C_CHECK_MODEL
{
	DWORD									ClientIP;								//客户地址
	TCHAR									szMachineID[LEN_MACHINE_ID];			//机器码
	DWORD									VersionNumber;							//版本号
	DWORD									dwChannel;								//渠道号
};

struct CMD_GP_S_CHECK_MODEL
{
	BYTE									bState;									//渠道状态: 1为审核模式,0为非审核模式
	BYTE									bPhoneCode;								//需要验证: 1为注册需要手机验证, 0不需要手机验证
};

struct CMD_GP_C_CHECK_PLAZAR_RES
{
	DWORD									dwResversion;							//版本号
	DWORD									dwChannel;								//渠道号
};

struct CMD_GP_S_CHECK_PLAZAR_RES
{
	bool									bSuccess;								//是否更新
	DWORD									dwServerResVerSion;						//服务器资源版本
	DWORD									dwResZipSize;							//更新的资源大小
	TCHAR									szResName[128];							//资源名称
	TCHAR									szResMD5[LEN_MD5];						//MD5大小 
	BYTE									byDownLoadLinkCount;					//更新条数
	TCHAR									szDownLoadUrl[512];						//更新地址
};

struct CMD_GP_C_CHECK_GAME_RES
{
	WORD									wKindID;								//游戏类型
	DWORD									dwChannel;								//渠道号
	DWORD									dwClinetVersion;						//客户端版本号
};

struct CMD_GP_S_CHECK_GAME_RES
{
	//游戏类型
	WORD									wKindID;								//游戏类型

																					//更新信息
	TCHAR									szProcessName[LEN_PROCESS];				//进程名称
	DWORD									szDownloadSize;							//下载大小
	TCHAR									szDownloadMd5[LEN_MD5];					//下载校验

																					//更新地址
	BYTE									byDownLoadLinkCount;					//下载条数
	TCHAR									szDownLoadUrl[DOWNLOAD_LINK_COUNT][64];	//下载地址
};

/////////////////////比赛相关/////////////////////////////////////////////////////
#define MDM_GT_PLAZA						1000

#define SUB_GT_USER_LOGON_REQ       		1										//用户登陆全局服务器
#define SUB_GT_USER_REQ_MATCHLIST   		2										//请求比赛列表
#define SUB_GT_USER_SIGN_UP_REQ     		3										//请求报名(客户端->比赛服务器)
#define SUB_GT_USER_UN_SIGN_UP_REQ  		4										//取消报名(客户端->比赛服务器)


#define SUB_GT_MATCH_PLAZA_LONGON   		100										//连接全局比赛服务器成功
#define SUB_GT_MATCH_PLAZA_FINISH   		101										//发送比赛数据完成
#define SUB_GT_MATCH_INFO           		102										//发送比赛信息
#define SUB_GT_MATCH_MESSAGE                103										//服务器返回描述消息
#define SUB_GT_MATCH_UPDATE                 104										//更新一次比赛人数
#define SUB_GT_MATCH_PREPARE_START          105										//比赛准备开始，通知客户端连接比赛服务器
#define SUB_GT_UPDATELATTERBUTTON           106										//更新报名状态

#define MDM_GT_MATCH_SERVER		    		10003
#define SUB_GT_COMMIT_SIGN_UP_USERS         1										//提交报名玩家数据到比赛服务器.包格式 cbMatchID,userid|accout,userid|accout,userid|accout,userid|accout
#define SUB_GT_MATCH_CREATE_NOTIFY          2										//通知比赛服务器比赛创建，数据结构同tagMatchInfo
#define SUB_GT_MATCH_SERVER_PREPARE_START   3										//通知比赛服务器在规定时间后开始某个比赛
#define SUB_GT_DELETELADDERUSER             4										//删除挂的人

#define SUB_GT_REG_MATCH_SERVER_REQ         100										//注册比赛服务器请求
#define SUB_GT_MATCH_READY_REQ				101										//比赛准备请求
#define SUB_GT_MATCH_MATCHPEOPLE_REQ        102										//发送客户端连接请求


typedef struct CMD_GT_RegMatchServerReq
{
	WORD									wKindID;								//类型ID
	DWORD									dwServerIP;								//ip地址
	WORD									wServerPort;							//端口
	WORD									wChairCount;							//几个人的游戏
}tagMatchServerParam;


//客户端请求登录
struct CMD_GT_UserLogon
{
	DWORD									dwUserID;								//用户 I D
	DWORD									dwGameID;								//用户GameID
	TCHAR									szAccountName[LEN_ACCOUNTS];			//用户名称
	BYTE									cbGender;								//用户性别
	BYTE									cbFaceID;								//用户头像
};

struct CMD_GT_ReqLogon
{
	int										RetLogon;								//0是成功，1是断线 2是在线 
};

struct CMD_GT_Message
{
	TCHAR									szErrorDescribe[128];					//错误消息
};

//比赛准备开始
struct CMD_GT_MatchServerPrepareStart
{
	int										cbMatchID;
	WORD									wSecondsAheadStartMatch;
};

//比赛人数更新
struct CMD_GT_MatchUpdate
{
	int 									cbMatchID;
	WORD									wKindID;
	WORD									wCurrentSignUp;							//当前报名人数
};

//用户请求报名，取消报名
struct CMD_GT_UserSignUpReq
{
	DWORD									dwUserID;								//用户 I D
	int 									cbMatchID;								//比赛ID
	int										OpenationType;							//操作类型 1：报名 0 取消
};

//简洁用户数据
struct tagClippedUserData 
{
	DWORD									dwUserID;								//用户 I D
	BYTE									cbGender;								//性别
	WORD									wFaceID;								//头像I D
	DWORD									dwGameID;
	TCHAR									szAccounts[LEN_ACCOUNTS];				//登陆帐号
	DWORD									wUserIndex;								//用户索引

};

//比赛准备开始
struct CMD_GT_MatchPrepareStart
{
	int 									cbMatchID;								//比赛场次ID
	DWORD									dwServerIP;								//链接的IP
	WORD									wServerPort;							//链接的端口
};

struct CMD_GT_DeleteLadderUser
{
	DWORD									dwUserID;
	DWORD									dwMatchID;
	DWORD									dwKindID;
	DWORD									dwDefMark;
};

#pragma pack()

#endif