#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//抽奖功能
#define LEN_WEEK					7									//星期长度
#define MAX_LOTTERY					24									//最多奖项

//任务数量
#define TASK_MAX_COUNT				128									//任务数量
#define LEN_TASK_NAME				64									//任务名称


//签到奖励
struct tagCheckInReward
{
	DWORD							dwSignID;					//签到ID
	LONGLONG						llSignReward;				//签到奖励
	DWORD							dwSignRewardType;			//奖励类型
	DWORD							dwPropID;					//送到道具ID
};

//会员参数
struct tagMemberParameter
{
	BYTE							cbMemberOrder;						//会员标识
	TCHAR							szMemberName[16];					//会员名称
	SCORE							lMemberPrice;						//会员价格
	SCORE							lPresentScore;						//赠送游戏币
};

//会员参数
struct tagMemberParameterNew
{
	BYTE							cbMemberOrder;						//会员标识
	TCHAR							szMemberName[16];					//会员名称
	DWORD                           dwMemberRight;                      //会员权限
	DWORD                           dwMemberTask;                       //会员任务
	DWORD                           dwMemberShop;                       //会员商城
	DWORD                           dwMemberInsure;                     //会员银行
	DWORD                           dwMemberDayPresent;                 //会员送金
	DWORD                           dwMemberDayGiftID;                  //会员礼包
};

//实名参数
struct tagAuthRealParameter
{
	DWORD							dwAuthentDisable;					//验证开启
	SCORE							dwAuthRealAward;					//奖励金币
};

//奖项子项
struct tagLotteryItem
{
	WORD							wPrizeID;						//奖品ID
	BYTE							cbPrizeType;					//奖品类型(cbPrizeType=8是代金券，第一次抽奖肯定是代金券；后面两次抽其他)
	SCORE							llPrizeNum;						//奖品数量(比如1000金币就是1000,1张房卡就是1)
	WORD							wPrizeRate;						//奖品概率(100为底)
	TCHAR							szPrizeName[32];				//奖品描述
};

//任务参数
struct tagTaskParameter
{
	//基本信息
	WORD							wTaskID;							//任务标识
	WORD							wTaskType;							//任务类型
	WORD							wTaskObject;						//任务目标
	BYTE							cbPlayerType;						//玩家类型
	WORD							wKindID;							//类型标识
	DWORD							dwTimeLimit;						//时间限制

	//奖励信息
	SCORE							lStandardAwardGold;					//奖励金币
	SCORE							lStandardAwardMedal;				//奖励奖牌
	SCORE							lMemberAwardGold;					//奖励金币
	SCORE							lMemberAwardMedal;					//奖励奖牌

	//描述信息
	TCHAR							szTaskName[LEN_TASK_NAME];			//任务名称
	TCHAR							szTaskDescribe[320];				//任务描述
};

//任务状态
struct tagTaskStatus
{
	WORD							wTaskID;							//任务标识
	WORD							wTaskProgress;						//任务进度
	BYTE							cbTaskStatus;						//任务状态
};

//低保参数
struct tagBaseEnsureParameter
{
	SCORE							lScoreCondition;					//游戏币条件
	SCORE							lScoreAmount;						//游戏币数量
	BYTE							cbTakeTimes;						//领取次数	
};

//推广信息
struct tagUserSpreadInfo
{
	DWORD							dwSpreadCount;						//推广人数
	SCORE							lSpreadReward;						//推广奖励
};

//////////////////////////////////////////////////////////////////////////////////

//服务命令

#define MDM_GP_USER_TASK_SERVICE		30								//用户任务服务

#define SUB_GP_INDIVIDUAL_RESULT		153								//完善资料

//签到服务
#define SUB_GP_CHECKIN_QUERY			220								//查询签到
#define SUB_GP_CHECKIN_INFO				221								//签到信息
#define SUB_GP_CHECKIN_DONE				222								//执行签到
#define SUB_GP_CHECKIN_RESULT			223								//签到结果

//任务服务
#define SUB_GP_TASK_LOAD				240								//任务加载
#define SUB_GP_TASK_TAKE				241								//任务领取
#define SUB_GP_TASK_REWARD				242								//任务奖励
#define SUB_GP_TASK_GIVEUP				243								//任务放弃
#define SUB_GP_TASK_INFO				250								//任务信息
#define SUB_GP_TASK_LIST				251								//任务信息
#define SUB_GP_TASK_RESULT				252								//任务结果
#define SUB_GP_TASK_GIVEUP_RESULT		253								//放弃结果

//低保服务
#define SUB_GP_BASEENSURE_LOAD			260								//加载低保
#define SUB_GP_BASEENSURE_TAKE			261								//领取低保
#define SUB_GP_BASEENSURE_PARAMETER		262								//低保参数
#define SUB_GP_BASEENSURE_RESULT		263								//低保结果

//推广服务
#define SUB_GP_SPREAD_QUERY				280								//推广奖励
#define SUB_GP_SPREAD_INFO				281								//奖励参数

//兑换服务
#define SUB_GP_EXCHANGE_QUERY			320								//兑换参数
#define SUB_GP_EXCHANGE_PARAMETER		321								//兑换参数
#define SUB_GP_PURCHASE_MEMBER			322								//购买会员
#define SUB_GP_PURCHASE_RESULT			323								//购买结果
#define SUB_GP_EXCHANGE_SCORE_BYINGOT	324								//兑换游戏币
#define SUB_GP_EXCHANGE_SCORE_BYBEANS	325								//兑换游戏币
#define SUB_GP_EXCHANGE_RESULT			326								//兑换结果

//会员服务
#define SUB_GP_MEMBER_PARAMETER			340								//会员参数
#define SUB_GP_MEMBER_QUERY_INFO		341								//会员查询
#define SUB_GP_MEMBER_DAY_PRESENT		342								//会员送金
#define SUB_GP_MEMBER_DAY_GIFT			343								//会员礼包
#define SUB_GP_MEMBER_PARAMETER_RESULT	350								//参数结果
#define SUB_GP_MEMBER_QUERY_INFO_RESULT	351								//查询结果
#define SUB_GP_MEMBER_DAY_PRESENT_RESULT	352							//送金结果
#define SUB_GP_MEMBER_DAY_GIFT_RESULT	353								//礼包结果

//抽奖服务
#define SUB_GP_LOTTERY_CONFIG_REQ		360								//请求配置
#define SUB_GP_LOTTERY_CONFIG			361								//抽奖配置
#define SUB_GP_LOTTERY_USER_INFO		362								//抽奖信息
#define SUB_GP_LOTTERY_START			363								//开始抽奖
#define SUB_GP_LOTTERY_RESULT			364								//抽奖结果

//操作结果
#define SUB_GP_TASK_OPERATE_SUCCESS		500								//操作成功
#define SUB_GP_TASK_OPERATE_FAILURE		501								//操作失败

//////////////////////////////////////////////////////////////////////////////////

//查询签到
struct CMD_GP_CheckInQueryInfo
{
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码
};

//签到信息
struct CMD_GP_CheckInInfo
{
	WORD							wSeriesDate;						//连续日期(最后一天未签到)
	bool							bTodayChecked;						//签到标识
	WORD							wTodayDate;							//今天是第几天(从1开始)
	tagCheckInReward				CheckInReward[LEN_WEEK];				//奖励金币	
};

//执行签到
struct CMD_GP_CheckInDone
{
	DWORD							dwUserID;							//用户标识
	TCHAR							szPassword[LEN_PASSWORD];			//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//签到结果
struct CMD_GP_CheckInResult
{
	bool							bSuccessed;							//成功标识
	TCHAR							szNotifyContent[128];				//提示内容
};

//////////////////////////////////////////////////////////////////////////////////
//抽奖服务

//请求配置
struct CMD_GP_LotteryConfigReq
{
	DWORD							dwUserID;							//用户标识
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	DWORD							dwChannel;							//用户渠道
};

//抽奖配置
struct CMD_GP_LotteryConfig
{
	WORD							wLotteryCount;						//奖项个数
	tagLotteryItem					LotteryItem[MAX_LOTTERY];			//奖项内容
};

//抽奖信息
struct CMD_GP_LotteryUserInfo
{
	DWORD							dwUserID;							//用户标识
	BYTE							cbCount;							//今天抽奖次数
	BYTE							cbMaxCount;							//最大次数
};

//开始抽奖
struct CMD_GP_LotteryStart
{
	DWORD							dwUserID;							//用户标识
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	DWORD							dwChannel;							//用户渠道
};

//抽奖结果
struct CMD_GP_LotteryResult
{
	bool							bSuccess;							//是否成功
	BYTE							cbCount;							//今天抽奖次数
	BYTE							cbMaxCount;							//最大次数
	DWORD							dwUserID;							//用户标识
	tagLotteryItem					LotteryItem;						//中奖内容
	TCHAR							szDescribeString[128];				//描述消息
};

//////////////////////////////////////////////////////////////////////////////////
//游戏服务
struct CMD_GP_QueryUserGameData
{
	WORD							wKindID;							//游戏标识
	DWORD							dwUserID;							//用户标识
	TCHAR							szDynamicPass[LEN_MD5];				//用户密码	
};

//////////////////////////////////////////////////////////////////////////////////

//操作失败
struct CMD_GP_OperateFailure
{
	DWORD							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//操作成功
struct CMD_GP_OperateSuccess
{
	DWORD							lResultCode;						//操作代码
	TCHAR							szDescribeString[128];				//成功消息
};

//////////////////////////////////////////////////////////////////////////////////
//远程服务

#define MDM_GP_REMOTE_SERVICE		4									//远程服务

//查找服务
#define SUB_GP_C_SEARCH_DATABASE	100									//数据查找
#define SUB_GP_C_SEARCH_CORRESPOND	101									//协调查找
#define SUB_GP_C_SEARCH_ALLCORRESPOND	102								//协调查找

//查找服务
#define SUB_GP_S_SEARCH_DATABASE	200									//数据查找
#define SUB_GP_S_SEARCH_CORRESPOND	201									//协调查找
#define SUB_GP_S_SEARCH_ALLCORRESPOND	202								//协调查找

//////////////////////////////////////////////////////////////////////////////////

//协调查找
struct CMD_GP_C_SearchCorrespond
{
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
};

struct CMD_GP_C_SearchAllCorrespond
{
	DWORD							dwCount;
	DWORD							dwGameID[1];							//游戏标识
};

//协调查找
struct CMD_GP_S_SearchCorrespond
{
	WORD							wUserCount;							//用户数目
	tagUserRemoteInfo				UserRemoteInfo[16];					//用户信息
};

struct CMD_GP_S_SearchAllCorrespond
{
	DWORD							dwCount;
	tagUserRemoteInfo				UserRemoteInfo[1];					//用户信息				
};

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif