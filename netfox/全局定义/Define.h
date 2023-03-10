#ifndef DEFINE_HEAD_FILE
#define DEFINE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//个人操作
#define PERSON_NONE_USELESS			0									//无用操作类型
#define PERSON_QUERY_INDIVIDUAL		1									//查询个人资料
#define PERSON_MODIFY_INDIVIDUAL	2									//修改个人资料
#define PERSON_MODIFY_MACHINE		3									//绑定解绑机器
#define PERSON_MODIFY_LOGON_PASS	4									//修改登录密码
#define PERSON_MODIFY_INSURE_PASS	5									//修改银行密码
#define PERSON_PHONE_RESET_PASS		6									//手机重置密码

//游戏活动
#define MAX_GAME_ACTIVE				2048								//游戏活动

//访问控制
#define DEFAULT_TIME_PERIOD			1000								//默认时间
#define DEFAULT_ACCESS_PERIOD		3									//默认频率
#define DEFALUT_TIMEOUT_PERIOD		60 * 1000							//默认超时

//访问类型
#define ACCESS_TYPE_LOGON			1									//登陆类型
#define ACCESS_TYPE_GAME			2									//游戏类型
#define ACCESS_TYPE_BANK			3									//银行类型
#define ACCESS_TYPE_PERSION			4									//个人类型
#define ACCESS_TYPE_TASK			5									//任务类型

//调试日志
#define ANDRIOD_DEBUG_LOG			0									//机器调试日志

//调试标记
#define ANDRIOD_DEBUG_FLAG			0									//机器调试标记

//平台名称
#define	PLATFROM_NAME_LEN			32									//平台名称

//平台是“多语言包”或仅“简体中文”；货币是“金币”或是“真钱”；目前仅小游戏中使用
#define PLATFROM_INTERNATIONAL											//该宏定义说明支持多语言包，注释后只是简体中文(只针对小游戏)
//#define PLATFROM_GAME_RMB												//该宏定义说明货币是真钱，注释后游戏货币是金币(只针对小游戏)

//网络日志
#define	MAX_PLATFROM_LEN			10									//平台长度
#define MAX_CATEGORY_LEN			25									//标题长度
#define	MAX_LOGER_LEN				2048								//日志长度

//房间彩金
#define MIN_HANDSEL_TYPE			1									//最小类型
#define MAX_HANDSEL_TYPE			8									//最大类型

//数值定义
#define SERVER_LINK_COUNT			20									//下发数量
#define DOWNLOAD_LINK_COUNT			5									//下载数量

//头像大小
#define FACE_CX						48									//头像宽度
#define FACE_CY						48									//头像高度

//长度定义
#define LEN_LESS_ACCOUNTS			6									//最短帐号
#define LEN_MAX_ACCOUNTS			20									//最长帐号
#define LEN_LESS_NICKNAME			2									//最短昵称
#define LEN_MAX_NICKNAME			14									//最长昵称
#define LEN_LESS_PASSWORD			6									//最短密码

//人数定义
#define MAX_CHAIR					500									//最大椅子
#define MAX_TABLE					512									//最大桌子
#define MAX_COLUMN					32									//最大列表
#define MAX_ANDROID					500									//最大机器
#define MAX_PROPERTY				128									//最大道具
#define MAX_WHISPER_USER			16									//最大私聊、

//红包定义
#define MIN_HONGBAO_COUNT			20									//最低红包
#define MAX_HONGBAO_COUNT			40									//最大红包

//列表定义
#define MAX_KIND					128									//最大类型
#define MAX_SERVER					1024								//最大房间

//参数定义
#define INVALID_CHAIR				0xFFFF								//无效椅子
#define INVALID_TABLE				0xFFFF								//无效桌子
#define  INVALID_WORD_INDEX         (WORD)(0xFFFF)	                    //无效索引
//税收定义
#define REVENUE_BENCHMARK			0L								    //税收起点
#define REVENUE_DENOMINATOR			1000L								//税收分母

//////////////////////////////////////////////////////////////////////////////////
//系统参数

//积分类型
#define SCORE						LONGLONG							//积分类型
#define SCORE_STRING				TEXT("%I64d")						//积分类型

//游戏状态
#define GAME_STATUS_FREE			0									//空闲状态
#define GAME_STATUS_PLAY			100									//游戏状态
#define GAME_STATUS_WAIT			200									//等待状态
#define GAME_STATUS_FORCE			99									//强制结束

//系统参数
#define LEN_USER_CHAT				256									//聊天长度
#define TIME_USER_CHAT				1L									//聊天间隔
#define TRUMPET_MAX_CHAR            256									//喇叭长度

//////////////////////////////////////////////////////////////////////////////////
//索引质数

//列表质数
#define PRIME_TYPE					11L									//种类数目
#define PRIME_KIND					53L									//类型数目
#define PRIME_NODE					101L								//节点数目
#define PRIME_PAGE					53L									//自定数目
#define PRIME_SERVER				1009L								//房间数目

//人数质数
#define PRIME_SERVER_USER			503L								//房间人数
#define PRIME_ANDROID_USER			503L								//机器人数
#define PRIME_PLATFORM_USER			100003L								//平台人数

//////////////////////////////////////////////////////////////////////////////////
//数据长度

//资料数据
#define LEN_MD5						33									//加密密码
#define LEN_USERNOTE				32									//备注长度
#define LEN_ACCOUNTS				32									//帐号长度
#define LEN_NICKNAME				32									//昵称长度
#define LEN_PASSWORD				33									//密码长度
#define LEN_GROUP_NAME				32									//社团名字
#define LEN_UNDER_WRITE				32									//个性签名

#define LEN_QUESTION               256
#define LEN_ANSWER                  30

//数据长度
#define LEN_QQ						16									//Q Q 号码
#define LEN_EMAIL					33									//电子邮件
#define LEN_USER_NOTE				256									//用户备注
#define LEN_SEAT_PHONE				33									//固定电话
#define LEN_MOBILE_PHONE			12									//移动电话
#define LEN_PASS_PORT_ID			19									//证件号码
#define LEN_COMPELLATION			16									//真实名字
#define LEN_DWELLING_PLACE			128									//联系地址

//机器标识
#define LEN_NETWORK_ID				13									//网卡长度
#define LEN_MACHINE_ID				33									//序列长度
#define LEN_TOKEN					513									//TOKEN长度

//列表数据
#define LEN_TYPE					32									//种类长度
#define LEN_KIND					32									//类型长度
#define LEN_NODE					32									//节点长度
#define LEN_PAGE					32									//定制长度
#define LEN_SERVER					32									//房间长度
#define LEN_PROCESS					32									//进程长度

//////////////////////////////////////////////////////////////////////////////////

//用户关系
#define	CP_NORMAL					0									//未知关系
#define	CP_FRIEND					1									//好友关系
#define	CP_DETEST					2									//厌恶关系
#define CP_SHIELD					3									//屏蔽聊天

//////////////////////////////////////////////////////////////////////////////////

//性别定义
#define GENDER_FEMALE				0									//女性性别
#define GENDER_MANKIND				1									//男性性别

//////////////////////////////////////////////////////////////////////////////////

//游戏模式
#define GAME_GENRE_GOLD				0x0001								//金币类型
#define GAME_GENRE_SCORE			0x0002								//点值类型
#define GAME_GENRE_MATCH			0x0004								//比赛类型
#define GAME_GENRE_EDUCATE			0x0008								//训练类型
#define GAME_GENRE_WATCH			0x0010								//观战类型
#define GAME_GENRE_RMB              0x0020                              //真钱类型
#define GAME_GENRE_ROOMCARD         0x0040                              //房卡类型


//分数模式
#define SCORE_GENRE_NORMAL			0x0100								//普通模式
#define SCORE_GENRE_POSITIVE		0x0200								//非负模式

//////////////////////////////////////////////////////////////////////////////////

//用户状态
#define US_NULL						0x00								//没有状态
#define US_FREE						0x01								//站立状态
#define US_SIT						0x02								//坐下状态
#define US_READY					0x03								//同意状态
#define US_LOOKON					0x04								//旁观状态
#define US_PLAYING					0x05								//游戏状态
#define US_OFFLINE					0x06								//断线状态

#define US_MATCHPALAYING           0x01
#define US_MATCHWAITING            0x02
#define US_MATCHOUT                0x03
//////////////////////////////////////////////////////////////////////////////////

//比赛状态
#define MS_NULL						0x00								//没有状态
#define MS_SIGNUP					0x01								//报名状态
#define MS_MATCHING					0x02								//比赛状态
#define MS_OUT						0x03								//淘汰状态

//////////////////////////////////////////////////////////////////////////////////

//房间规则
#define SRL_LOOKON					0x00000001							//旁观标志
#define SRL_OFFLINE					0x00000002							//断线标志
#define SRL_SAME_IP					0x00000004							//同网标志

//房间规则
#define SRL_ROOM_CHAT				0x00000100							//聊天标志
#define SRL_GAME_CHAT				0x00000200							//聊天标志
#define SRL_WISPER_CHAT				0x00000400							//私聊标志
#define SRL_HIDE_USER_INFO			0x00000800							//隐藏标志

//////////////////////////////////////////////////////////////////////////////////
//列表数据

//无效属性
#define UD_NULL						0									//无效子项
#define UD_IMAGE					100									//图形子项
#define UD_CUSTOM					200									//自定子项

//基本属性
#define UD_GAME_ID					1									//游戏标识
#define UD_USER_ID					2									//用户标识
#define	UD_NICKNAME					3									//用户昵称

//扩展属性
#define UD_GENDER					10									//用户性别
#define UD_GROUP_NAME				11									//社团名字
#define UD_UNDER_WRITE				12									//个性签名

//状态信息
#define UD_TABLE					20									//游戏桌号
#define UD_CHAIR					21									//椅子号码

//积分信息
#define UD_SCORE					30									//用户分数
#define UD_GRADE					31									//用户成绩
#define UD_USER_MEDAL				32									//用户经验
#define UD_EXPERIENCE				33									//用户经验
#define UD_LOVELINESS				34									//用户魅力
#define UD_WIN_COUNT				35									//胜局盘数
#define UD_LOST_COUNT				36									//输局盘数
#define UD_DRAW_COUNT				37									//和局盘数
#define UD_FLEE_COUNT				38									//逃局盘数
#define UD_PLAY_COUNT				39									//总局盘数

//积分比率
#define UD_WIN_RATE					40									//用户胜率
#define UD_LOST_RATE				41									//用户输率
#define UD_DRAW_RATE				42									//用户和率
#define UD_FLEE_RATE				43									//用户逃率
#define UD_GAME_LEVEL				44									//游戏等级

//扩展信息
#define UD_NOTE_INFO				50									//用户备注
#define UD_LOOKON_USER				51									//旁观用户

//图像列表
#define UD_IMAGE_FLAG				(UD_IMAGE+1)						//用户标志
#define UD_IMAGE_GENDER				(UD_IMAGE+2)						//用户性别
#define UD_IMAGE_STATUS				(UD_IMAGE+3)						//用户状态
#define UD_IMAGE_FLATFORM			(UD_IMAGE+4)						//用户平台

//////////////////////////////////////////////////////////////////////////////////
//数据库定义

#define DB_ERROR 					-1  								//处理失败
#define DB_SUCCESS 					0  									//处理成功
#define DB_NEEDMB 					18 									//处理失败

//////////////////////////////////////////////////////////////////////////////////
//道具标示
#define PT_USE_MARK_DOUBLE_SCORE    0x0001								//双倍积分
#define PT_USE_MARK_FOURE_SCORE     0x0002								//四倍积分
#define PT_USE_MARK_GUARDKICK_CARD  0x0010								//防踢道具
#define PT_USE_MARK_POSSESS         0x0020								//附身道具 
#define PT_USE_MARK_TALK_CARD		0x0040								//私聊道具

#define MAX_PT_MARK                 /*4*/5                                   //标识数目

//有效范围
#define VALID_TIME_DOUBLE_SCORE     3600                                //有效时间
#define VALID_TIME_FOUR_SCORE       3600                                //有效时间
#define VALID_TIME_GUARDKICK_CARD   3600                                //防踢时间
#define VALID_TIME_POSSESS          3600                                //附身时间
#define VALID_TIME_KICK_BY_MANAGER  3600                                //游戏时间 

//////////////////////////////////////////////////////////////////////////////////
//设备类型
#define DEVICE_TYPE_PC              0x00                                //PC
#define DEVICE_TYPE_ANDROID         0x10                                //Android
#define DEVICE_TYPE_ITOUCH          0x20                                //iTouch
#define DEVICE_TYPE_IPHONE          0x40                                //iPhone
#define DEVICE_TYPE_IPAD            0x80                                //iPad

/////////////////////////////////////////////////////////////////////////////////
//手机定义

//视图模式
#define	VIEW_MODE_ALL				0x0001								//全部可视
#define	VIEW_MODE_PART				0x0002								//部分可视

//信息模式
#define VIEW_INFO_LEVEL_1			0x0010								//部分信息
#define VIEW_INFO_LEVEL_2			0x0020								//部分信息
#define VIEW_INFO_LEVEL_3			0x0040								//部分信息
#define VIEW_INFO_LEVEL_4			0x0080								//部分信息

//其他配置
#define RECVICE_GAME_CHAT			0x0100								//接收聊天
#define RECVICE_ROOM_CHAT			0x0200								//接收聊天
#define RECVICE_ROOM_WHISPER		0x0400								//接收私聊

//行为标识
#define BEHAVIOR_LOGON_NORMAL       0x0000                              //普通登录
#define BEHAVIOR_LOGON_IMMEDIATELY  0x1000                              //立即登录

/////////////////////////////////////////////////////////////////////////////////
//处理结果
#define RESULT_ERROR 					-1  								//处理错误
#define RESULT_SUCCESS 					0  									//处理成功
#define RESULT_FAIL 					1  									//处理失败

/////////////////////////////////////////////////////////////////////////////////
//变化原因
#define SCORE_REASON_WRITE              0                                   //写分变化
#define SCORE_REASON_INSURE             1                                   //银行变化
#define SCORE_REASON_PROPERTY           2                                   //道具变化
#define SCORE_REASON_MATCH_FEE          3                                   //比赛报名
#define SCORE_REASON_MATCH_QUIT         4                                   //比赛退赛
#define SCORE_REASON_CARDROOM			5                                   //房卡房切换

/////////////////////////////////////////////////////////////////////////////////

//登录房间失败原因
#define LOGON_FAIL_SERVER_FAST_PERIOD		100								//频率过快
#define LOGON_FAIL_SERVER_INVALIDATION		200                             //房间失效
#define LOGON_FAIL_SERVER_USER_COUNT_MAX	300								//房间人满

//红包类型
#define SEND_HONG_BAO_SCORE				0									//发送红包
#define RECV_HONG_BAO_SCORE				1									//接收红包

#define LOGINSERVER						_T("LoginServer")
#define PERSONCENTERSERVER				_T("PersonCenterServer")
#define GAMEERVER						_T("GamerServer")
#define CHATSERVER						_T("ChatServer")
#define BANKERSERVER					_T("BankServer")
#define TASKSERVER						_T("TaskServer")
#define NEWPHPSERVER					_T("NewPhpServer")
#define	STATISTICSSERVER				_T("StatisticsServer")
#define LOGINCONSERVER					_T("LoginConServer")
#define LOGONUNIFIEDORDERSERVER			_T("LogonUnifiedOrderServer")

#define SCORECHANGE						_T("金币变化日志")

#define CPushLog			CServerPushLog::SharedLog()->PushServerLog

#define CPushKernelLog		CServerPushLog::SharedLog()->PushKernelLog

#define CPushGameLog		CServerPushLog::SharedLog()->PushGameLog

#define CPushScoreLog		CServerPushLog::SharedLog()->PushScoreLog

////////////////////////////////////////////////////////////////////////////////
#endif