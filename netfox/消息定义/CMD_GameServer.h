#ifndef CMD_GAME_SERVER_HEAD_FILE
#define CMD_GAME_SERVER_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//用户权限
#define MASTER_FLAG					86474702
#define MASTER_FLAG_PLAY            78985426

//////////////////////////////////////////////////////////////////////////////////
//登录命令

#define MDM_GR_LOGON					1								//登录信息

//登录模式
#define SUB_GR_LOGON_USERID				1								//I D 登录
#define SUB_GR_ANDROID_SUCCESS			2                               //机器人直接登陆成功

//登录结果
#define SUB_GR_LOGON_SUCCESS			100								//登录成功
#define SUB_GR_LOGON_FAILURE			101								//登录失败
#define SUB_GR_LOGON_FINISH				102								//登录完成
#define SUB_GR_LOGON_GAMERECORD			104                             //游戏记录
#define SUB_GR_LOGON_UPDATAADDRELIST	105								//下发IP

//////////////////////////////////////////////////////////////////////////////////

//I D 登录
struct CMD_GR_UserEnterRoom
{
	//版本信息
	DWORD							dwPlazaVersion;						//广场版本
	DWORD							dwFrameVersion;						//框架版本
	DWORD							dwClientVersion;					//进程版本

	//登录信息
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	WORD							wKindID;							//类型索引
	DWORD							dwClientIP;							//玩家IP
	int                             dwVersion;                          //客户端版本
	int                             dwLoginFrom;                        //登录模式
	int								intChannel;							//渠道号
};

//登录成功
struct CMD_GR_LogonSuccess
{
	DWORD							dwUserRight;						//用户权限
	DWORD							dwMasterRight;						//管理权限
};

//登录失败
struct CMD_GR_LogonFailure
{
	WORD                            wLockServerid;
	LONG							lErrorCode;							//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//私人场命令

#define MDM_GR_PRIVATE						10							//私人场命令

#define SUB_GR_PRIVATE_INFO					401							//私人场信息
#define SUB_GR_CREATE_PRIVATE				402							//创建私人场
#define SUB_GR_CREATE_PRIVATE_SUCESS		403							//创建私人场成功
#define SUB_GR_JOIN_PRIVATE					404							//加入私人场
#define SUB_GF_PRIVATE_ROOM_INFO			405							//私人场房间信息
#define SUB_GR_PRIVATE_DISMISS				406							//私人场请求解散
#define SUB_GF_PRIVATE_END					407							//私人场结算
#define SUB_GR_RIVATE_AGAIN					408							//创建私人场
#define SUB_GR_RIVATE_CUR_ROOM				409							//当前已开房间	
#define SUB_GR_ROOMINFO_RECORD				410							//开房间记录      
#define SUB_GR_PIVATE_END_RECORD			411							//总局记录
#define SUB_GR_PIVATE_MAINUSER_DISMISS		412							//带开者解散房间
#define SUB_GR_PRIVATE_GAMEPLAYDISSREQ		413							//房主申请解散 
#define SUB_GF_PRIVATE_COINCARDROOMRESULT	500							//金币开房场结算

//代开申请解散
struct CMD_GR_GameplayDissReq
{
	TCHAR      szUserName[LEN_NICKNAME];
};

//带开者解散房间
struct CMD_GR_MainUserDismiss
{
	DWORD						   dwUserID;
	DWORD						   dwRoomID;
};

//代开记录回复
struct CMD_GR_RoomInfoRecord
{
	DWORD							dwUserID;							//游戏规则
};

struct tagInvitationInfo
{
	DWORD							dwUserId;							//
	SCORE							InsureScore;						//
	TCHAR							szNotifyContent[128];				//
};

//私人场信息 
struct CMD_GR_Private_Info
{
	enum { MAX_IDEX = 4 };
	WORD							wKindID;
	SCORE							lCostGold;
	BYTE							bPlayCout[MAX_IDEX];					//玩家局数
	SCORE							lPlayCost[MAX_IDEX];					//消耗金币
	SCORE							lCardCost[MAX_IDEX];					//消耗房卡
};

//创建房间
struct CMD_GR_Create_Private_Check
{
	BYTE							cbPlayCnts;								//游戏人数
	BYTE							cbGameType;								//游戏类型（0私有房间，1公有房间)
	BYTE							bPlayCoutIdex;							//游戏局数
	BYTE							bGameTypeIdex;							//游戏类型
	DWORD							bGameRuleIdex;							//游戏规则
	WORD							dBetCrads;								//压住的房卡数
	BYTE							bHelpCreateRoom;						//代开房间
	BYTE                            bAAPlayGame;							//是否AA付
};

//重新开始
struct CMD_GR_Again_Private
{
	char							stHttpChannel[LEN_NICKNAME];			//http获取
};

//创建房间
struct CMD_GF_Create_Private_Sucess
{
	SCORE							lCurSocre;								//当前剩余
	DWORD							dwRoomNum;								//房间ID
};

//创建房间
struct CMD_GR_Join_Private
{
	DWORD							dwRoomNum;								//房间ID
};

//私人场房间信息
struct CMD_GF_Private_Room_Info
{
	WORD							KindId;				//游戏类型
	BYTE							bPlayCoutIdex;		//玩家局数
	BYTE							bGameTypeIdex;		//游戏类型
	DWORD							bGameRuleIdex;		//游戏规则

	BYTE							bStartGame;			//开始游戏
	DWORD							dwPlayCout;			//游戏局数
	DWORD							dwRoomNum;			//房间号
	DWORD							dwCreateUserID;		//创建者
	DWORD							dwPlayTotal;		//总局数

	BYTE							cbRoomType;			//房间类型

	std::vector<int>				kWinLoseScore;		//输赢分数

	void StreamValue(datastream& kData, bool bSend)
	{
		Stream_VALUE(KindId);
		Stream_VALUE(bPlayCoutIdex);
		Stream_VALUE(bGameTypeIdex);
		Stream_VALUE(bGameRuleIdex);
		Stream_VALUE(bStartGame);
		Stream_VALUE(dwPlayCout);
		Stream_VALUE(dwRoomNum);
		Stream_VALUE(dwCreateUserID);
		Stream_VALUE(dwPlayTotal);
		Stream_VALUE(cbRoomType);
		Stream_VALUE(kWinLoseScore);
	}
};

//私人场房间信息
struct CMD_GF_Private_Room_Info_New
{
	WORD            KindId;
	BYTE			bPlayCoutIdex;		//玩家局数
	BYTE			bGameTypeIdex;		//游戏类型
	DWORD			bGameRuleIdex;		//游戏规则 

	BYTE			bStartGame;
	DWORD			dwPlayCout;			//游戏局数
	DWORD			dwRoomNum;
	DWORD			dwCreateUserID;
	DWORD			dwPlayTotal;		//总局数

	BYTE			cbRoomType;
	BYTE			cbPlayUser;			//玩家数量
	DWORD           kWinLoseScore[MAX_CHAIR];
	bool			bHelpCreateRoom;							//是否代开
	TCHAR			szRoomOwnerName[LEN_NICKNAME];				//房主昵称
};

struct CMD_GF_AlternateOpenDescription
{
	DWORD			dwKindID;			//kindid
	BYTE			bPlayCoutIdex;		//玩家局数
	BYTE			bGameTypeIdex;		//游戏类型
	DWORD			bGameRuleIdex;		//游戏规则 
	BYTE			cbPlayUser;			//玩家数量
	DWORD			dwCreateUserID;		//创建人
	DWORD			dwRoomNumber;		//房间号
	DWORD			bOperationChest;	//大赢家&人人分
	BYTE            cbPlayCnts;			//游戏人数
	TCHAR			szRoomOwnerName[LEN_NICKNAME];				//房主昵称
	DWORD			dwStartPlayCout;	//游戏开局数

};


//解散房间
struct CMD_GR_Dismiss_Private
{
	BYTE			bDismiss;			//解散
};

//私人场解散信息
struct CMD_GF_Private_Dismiss_Info
{
	CMD_GF_Private_Dismiss_Info()
	{
		ZeroMemory(this, sizeof(CMD_GF_Private_Dismiss_Info));
	}
	BYTE			 disMisChair;
	USHORT           Lefttime;
	DWORD			dwDissUserCout;
	DWORD			dwDissChairID[MAX_CHAIR];
	DWORD			dwNotAgreeUserCout;
	DWORD			dwNotAgreeChairID[MAX_CHAIR];
};

#define MAX_PRIVATE_ACTION 8
//私人场结算信息
struct CMD_GF_Private_End_Info
{
	std::vector<SCORE> lPlayerWinLose;
	std::vector<BYTE> lPlayerAction;

	void StreamValue(datastream& kData, bool bSend)
	{
		Stream_VALUE(lPlayerWinLose);
		Stream_VALUE(lPlayerAction);
	}
};

//金币开房场结算信息
struct CMD_GF_CoinCardRoomResult
{
	WORD			wChairID;
	DWORD			dwGameID;
	TCHAR			szNickName[LEN_NICKNAME];
	int				nWinCount;
	LONGLONG		llWinScore;
};

//一轮代开记录
struct CMD_GF_Private_HelpRecord
{
	DWORD			dwUserID;
	DWORD			dwRoomCardID;
	DWORD			dwServerID;
	DWORD			dwKindID;
	TCHAR			szRoomName[32];
	TCHAR			szUserName[512];
	TCHAR			szUserScore[512];
	TCHAR			szUserChestScore[512];
};

//////////////////////////////////////////////////////////////////////////////////
//配置命令

#define MDM_GR_CONFIG				2									//配置信息

#define SUB_GR_CONFIG_SERVER		100									//房间配置
#define SUB_GR_CONFIG_FINISH		101									//配置完成
#define SUB_GR_CONFIG_USER_RIGHT	102									//玩家权限
#define SUB_GR_LOGON_VOICEGID       103                                 //语音聊天

//////////////////////////////////////////////////////////////////////////////////

//语音聊天
struct CMD_GR_VOICEID
{
	TCHAR       					dwVoiceGid[37];						//语音标志
	DWORD                           dwChannel;							//发送渠道
};


//用户创建房卡
struct CMD_GP_CREATEROOMCATD
{
	DWORD							dwUserID;							//用户标识
};

//房卡创建回复
struct CMD_GP_RoomCardSuccess
{
	DWORD							CardNumber;							//房间序号 
};


//列表配置
struct CMD_GR_ConfigColumn
{
	BYTE							cbColumnCount;						//列表数目
	tagColumnItem					ColumnItem[MAX_COLUMN];				//列表描述
};

//房间配置
struct CMD_GR_PcConfigServer
{
	//房间属性
	WORD							wTableCount;						//桌子数目
	WORD							wChairCount;						//椅子数目

	//房间配置
	WORD							wServerType;						//房间类型
	DWORD							dwServerRule;						//房间规则
	SCORE                           lGoldLeast;
	SCORE                           lGoldMost;
};
//房间配置
struct CMD_GR_ConfigServer
{
	//房间属性
	WORD							wTableCount;						//桌子数目
	WORD							wChairCount;						//椅子数目

	//房间配置
	WORD							wServerType;						//房间类型
	DWORD							dwServerRule;						//房间规则
};
//道具配置
struct CMD_GR_ConfigProperty
{
	BYTE							cbPropertyCount;					//道具数目
	tagPropertyInfo					PropertyInfo[MAX_PROPERTY];			//道具描述
};

//玩家权限
struct CMD_GR_ConfigUserRight
{
	DWORD							dwUserRight;						//玩家权限
};

//////////////////////////////////////////////////////////////////////////////////
//用户命令
#define MDM_GR_USER						3								//用户信息

//用户动作
#define SUB_GR_USER_RULE				1								//用户规则
#define SUB_GR_USER_LOOKON				2								//旁观请求
#define SUB_GR_USER_SITDOWN				3								//坐下请求
#define SUB_GR_USER_STANDUP				4								//起立请求
#define SUB_GR_USER_INVITE				5								//用户邀请
#define SUB_GR_USER_INVITE_REQ			6								//邀请请求
#define SUB_GR_USER_REPULSE_SIT  		7								//拒绝玩家坐下
#define SUB_GR_USER_KICK_USER			8                               //踢出用户
#define SUB_GR_USER_INFO_REQ			9                               //请求用户信息
#define SUB_GR_USER_CHAIR_REQ			10                              //请求更换位置
#define SUB_GR_USER_CHAIR_INFO_REQ		11                              //请求椅子用户信息
#define SUB_GR_USER_WAIT_DISTRIBUTE		12								//等待分配
#define SUB_GR_USER_LEAVE_ROOM			13								//离开房间
#define SUB_GR_USER_DISTRIBUTION		14								//分配请求
#define SUB_GR_ANDROIDEXIT				71								//机器人下线
#define SUB_GR_ANDROIDMATCHUSER			72								//真人匹配
#define SUB_GR_ANDROIDMATCHUSERFAIL		73								//真人匹配失败
#define SUB_GR_UPDATEVOICEID			75								//更新语音ID

//用户状态
#define SUB_GR_USER_ENTER				100								//用户进入
#define SUB_GR_USER_SCORE				101								//用户分数
#define SUB_GR_USER_STATUS				102								//用户状态
#define SUB_GR_REQUEST_FAILURE			103								//请求失败
#define SUB_GR_REQUEST_SUCCESS			104								//请求成功
#define SUB_GR_MASTERLOGON_FAILURE		105								//请求成功
#define SUB_GR_IPHONE_SUCCESS			106                             //手机检测成功

#define SUB_GR_USER_CONTROL_XMS		    115                             //发送信息 弹出控制框
#define SUB_GR_USER_CONTROL_XMS_ONE		116                             //发送被控人数，服务器—》客户端


#define SUB_GR_USER_CONTROL_XMS_PM		117                             //发送被控pc和mob，客户端 —》服务器
#define SUB_GR_USER_CONTROL_XMS_CON		118                             //发送被控人，客户端 —》服务器
#define SUB_GR_USER_CONTROL_XMS_ROM		119                             //发送删除的控制者，客户端 —》服务器

#define SUB_GR_USER_CONTROL_XMS_UPONE	120								//发送被控人数，服务器—》客户端
#define SUB_GR_USER_CONTROL_XMS_UPTAB	121								//发送被控人数，服务器—》客户端
#define SUB_GR_USER_CONTROL_XMS_FINSH	122								//发送被控人数，服务器—》客户端
#define SUB_GR_USER_CONTROL_XMS_DELONE	123								//发送被控人数，服务器—》客户端

//聊天命令
#define SUB_GR_USER_CHAT				211								//聊天消息
#define SUB_GR_USER_EXPRESSION			212								//表情消息
#define SUB_GR_WISPER_CHAT				213								//私聊消息
#define SUB_GR_WISPER_EXPRESSION		214								//私聊表情
#define SUB_GR_COLLOQUY_CHAT			215								//会话消息
#define SUB_GR_COLLOQUY_EXPRESSION		216								//会话表情


//////////////////////////////////////////////////////////////////////////////////

//房间控制信息
struct  CMD_PlazaConPack
{
	WORD			wServerID;						//房间I D

	BYTE			byMobWinOrLost;					//手机端1输2赢,其他未用
	BYTE			byMobdds;						//手机端输赢几率
	BYTE			byPcWinOrLost;					//电脑端1输2赢,其他未用
	BYTE			byPcdds;						//电脑端输赢几率
	FLOAT			fServerDegree;					//房间难度

	LONGLONG        llServerStort;					//整体库存(所有库存之和，玩家根据身上财富分级，每级玩家使用对应库存)
	LONGLONG        llServerStortLv1;				//一级库存
	LONGLONG        llServerStortLv2;				//二级库存
	LONGLONG        llServerStortLv3;				//三级库存
	LONGLONG        llServerStortLv4;				//四级库存
	LONGLONG        llServerStortLv5;				//五级库存

	LONGLONG        llLeast;						//最低库存
};

//分级库存信息
struct  CMD_ServerStort
{
	INT				iServerID;
	LONGLONG        llServerStort;
	LONGLONG        llServerStortLv1;
	LONGLONG        llServerStortLv2;
	LONGLONG        llServerStortLv3;
	LONGLONG        llServerStortLv4;
	LONGLONG        llServerStortLv5;
	LONGLONG        llLeast;
	FLOAT			fServerDegree;
};

//用户控制信息
struct  CMD_UserConPack
{
	DWORD							dwGameID;							//玩家GameID
	DWORD							dwUserID;							//玩家UserID
	int								nWinType;							//1输,2赢,其他无
	int								nRate;								//几率(100为底)
	int								nGameCount;							//局数(等于0表示按照几率控制；大于0则100%控制，最大10局)
	LONGLONG						llMaxScore;							//控制金币
	LONGLONG						llCashScore;						//兑现金币(初始为0，时常更新)
	LONGLONG						llNodeScore;						//节点金币(初始为0，暂时只对捕鱼游戏有效)
	LONGLONG						llStartTime;						//起始时间(年4位,月日时分秒各2位)，如： 201801220945
	LONGLONG						llEndTime;							//结束时间(年4位,月日时分秒各2位)，如： 201801221159
	LONGLONG						llPrizeScore;						//中奖分数,0无限制,非0限制
};

//旁观请求
struct CMD_GR_UserLookon
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
};

//旁观请求
struct CMD_GR_UserIphone
{
	DWORD							dwUserID;
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
};

//坐下请求
struct CMD_GR_UserSitDown
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
	byte                            cbIphoneSit;                        //手机端坐下                            
	TCHAR							szPassword[LEN_PASSWORD];			//桌子密码
};

//真人匹配
struct CMD_GR_UserMatch
{
	DWORD							dwUserID;							//玩家标识
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
};

//分配用户
struct CMD_GR_UserDistribution
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
};

//离开房间
struct CMD_GR_UserLeaveRoom
{

};

//起立请求
struct CMD_GR_UserStandUp
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
	BYTE							cbForceLeave;						//强行离开
};

//邀请用户 
struct CMD_GR_UserInvite
{
	WORD							wTableID;							//桌子号码
	DWORD							dwUserID;							//用户 I D
};

//邀请用户请求 
struct CMD_GR_UserInviteReq
{
	WORD							wTableID;							//桌子号码
	DWORD							dwUserID;							//用户 I D
};

//用户分数
struct CMD_GR_UserScore
{
	DWORD							dwUserID;							//用户标识
	BYTE                            cbReason;
	tagUserScore					UserScore;							//积分信息
};

//用户分数
struct CMD_GR_MobileUserScore
{
	DWORD							dwUserID;							//用户标识
	tagMobileUserScore				UserScore;							//积分信息
};

//用户状态
struct CMD_GR_UserStatus
{
	DWORD							dwUserID;							//用户标识
	tagUserStatus					UserStatus;							//用户状态

	LONGLONG						lScore;								//身上金币
	LONGLONG						lInsure;							//所有金币
};

//请求失败
struct CMD_GR_RequestFailure
{
	LONG							lErrorCode;							//错误代码
	TCHAR							szDescribeString[256];				//描述信息
};


//用户聊天
struct CMD_GR_C_UserChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
	DWORD                           dwChannel;							//发送渠道
};

//用户聊天
struct CMD_GR_S_UserChat
{
	DWORD							dwSendUserID;						//发送用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//用户表情
struct CMD_GR_C_UserExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwTargetUserID;						//目标用户
};

//用户表情
struct CMD_GR_S_UserExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szSendUserNickName[32];				//发送用户昵称
	TCHAR							szSendUserUnderWrite[32];			//发送用户签名
	TCHAR							szTargetUserNickName[32];			//目标用户昵称
	TCHAR							szTargetUserUnderWrite[32];			//目标用户签名
};

//用户语音
struct CMD_GR_C_UserVoice
{
	WORD							wItemIndex;							//语音索引
	DWORD							dwTargetUserID;						//目标用户
};

//用户语音
struct CMD_GR_S_UserVoice
{
	WORD							wItemIndex;							//语音索引
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szSendUserNickName[32];				//发送用户昵称
	TCHAR							szSendUserUnderWrite[32];			//发送用户签名
	TCHAR							szTargetUserNickName[32];			//目标用户昵称
	TCHAR							szTargetUserUnderWrite[32];			//目标用户签名
};

//用户私聊
struct CMD_GR_C_WisperChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//用户私聊
struct CMD_GR_S_WisperChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	bool							bCardUser;							//是否购卡用户
	TCHAR							szSendUserNickName[32];				//发送用户昵称
	TCHAR							szSendUserUnderWrite[32];			//发送用户签名
	TCHAR							szTargetUserNickName[32];			//目标用户昵称
	TCHAR							szTargetUserUnderWrite[32];			//目标用户签名							
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//私聊表情
struct CMD_GR_C_WisperExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwTargetUserID;						//目标用户
};

//私聊表情
struct CMD_GR_S_WisperExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	bool							bCardUser;							//是否购卡用户
	TCHAR							szSendUserNickName[32];				//发送用户昵称
	TCHAR							szSendUserUnderWrite[32];			//发送用户签名
	TCHAR							szTargetUserNickName[32];			//目标用户昵称
	TCHAR							szTargetUserUnderWrite[32];			//目标用户签名
};

//用户会话
struct CMD_GR_ColloquyChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwConversationID;					//会话标识
	DWORD							dwTargetUserID[16];					//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//邀请用户
struct CMD_GR_C_InviteUser
{
	WORD							wTableID;							//桌子号码
	DWORD							dwSendUserID;						//发送用户
};

//邀请用户
struct CMD_GR_S_InviteUser
{
	DWORD							dwTargetUserID;						//目标用户
};

//用户拒绝黑名单坐下
struct CMD_GR_UserRepulseSit
{
	WORD							wTableID;							//桌子号码
	WORD							wChairID;							//椅子位置
	DWORD							dwUserID;							//用户 I D
	DWORD							dwRepulseUserID;					//用户 I D
};

//////////////////////////////////////////////////////////////////////////////////

//规则标志
#define UR_LIMIT_SAME_IP			0x01								//限制地址
#define UR_LIMIT_WIN_RATE			0x02								//限制胜率
#define UR_LIMIT_FLEE_RATE			0x04								//限制逃率
#define UR_LIMIT_GAME_SCORE			0x08								//限制积分

//用户规则
struct CMD_GR_UserRule
{
	BYTE							cbRuleMask;							//规则掩码
	WORD							wMinWinRate;						//最低胜率
	WORD							wMaxFleeRate;						//最高逃率
	LONG							lMaxGameScore;						//最高分数 
	LONG							lMinGameScore;						//最低分数
	TCHAR							szPassword[LEN_PASSWORD];			//桌子密码
};

//请求用户信息
struct CMD_GR_UserInfoReq
{
	DWORD                           dwUserIDReq;                        //请求用户
	WORD							wTablePos;							//桌子位置
};

//请求用户信息
struct CMD_GR_ChairUserInfoReq
{
	WORD							wTableID;							//桌子号码
	WORD							wChairID;							//椅子位置
};

//////////////////////////////////////////////////////////////////////////////////
//状态命令

#define MDM_GR_STATUS				4									//状态信息

#define SUB_GR_TABLE_INFO			100									//桌子信息
#define SUB_GR_TABLE_STATUS			101									//桌子状态

//////////////////////////////////////////////////////////////////////////////////

//桌子信息
struct CMD_GR_TableInfo
{
	WORD							wTableCount;						//桌子数目
	tagTableStatus					TableStatusArray[MAX_TABLE];		//桌子状态
};

//桌子状态
struct CMD_GR_TableStatus
{
	WORD							wTableID;							//桌子号码
	tagTableStatus					TableStatus;						//桌子状态
};

//////////////////////////////////////////////////////////////////////////////////
//银行命令

#define MDM_GR_INSURE					5								//用户信息

//银行命令
#define SUB_GR_QUERY_INSURE_INFO		1								//查询银行
#define SUB_GR_SAVE_SCORE_REQUEST		2								//存款操作
#define SUB_GR_TAKE_SCORE_REQUEST		3								//取款操作


#define SUB_GR_GETUSER_SCORE_SERVER     11                              //返点

#define SUB_GR_TRANSFER_ROOMCARD		12								//房卡赠送
#define SUB_GR_ROOMCARDTOSOCRE			13								//房卡转游戏币

#define SUB_GR_USER_INSURE_INFO			100								//银行资料
#define SUB_GR_USER_INSURE_SUCCESS		101								//银行成功
#define SUB_GR_USER_INSURE_FAILURE		102								//银行失败
#define SUB_GR_USER_PASSWORD_RESULT		104								//查询密码
#define SUB_GR_USER_CHANGE_RESULT		105								//修改结果
#define SUB_GR_QUERY_RETURN_SCORE_SERVER 107                            //返回发送

//////////////////////////////////////////////////////////////////////////////////

//查询银行
struct CMD_GR_C_QueryInsureInfoRequest
{
	DWORD							dwUserID;							//玩家标记
	BYTE                            cbActivityGame;                     //游戏动作
	TCHAR							szInsurePass[LEN_PASSWORD];			//银行密码
};

//存款请求
struct CMD_GR_C_SaveScoreRequest
{
	DWORD							dwUserID;							//玩家标记
	BYTE                            cbActivityGame;                     //游戏动作
	SCORE							lSaveScore;							//存款数目
};

//取款请求
struct CMD_GR_C_TakeScoreRequest
{
	DWORD							dwUserID;							//玩家标记
	BYTE                            cbActivityGame;                     //游戏动作
	SCORE							lTakeScore;							//取款数目
	TCHAR							szInsurePass[LEN_PASSWORD];			//银行密码
};

//查询密码
struct CMD_GR_C_CheckInsurePassword
{
	TCHAR							szInsurePass[LEN_PASSWORD];			//银行密码
};

//银行资料
struct CMD_GR_S_UserInsureInfo
{
	BYTE                            cbActivityGame;                     //游戏动作
	WORD							wRevenueTake;						//税收比例
	WORD							wRevenueTransfer;					//税收比例
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	SCORE							lTransferPrerequisite;				//转账条件
};

//查询密码
struct CMD_GR_S_CheckUserPassword
{
	LONG							lResultCode;						//查询结果
};

//修改密码
struct CMD_GR_S_ChangeInsurePassword
{
	LONG							lResultCode;						//查询结果
	TCHAR							szDescribeString[128];				//描述消息
};

//银行成功
struct CMD_GR_S_UserInsureSuccess
{
	BYTE                            cbActivityGame;                     //游戏动作
	SCORE							lUserScore;							//身上金币
	SCORE							lUserInsure;						//银行金币
	TCHAR							szDescribeString[128];				//描述消息
	TCHAR							szDateTime[64];						//操作时间
};

//银行失败
struct CMD_GR_S_UserInsureFailure
{
	BYTE                            cbActivityGame;                     //游戏动作
	LONG							lErrorCode;							//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//////////////////////////////////////////////////////////////////////////////////
//管理命令

#define MDM_GR_MANAGE				6									//管理命令

#define SUB_GR_SEND_WARNING			1									//发送警告
#define SUB_GR_SEND_MESSAGE			2									//发送消息
#define SUB_GR_LOOK_USER_IP			3									//查看地址
#define SUB_GR_KILL_USER			4									//踢出用户
#define SUB_GR_LIMIT_ACCOUNS		5									//禁用帐户
#define SUB_GR_SET_USER_RIGHT		6									//权限设置

//房间设置
#define SUB_GR_QUERY_OPTION			7									//查询设置
#define SUB_GR_OPTION_SERVER		8									//房间设置
#define SUB_GR_OPTION_CURRENT		9									//当前设置

#define SUB_GR_LIMIT_USER_CHAT		10									//限制聊天

#define SUB_GR_KICK_ALL_USER		11									//踢出用户
#define SUB_GR_DISMISSGAME		    12									//解散游戏
#define SUB_GR_ADDANDIRO		    13									//解散游戏
#define SUB_GR_DELANDIRO		    14									//解散游戏
//////////////////////////////////////////////////////////////////////////////////

//系统消息
struct CMD_GR_SendMessage
{
	BYTE							cbAllGame;							//所有游戏
	BYTE							cbFishGame;							//捕鱼游戏
	BYTE                            cbLoop;                             //循环标志
	DWORD                           dwTimeRate;                         //循环间隔
	__time64_t                      tConcludeTime;                      //结束时间
	WORD							wChatLength;						//信息长度
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//系统消息
};

//踢出用户
struct CMD_GR_KickUser
{
	DWORD							dwTargetUserID;						//目标用户
};

//禁用帐户
struct CMD_GR_LimitAccounts
{
	DWORD							dwTargetUserID;						//目标用户
};

//权限设置
struct CMD_GR_SetUserRight
{
	//目标用户
	DWORD							dwTargetUserID;						//目标用户

	//绑定变量
	BYTE							cbGameRight;						//帐号权限
	BYTE							cbAccountsRight;					//帐号权限

	//权限变化
	BYTE							cbLimitRoomChat;					//大厅聊天
	BYTE							cbLimitGameChat;					//游戏聊天
	BYTE							cbLimitPlayGame;					//游戏权限
	BYTE							cbLimitSendWisper;					//发送消息
	BYTE							cbLimitLookonGame;					//旁观权限
};

//房间设置
struct CMD_GR_OptionCurrent
{
	DWORD							dwRuleMask;							//规则掩码
	tagServerOptionInfo				ServerOptionInfo;					//房间配置
};

//房间设置
struct CMD_GR_ServerOption
{
	tagServerOptionInfo				ServerOptionInfo;					//房间配置
};

//踢出所有用户
struct CMD_GR_KickAllUser
{
	TCHAR							szKickMessage[LEN_USER_CHAT];		//踢出提示
};

//解散游戏
struct CMD_GR_DismissGame
{
	WORD							wDismissTableNum;		            //解散桌号
};
//////////////////////////////////////////////////////////////////////////////////

//设置标志
#define OSF_ROOM_CHAT				1									//大厅聊天
#define OSF_GAME_CHAT				2									//游戏聊天
#define OSF_ROOM_WISPER				3									//大厅私聊
#define OSF_ENTER_TABLE				4									//进入游戏
#define OSF_ENTER_SERVER			5									//进入房间
#define OSF_SEND_BUGLE				12									//发送喇叭

//房间设置
struct CMD_GR_OptionServer
{
	BYTE							cbOptionFlags;						//设置标志
	BYTE							cbOptionValue;						//设置标志
};

//限制聊天
struct CMD_GR_LimitUserChat
{
	DWORD							dwTargetUserID;						//目标用户
	BYTE							cbLimitFlags;						//限制标志
	BYTE							cbLimitValue;						//限制与否
};
//////////////////////////////////////////////////////////////////////////////////
//比赛命令

#define MDM_GR_MATCH				7									//比赛命令

#define SUB_GR_MATCH_FEE			400									//报名费用
#define SUB_GR_MATCH_NUM			401									//等待人数
#define SUB_GR_LEAVE_MATCH			402									//退出比赛
#define SUB_GR_MATCH_INFO			403									//比赛信息
#define SUB_GR_MATCH_WAIT_TIP		404									//等待提示
#define SUB_GR_MATCH_RESULT			405									//比赛结果
#define SUB_GR_MATCH_STATUS			406									//比赛状态
#define SUB_GR_MATCH_DESC			408									//比赛描述


#define MDM_GR_SERVER_ADMIN         8
#define SUB_GR_GAME_CONTROL_NineXiao  500                               //控制命令
#define SUB_GR_GAME_CONTROL_RedNineBattle  501                          //控制命令
#define SUB_GR_GAME_CONTROL_TransferBattle  502                         //控制命令
#define SUB_GR_GAME_CONTROL_BaccaratNew  503                            //控制命令
#define SUB_GR_GAME_CONTROL_OxBattle  504                               //控制命令

////////////////改动以下时 请将游戏里面CMD_GAME.H的同时改动////////////////////////////
#define SUB_GR_MATCH_INFO_ER_SPARROWS	410								//比赛信息(2人麻将)
//赛事信息（2人麻将特有）
struct CMD_GR_Match_ER_Sparrows
{
	WORD							wALLGameRound;						//总共游戏轮
	WORD							wALLGameRoundFan;					//总共游戏翻
	WORD							wCurGameRound;						//当前游戏轮
	WORD							wCurGameRoundFan;					//当前游戏番
	WORD							wLocalGameFan;						//本地游戏番
	WORD							wOtherGameFan;						//对家游戏番
	WORD							wNextGameRoundFan;					//下轮游戏番
};

//比赛人数
struct CMD_GR_Match_Num
{
	DWORD							dwWaitting;							//等待人数
	DWORD							dwTotal;							//开赛人数
};

//赛事信息
struct CMD_GR_Match_Info
{
	TCHAR							szTitle[4][64];						//信息标题
	WORD							wGameCount;							//游戏局数
};

//提示信息
struct CMD_GR_Match_Wait_Tip
{
	SCORE							lScore;								//当前积分
	WORD							wRank;								//当前名次
	WORD							wCurTableRank;						//本桌名次
	WORD							wUserCount;							//当前人数
	WORD							wPlayingTable;						//游戏桌数
	TCHAR							szMatchName[LEN_SERVER];			//比赛名称
};

//比赛结果
struct CMD_GR_MatchResult
{
	TCHAR							szDescribe[256];					//得奖描述
	DWORD							dwGold;								//金币奖励
	DWORD							dwMedal;							//奖牌奖励
	DWORD							dwExperience;						//经验奖励
};

#define MAX_MATCH_DESC				4									//最多描述
//比赛描述
struct CMD_GR_MatchDesc
{
	TCHAR							szTitle[MAX_MATCH_DESC][16];		//信息标题
	TCHAR							szDescribe[MAX_MATCH_DESC][64];		//描述内容
	COLORREF						crTitleColor;						//标题颜色
	COLORREF						crDescribeColor;					//描述颜色
};

//////////////////////////////////////////////////////////////////////////////////
//框架命令

#define MDM_GF_FRAME				100									//框架命令

//////////////////////////////////////////////////////////////////////////////////
//框架命令

//用户命令
#define SUB_GF_GAME_OPTION			1									//游戏配置
#define SUB_GF_USER_READY			2									//用户准备
#define SUB_GF_LOOKON_CONFIG		3									//旁观配置

//聊天命令
#define SUB_GF_USER_CHAT			10									//用户聊天
#define SUB_GF_USER_EXPRESSION		11									//用户表情

//游戏信息
#define SUB_GF_GAME_STATUS			100									//游戏状态
#define SUB_GF_GAME_SCENE			101									//游戏场景
#define SUB_GF_LOOKON_STATUS		102									//旁观状态

//系统消息
#define SUB_GF_SYSTEM_MESSAGE		200									//系统消息
#define SUB_GF_ACTION_MESSAGE		201									//动作消息

//游戏配置
struct CMD_GF_GameOption
{
	BYTE							cbAllowLookon;						//旁观标志
	DWORD							dwFrameVersion;						//框架版本
	DWORD							dwClientVersion;					//游戏版本
};

//旁观配置
struct CMD_GF_LookonConfig
{
	DWORD							dwUserID;							//用户标识
	BYTE							cbAllowLookon;						//允许旁观
};

//旁观状态
struct CMD_GF_LookonStatus
{
	BYTE							cbAllowLookon;						//允许旁观
};

//游戏环境
struct CMD_GF_GameStatus
{
	BYTE							cbGameStatus;						//游戏状态
	BYTE							cbAllowLookon;						//旁观标志
};

//用户聊天
struct CMD_GF_C_UserChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//用户聊天
struct CMD_GF_S_UserChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwChatColor;						//信息颜色
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szSendUserNickName[32];				//发送用户昵称
	TCHAR							szSendUserUnderWrite[32];			//发送用户签名
	TCHAR							szTargetUserNickName[32];			//目标用户昵称
	TCHAR							szTargetUserUnderWrite[32];			//目标用户签名
	TCHAR							szChatString[LEN_USER_CHAT];		//聊天信息
};

//用户表情
struct CMD_GF_C_UserExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwTargetUserID;						//目标用户
};

//用户表情
struct CMD_GF_S_UserExpression
{
	WORD							wItemIndex;							//表情索引
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szSendUserNickName[32];				//发送用户昵称
	TCHAR							szSendUserUnderWrite[32];			//发送用户签名
	TCHAR							szTargetUserNickName[32];			//目标用户昵称
	TCHAR							szTargetUserUnderWrite[32];			//目标用户签名
};


//查询银行(框架5678小游戏)
struct CMD_GF_QueryUserInsureScore5678Game
{
	SCORE							lScore;								//用户金币
	SCORE							lGameScore;							//游戏金币
};


//银行成功(框架5678小游戏)
struct CMD_GP_C_UserInsureSuccess5678Game
{
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameKindID;						//游戏ID
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	TCHAR							szDescribeString[128];				//描述消息
};

//银行失败(框架5678小游戏)
struct CMD_GR_S_UserInsureFailure5678Game
{
	//BYTE                            cbActivityGame;                     //游戏动作
	LONG							lErrorCode;							//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};
//////////////////////////////////////////////////////////////////////////////////
//游戏命令

#define MDM_GF_GAME					200									//游戏命令
#define SUB_GR_USER_CHAT			211									//用户聊天 
#define SUB_GR_GAME_EXPRESSION		212									//表情消息
#define SUB_GR_EXPRESSIONREPLY      213									//表情消息回复
#define SUB_GR_GAME_VOICE			214									//语音消息
#define SUB_GR_VOICEREPLY			215									//语音消息回复

//////////////////////////////////////////////////////////////////////////////////
//携带信息

//其他信息
#define DTP_GR_TABLE_PASSWORD		1									//桌子密码

//用户属性
#define DTP_GR_NICK_NAME			10									//用户昵称
#define DTP_GR_GROUP_NAME			11									//社团名字
#define DTP_GR_UNDER_WRITE			12									//个性签名
#define DTP_GR_USER_QQ              13
#define DTP_GR_FACEIMAGE			14									//微信头像

//附加信息
#define DTP_GR_USER_NOTE			20									//用户备注
#define DTP_GR_CUSTOM_FACE			21									//自定头像

//////////////////////////////////////////////////////////////////////////////////

//请求错误
#define REQUEST_FAILURE_NORMAL		0									//常规原因
#define REQUEST_FAILURE_NOGOLD		1									//金币不足
#define REQUEST_FAILURE_NOSCORE		2									//积分不足
#define REQUEST_FAILURE_PASSWORD	3									//密码错误
#define REQUEST_FAILURE_SETCHAIR	4									//坐下座位失败


//////////////////////////////////////////////////////////////////////////////////

#define MDM_GF_GAME_TABLE			300									//桌子游戏命令

#define SUB_GF_GAME_TABLE_START		1									//桌子开始
#define SUB_GF_GAME_TABLE_JETTON	2									//桌子下注
#define SUB_GF_GAME_TABLE_END		3									//桌子结束

struct  CMD_GF_TABLE_START
{
	WORD							wTableID;
	BYTE							byHistoryScore[7][4];
};

struct  CMD_GF_TABLE_END
{
	WORD							wTableID;
	LONGLONG						llUserScore[16];
};

//用户抽宝箱 index = 0 gold是金币 index =1 gold是房卡 index = 2 gold 数值代表电话卡
struct CMD_GF_USERLUCKDRAW
{
	SCORE							OddsGold;
	BYTE							isFirst;
	BYTE							ChestCnts;
	WORD							WinChairID;
};

//////////////////////////////////////////////////////////////////////////
#define MDM_GR_MATCH_GAME					9000						//比赛命令

#define SUB_GR_ENTER_MATCH_REQ				1							//进入请求
#define SUB_GR_MATCH_ACITVE					2							//激活用户
#define SUB_GR_SENDUSERFINISH				3							//客户端接收完成

#define SUB_GR_USER_ENTER_MATCH				103			 				//用户进入比赛 ,结果同tagMatchUserData
#define SUB_GR_CELL_SCORE_UPDATE			104							//分数变更
#define SUB_GR_TABLE_USER_IDS				105							//发送桌子信息
#define SUB_GR_USER_MATCH_STATUS			106							//比赛状态
#define SUB_GR_MATCH_UPDATESTATUS			107							//更新比赛状态
#define SUB_GR_MATCH_AWAIT					108							//等待备战

//请求进入
struct CMD_GR_EnterMatchRequest
{
	DWORD								dwUserID;						//用户 I D
	int 								cbMatchID;						//比赛ID
};

//比赛用户信息
struct CMD_GR_UserCome
{
	tagMatchUserData					MatchData;
};

//比赛结构详细信息
struct CMD_GR_MatchInfo
{
	tagMatchInfo						MatchInfo;
};

//比赛用户分数
struct CMD_GR_MatchUserScore
{
	int									cbMatchID;
	DWORD								dwUserID;						//用户 I D
	int									wTableID;
	tagUserScore						UserScore;						//积分信息
};

//用户桌子&椅子
struct CMD_GR_TableUserID
{
	WORD								wTableID;
	WORD								wChairID;
	DWORD								dwUserID;						//用户 I D
};

//更新用户比赛信息
struct CMD_GR_UserMatchStatus
{
	BYTE								cbIsOut;						//比赛出局
	BYTE								cbLevel;		                //那个阶段
	BYTE								defaultMark;                    //那一场比赛
	BYTE								cbGoodsType;                    //实物的类型
	bool								bGoodsreward;                   //实物奖励
	WORD								cbRank;                         //第几名
	DWORD								dwMatchUserCounts;              //总共多少人比赛  
	DWORD								dwWinScore;                     //奖励你多少钱
	DWORD								dwWinlucky;                     //幸运奖金
};

struct CMD_GR_MatchStatus
{
	DWORD								dwUserID;						//数据库ID
	DWORD								dwMatchID;
	BYTE								cbUserStatus;					//用户状态
	BYTE								UserLadder;                     //用户到了第几层  
};

//等待提示
struct CMD_GR_MatchAwait
{
	BYTE								cbLaveTable;
	BYTE								cbUserLadder;
	DWORD								cbUserID;
};

struct WinUserInfo
{
	DWORD								szTimer;
	SCORE								lWinScore;
	BYTE								cbType;
	TCHAR								szUserName[32];
};

//游戏牌型记录
struct GameTypeInfo
{
	DWORD								dwType[20];						//各牌型个数
};

#pragma pack()

#endif