#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#pragma pack(1)

#define  MDM_GP_KZ_SERVER				20
#define  SUB_GP_KZ_REGISTERKZ			1
#define  SUB_GP_KZ_TEST					2
//////////////////////////////////////////////////////////////////////////////////
#define MDM_GP_LOGON					1								//广场登录

//登录模式
#define SUB_GP_MASTER_LOGON_ACCOUNTS	19								//管理员帐号登录

//登录结果
#define SUB_GP_LOGON_FAILURE			101								//登录失败
#define SUB_GP_VALIDATE_MBCARD			103                             //登录失败


//帐号登录
struct CMD_GP_MasterLogonAccounts
{
	TCHAR					szMachineID[LEN_MACHINE_ID];				//机器序列
	DWORD					dwClientIp;
	//登录信息
	TCHAR					szPassword[LEN_MD5];						//登录密码
	TCHAR					szAccounts[LEN_ACCOUNTS];					//登录帐号
};

//登录成功
struct CMD_GP_LogonSuccess
{
	//属性资料
	WORD					wFaceID;									//头像标识
	DWORD					dwUserID;									//用户 I D
	DWORD					dwGameID;									//游戏 I D

	//用户成绩
	SCORE					lUserScore;									//用户金币
	SCORE					lUserInsure;								//用户银行

	//用户信息
	BYTE					cbGender;									//用户性别
	BYTE					cbMoorMachine;								//锁定机器
	TCHAR					szAccounts[LEN_ACCOUNTS];					//登录帐号
	TCHAR					szNickName[LEN_ACCOUNTS];					//用户昵称

	//附加信息
	bool					IsLockMobile;								//手机绑定
	int                     RegisterFrom;								//注册渠道
	int                     IsChangePWD;								//修改过密码   0没改，1修改过
	int                     GameLogonTimes;								//是否是第一次登陆 0第一次

	//配置信息
	BYTE                    cbShowServerStatus;							//显示服务器状态
	BYTE                    byServerLinkCount;							//下发服务器个数
	BYTE                    byDownLoadLinkCount;						//下载服务器个数
	TCHAR					szDownServerLinkUrl[SERVER_LINK_COUNT][32];	//下发服务器列表
	TCHAR					szDownLoadUrl[DOWNLOAD_LINK_COUNT][32];		//下载服务器列表
};

//登录失败
struct CMD_GP_LogonFailure
{
	LONG					lResultCode;								//错误代码
	TCHAR					szDescribeString[128];						//描述消息
};

//登陆完成
struct CMD_GP_LogonFinish
{
	DWORD					dwUserid;									//控制号ID
	int						nControlGrade;								//等级
};

//////////////////////////////////////////////////////////////////////////////////
//列表命令

#define MDM_GP_SERVER_LIST			2									//列表信息

//获取命令
#define SUB_GP_GET_LIST				1									//获取列表
#define SUB_GP_GET_SERVER			2									//获取房间
#define SUB_GP_GET_ONLINE			3									//获取在线
#define SUB_GP_GET_COLLECTION		4									//获取收藏
#define SUB_GP_GET_KIND_ALL			5
#define SUB_GP_GET_SERVER_ALL		6									//获取房间
//列表信息
#define SUB_GP_LIST_TYPE			100									//类型列表
#define SUB_GP_LIST_KIND			101									//种类列表
#define SUB_GP_LIST_NODE			102									//节点列表
#define SUB_GP_LIST_PAGE			103									//定制列表
#define SUB_GP_LIST_SERVER			104									//房间列表
#define SUB_GP_VIDEO_OPTION			105									//视频配置
#define SUB_GP_GET_LIST_KIND		106									//获取类型
#define SUB_GP_GET_LIST_SERVER		107									//获取房间
#define SUB_GP_GET_SYSTEM_MESSAGE   108                                 //获取公告
#define SUB_GP_GET_NEWSUM           109                                 //获取公告数量

#define SUB_GP_GAME_SETTING         120                                 //获取游戏配置
//完成信息
#define SUB_GP_LIST_FINISH			200									//发送完成
#define SUB_GP_SERVER_FINISH		201									//房间完成

//在线信息
#define SUB_GR_KINE_ONLINE			300									//类型在线
#define SUB_GR_SERVER_ONLINE		301									//房间在线

//////////////////////////////////////////////////////////////////////////////////

//获取在线
struct CMD_GP_GetOnline
{
	WORD					wServerCount;								//房间数目
	WORD					wOnLineServerID[MAX_SERVER];				//房间标识
};

//游戏在线
struct CMD_GP_KindOnline
{
	WORD					wKindID;									//类型标识
	DWORD					dwOnLineCount;								//在线人数
};

//房间在线
struct CMD_GP_ServerOnline
{
	WORD					wServerID;									//房间标识
	DWORD					dwOnLineCount;								//在线人数
};

//获取游戏配置
struct CMD_GP_GameSetting
{
	int						nBairenKindCount;
	WORD					wBairenKindData[50];
	int						nShowKindCount;
	WORD					wShowKindData[100];
};
//////////////////////////////////////////////////////////////////////////////////
//服务命令

#define MDM_GP_USER_SERVICE					3							//用户服务

#define SUB_GP_USER_CONTROL_XMS_CON			104 
#define SUB_GP_USER_CONTROL_XMS_DELCON		105
#define SUB_GP_USER_CONTROL_XMS_PM			106							//房间控制信息

#define SUB_GP_AddServerStock				118							//增减房间库存
#define SUB_GP_AddServerStockReturn			119							//增减房间库存，返回
#define SUB_GP_QueryServerStock				120							//查询房间库存
#define SUB_GP_QueryServerStockReturn		121							//查询房间库存，返回

//房间难度
#define SUB_GP_QueryServerDegree			128							//查询房间难度
#define SUB_GP_QueryServerDegreeReturn		129							//查询房间难度返回
#define SUB_GP_SetServerDegree				130							//设置房间难度
#define	SUB_GP_SetServerDegreeReturn		131							//设置房间难度返回

//百人游戏发送和接收消息
#define SUB_GP_ConToBairenMessage			132							//控制发往百人
#define SUB_GP_ConToBairenMessageReturn		133							//控制发往百人返回

//控制功能
#define SUB_GP_CONTROL_FORBID_CHAT			140							//禁止聊天
#define SUB_GP_CONTROL_ALLOW_CHAT			141							//允许聊天
#define SUB_GP_CONTROL_KICK_USER			142							//踢出玩家

//操作结果
#define SUB_GP_USER_CONTROL_XMS				902							//发送信息 弹出控制框
#define SUB_GP_USER_CONTROL_XMS_ONE			903							//发送被控人数，服务器—》客户端

#define SUB_GP_USER_CONTROL_XMS_UPONE		904							//发送被控人数，服务器—》客户端
#define SUB_GP_USER_CONTROL_XMS_FINSH		905							//发送被控人数，服务器—》客户端
#define SUB_GP_USER_CONTROL_XMS_DELONE		906							//发送被控人数，服务器—》客户端
#define SUB_GP_ONLINE_USER					907							//玩家上线
#define SUB_GP_ONLINE_USER_LEAVE			908							//玩家离线
#define SUB_GP_ONLINE_USER_UPSCORE			909							//玩家分数
#define SUB_GP_USER_CONTROL_UPXMS			910							//发送信息 弹出控制框
#define SUB_GP_ONLINE_USER_WRITE_SCORE		911							//发送信息 弹出控制框
#define SUB_GP_USER_CONTROL_UPXMSSTOCK		912							//发送信息 弹出控制框
#define SUB_GP_USER_CONTROL_NOTSTOCK		913							//发送信息 弹出控制框

//房间彩金
#define SUB_GP_ROOM_HANDESEL_LIST			949							//房间彩金列表

#define SUB_GP_GET_ROOM_HANDESEL			950							//获取房间彩金		
#define SUB_GP_GET_ROOM_HANDESEL_RETURN		951							//获取房间彩金返回

#define SUB_GP_ADD_ROOM_HANDESEL			952							//修改房间彩金		
#define SUB_GP_ADD_ROOM_HANDESEL_RETURN		953							//修改获取房间彩金返回

#define SUB_GP_SET_ROOM_HANDESEL_DEGREE		954							//设置房间难度		
#define SUB_GP_SET_ROOM_HANDESEL_DEGREE_RETURN	955						//设置获取房间难度返回

////////////////////////////////////////////////////////////////////////////////////////

//禁止聊天
struct CMD_GP_ControlForbidChat
{
	DWORD					dwUserID;									//玩家标识
	WORD					wServerID;									//房间序号
};

//允许聊天
struct CMD_GP_ControlAllowChat
{
	DWORD					dwUserID;									//玩家标识
	WORD					wServerID;									//房间序号
};

//踢出玩家
struct CMD_GP_ControlKickUser
{
	DWORD					dwUserID;									//玩家标识
	WORD					wServerID;									//房间序号
};

//库存不足
struct CMD_GP_NotStock
{
	WORD					lResultCode;								//错误代码
	WORD                    wServerID;
};

//在线用户
struct CMD_LineUser
{
	DWORD		            dwUserID;
	DWORD		            dwGameID;
	TCHAR					szNickName[LEN_ACCOUNTS];					//用户昵称

	SCORE					llCurScore;									//身上金币
	SCORE					llInsureScore;								//银行金币

	WORD                    wKindId;									//游戏ID
	WORD                    wServerId;									//房间ID

	DWORD					dwChannel;									//登录渠道
};

//用户控制
struct CMD_ControlUserInfo
{
	DWORD					dwGameID;									//玩家GameID
	DWORD					dwUserID;									//玩家UserID
	DWORD					dwConUserID;								//控制号UserID
	int						nWinType;									//1输,2赢,其他无
	int						nRate;										//几率(100为底)
	int						nGameCount;									//局数(等于0表示按照几率控制；大于0则100%控制，最大10局)
	LONGLONG				llMaxScore;									//控制金币
	LONGLONG				llCashScore;								//兑现金币(初始为0，时常更新)
	LONGLONG				llNodeScore;								//节点金币(初始为0，暂时只对捕鱼游戏有效)
	LONGLONG				llStartTime;								//起始时间(年4位,月日时分秒各2位)，如： 201801220945
	LONGLONG				llEndTime;									//结束时间(年4位,月日时分秒各2位)，如： 201801221159
	LONGLONG				llPrizeScore;								//中奖分数,0无限制,非0限制

	WORD					wServerID;									//房间信息
};

//房间控制
struct CMD_ControlServerData
{
	BYTE					byMobWinOrLost;
	BYTE					byMobdds;
	BYTE					byPcWinOrLost;
	BYTE					byPcdds;
	WORD					wKind;
	WORD					wServerId;

	LONGLONG				llServerStort;								//总的库存
	LONGLONG				llServerStortLv1;							//一级库存
	LONGLONG				llServerStortLv2;							//二级库存
	LONGLONG				llServerStortLv3;							//三级库存
	LONGLONG				llServerStortLv4;							//四级库存
	LONGLONG				llServerStortLv5;							//五级库存
	LONGLONG				llLeast;									//库存底限
	FLOAT					fServerDegree;								//房间难度		
};

//获取房间彩金
struct CMD_C_ControlServerRoomHandsel
{
	WORD					wServerId;									//房间信息	
};

struct CMD_S_ControlServerRoomHandsel
{
	WORD					wServerId;									//房间信息
	WORD					wKindID;									//房间类型
	TCHAR					szServerName[LEN_SERVER];					//房间名称
	FLOAT					fDegree;									//房间难度
	LONGLONG				llHandselLv1;								//房间彩金1
	LONGLONG				llHandselLv2;								//房间彩金2
	LONGLONG				llHandselLv3;								//房间彩金3
	LONGLONG				llHandselLv4;								//房间彩金4
	LONGLONG				llHandselLv5;								//房间彩金5
	LONGLONG				llHandselLv6;								//房间彩金6
	LONGLONG				llHandselLv7;								//房间彩金7
	LONGLONG				llHandselLv8;								//房间彩金8
};

//增加房间彩金
struct CMD_C_AddControlServerRoomHandsel
{
	WORD					wServerId;									//房间信息
	bool					bAddHandsel;								//true加，false减
	WORD					wType;										//彩金类型
	LONGLONG				llHandsel;									//彩金数值	
};

struct CMD_S_AddControlServerRoomHandsel
{
	WORD					wServerId;									//房间信息
	WORD					wKindID;									//房间类型
	TCHAR					szServerName[LEN_SERVER];					//房间名称
	FLOAT					fDegree;									//房间难度
	LONGLONG				llHandselLv1;								//房间彩金1
	LONGLONG				llHandselLv2;								//房间彩金2
	LONGLONG				llHandselLv3;								//房间彩金3
	LONGLONG				llHandselLv4;								//房间彩金4	
	LONGLONG				llHandselLv5;								//房间彩金5
	LONGLONG				llHandselLv6;								//房间彩金6
	LONGLONG				llHandselLv7;								//房间彩金7
	LONGLONG				llHandselLv8;								//房间彩金8
};

//房间彩金难度
struct CMD_C_SetControlServerRoomHandselDegree
{
	WORD					wServerId;									//房间信息
	FLOAT					fDegree;									//彩金难度	
};

struct CMD_S_SetControlServerRoomHandselDegree
{
	WORD					wServerId;									//房间信息
	WORD					wKindID;									//房间类型
	TCHAR					szServerName[LEN_SERVER];					//房间名称
	FLOAT					fDegree;									//房间难度
	LONGLONG				llHandselLv1;								//房间彩金1
	LONGLONG				llHandselLv2;								//房间彩金2
	LONGLONG				llHandselLv3;								//房间彩金3
	LONGLONG				llHandselLv4;								//房间彩金4
	LONGLONG				llHandselLv5;								//房间彩金5
	LONGLONG				llHandselLv6;								//房间彩金6
	LONGLONG				llHandselLv7;								//房间彩金7
	LONGLONG				llHandselLv8;								//房间彩金8
};

//个人控制结构图
struct  CMD_C_WriteControlUser
{
	DWORD					dwGameID;									//玩家GameID
	int						nWinType;									//1输,2赢,其他无
	int						nRate;										//几率(100为底)
	int						nGameCount;									//局数(等于0表示按照几率控制；大于0则100%控制，最大10局)
	LONGLONG				llMaxScore;									//控制金币
	LONGLONG				llStartTime;								//起始时间(年4位,月日时分秒各2位)，如： 201801220945
	LONGLONG				llEndTime;									//结束时间(年4位,月日时分秒各2位)，如： 201801221159
	LONGLONG				llPrizeScore;								//中奖分数,0无限制,非0限制
	WORD					wServerID;
};

//个人控制结构图
struct  CMD_S_WriteControlUser
{
	DWORD					dwGameID;									//玩家GameID
	int						nWinType;									//1输,2赢,其他无
	int						nRate;										//几率(100为底)
	int						nGameCount;									//局数(等于0表示按照几率控制；大于0则100%控制，最大10局)
	LONGLONG				llMaxScore;									//控制金币
	LONGLONG				llCashScore;								//兑现金币(初始为0，时常更新)
	LONGLONG				llNodeScore;								//节点金币(初始为0，暂时只对捕鱼游戏有效)
	LONGLONG				llStartTime;								//起始时间(年4位,月日时分秒各2位)，如： 201801220945
	LONGLONG				llEndTime;									//结束时间(年4位,月日时分秒各2位)，如： 201801221159
	LONGLONG				llPrizeScore;								//中奖分数,0无限制,非0限制

	WORD					wServerID;									//房间ID
	int						nCount;										//保存的数据条数
};

//房间控制结构图
struct CMD_C_WriteControlServer
{
	TCHAR					MobOdds[4];									//手机控制概率
	TCHAR					Mobflag[4];									//手机控制类型
	TCHAR					PcOdds[4];									//pc控制概率
	TCHAR					Pcflag[4];									//pc控制类型

	WORD					wConType;									//0房间，1游戏，2所有
	WORD					wServerId;									//房间ID
	WORD					wKindID;									//游戏类型

	TCHAR					szServerName[32];							//房间名称
};

//个人控制结构图
struct  CMD_S_WriteControlServer
{
	TCHAR					MobOdds[4];									//手机控制概率
	TCHAR					Mobflag[4];									//手机控制类型
	TCHAR					PcOdds[4];									//pc控制概率
	TCHAR					Pcflag[4];									//pc控制类型

	WORD					wConType;									//0房间，1游戏，2所有
	WORD					wKind;										//游戏类型
	WORD					wServerId;									//房间ID

	LONGLONG				llServerStort;								//总的库存
	LONGLONG				llServerStortLv1;							//一级库存
	LONGLONG				llServerStortLv2;							//二级库存
	LONGLONG				llServerStortLv3;							//三级库存
	LONGLONG				llServerStortLv4;							//四级库存
	LONGLONG				llServerStortLv5;							//五级库存
	LONGLONG				llLeast;									//库存底限
	FLOAT					fServerDegree;								//房间难度

	LONGLONG				llStockLevelScore;							//分级条件

	int						Count;										//保存的数据条数
};


//个人控制结构图
struct CMD_S_UpdataControlServer
{
	TCHAR					MobOdds[4];									//手机控制概率
	TCHAR					Mobflag[4];									//手机控制类型
	TCHAR					PcOdds[4];									//pc控制概率
	TCHAR					Pcflag[4];									//pc控制类型

	WORD					wConType;
	WORD					wKind;
	WORD					wServerId;
	int						Count;										//保存的数据条数
};

//删除玩家控制
struct CMD_C_ControlDelUser
{
	DWORD					dwGameID;
	WORD					wServerID;
};

//删除玩家控制，发往客户端
struct CMD_S_ControlDelUser
{
	DWORD					dwGameID;
	WORD					wServerID;
	int						nCount;
};

//用户进入
struct CMD_S_PlayUserUpdataScore
{

	DWORD					dwGameID;									//游戏标识
	SCORE					llCurScore;									//身上金币
	SCORE					llInsureScore;								//银行金币
	WORD                    wServerID;
};

//用户进入
struct CMD_S_PlayUserWriteScore
{

	DWORD					dwGameID;									//游戏标识
	LONGLONG                llChanScore;
	BYTE                    byWriteMode;
	WORD                    wServerID;
};

////////////////////////////////////////////////////////////////////////////////////////////////
//操作失败
struct CMD_GP_OperateFailure
{
	LONG					lResultCode;								//错误代码
	TCHAR					szDescribeString[128];						//描述消息
};

//操作成功
struct CMD_GP_OperateSuccess
{
	LONG					lResultCode;								//操作代码
	TCHAR					szDescribeString[128];						//成功消息
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
	DWORD					dwGameID;									//游戏标识
	TCHAR					szNickName[LEN_NICKNAME];					//用户昵称
};

//协调查找
struct CMD_GP_S_SearchCorrespond
{
	WORD					wUserCount;									//用户数目
	tagUserRemoteInfo		UserRemoteInfo[16];							//用户信息
};

//////////////////////////////////////////////////////////////////////////////////
//后台服务

#define MDM_GP_WEB_SERVICE			5									//后台服务

#define SUB_GP_C_CONTROL_USER		100									//账号控制

//个人控制结构图
struct  CMD_C_WebControlUser
{
	int						iEmployUserID;								//雇员UserID
	CHAR					szReason[128];								//控制原因
	DWORD					dwGameID;									//玩家GameID
	int						nWinType;									//1输,2赢,其他无
	int						nRate;										//几率(100为底)
	int						nGameCount;									//局数(等于0表示按照几率控制；大于0则100%控制，最大10局)
	LONGLONG				llMaxScore;									//控制金币
	LONGLONG				llStartTime;								//起始时间(年4位,月日时分秒各2位)，如： 201801220945
	LONGLONG				llEndTime;									//结束时间(年4位,月日时分秒各2位)，如： 201801221159
	LONGLONG				llPrizeScore;								//中奖分数,0无限制,非0限制
};

#pragma pack()

#endif