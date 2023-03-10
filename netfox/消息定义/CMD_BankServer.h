#ifndef CMD_BankServer_HEAD_FILE
#define CMD_BankServer_HEAD_FILE

#pragma pack(1)

//服务命令
#define MDM_GR_INSURE						3							//银行命令

//银行服务
#define SUB_GP_USER_SAVE_SCORE				400							//存款操作
#define SUB_GP_USER_TAKE_SCORE				401							//取款操作
#define SUB_GP_USER_TRANSFER_SCORE			402							//转账操作
#define SUB_GP_USER_INSURE_INFO				403							//银行资料
#define SUB_GP_QUERY_INSURE_INFO			404							//查询银行
#define SUB_GP_USER_INSURE_SUCCESS			405							//银行成功
#define SUB_GP_USER_INSURE_FAILURE			406							//银行失败
#define SUB_GP_QUERY_USER_INFO_REQUEST		407							//查询用户
#define SUB_GP_QUERY_USER_INFO_RESULT		408							//用户信息
#define SUB_GP_USER_LOGON_BANK				409							//登录银行
#define SUB_GP_USER_LOGON_BANK_RESULT		410							//登录银行结果
#define SUB_GP_QUERY_BANK_DETAIL			412							//查询详细
#define SUB_GP_BANK_DETAIL_RESULT			413							//详细结果
#define SUB_GP_USE_ITEM_SUCCESS				415                         //使用物品成功
#define SUB_GP_USE_ITEM_FAILURE				416                         //使用物品失败
#define SUB_GP_UPDATE_USER_ITEM				417                         //更新我的物品信息
#define SUB_GP_USE_ITEM						418                         //使用物品
#define SUB_GP_BINDWEIXI					421							//绑定微信
#define SUB_GP_QUERY_MONNEY					422							//查询货币
#define SUB_GP_QUERY_MONNEY_RESULT			423							//查询货币结果
#define SUB_GP_ENSURE_TRANSFER				424							//确认转账
#define SUB_GP_ENSURE_TRANSFER_RESULT		425							//确认转账结果
#define SUB_GR_QUERY_TRANSFER_CENSUS		432							//转账统计
#define SUB_GP_QUERY_TRANSFER_CENSUS_RESULT	433							//转账统计结果
#define SUB_GP_QUERY_CHARGE_RECORD			434							//查询充值记录
#define SUB_GP_QUERY_CHARGE_RECORD_RESULT	435							//查询充值记录结果

//操作结果
#define SUB_GP_OPERATE_SUCCESS				900							//操作成功
#define SUB_GP_OPERATE_FAILURE				901							//操作失败

//登录银行
struct CMD_GP_UserLogonBank
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//银行密码
};

struct CMD_GP_UserLogonResult
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//银行资料
struct CMD_GP_UserInsureInfo
{
	WORD							wRevenueTake;						//税收比例
	WORD							wRevenueTransfer;					//税收比例
	WORD							wServerID;							//房间标识
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	SCORE							lTransferPrerequisite;				//转账条件
	TCHAR							szLastLoginTime[64];				//上次登录
	BYTE							cbAccountsProtect;					//账号保护
	SCORE							lFreezeScore;						//冻结金币
};

//存入金币
struct CMD_GP_UserSaveScore
{
	DWORD							dwPlazaVersion;						//广场版本
	DWORD							dwUserID;							//用户 I D
	DWORD                           dwIP;
	SCORE							lSaveScore;							//存入金币
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//提取金币
struct CMD_GP_UserTakeScore
{
	DWORD							dwPlazaVersion;						//广场版本
	DWORD							dwUserID;							//用户 I D
	DWORD                           dwIP;
	SCORE							lTakeScore;							//提取金币
	TCHAR							szPassword[LEN_MD5];				//银行密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
};

//银行成功
struct CMD_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
	TCHAR							szDescribeString[128];				//描述消息
	TCHAR							szDateTime[64];						//操作时间
	TCHAR							szFreezeID[30];						//转账订单
};

//银行失败
struct CMD_GP_UserInsureFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//提取结果
struct CMD_GP_UserTakeResult
{
	DWORD							dwUserID;							//用户 I D
	SCORE							lUserScore;							//用户金币
	SCORE							lUserInsure;						//银行金币
};

//查询银行
struct CMD_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szPassword[LEN_MD5];				//银行密码
};

//查询明细
struct CMD_GP_QueryBankDetail
{
	DWORD							dwPlazaVersion;						//广场版本
	DWORD							dwUserID;							//用户 I D
	BYTE							cbTransferIn;						//转账类型 转入:1、转出:0
	BYTE							cbFreezeType;						//转账订单状态，默认0发起转账，1确认成功, 2撤回由客服操作,3查询全部
	TCHAR							szStartDate[64];					//开始日期
	TCHAR							szEndDate[64];						//结束日期
};

//明细结果
struct CMD_GP_BankDetailResult
{
	BYTE							cbState;							//详细状态
	BYTE                            cbTransferIn;						//转入转出
	TCHAR							szNickName[LEN_NICKNAME];			//目标昵称
	DWORD							dwGameID;							//目标标识
	SCORE							lSwapScore;							//转账金额
	TCHAR							szDateTime[64];						//转账日期
	BYTE							cbFreezeType;						//转账订单状态，默认0发起转账，1确认成功, 2撤回由客服操作
	SCORE							lFreezeScore;						//转账冻结金额，订单成功转到目标账户，撤回转回源账户
	TCHAR							szFreezeID[30];						//转账订单ID，系统转账的时候生成
};

//确认转账
struct CMD_GP_EnsureTransfer
{
	DWORD							dwUserID;							//用户 I D
	TCHAR							szFreezeID[30];						//转账订单号
};

//确认转账结果
struct CMD_GP_EnsureTransferResult
{
	LONG							lResultCode;						//操作代码
	TCHAR							szFreezeID[30];						//转账订单号
	TCHAR							szDescribeString[128];				//成功消息
};

//查询用户
struct CMD_GP_QueryUserInfoRequest
{
	DWORD							dwUserID;							//玩家标志
	BYTE                            cbByNickName;                       //昵称赠送
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
};

//转账金币
struct CMD_GP_UserTransferScore
{
	DWORD							dwPlazaVersion;						//广场版本
	DWORD							dwUserID;							//用户 I D
	DWORD                           dwIP;
	DWORD                           dwGameID;
	LONGLONG						lTransferScore;						//转账金币
	TCHAR							szPassword[LEN_MD5];				//银行密码
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	TCHAR                           szCdkey[LEN_MD5];
};

//转账用户
struct CMD_GP_UserTransferUserInfo
{
	DWORD							dwTargetGameID;						//目标用户
	TCHAR							szNickName[LEN_NICKNAME];			//目标用户
};

//查询货币
struct CMD_GP_QueryMonery
{
	DWORD							dwUserID;							//用户标志
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
};

//查询货币结果
struct CMD_GP_QueryMoneyResult
{
	DWORD						    dwResultCode;						//查询结果
	SCORE							lScore;								//身上金币
	DWORD							dwTotalCharge;						//累计充值
	SCORE							lRoomCards;							//房卡数量
};

//转账统计
struct CMD_GP_QueryTransferCensus
{
	DWORD							dwUserID;							//用户标志
	TCHAR							szLogonPass[LEN_MD5];				//登录密码
	TCHAR							szStartDate[64];					//开始日期
	TCHAR							szEndDate[64];						//结束日期
};

//转账统计结果
struct CMD_GP_QueryTransferCensusResult
{
	SCORE							llTransferInScore;					//转入金币
	SCORE							llTransferOutScore;					//转出金币
	SCORE							llTransferInFreezeScore;			//转入冻结
	SCORE							llTransferOutFreezeScore;			//转出冻结
};

//充值记录
struct CMD_GP_QueryChargeRecord
{
	DWORD							dwUserID;
	TCHAR							szDateTime[64];						//查询日期
};

//充值记录结果
struct CMD_GP_QueryChargeRecordResult
{
	TCHAR							szOrderID[50];						//充值订单
	DOUBLE							llCardPrice;						//充值金额
	SCORE							llCardGold;							//充值金币
	WORD							wOrderStatus;						//订单状态(0失败,非0成功)
	TCHAR							szOrderTime[64];					//查询日期
};

//操作失败
struct CMD_GP_OperateFailure
{
	LONG							lResultCode;						//错误代码
	TCHAR							szDescribeString[128];				//描述消息
};

//操作成功
struct CMD_GP_OperateSuccess
{
	LONG							lResultCode;						//操作代码
	TCHAR							szDescribeString[128];				//成功消息
};

#pragma pack()

#endif