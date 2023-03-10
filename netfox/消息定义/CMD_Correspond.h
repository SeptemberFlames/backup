#ifndef CMD_CORRESPOND_HEAD_FILE
#define CMD_CORRESPOND_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//注册命令

#define MDM_CS_REGISTER							1						//服务注册

//服务注册
#define SUB_CS_C_REGISTER_PLAZA					100						//注册广场
#define SUB_CS_C_REGISTER_SERVER				101						//注册房间
#define SUB_CS_C_REGISTER_CONSERVER				102						//注册房间
#define SUB_CS_C_REGISTER_CHAT					103						//注册聊天	
#define SUB_CS_C_REGISTER_PERSONAL				104						//注册约战房间限制
#define SUB_CS_C_REGISTER_LOGON_CHECK			105						//注册审核
#define SUB_CS_C_REGISTER_PERSONAL_CENTER		106						//注册个人
#define SUB_CS_C_REGISTER_BANK					107						//注册银行
#define SUB_CS_C_REGISTER_BANK_5678				108						//注册银行
#define SUB_CS_C_REGISTER_MALL					109						//注册商城
#define SUB_CS_C_REGISTER_TASK					110						//注册任务
#define SUB_CS_C_REGISTER_CHARGE				111						//充值任务
#define SUB_CS_C_REGISTER_SYNC					112						//同步服务
#define SUB_CS_C_REGISTER_HORN					113                     //喇叭服务
#define SUB_CS_C_REGISTER_AGENT_LIEBIAN			114                     //代理裂变服务
#define SUB_CS_C_REGISTER_CHARGE_LIEBIAN		115                     //充值裂变服务
#define SUB_CS_C_REGISTER_STATISTICS			116                     //统计服务

//注册结果
#define SUB_CS_S_REGISTER_FAILURE				200						//注册失败

//////////////////////////////////////////////////////////////////////////////////

//注册广场
struct CMD_CS_C_RegisterPlaza
{
	TCHAR							szServerAddr[32];					//服务地址
	TCHAR							szServerName[LEN_SERVER];			//服务器名
};

//注册游戏
struct CMD_CS_C_RegisterServer
{
	WORD							wKindID;							//名称索引
	WORD							wNodeID;							//节点索引
	WORD							wSortID;							//排序索引
	WORD							wServerID;							//房间索引
	WORD							wServerPort;						//房间端口
	WORD							wServerType;						//房间类型
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwFullCount;						//满员人数
	TCHAR							szServerAddr[32];					//服务地址
	TCHAR							szServerName[LEN_SERVER];			//房间名称
	TCHAR							szServerNameShow[LEN_SERVER];		//显示名称
	LONGLONG						lMaxEnterScore;						//最大进入分数
	LONGLONG						lMinEnterScore;						//最小进入分数
	BOOL							bReconectCorrespond;				//重连标志
};

//注册失败
struct CMD_CS_S_RegisterFailure
{
	LONG							lErrorCode;							//错误代码
	TCHAR							szDescribeString[128];				//错误消息
};

//////////////////////////////////////////////////////////////////////////////////
//服务信息

#define MDM_CS_SERVICE_INFO			2									//服务信息

//房间信息
#define SUB_CS_C_SERVER_ONLINE		1									//房间人数
#define SUB_CS_C_SERVER_MODIFY		2									//房间修改

//广场命令
#define SUB_CS_S_PLAZA_INFO			100									//广场信息
#define SUB_CS_S_PLAZA_INSERT		101									//广场列表
#define SUB_CS_S_PLAZA_REMOVE		103									//广场删除
#define SUB_CS_S_PLAZA_FINISH		104									//广场完成

//房间命令
#define SUB_CS_S_SERVER_INFO		110									//房间信息
#define SUB_CS_S_SERVER_ONLINE		111									//房间人数
#define SUB_CS_S_SERVER_INSERT		112									//房间列表
#define SUB_CS_S_SERVER_MODIFY		113									//房间修改
#define SUB_CS_S_SERVER_REMOVE		114									//房间删除
#define SUB_CS_S_SERVER_FINISH		115									//房间完成

//同步命令
#define SUB_CS_S_REGISTER_SYNC		116									//注册同步

//////////////////////////////////////////////////////////////////////////////////

//房间数据
struct CMD_CS_C_ServerModify
{
	WORD							wSortID;							//排序索引
	WORD							wKindID;							//类型索引
	WORD							wNodeID;							//节点索引
	WORD							wServerPort;						//房间端口
	WORD							wServerType;						//房间类型
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwFullCount;						//满员人数
	TCHAR							szServerAddr[32];					//服务地址
	TCHAR							szServerName[LEN_SERVER];			//房间名称																
	SCORE							lMinEnterScore;						//最低积分
	SCORE							lMaxEnterScore;						//最高积分
};

//房间人数
struct CMD_CS_C_ServerOnLine
{
	DWORD							dwOnLineCount;						//在线人数
};

//////////////////////////////////////////////////////////////////////////////////

//广场删除
struct CMD_CS_S_PlazaRemove
{
	DWORD							dwPlazaID;							//广场标识
};

//房间人数
struct CMD_CS_S_ServerOnLine
{
	WORD							wServerID;							//房间标识
	DWORD							dwOnLineCount;						//在线人数
};

//房间修改
struct CMD_CS_S_ServerModify
{
	WORD							wSortID;							//排序索引
	WORD							wKindID;							//类型索引
	WORD							wNodeID;							//节点索引
	WORD							wServerID;							//房间标识
	WORD							wServerPort;						//房间端口
	WORD							wServerType;						//房间类型
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwFullCount;						//满员人数
	TCHAR							szServerAddr[32];					//服务地址
	TCHAR							szServerName[LEN_SERVER];			//房间名称
	LONGLONG						lMaxEnterScore;						//最大进入分数
	LONGLONG						lMinEnterScore;						//最小进入分数
};

//房间删除
struct CMD_CS_S_ServerRemove
{
	WORD							wServerID;							//房间标识
};

//////////////////////////////////////////////////////////////////////////////////
//用户汇总

#define MDM_CS_USER_COLLECT						3						//用户汇总

//用户状态
#define SUB_CS_C_USER_ENTER						1						//用户进入
#define SUB_CS_C_USER_LEAVE						2						//用户离开
#define SUB_CS_C_USER_FINISH					3						//用户完成
#define SUB_CS_C_PLAY_USER_ENTER				4						//用户进入
#define SUB_CS_C_PLAY_USER_LEAVE				5						//用户离开
#define SUB_CS_C_PLAY_USER_TANCON				6						//用户控制更新，WinType非1非2就删除
#define SUB_CS_C_PLAY_USER_WRITECON				7						//玩家写分修改控制
#define SUB_CS_C_PLAY_USER_UPDATASCORE			8						//用户金币
#define SUB_CS_C_PLAY_USER_IN_INSURE			9						//用户打开银行
#define SUB_CS_C_PLAY_USER_OUT_INSURE			10						//用户关闭银行
#define SUB_CS_C_USER_MANAGE_SETTING			11						//配置送收分消息(//捕鱼控制专用，客户端发包)
#define SUB_CS_C_SYSTEM_WRITECON				12						//系统设置修改控制
#define SUB_CS_C_CONTROL_TO_GAME				13						//控制服务器到游戏消息(消息定义EMConAndGame_MsgIndex)
#define SUB_CS_C_PLAY_USER_DELETANCON			14						//服务器删除用户控制

//控制功能
#define SUB_CS_C_CONTROL_FORBID_CHAT			15						//控制禁止聊天
#define SUB_CS_C_CONTROL_ALLOW_CHAT				16						//控制允许聊天
#define SUB_CS_C_CONTROL_KICK_USER				17						//控制踢出玩家

//用户状态
#define SUB_CS_S_COLLECT_REQUEST				100						//汇总请求
#define SUB_CS_S_PLAY_USER_ENTER				101						//用户进入
#define SUB_CS_S_PLAY_USER_LEAVE				102						//用户离开
#define SUB_CS_S_PLAY_USER_TANCON				103						//用户控制消息
#define SUB_CS_S_PLAY_USER_WRITECON				104						//玩家写分修改控制
#define SUB_CS_S_PLAY_USER_UPDATASCORE			105						//更新用户财富
#define SUB_CS_S_PLAY_USER_ONLINE				106						//用户进入
#define SUB_CS_S_PLAY_USER_IN_INSURE			107						//用户离开
#define SUB_CS_S_PLAY_USER_OUT_INSURE			108						//用户离开
#define SUB_CS_S_PLAY_GAMEUSER_IN_INSURE		109						//进入银行
#define SUB_CS_S_PLAY_GAMEUSER_OUT_INSURE		110						//离开银行
#define SUB_CS_S_USER_MANAGE_SETTING			111						//配置送收分消息(捕鱼控制专用，客户端发包)
#define SUB_CS_S_SYSTEM_WRITECON				112						//系统设置修改控制
#define SUB_CS_S_PLAY_USER_DELECON				113						//服务器删除玩家控制
#define SUB_CS_C_BET_START						211						//下注开始
#define SUB_CS_C_BET_SUCCESS					212						//下注成功
#define SUB_CS_C_BET_STOP						213						//下注结束
#define SUB_CS_C_SET_SERVER_DRGREE				214						//设置房间难度
#define SUB_CS_C_SERVER_CON_RETURN				215						//房间控制返回消息
#define SUB_CS_C_FISHING3D_USER_SIT_DOWN		216						//捕鱼玩家坐下
#define SUB_CS_C_CONTROL_ROOM_HANDSEL_SYNC		217						//房间彩金同步
#define SUB_CS_C_ADD_CONTROL_ROOM_HANDSEL 218							//修改房间彩金
#define SUB_CS_C_SET_CONTROL_ROOM_HANDSEL_DRGREE 219					//设置房间彩金难度

//////////////////////////////////////////////////////////////////////////////////

//在线玩家
struct tagControlOnlineUser
{
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	LONGLONG						llCurScore;
	LONGLONG						llInsureScore;
	WORD                            wKindId;
	WORD                            wServerId;

	DWORD							dwChannel;							//登录渠道
};

//控制信息
struct tagControlUserInfo
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

struct CMD_CS_C_UserInsureTran
{
	DWORD   dwTranGameID;
	LONGLONG  llTranScore;
};

//用户进入
struct CMD_CS_C_UserEnter
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	//辅助信息
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级

};

//用户离开
struct CMD_CS_C_UserLeave
{
	DWORD							dwUserID;							//用户UserID
	DWORD							dwGameID;							//用户GameID
	WORD                            wServerId;
};

//用户进入
struct CMD_CS_C_PlayUserEnter
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	SCORE							llCurScore;
	SCORE							llInsureScore;

	WORD                            wKindId;							//类型ID
	WORD                            wServerId;							//房间ID

	DWORD							dwChannel;							//登录渠道
};

//玩家写分控制信息更新
struct CMD_CS_C_PlayUserWriteCon
{
	DWORD							dwGameID;							//玩家GameID
	WORD                            wServerID;							//房间标识
	int								nGameCount;							//控制局数
	LONGLONG                        llCashScore;						//兑现金币，增加
	LONGLONG						llNodeScore;						//节点金币
	TCHAR							szKey[256];							//控制key
	BYTE							byUpdataServer;						//分级库存
	bool							bUpdateaServer;						//是否更新
	bool							bBaiRenServer;						//是否百人
};

//服务器删除玩家控制信息
struct CMD_CS_C_PlayUserDeleCon
{
	DWORD							dwGameID;							//玩家GameID
};

//服务器禁止聊天
struct CMD_CS_C_ControlForidChat
{
	DWORD							dwUserID;							//用户标识
	WORD							wServerID;							//房间序号
};


//服务器允许聊天
struct CMD_CS_C_ControlAllowChat
{
	DWORD							dwUserID;							//用户标识
	WORD							wServerID;							//房间序号
};

//服务器踢出玩家
struct CMD_CS_C_ControlKickUser
{
	DWORD							dwUserID;							//用户标识
	WORD							wServerID;							//房间序号
};

//系统修改控制信息更新
struct CMD_CS_C_SystemWriteCon
{
	WORD                            wServerID;							//房间标识
	LONGLONG                        llCashScore;						//兑现金币，增加
	TCHAR							szKey[256];							//控制key
	BYTE							byUpdataServer;						//分级库存
	bool							bUpdateaServer;						//是否更新
};

//用户下注
struct CMD_CS_C_PlayUserBet
{
	DWORD							dwUserID;
	DWORD							dwGameID;
	TCHAR							szNickName[LEN_NICKNAME];
	WORD                            wKindID;
	TCHAR                           szKindName[LEN_KIND];
	WORD                            wServerID;
	TCHAR                           szServerName[LEN_SERVER];
	WORD                            wTableID;
	WORD                            wChairID;
	WORD                            wAreaID;//当前下注区域,//控赢区域
	BYTE                            cbTimeLeave;//剩余下注时间,//控赢局数
	LONGLONG                        llScore;
	WORD                            wAreaCount;
	LONGLONG                        llScoreUserAll[30];
	TCHAR                        	szAreaName[30][10];
};

//设置房间难度
struct CMD_CS_C_SetServerDegree
{
	WORD                            wServerID;
	FLOAT							fServerDegree;
};

struct CMD_CS_C_Fishing3DGameServerBase
{
	int nMsgIndex;
};

//捕鱼玩家坐下
struct CMD_CS_C_Fishing3DUserSitDown
{
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	WORD                            wKindID;
	WORD                            wServerID;
	WORD							wTableID;
	WORD							wChairID;
};

//控制服务器和游戏服务器之间消息定义
enum EMConAndGame_MsgIndex
{
	//11增减非捕鱼房间库存，12获取非捕鱼房间库存，13获取被控玩家总库存
	emConAndGame_AddNoFishingStock = 11,			//增减非捕鱼房间库存，
	emConAndGame_QueryNoFishingStock,				//获取非捕鱼房间库存，

	emConAndGame_UpdateControlServerStock,				//同步库存数据

	emConAndGame_ConToBairenMin = 200,				//200-299控制到百人游戏
	emConAndGame_BrQueryStatus,						//获取场景

	emConAndGame_BrQueryControl = 225,				//查询控制
	emConAndGame_BrExcuteControl,					//执行控制
	emConAndGame_BrCancelControl,					//删除控制

	emConAndGame_BrFqzs3DQueryShaYuInfo = 250,		//“飞禽走兽3D”获取鲨鱼信息
	emConAndGame_BrFqzs3DModifyShaYuLimit,			//“飞禽走兽3D”修改将出鲨鱼局数限制
	emConAndGame_BrJsysQueryShaYuInfo,				//“金鲨银鲨”获取鲨鱼信息
	emConAndGame_BrJsysModifyShaYuLimit,			//“金鲨银鲨”修改将出鲨鱼局数限制

	emConAndGame_ConToBairenMax = 299,				//200-299控制到百人游戏

	emConAndGame_BairenToConMin = 300,				//300-399百人游戏到控制
	emConAndGame_BrQueryStatusResult,				//获取场景返回
	emConAndGame_BrOnLineUserInfo,					//房间玩家信息
	emConAndGame_BrSwitchStatus,					//状态切换(空闲、下注、结算)
	emConAndGame_BrGameStart,						//游戏开始(庄家、真人信息)
	emConAndGame_BrUserEnter,						//玩家进入
	emConAndGame_BrUserLeave,						//玩家离开
	emConAndGame_BrUserBetSocore,					//玩家下注（比较全面）
	emConAndGame_BrUserBetSocoreSimple,				//玩家下注（简单，只有自己当前区域）
	emConAndGame_BrUserBetSocoreClear,				//玩家清除下注
	emConAndGame_BrUserBetSocoreAndroid,			//机器人下注（机器人下注，总的下注，1秒刷新一次）
	emConAndGame_BrGameEnd,							//结算信息
	emConAndGame_BrSysWinScore,						//当前系统输赢

	emConAndGame_BrUpdateControl = 325,				//更新控制信息

	emConAndGame_BrFqzs3DQueryShaYuInfoResult = 350,//“飞禽走兽3D”获取鲨鱼信息返回
	emConAndGame_BrFqzs3DModifyShaYuLimitResult,	//“飞禽走兽3D”修改将出鲨鱼局数限制返回
	emConAndGame_BrJsysQueryShaYuInfoResult,		//“金鲨银鲨”获取鲨鱼信息返回
	emConAndGame_BrJsysModifyShaYuLimitResult,	//“金鲨银鲨”修改将出鲨鱼局数限制返回

	emConAndGame_BairenToConMax = 399,				//300-399百人游戏到控制
	emConAndGame_EndIndex,
};

//11,控制服务器和游戏服务器之间消息定义EMConAndGame_MsgIndex
struct CMD_CS_C_QueryFishingStockLevel_ConToGame
{
	int					nMsgIndex;					//11获取鱼库存分级值
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
};

//11返回,控制服务器和游戏服务器之间消息定义EMConAndGame_MsgIndex
struct CMD_CS_C_QueryFishingStockLevel_GameToCon
{
	int					nMsgIndex;					//11获取鱼库存分级值，返回
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	LONGLONG			llStock;
};

//12,控制服务器和游戏服务器之间消息定义EMConAndGame_MsgIndex
struct CMD_CS_C_SetFishingStockLevel_ConToGame
{
	int					nMsgIndex;					//12设置鱼库存分级值
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wServerID;					//房间ID
	LONGLONG			llStock;					//库存分集
};

//12返回,控制服务器和游戏服务器之间消息定义EMConAndGame_MsgIndex
struct CMD_CS_C_SetFishingStockLevel_GameToCon
{
	int					nMsgIndex;					//12设置鱼库存分级值，返回
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	LONGLONG			llStock;
};

//13,控制服务器和游戏服务器之间消息定义EMConAndGame_MsgIndex
struct CMD_CS_C_QueryFishingStock_ConToGame
{
	int					nMsgIndex;					//13获取鱼库存
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wServerID;					//房间ServerID
};

//13返回,控制服务器和游戏服务器之间消息定义EMConAndGame_MsgIndex
struct CMD_CS_C_QueryFishingStock_GameToCon
{
	int					nMsgIndex;					//13获取鱼库存，返回
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wServerID;					//房间ServerID
	int					nCount;						//库存数
	LONGLONG			llStockData[5];				//具体库存值
};

//14,控制服务器和游戏服务器之间消息定义EMConAndGame_MsgIndex
struct CMD_CS_C_AddFishingStock_ConToGame
{
	int					nMsgIndex;					//14增减鱼库存
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wServerID;					//房间ServerID
	bool				bAddStock;					//true加，false减
	int					nStockIndex;				//库存索引
	LONGLONG			llStock;					//库存值
};

//14返回,控制服务器和游戏服务器之间消息定义EMConAndGame_MsgIndex
struct CMD_CS_C_AddFishingStock_GameToCon
{
	int					nMsgIndex;					//14增减鱼库存，返回
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	bool				bSuccess;					//是否成功
	WORD				wServerID;					//房间ServerID
	bool				bAddStock;					//true加，false减
	int					nStockIndex;				//库存索引
	LONGLONG			llStock;					//库存值
	LONGLONG			llLastStock;				//原来库存
	LONGLONG			llNowStock;					//当前库存
	int					nCount;						//库存数
	LONGLONG			llStockData[5];				//具体库存值
	FLOAT				fServerDegree;				//房间难度
};

//15,控制服务器和游戏服务器之间消息定义EMConAndGame_MsgIndex
struct CMD_CS_C_AddNoFishingStock_ConToGame
{
	int					nMsgIndex;					//15增减非捕鱼房间库存
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wServerID;					//房间ServerID
	bool				bAddStock;					//true加，false减
	int					nStockIndex;				//库存索引
	LONGLONG			llStock;					//库存值
};

//15返回,控制服务器和游戏服务器之间消息定义EMConAndGame_MsgIndex
struct CMD_CS_C_AddNoFishingStock_GameToCon
{
	int					nMsgIndex;					//15增减非捕鱼房间库存，返回
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	bool				bSuccess;					//是否成功
	WORD				wServerID;					//房间ServerID
	bool				bAddStock;					//true加，false减
	int					nStockIndex;				//库存索引
	LONGLONG			llStock;					//库存值
	LONGLONG			llTotalStock;				//总库存
	LONGLONG			llLeastStock;				//最小库存
	int					nCount;						//库存数
	LONGLONG			llStockData[5];				//具体库存值
	FLOAT				fServerDegree;				//房间难度
};

//16,控制服务器和游戏服务器之间消息定义EMConAndGame_MsgIndex
struct CMD_CS_C_QueryNoFishingStock_ConToGame
{
	int					nMsgIndex;					//16获取非捕鱼房间库存
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wServerID;					//房间ServerID
};

//16返回,控制服务器和游戏服务器之间消息定义EMConAndGame_MsgIndex
struct CMD_CS_C_QueryNoFishingStock_GameToCon
{
	int					nMsgIndex;					//16获取非捕鱼房间库存，返回
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wServerID;					//房间ServerID
	LONGLONG			llTotalStock;				//总库存
	LONGLONG			llLeastStock;				//最小库存
	int					nCount;						//库存数
	LONGLONG			llStockData[5];				//具体库存值
	FLOAT				fServerDegree;				//房间难度
};

//17,控制服务器和游戏服务器之间消息定义EMConAndGame_MsgIndex
struct CMD_CS_C_QueryAllControlStock_ConToGame
{
	int					nMsgIndex;					//17获取被控玩家总库存
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wServerID;					//房间ServerID
};

//17返回,控制服务器和游戏服务器之间消息定义EMConAndGame_MsgIndex
struct CMD_CS_C_QueryAllControlStock_GameToCon
{
	int					nMsgIndex;					//17获取被控玩家总库存，返回
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wServerID;					//房间ServerID
	LONGLONG			llTotalStock;				//总库存
};

//18返回,控制服务器和游戏服务器之间消息定义EMConAndGame_MsgIndex
struct CMD_CS_C_UpdateFishingStock_GameToCon
{
	int					nMsgIndex;					//15增减非捕鱼房间库存，返回
	DWORD				dwControlID;				//控制号ID
	WORD				wServerID;					//房间ServerID

	LONGLONG			llStock;					//库存值
	LONGLONG			llTotalStock;				//总库存
	LONGLONG			llLeastStock;				//最小库存

	int					nCount;						//库存数
	LONGLONG			llStockData[5];				//具体库存值
};

//18,控制服务器和游戏服务器之间消息定义EMConAndGame_MsgIndex
struct CMD_CS_C_QueryFishingStockTax_ConToGame
{
	int					nMsgIndex;					//18获取鱼库存税收值
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wServerID;					//房间ServerID，只能查询某个捕鱼房间
};

//18返回,控制服务器和游戏服务器之间消息定义EMConAndGame_MsgIndex
struct CMD_CS_C_QueryFishingStockTax_GameToCon
{
	int					nMsgIndex;					//18获取鱼库存税收值，返回
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wKindID;					//游戏KindID，具体游戏
	WORD				wServerID;					//房间ServerID，具体房间
	int					nRate;						//几率(100无控制，90是少收10%，150是多收50%)
	LONGLONG			llMaxScore;					//控制金币
	LONGLONG			llCashScore;				//兑现金币(初始为0，时常更新)
	LONGLONG			llStartTime;				//起始时间(年4位,月日时分秒各2位)，如： 201801220945
	LONGLONG			llEndTime;					//结束时间(年4位,月日时分秒各2位)，如： 201801221159
	LONGLONG			llAllCashScore;				//所有的兑现金币
};

//19,控制服务器和游戏服务器之间消息定义EMConAndGame_MsgIndex
struct CMD_CS_C_SetFishingStockTax_ConToGame
{
	int					nMsgIndex;					//19设置鱼库存税收值
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	//wServerID!=0：此房间；wServerID=0+wKindID=0：捕鱼所有房间；wServerID=0+wKindID!=0：wKindID游戏的所有房间；
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
	int					nRate;						//几率(100无控制，90是少收10%，150是多收50%)
	LONGLONG			llMaxScore;					//控制金币
	LONGLONG			llStartTime;				//起始时间(年4位,月日时分秒各2位)，如： 201801220945
	LONGLONG			llEndTime;					//结束时间(年4位,月日时分秒各2位)，如： 201801221159
};

//19返回,控制服务器和游戏服务器之间消息定义EMConAndGame_MsgIndex
struct CMD_CS_C_SetFishingStockTax_GameToCon
{
	int					nMsgIndex;					//19设置鱼库存税收值，返回
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wKindID;					//游戏KindID，具体游戏
	WORD				wServerID;					//房间ServerID，具体房间
	int					nRate;						//几率(100无控制，90是少收10%，150是多收50%)
	LONGLONG			llMaxScore;					//控制金币
	LONGLONG			llCashScore;				//兑现金币(初始为0，时常更新)
	LONGLONG			llStartTime;				//起始时间(年4位,月日时分秒各2位)，如： 201801220945
	LONGLONG			llEndTime;					//结束时间(年4位,月日时分秒各2位)，如： 201801221159
	LONGLONG			llAllCashScore;				//所有的兑现金币
};

//控制到百人游戏之间基本结构
struct CMD_CS_C_BrBaseMessage_ConToBairen
{
	int					nMsgIndex;					//附加索引
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
};

//玩家信息
struct tagBrUserInfo
{
	DWORD				dwGameID;					//GameID
	bool				bAndroid;					//机器人
	WORD				wServerID;					//玩家所处游戏房间
	LONGLONG			llUserScore;				//用户金币
	LONGLONG			llEnterScore;				//坐下金币
	LONGLONG			llAllWinScore;				//总输赢
	LONGLONG			llLastWinScore;				//上局输赢
	TCHAR				szUserName[LEN_NICKNAME];	//昵称
	int					nNowBetArea;				//下注区域0-29，每个游戏不同
	LONGLONG			llNowBetScore;				//具体下注
	LONGLONG			llUserBetScore[30];			//各门下注
};

//百人玩家输赢信息
struct tagBrUserEndInfo
{
	DWORD				dwGameID;					//GameID
	LONGLONG			llAllWinScore;				//总输赢
	LONGLONG			llLastWinScore;				//上局输赢
};

//200-299控制到百人游戏
//emConAndGame_BrQueryStatus,						//获取场景
struct CMD_CS_C_BrQueryStatus_ConToBairen
{
	int					nMsgIndex;					//附加索引
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
};

//emConAndGame_BrQueryControl = 225,				//查询控制
struct CMD_CS_C_BrQueryControl_ConToBairen
{
	int					nMsgIndex;					//附加索引
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
};

//emConAndGame_BrExcuteControl,					//执行控制
struct CMD_CS_C_BrExcuteControl_ConToBairen
{
	int					nMsgIndex;					//附加索引
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
	BYTE				cbConType;					//控制类型：0庄家输赢；1控制区域；2全区域控制
	BYTE				cbConArea;					//控制区域：0庄家输赢[0庄赢,1庄输]；1控制区域[0-29根据游戏不同而不同]
	BYTE				cbConAreaData[30];			//全区域控制：[0-29根据游戏不同而不同]=0随机,1输,2赢
	int					nConTime;					//控制次数
};

//emConAndGame_BrCancelControl,					//删除控制
struct CMD_CS_C_BrCancelControl_ConToBairen
{
	int					nMsgIndex;					//附加索引
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
};

//“飞禽走兽3D”获取鲨鱼信息
//emConAndGame_BrFqzs3DQueryShaYuInfo = 250,		//“飞禽走兽3D”获取鲨鱼信息
struct CMD_CS_C_BrFqzs3DQueryShaYuInfo_ConToBairen
{
	int					nMsgIndex;					//附加索引
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
};

//emConAndGame_BrFqzs3DModifyShaYuLimit,		//“飞禽走兽3D”修改将出鲨鱼局数限制
struct CMD_CS_C_BrFqzs3DModifyShaYuLimit_ConToBairen
{
	int					nMsgIndex;					//附加索引
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
	int					nLimitShaYuCount;			//将出鲨鱼局数
};

//“金鲨银鲨”获取鲨鱼信息
//emConAndGame_BrJsysQueryShaYuInfo = 250,		//“金鲨银鲨”获取鲨鱼信息
struct CMD_CS_C_BrJsysQueryShaYuInfo_ConToBairen
{
	int					nMsgIndex;					//附加索引
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
};

//emConAndGame_BrJsysModifyShaYuLimit,		//“金鲨银鲨”修改将出鲨鱼局数限制
struct CMD_CS_C_BrJsysModifyShaYuLimit_ConToBairen
{
	int					nMsgIndex;					//附加索引
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
	int					nLimitShaYuCount;			//将出鲨鱼局数
};

//300-399百人游戏到控制
// 	emConAndGame_BrQueryStatusResult,				//获取场景返回
struct CMD_CS_C_BrQueryStatus_BairenToCon
{
	int					nMsgIndex;					//附加索引
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
	int					nGameStatus;				//游戏状态
	TCHAR				szStatusName[16];			//游戏状态(空闲，下注，结算，)
	int					nLeaveTime;					//剩余时间
	bool				bIsBetStatus;				//是否下注状态
	DWORD				dwBankerGameID;				//庄家GameID
	int					nBankerType;				//0无庄，1系统坐庄，2玩家坐庄，3机器人坐庄
	TCHAR				szBankerInfo[64];			//庄家信息
	int					nBetAreaCount;				//下注区域名称个数
	TCHAR				szBetAreaName[30][32];		//下注区域名称(0开始)
	int					nBetAreaIndex[30];			//下注区域，对应服务端的下注区域
	LONGLONG			llPlayerBetScore[30];		//真人下注
	LONGLONG			llRobotBetScore[30];		//机器人下注
};

//emConAndGame_BrOnLineUserInfo,					//房间玩家信息
struct CMD_CS_C_BrOnLineUserInfo_BairenToCon
{
	int					nMsgIndex;					//附加索引
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
	tagBrUserInfo		BrUserInfo;					//玩家信息
};

// 	emConAndGame_BrSwitchStatus,					//状态切换(空闲、下注、结算)
struct CMD_CS_C_BrSwitchStatus_BairenToCon
{
	int					nMsgIndex;					//附加索引
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
	int					nGameStatus;				//游戏状态
	TCHAR				szStatusName[16];			//游戏状态(空闲，下注，结算，)
	int					nLeaveTime;					//剩余时间
	bool				bIsBetStatus;				//是否下注状态
};

// 	emConAndGame_BrGameStart,						//游戏开始(庄家、真人信息)
struct CMD_CS_C_BrGameStart_BairenToCon
{
	int					nMsgIndex;					//附加索引
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
	DWORD				dwBankerGameID;				//庄家GameID
	int					nBankerType;				//0无庄，1系统坐庄，2玩家坐庄，3机器人坐庄
	TCHAR				szBankerInfo[64];			//庄家信息
	int					nPlayerCount;				//真人个数
	DWORD				dwGameIDData[MAX_CHAIR];	//真人GameID
};

// 	emConAndGame_BrUserEnter,						//玩家进入
struct CMD_CS_C_BrUserEnter_BairenToCon
{
	int					nMsgIndex;					//附加索引
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
	WORD				wChairID;					//位置
	bool				bIsAndroid;					//机器人
	tagBrUserInfo		playerInfo;					//真人信息
};

// 	emConAndGame_BrUserLeave,						//玩家离开
struct CMD_CS_C_BrUserLeave_BairenToCon
{
	int					nMsgIndex;					//附加索引
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
	WORD				wChairID;					//位置
	DWORD				dwUserID;					//玩家UserID
	DWORD				dwGameID;					//玩家GameID
};

// 	emConAndGame_BrUserBetSocore,					//玩家下注（比较全面）
struct CMD_CS_C_BrUserBetSocore_BairenToCon
{
	int					nMsgIndex;					//附加索引
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
	WORD				wChairID;					//位置
	DWORD				dwGameID;					//GameID
	bool				bIsAndroid;					//机器人
	int					nNowBetArea;				//下注区域0-29，每个游戏不同
	LONGLONG			llNowBetScore;				//具体下注
	LONGLONG			llUserBetScore[30];			//各门下注
	LONGLONG			llPlayerBetScore[30];		//真人下注
	LONGLONG			llRobotBetScore[30];		//机器人下注
};

//emConAndGame_BrUserBetSocoreSimple,				//玩家下注（简单，只有自己当前区域）
struct CMD_CS_C_BrUserBetSocoreSimple_BairenToCon
{
	int					nMsgIndex;					//附加索引
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
	WORD				wChairID;					//位置
	DWORD				dwGameID;					//GameID
	bool				bIsAndroid;					//机器人
	int					nNowBetArea;				//下注区域0-29，每个游戏不同
	LONGLONG			llNowBetScore;				//具体下注
	LONGLONG			llUserAreaBetScore;			//玩家该区域总下注
	LONGLONG			llAllAreaBetScore;			//真人该区域总下注
};

//emConAndGame_BrUserBetSocoreClear,				//玩家清除下注
struct CMD_CS_C_BrUserBetSocoreClear_BairenToCon
{
	int					nMsgIndex;					//附加索引
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
	WORD				wChairID;					//位置
	DWORD				dwGameID;					//GameID
	bool				bIsAndroid;					//机器人
	LONGLONG			llUserBetScore[30];			//各门下注
	LONGLONG			llPlayerBetScore[30];		//真人下注
};

//emConAndGame_BrUserBetSocoreAndroid,			//机器人下注（机器人下注，总的下注，1秒刷新一次）
struct CMD_CS_C_BrUserBetSocoreAndroid_BairenToCon
{
	int					nMsgIndex;					//附加索引
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
	LONGLONG			llRobotBetScore[30];		//机器人下注
};

// 	emConAndGame_BrGameEnd,							//结算信息
struct CMD_CS_C_BrGameEnd_BairenToCon
{
	int					nMsgIndex;					//附加索引
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
	int					nPlayerCount;
	tagBrUserEndInfo	BrUserEndInfo[MAX_CHAIR];
};

//emConAndGame_BrSysWinScore,						//当前系统输赢
struct CMD_CS_C_BrSysWinScore_BairenToCon
{
	int					nMsgIndex;					//附加索引
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
	LONGLONG			llSysWinScore[30];			//各门系统下注
};

//emConAndGame_BrUpdateControl = 325,				//更新控制信息
struct CMD_CS_C_BrUpdateControl_BairenToCon
{
	int					nMsgIndex;					//附加索引
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
	BYTE				cbConType;					//控制类型：0庄家输赢；1控制区域；2全区域控制
	BYTE				cbConArea;					//控制区域：0庄家输赢[0庄赢,1庄输]；1控制区域[0-29根据游戏不同而不同]
	BYTE				cbConAreaData[30];			//全区域控制：[0-29根据游戏不同而不同]=0随机,1输,2赢
	int					nConTime;					//控制次数
	TCHAR				szConBuf[256];				//控制信息
};

// 	emConAndGame_BrFqzs3DQueryShaYuInfoResult = 350,//“飞禽走兽3D”获取鲨鱼信息返回
struct CMD_CS_C_BrFqzs3DQueryShaYuInfo_BairenToCon
{
	int					nMsgIndex;					//附加索引
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
	LONGLONG			llNowStock;					//当前库存
	LONGLONG			llMinShaYuStock;			//出鲨鱼要求库存
	int					nNoShaYuCount;				//未出鲨鱼局数
	int					nLimitShaYuCount;			//将出鲨鱼局数
	BYTE				cbNextJinSha;				//下次是否出金鲨
	TCHAR				szText[256];				//描述信息
};

// 	emConAndGame_BrJsysQueryShaYuInfoResult = 350,//“金鲨银鲨”获取鲨鱼信息返回
struct CMD_CS_C_BrJsysQueryShaYuInfo_BairenToCon
{
	int					nMsgIndex;					//附加索引
	DWORD				dwSocketID;					//客户端Socket
	DWORD				dwControlID;				//控制号ID
	WORD				wKindID;					//游戏KindID
	WORD				wServerID;					//房间ServerID
	LONGLONG			llNowStock;					//当前库存
	LONGLONG			llMinShaYuStock;			//出鲨鱼要求库存
	int					nNoShaYuCount;				//未出鲨鱼局数
	int					nLimitShaYuCount;			//将出鲨鱼局数
	BYTE				cbNextJinSha;				//下次是否出金鲨
	TCHAR				szText[256];				//描述信息
};

struct CMD_CS_C_UserInsureInOut
{
	DWORD				dwTranGameID;
	LONGLONG			llTranScore;
	WORD				wKindId;
};

//更新用户财富
struct CMD_CS_C_PlayUserUpdataScore
{

	DWORD				dwGameID;					//游戏标识
	SCORE               llCurScore;					//当前金币
	SCORE               llInsureScore;				//银行金币
	WORD                wServerID;
};

//wxr20170103 任务
//SUB_CS_C_TASK_PLAY_GAME_INFO=218任务(玩家输赢信息)
struct CMD_CS_C_PlayGameInfo
{
	DWORD				dwUserID;					//用户ID(为0表示该信息无效)
	WORD				wKindID;					//游戏id
	DWORD				dwPlayTimes;				//玩的次数
	LONGLONG			llWinScore;					//赢的分数
	LONGLONG			llBetScore;					// 下注的总数
};

/////////////////////////////////////////////////////

//增减房间库存
struct CMD_GP_AddServerStock
{
	WORD				wServerID;					//房间ServerID
	bool				bAddStock;					//true加，false减
	int					nStockIndex;				//库存索引
	LONGLONG			llAddStock;					//增减库存数
};
//增减房间库存，返回
struct CMD_GP_AddServerStockReturn
{
	bool				bSuccess;					//是否成功
	WORD				wServerID;					//房间ServerID
	bool				bAddStock;					//true加，false减
	int					nStockIndex;
	LONGLONG			llAddStock;					//增减库存数
	LONGLONG			llTotalStock;				//库存总数
	LONGLONG			llLeastStock;				//库存最小数
	int					nCount;						//库存数
	LONGLONG			llStockData[5];				//具体库存值
	FLOAT				fServerDegree;				//房间难度
};

//查询房间库存
struct CMD_GP_QueryServerStock
{
	WORD				wServerID;					//房间ServerID
};

//查询房间库存，返回
struct CMD_GP_QueryServerStockReturn
{
	WORD				wServerID;					//房间ServerID
	LONGLONG			llTotalStock;				//库存总数
	LONGLONG			llLeastStock;				//库存最小数
	FLOAT				fServerDegree;				//房间难度
	int					nStockCount;				//库存个数
	LONGLONG			llStockData[5];				//具体库存
};

//Game5678登录过来的控制信息配置
struct CMD_GP_NEWREGISTER_KZ
{
	DWORD				dwGameID;
	DWORD				dwIsLockMobile;
};


//获取房间难度
struct CMD_GP_QueryServerDegree
{
	WORD				wServerID;					//房间I D
};

//获取房间难度返回
struct CMD_GP_QueryServerDegreeRetrun
{
	WORD				wServerID;					//房间I D
	WORD				wKindID;					//类型I D
	FLOAT				fServerDegree;				//房间难度
	TCHAR				szTip[256];					//提示信息
};

//设置房间难度
struct CMD_GP_SetServerDegree
{
	WORD				wServerID;					//房间I D
	FLOAT				fServerDegree;				//房间难度
};

//设置房间难度返回
struct CMD_GP_SetServerDegreeReturn
{
	WORD				wServerID;					//房间I D
	WORD				wKindID;					//类型I D
	FLOAT				fServerDegree;				//房间难度
	TCHAR				szTip[256];					//提示信息
};


//////////////////////////////////////////////////////////////////////////////////
//远程服务

#define MDM_CS_REMOTE_SERVICE		4				//远程服务

//查找服务
#define SUB_CS_C_SEARCH_DATABASE	1				//数据查找
#define SUB_CS_C_SEARCH_CORRESPOND	2				//协调查找

//查找结果
#define SUB_CS_S_SEARCH_DATABASE	100				//数据查找
#define SUB_CS_S_SEARCH_CORRESPOND	101				//协调查找

//////////////////////////////////////////////////////////////////////////////////

//协调查找
struct CMD_CS_C_SearchCorrespond
{
	//连接参数
	DWORD				dwSocketID;					//网络标识
	DWORD				dwClientAddr;				//连接地址

	//查找数据
	DWORD				dwGameID;					//游戏标识
	TCHAR				szNickName[LEN_NICKNAME];	//用户昵称
};

//协调查找
struct CMD_CS_S_SearchCorrespond
{
	//连接信息
	DWORD				dwSocketID;					//网络标识
	DWORD				dwClientAddr;				//连接地址

	//数据信息
	WORD				wUserCount;					//用户数目
	tagUserRemoteInfo	UserRemoteInfo[16];			//用户信息
};

//////////////////////////////////////////////////////////////////////////////////

//管理服务
#define MDM_CS_MANAGER_SERVICE			5			//管理服务

#define SUB_CS_C_SYSTEM_MESSAGE 		1			//系统消息
#define SUB_CS_C_HORN_TRUMPET 			2			//商人喇叭
#define SUB_CS_C_GAME_TRUMPET 			3			//游戏喇叭
#define SUB_CS_C_SERVER_TRANCON_ALL 	4			//房间控制
#define SUB_CS_C_SERVER_TRANCON_BASE 	5			//房间控制
#define SUB_CS_C_SERVER_ROOM_HANDSEL	6			//房间彩金

#define SUB_CS_S_SYSTEM_MESSAGE 		100			//系统消息
#define SUB_CS_S_SERVER_TRANCON_ALL		101			//房间控制
#define SUB_CS_S_SERVER_TRANCON_BASE	102			//房间控制
#define SUB_CS_S_SERVER_ROOM_HANDSEL	103			//房间彩金
#define SUB_CS_S_SERVER_FORFEND_GAME	104			//禁止游戏
#define SUB_CS_S_SERVER_STOP			105			//停止服务


#define SUB_CS_S_HORN_TRUMPET  			201			//商人喇叭
#define SUB_CS_S_DEFEND_DEFEND			202			//维护喇叭
#define SUB_CS_S_GAME_TRUMPET  			203			//游戏喇叭

//发送喇叭
struct CMD_CS_S_SendTrumpet
{
	WORD				wPropertyIndex;
	DWORD				dwSendUserID;
	DWORD				TrumpetColor;
	TCHAR				szSendNickName[32];	
	TCHAR				szTrumpetContent[TRUMPET_MAX_CHAR];
};

struct CMD_CS_S_LABARECORD
{
	DWORD				dwUserID;
	DWORD				dwToUserID;
	TCHAR				szMessage[255];
};

struct CMD_CS_S_ServerTranConAll
{
	WORD				wConType;					//0房间，1游戏，2所有
	WORD				wKindId;					//游戏类型

	WORD				wServerId;					//房间ID
	BYTE				byMobWinOrLost;				//手机赢或输
	BYTE				byMobdds;					//手机几率
	BYTE				byPcWinOrLost;				//电脑赢或输
	BYTE				byPcdds;					//电脑几率

	LONGLONG			llServerStort;				//总的库存

	LONGLONG			llServerStortLv1;			//一级库存
	LONGLONG			llServerStortLv2;			//二级库存
	LONGLONG			llServerStortLv3;			//三级库存
	LONGLONG			llServerStortLv4;			//四级库存
	LONGLONG			llServerStortLv5;			//五级库存
	LONGLONG			llLeast;					//库存底限

	FLOAT				fServerDegree;				//房间难度		
};

struct CMD_CS_S_ServerTranConBase
{
	WORD				wConType;					//0房间，1游戏，2所有
	WORD				wKindId;					//游戏类型
	WORD				wServerId;					//房间ID

	BYTE				byMobWinOrLost;				//手机赢或输
	BYTE				byMobdds;					//手机几率
	BYTE				byPcWinOrLost;				//电脑赢或输
	BYTE				byPcdds;					//电脑几率

	FLOAT				fServerDegree;				//房间难度		
};

//房间彩金
struct CMD_CS_S_ServerRoomHandsel
{
	WORD				wServerId;					//房间信息
	WORD				wKindID;					//房间类型
	TCHAR				szServerName[LEN_SERVER];	//房间名称
	FLOAT				fDegree;					//房间难度
	LONGLONG			llHandselLv1;				//房间彩金1
	LONGLONG			llHandselLv2;				//房间彩金2
	LONGLONG			llHandselLv3;				//房间彩金3
	LONGLONG			llHandselLv4;				//房间彩金4
	LONGLONG			llHandselLv5;				//房间彩金5
	LONGLONG			llHandselLv6;				//房间彩金6
	LONGLONG			llHandselLv7;				//房间彩金7
	LONGLONG			llHandselLv8;				//房间彩金8
};

//修改房间彩金
struct CMD_CS_C_AddServerRoomHandsel
{
	WORD                wServerId;
	bool				bAddHandsel;				//true加，false减
	WORD				wType;
	LONGLONG			llHandsel;
};

//设置房间难度
struct CMD_CS_C_SetServerRoomHandselDegree
{
	WORD                wServerId;
	FLOAT				fDegree;
};

//发送喇叭
struct CMD_GR_C_SendTrumpet
{
	DWORD				dwUserID;
	TCHAR				szTrumpetContent[TRUMPET_MAX_CHAR];
	DWORD				dwChannel;
};

//接收喇叭
struct CMD_GR_S_HornTrumpet
{
	DWORD				dwSendUserID;
	TCHAR				szTrumpetContent[TRUMPET_MAX_CHAR];
	DWORD				dwChannel;						    
};

//维护喇叭
struct CMD_GR_S_DefendTrumpet
{
	TCHAR				szTrumpetContent[TRUMPET_MAX_CHAR];
};

//游戏喇叭
struct CMD_GR_S_GameTrumpet
{
	TCHAR				szTrumpetContent[256];				//消息内容
	DWORD				dwChannel;							//发送渠道
};

//////////////////////////////////////////////////////////////////////////////////


//聊天命令
#define SUB_CS_S_CHAT_INSERT		130						//聊天服务
#define SUB_SS_S_ONLINE				140						//用户上线
#define SUB_SS_S_OFFLINE			141						//用户离线
#define SUB_SS_S_STATUS				142						//用户状态
#define SUB_SS_S_GAMESTATUS			143						//游戏状态
#define SUB_SS_S_SYNCHRFRIEND		144						//同步好友	
#define SUB_SS_S_SYNCHARFINISH		145						//同步完成
#define SUB_SS_S_MODIFYFRIEND		146						//修改好友
#define SUB_SS_S_REMOVEGROUP		147						//移除分组
#define SUB_CS_S_USER_GAMESTATE		101						//用户状态


//用户状态
struct CMD_CS_S_UserGameStatus
{
	DWORD				dwUserID;							//用户标识
	BYTE				cbGameStatus;						//用户状态
	WORD				wKindID;							//游戏标识
	WORD				wServerID;							//房间标识
	WORD				wTableID;							//桌子标识
	WORD				wChairID;							//椅子位置
	TCHAR				szServerName[LEN_SERVER];			//房间名称
};


#define SUB_CS_S_LOGON_CHECK_INSERT		150					//审核服务
#define SUB_CS_S_PERSONNAL_INSERT		160					//个人服务
#define SUB_CS_S_BANK_INSERT			170					//银行服务
#define SUB_CS_S_BANK_5678_INSERT		180					//银行服务
#define SUB_CS_S_MALL_INSERT			190					//商城服务
#define SUB_CS_S_TASK_INSERT			200					//任务服务
#define SUB_CS_S_CHARGE_INSERT			210					//充值服务
#define SUB_CS_S_SYNC_INSERT			220					//同步服务
#define SUB_CS_S_HRON_INSERT			230					//喇叭服务
#define SUB_CS_S_LIEBIAN_AGENT_INSERT	240					//代理裂变服务
#define SUB_CS_S_LIEBIAN_CHARGE_INSERT	250					//充值裂变服务
#define SUB_CS_S_STATISTICS_INSERT		260					//统计服务

enum EM_GM_InitDataIndex									//游戏服发往控制服(协调服转发emCtrlMsg_Cs_GsToMs)
{
	emGMIDI_WinLevel,										//输赢等级-BYTE
	emGMIDI_CaiJinCtrl,										//彩金控制-DWORD,DWORD,DWORD
};

enum EM_CtrlMsg_Gs_Ms										//游戏服发往控制服(协调服转发emCtrlMsg_Cs_GsToMs)
{
	emCtrlMsg_GM_InitData,									//初始化数据
	emCtrlMsg_GM_UpdateCaiJin,
	emCtrlMsg_GM_ErrMsg,
	emCtrlMsg_GM_CaiJinValue,								//彩金值
	emCtrlMsg_GM_ControlWin,								//输赢调控
};

struct SGstoMsHead
{
	DWORD				dwSocketID;
	BYTE				nCmd;
	WORD				wServerID;

	SGstoMsHead()
	{
		memset(this, 0, sizeof(*this));
	}
};

//机器服务
#define MDM_CS_MANAGER_ANDRIOD			6					//机器服务

#define SUB_CS_C_RELEASE_ANDRIOD		1					//释放机器
#define SUB_CS_C_REQUEST_ANDRIOD 		2					//申请机器
#define SUB_CS_S_REQUEST_ANDRIOD		3					//应答机器
#define SUB_CS_C_RECYCLE_ANDRIOD 		4					//回收机器
#define SUB_CS_S_RECYCLE_ANDRIOD		5					//应答机器

//释放机器
struct CMD_GR_C_ReleaseAndriod
{
	WORD			wServerID;								//房间序号
};


//请求机器
struct CMD_GR_C_RequestAndriod
{
	WORD			wServerID;								//房间序号
	WORD			wKindID;								//游戏类型
	WORD			wChairCount;							//椅子数量
	WORD			wAndroidCnt;							//加载数量
	BYTE			cbAllowYoukeAndriod;					//允许游客
};

struct CMD_GR_S_RequestAndriod
{
	WORD			wServerID;								//房间序号
	WORD			wKindID;								//游戏类型
	WORD			wChairCount;							//椅子数量
	WORD			wAndroidCnt;							//加载数量
	tagAndriodInfo	vAndriodInfo[10];						//机器数组
};

//回收机器
struct CMD_GR_C_RecycleAndriod
{
	WORD			wServerID;								//房间标识
	DWORD			dwUserID;								//机器信息
};

struct CMD_GR_S_RecycleAndriod
{
	WORD			wServerID;								//房间标识
	DWORD			dwUserID;								//机器信息
};

#pragma pack()

#endif