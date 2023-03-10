#ifndef CMD_MallSERVER_HEAD_FILE
#define CMD_MallSERVER_HEAD_FILE

#pragma pack(1)

//////////////////////////////////充值服务器命令////////////////////////////////////

//充值命令
#define  MDM_CHARGE_COMMAND					888							//充值请求主命令

//消息命令
#define SUB_GP_CHARGE_SHOPINFOLIST			1							//充值请求商品信息
#define SUB_GP_CHARGE_SHOPINFOLISTRESULT	2							//请求商城数据结果

#define SUB_GP_CHARGE_QUREYUSER				3							//充值查询用户信息
#define SUB_GP_CHARGE_QUREYUSERRESULT		4							//用户充值查询结果

#define SUB_GP_CHARGE_UNIFIEDORDER			5							//用户统一下单支付
#define SUB_GP_CHARGE_UNIFIEDORDERRESULT	6							//UnifiedOrder统一下单支付，返回

#define SUB_GP_CHARGE_CASHORDER				7							//UnifiedOrder兑现订单
#define SUB_GP_CHARGE_CASHORDERRESULT		8							//UnifiedOrder兑现订单，返回


//商品结构
struct tagShopInfo
{
	TCHAR							App_pro_id[32];						//苹果订单id
	DWORD							Beans_num;							//所获得的金币数量
	DWORD							Beans_id;							//商品的唯一id
	TCHAR							Beans_name[64];						//商品名称
	DWORD							Beans_price;						//商品价格
	TCHAR							Pay_Type[10];						//支付类型 1 ios 2 支付宝 3 微信 4 爱贝
	DWORD							IapID;								//爱贝id
};

//金币数据列表请求
struct  CMD_GP_ChargeShopInfoList
{
	DWORD							userid;								//用户userID
	DWORD							platfromType;						//平台标识 1.IOS 2.Android 3.PC
};

//金币数据列表请求结果
struct CMD_GP_ChargeShopInfoListResult
{
	WORD							listCount;							//金币列表数量
	tagShopInfo						happyBeansList[16];					//金币列表信息
};

//充值查询
struct CMD_GP_ChargeQueryUser
{
	DWORD							dwGameID;							//用户标志
	DWORD							dwPlatfrom;							//用户平台
};

//充值查询
struct CMD_GP_ChargeQueryUserResult
{
	DWORD							dwResult;							//返回结果
	TCHAR							szAccounts[LEN_ACCOUNTS];			//用户账号
	TCHAR							szRegisterDate[32];					//注册时间
	TCHAR							szErrorDescribe[128];				//描述信息			
};

//用户申请订单号Ex
struct CMD_GP_UserUnifiedOrder
{
	TCHAR       szGameID[32];			// GameID或账号
	bool        bIsGameID;				// true是GameID，false就是账号
	DWORD		dwPayType;				// 支付类型数组下标, [100--1000都当作有效，其他无效]这个不再检测
	DWORD		dwGoodsId;				// 商品数组下标。暂时(9--25)
	TCHAR		szIP[16];				// 客户单地址 127.0.0.1/255.255.255.255
	double		dScore;					// 商品数组价格，暂时(6,--- 2000)
	DWORD		dwChannel;				// 用户渠道
	TCHAR       szOrderKey[8];			// 订单关键字
};

//统一下单结果
struct CMD_GP_UserUnifiedOrderResult
{
	DWORD							dwReturnVal;      					//返回值：0正确，1GameID不存在，2商品ID不存在，1000支付类型错误，1001商品下标错误，1002商品价格错误
	TCHAR							szReturnErr[256]; 					//错误文本, 正确的话填$OrderID
	double							dScore;								//金币
	DWORD							dwPayType;							//支付类型数组下标, [100--1000都当作有效，其他无效]这个不再检测
	DWORD							dwGoodsId;							//商品数组下标。暂时(9--25)
	DWORD							dwGameID;							//玩家GameID
	double							dReturnPrice;						//返回价格
};

//兑现订单
struct CMD_GP_UserCashOrder
{
	TCHAR		szOrderName[128];		//订单编号
	double		dScore;					//实际支付金额
};

//兑现订单结果
struct CMD_GP_UserCashOrderResult
{
	TCHAR							szOrderName[128];					//订单编号
	double							dScore;								//实际支付金额
	TCHAR							szRetText[128];						//返回错误原因
	DWORD							dwResult;							//0执行成功，非0失败
	DWORD							dwGameID;							//返回玩家GameID
	TCHAR							szAccounts[32];						//返回玩家账号
	DWORD							dwCardGold;							//返回金币
};


#pragma pack()

#endif