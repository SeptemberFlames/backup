#ifndef PACKET_HEAD_FILE
#define PACKET_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////

//端口定义

#define MAX_CONTENT					2048									//并发容量
#define PORT_AUTO_SELECT			INVALID_WORD						//自动端口
#define MAX_LONG_CONTENT					8192									//并发容量
#ifdef _DEBUG
#define PORT_CHAT					8630								//聊天好友端口
#define PORT_CON					8810								//登陆端口
#define PORT_TASK					8820								//任务端口
#define PORT_PHP					33199								//登陆端口，供后台使用
#define PORT_UNIFIED_ORDER_MANAGER	33200								//登陆端口，UnifiedOrder充值下单管理版
#define PORT_UNIFIED_ORDER			33210								//登陆端口，UnifiedOrder充值下单用户版
#define PORT_WEB					33211								//登陆端口，供Web服务器用
#define PORT_LOGONS					8300								//登陆端口
#define PORT_LOGON					8500								//登陆端口
#define PORT_UPDATE					8920								//登陆端口
#define PORT_LOGON_IOS					62001								//登陆端口
#define PORT_LOGON_AZ					62002								//登陆端口
#define PORT_CENTER					8510								//协调端口
#define PORT_MANAGER				8420								//管理端口
#define PORT_AnLOGONS					8900								//登陆辅助端口
#define PORT_AnLOGON					9000								//登陆辅助端口
#define PORT_TRANSFER                8700                                 //中转服务器
#define PORT_GLOBLA                  9700                                 //调度服务器
#else
#define PORT_CHAT					8630								//聊天好友端口
#define PORT_CON					8810								//登陆端口
#define PORT_TASK					8820								//任务端口
#define PORT_PHP					33199								//登陆端口，供后台使用
#define PORT_UNIFIED_ORDER_MANAGER	33200								//登陆端口，UnifiedOrder充值下单管理版
#define PORT_UNIFIED_ORDER			33210								//登陆端口，UnifiedOrder充值下单用户版
#define PORT_WEB					33211								//登陆端口，供Web服务器用
#define PORT_LOGONS					8300								//登陆端口
#define PORT_LOGON					8400								//登陆端口
#define PORT_UPDATE					8920								//登陆端口
#define PORT_GLOBLA                  9700                                 //调度服务器
#define PORT_HORN                   63044                                 //全局喇叭端口
#define PORT_LOGON_IOS					62001								//登陆端口
#define PORT_LOGON_AZ					62002								//登陆端口
#define PORT_CENTER					8510								//协调端口
#define PORT_MANAGER				8420								//管理端口
#define PORT_AnLOGONS					8900								//登陆辅助端口
#define PORT_AnLOGON					9000								//登陆辅助端口
#endif
//////////////////////////////////////////////////////////////////////////////////
//网络定义

//数据类型
#define DK_MAPPED					0x01								//映射类型
#define DK_ENCRYPT					0x02								//加密类型
#define DK_COMPRESS					0x04								//压缩类型

//长度定义
#define SOCKET_TCP_BUFFER			16384								//网络缓冲
#define SOCKET_TCP_PACKET			(SOCKET_TCP_BUFFER-sizeof(TCP_Head))//网络缓冲

//长度定义
#define SOCKET_UDP_BUFFER			16384								//网络缓冲
#define SOCKET_UDP_PACKET			(SOCKET_UDP_BUFFER-sizeof(UDP_Head))//网络缓冲

#define SOCKET_ZERO "000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
#define SOCKET_ONE  "111111111111111111111111111111111111111111111111111111111111111111111111111111111111"

//////////////////////////////////////////////////////////////////////////////////
//结构定义

// 包标示结构
struct tagSuperInfo
{
	ULONGLONG	ulCmdNo;
	TCHAR	szMachineID[LEN_MACHINE_ID+8];	
};

#define BEGINFLAG 0x55555555
#define ENDFLAG	  0x66666666

#define TCP_HEAD_CHECK_CODE	69696969								//TCP_Head.nCheckCode校验码

// typedef struct _NetMessageHead
// {
// 	DWORD						uMessageBeginFlag;					//包头标志
// 	DWORD						dwCmdFlag;							//命令标识
// 	DWORD						dwClientAddr;						//连接地址
// 	int                         mRandCnt;                           //随机种子
// 	DWORD                       extend;
// 	char                        md5key[LEN_MD5];                             //md5key             
// 	DWORD						dwPort;								//连接端口
// 	WORD						dwPacketSize;						//数据大小
// 	WORD						dwEncodeSize;						//数据大小
// 	DWORD						uMessageEndFlag;					//包尾标志
// 	LONGLONG					ulCmdNo;		
// }NetMessageHead;
// 
// 
// //网络内核
// typedef struct
// {
// 	WORD                          wEnPacketSize;
// 	char                          dwClientPort[84];
// 	LONGLONG					   ulCmdNo;
// 	NetMessageHead					netHead;
// 	BYTE							cbDataKind;							//数据类型
// 	BYTE							cbCheckCode;						//效验字段
// 	WORD							wPacketSize;						//数据大小
// }TCP_Info;
//网络命令
struct TCP_Command
{
	WORD							wMainCmdID;							//主命令码
	WORD							wSubCmdID;							//子命令码
};

//网络包头
struct TCP_Head
{
	//TCP_Info						TCPInfo;							//基础结构
	//TCP_Command						CommandInfo;						//命令信息
	//WxrAdd20170724新包头
	DWORD							dwDataLen;							//数据包长度，包头+包体
	WORD							wMainCmdID;							//消息主ID
	WORD							wSubCmdID;							//消息子ID
	DWORD							dwMsgIndex;							//消息编号
	DWORD							dwDataType;							//数据类型(加密、映射、压缩)
	DWORD							dwUserID;							//用户UserID
	LONGLONG						llUserTempID;						//临时ID(0-999内部，1000-999999客户端直连，百万以外客户端中转)
	DWORD							dwUnusedA;							//备用A
	DWORD							dwUnusedB;							//备用B
	int								nCheckCode;							//校验码(暂定69696969)
};

//网络检查
struct TCP_Validate
{
	TCHAR							szValidateKey[64];					//验证字符
};

//网络缓冲
struct TCP_Buffer
{
	TCP_Head						Head;								//数据包头
	BYTE							cbBuffer[SOCKET_TCP_PACKET];		//数据缓冲
};

//////////////////////////////////////////////////////////////////////////////////

//网络内核
struct UDP_Info
{
	BYTE							cbDataKind;							//数据类型
	BYTE							cbCheckCode;						//效验字段
	WORD							wPacketSize;						//数据大小
	WORD							wPacketIndex;						//数据序列
	WORD							wConnectIndex;						//连接索引
	//DWORD							dwClientAddr;						//连接地址
	//tagSuperInfo					superInfo;
};


//网络命令
struct UDP_Command
{
	WORD							wMainCmdID;							//主命令码
	WORD							wSubCmdID;							//子命令码
};


//网络包头
struct UDP_Head
{
	UDP_Info						UDPInfo;							//基础结构
	UDP_Command						CommandInfo;						//命令信息
};

//网络缓冲
struct UDP_Buffer
{
	UDP_Head						Head;								//数据包头
	BYTE							cbBuffer[SOCKET_UDP_PACKET];		//数据缓冲
};

//////////////////////////////////////////////////////////////////////////////////
//服务端发送检测命令0_1消息，客户端多连接模块将返回0_2空包

//内核命令
#define MDM_KN_COMMAND				0									//内核命令
#define SUB_KN_DETECT_SOCKET		1									//检测命令
#define SUB_KN_VALIDATE_SOCKET		2									//验证命令
#define MDM_KN_COMMAND_LAJI			65534								//内核命令
#define MDM_KN_COMMAND_LAJI2		65533								//内核命令
//////////////////////////////////////////////////////////////////////////////////

//传输数据

#define IPC_VER						1									//版本标识
#define IPC_PACKET					(10240-sizeof(IPC_Head))			//最大包长
#define IPC_BUFFER					(sizeof(IPC_Head)+IPC_PACKET)		//缓冲长度

//数据包头
struct IPC_Head
{
	WORD							wVersion;							//版本标识
	WORD							wPacketSize;						//数据大小
	WORD							wMainCmdID;							//主命令码
	WORD							wSubCmdID;							//子命令码
};

//IPC 包结构
struct IPC_Buffer
{
	IPC_Head						Head;								//数据包头
	BYTE							cbBuffer[IPC_PACKET];				//数据缓冲
};

//////////////////////////////////////////////////////////////////////////////////
//数据定义

//加密密钥
const DWORD g_dwPacketKey=0xA55AA55A;

//发送映射
const BYTE g_SendByteMap[256]=
{
	0x70,0x2F,0x40,0x5F,0x44,0x8E,0x6E,0x45,0x7E,0xAB,0x2C,0x1F,0xB4,0xAC,0x9D,0x91,
	0x0D,0x36,0x9B,0x0B,0xD4,0xC4,0x39,0x74,0xBF,0x23,0x16,0x14,0x06,0xEB,0x04,0x3E,
	0x12,0x5C,0x8B,0xBC,0x61,0x63,0xF6,0xA5,0xE1,0x65,0xD8,0xF5,0x5A,0x07,0xF0,0x13,
	0xF2,0x20,0x6B,0x4A,0x24,0x59,0x89,0x64,0xD7,0x42,0x6A,0x5E,0x3D,0x0A,0x77,0xE0,
	0x80,0x27,0xB8,0xC5,0x8C,0x0E,0xFA,0x8A,0xD5,0x29,0x56,0x57,0x6C,0x53,0x67,0x41,
	0xE8,0x00,0x1A,0xCE,0x86,0x83,0xB0,0x22,0x28,0x4D,0x3F,0x26,0x46,0x4F,0x6F,0x2B,
	0x72,0x3A,0xF1,0x8D,0x97,0x95,0x49,0x84,0xE5,0xE3,0x79,0x8F,0x51,0x10,0xA8,0x82,
	0xC6,0xDD,0xFF,0xFC,0xE4,0xCF,0xB3,0x09,0x5D,0xEA,0x9C,0x34,0xF9,0x17,0x9F,0xDA,
	0x87,0xF8,0x15,0x05,0x3C,0xD3,0xA4,0x85,0x2E,0xFB,0xEE,0x47,0x3B,0xEF,0x37,0x7F,
	0x93,0xAF,0x69,0x0C,0x71,0x31,0xDE,0x21,0x75,0xA0,0xAA,0xBA,0x7C,0x38,0x02,0xB7,
	0x81,0x01,0xFD,0xE7,0x1D,0xCC,0xCD,0xBD,0x1B,0x7A,0x2A,0xAD,0x66,0xBE,0x55,0x33,
	0x03,0xDB,0x88,0xB2,0x1E,0x4E,0xB9,0xE6,0xC2,0xF7,0xCB,0x7D,0xC9,0x62,0xC3,0xA6,
	0xDC,0xA7,0x50,0xB5,0x4B,0x94,0xC0,0x92,0x4C,0x11,0x5B,0x78,0xD9,0xB1,0xED,0x19,
	0xE9,0xA1,0x1C,0xB6,0x32,0x99,0xA3,0x76,0x9E,0x7B,0x6D,0x9A,0x30,0xD6,0xA9,0x25,
	0xC7,0xAE,0x96,0x35,0xD0,0xBB,0xD2,0xC8,0xA2,0x08,0xF3,0xD1,0x73,0xF4,0x48,0x2D,
	0x90,0xCA,0xE2,0x58,0xC1,0x18,0x52,0xFE,0xDF,0x68,0x98,0x54,0xEC,0x60,0x43,0x0F
};

//接收映射
const BYTE g_RecvByteMap[256]=
{
	0x51,0xA1,0x9E,0xB0,0x1E,0x83,0x1C,0x2D,0xE9,0x77,0x3D,0x13,0x93,0x10,0x45,0xFF,
	0x6D,0xC9,0x20,0x2F,0x1B,0x82,0x1A,0x7D,0xF5,0xCF,0x52,0xA8,0xD2,0xA4,0xB4,0x0B,
	0x31,0x97,0x57,0x19,0x34,0xDF,0x5B,0x41,0x58,0x49,0xAA,0x5F,0x0A,0xEF,0x88,0x01,
	0xDC,0x95,0xD4,0xAF,0x7B,0xE3,0x11,0x8E,0x9D,0x16,0x61,0x8C,0x84,0x3C,0x1F,0x5A,
	0x02,0x4F,0x39,0xFE,0x04,0x07,0x5C,0x8B,0xEE,0x66,0x33,0xC4,0xC8,0x59,0xB5,0x5D,
	0xC2,0x6C,0xF6,0x4D,0xFB,0xAE,0x4A,0x4B,0xF3,0x35,0x2C,0xCA,0x21,0x78,0x3B,0x03,
	0xFD,0x24,0xBD,0x25,0x37,0x29,0xAC,0x4E,0xF9,0x92,0x3A,0x32,0x4C,0xDA,0x06,0x5E,
	0x00,0x94,0x60,0xEC,0x17,0x98,0xD7,0x3E,0xCB,0x6A,0xA9,0xD9,0x9C,0xBB,0x08,0x8F,
	0x40,0xA0,0x6F,0x55,0x67,0x87,0x54,0x80,0xB2,0x36,0x47,0x22,0x44,0x63,0x05,0x6B,
	0xF0,0x0F,0xC7,0x90,0xC5,0x65,0xE2,0x64,0xFA,0xD5,0xDB,0x12,0x7A,0x0E,0xD8,0x7E,
	0x99,0xD1,0xE8,0xD6,0x86,0x27,0xBF,0xC1,0x6E,0xDE,0x9A,0x09,0x0D,0xAB,0xE1,0x91,
	0x56,0xCD,0xB3,0x76,0x0C,0xC3,0xD3,0x9F,0x42,0xB6,0x9B,0xE5,0x23,0xA7,0xAD,0x18,
	0xC6,0xF4,0xB8,0xBE,0x15,0x43,0x70,0xE0,0xE7,0xBC,0xF1,0xBA,0xA5,0xA6,0x53,0x75,
	0xE4,0xEB,0xE6,0x85,0x14,0x48,0xDD,0x38,0x2A,0xCC,0x7F,0xB1,0xC0,0x71,0x96,0xF8,
	0x3F,0x28,0xF2,0x69,0x74,0x68,0xB7,0xA3,0x50,0xD0,0x79,0x1D,0xFC,0xCE,0x8A,0x8D,
	0x2E,0x62,0x30,0xEA,0xED,0x2B,0x26,0xB9,0x81,0x7C,0x46,0x89,0x73,0xA2,0xF7,0x72
};

//////////////////////////////////////////////////////////////////////////////////
//新中转增加

//更新IP
enum enumXzggUpdateIpType
{
	UPDATE_IP_NoDef = 0,
	UPDATE_IP_Cli_Request_Ip,												//客户端请求IP，可以把不想使用的写到strJsonIp中

	UPDATE_IP_Svr_New_Ip = 100,												//中转返回可用IP
	UPDATE_IP_Svr_Stop_Ip,													//中转返回将要停止使用的IP
};

//单条IP信息
struct XZGG_OneIpInfo
{
	char							strIp[20];								//ip
	WORD							wPort;									//端口
};

//申请类型：心跳、请求关闭等信息
enum enumXzggRequestType
{
	REQUEST_TYPE_NoDef = 0,
	REQUEST_TYPE_HeartPort,													//其中一个端口心跳
	REQUEST_TYPE_ClosePort,													//请求关闭这个端口和逻辑服务器的链接
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//中转命令
#define MDM_XZGG_COMMAND_SOCKET			61000								//中转主命令

#define SUB_CLI_XZGG_FIRST_REGISTER		4201								//首次注册
#define SUB_SVR_XZGG_REGISTER_RESPONE	4202								//首次注册返回
#define SUB_SVR_XZGG_SOCKET_CLOSE_LATER	4203								//网络即将关闭
#define SUB_CLI_XZGG_ACCEPT_MSG			4204								//客户端请求是否接收消息
#define SUB_SVR_XZGG_ACCEPT_MSG			4205								//服务端返回是否接收消息
#define SUB_CLI_XZGG_REQUEST_TYPE		4207								//申请类型：心跳、请求关闭等信息
#define SUB_SVR_XZGG_REQUEST_TYPE		4208								//服务端返回申请类型：心跳、请求关闭等信息
#define SUB_CLI_XZGG_UPDATE_IP			4211								//更新IP：客户端请求新的IP
#define SUB_SVR_XZGG_UPDATE_IP			4212								//更新IP：服务端返回新的IP
#define SUB_CLI_XZGG_TEST_SPEED			4213								//客户端测试网速
#define SUB_SVR_XZGG_TEST_SPEED			4214								//服务端返回测试网速
#define SUB_XZGG_ACCOUNTS_INFO			4251								//服务器通知中转玩家帐号属性
#define SUB_XZGG_SIMPLE_TEST			4252								//简单测试消息，原样返回
#define SUB_XZGG_CLIETN_IP_INFO			4253								//中转告诉qzh逻辑服务器这个客户端的ip地址


//4201首次注册
struct XZGG_Cli_FirstRegister
{
	int								nPort;								//客户端要链接的端口
	char							strDevNum[100];						//客户端设备号，保证不重复
	char							strCheckCode[128];					//约定的安全验证码
};

//4202首次注册返回
struct XZGG_Svr_RegisterRespone
{
	int								nErrorID;							//=1成功，其他错误
	char							strError[128];						//错误描述
	char							strDevNum[100];						//设备号
	int								nPort;								//返回客户端要链接的端口
	int								nPostServerID;						//后置Gt的ServerID，同组链接该值一样
};

//SCmdID=4203,你的网络将被关闭
struct XZGG_Svr_SocketCloseLater
{
	int								iWhy;								//EXGGroupErrID
	char							strErr[128];						//关闭原因描述
	DWORD							dwPort;								//端口
};

//4204客户端请求是否接收消息
struct XZGG_Cli_AcceptMsg
{
	bool							bAcceptMsg;						//是否接收消息
};

//4205服务端返回拒绝接收消息
struct XZGG_Svr_AcceptMsg
{
	int								nErrorID;						//1成功；大于1失败
	char							strErrorDesc[64];				//错误描述
	bool							bAcceptMsg;						//是否接收消息
};

//4207申请类型：心跳、请求关闭等信息
struct XZGG_Cli_RequestType
{
	enumXzggRequestType				enType;							//申请类型。REQUEST_TYPE_HeartPort心跳，REQUEST_TYPE_ClosePort申请关闭与wPort逻辑服务器链接
	DWORD							dwPort;							//端口
};

//4208服务端返回申请类型：心跳、请求关闭等信息
struct XZGG_Svr_RequestType
{
	enumXzggRequestType				enType;							//申请类型。REQUEST_TYPE_HeartPort心跳，REQUEST_TYPE_ClosePort申请关闭与wPort逻辑服务器链接
	DWORD							dwPort;							//端口
};



//4211更新IP：客户端请求新的IP
struct XZGG_Cli_UpdateIp
{
	enumXzggUpdateIpType			enType;								//请求类型。UPDATE_IP_Cli_Request_Ip客户端请求IP，可以把不想使用的写到strJsonIp中
	char							strJsonIp[256];						//必须为json格式，方便以后扩充
};

//4212更新IP：服务端返回新的IP
struct XZGG_Svr_UpdateIp
{	
	enumXzggUpdateIpType			enType;								//推送类型。UPDATE_IP_Svr_New_Ip中转返回可用IP；UPDATE_IP_Svr_Stop_Ip中转返回将要停止使用的IP
	int								nIpCount;							//推送数目
	XZGG_OneIpInfo					stuIpInfo[50];						//ip地址列表
};

//4213客户端测试网速
struct XZGG_Cli_TestSpeed
{
	char							strTestData[128];					//测试数据，服务器会原样返回
};

//4214服务端返回测试网速
struct XZGG_Svr_TestSpeed
{
	char							strTestData[128];					//测试数据，服务器会原样返回
};

//4251服务器通知中转玩家帐号属性
struct XZGG_Svr_AccountsInfo
{
	char							czAccounts[64];						//帐号
	int								nVipLevel;							//VIP等级
};

//4252简单测试消息
struct XZGG_SimpleTestMsg
{
	int								nDataLen;							//数据长度
	char							chTestData[1024];					//数据内容
};

//4253中转告诉qzh逻辑服务器这个客户端的ip地址
struct XZGG_Cli_ClientIpInfo
{
	char   ClientIp[20];
};

enum enumXzggRegisterError
{
	EGGERRID_NoDef = 0,
	EGGERRID_XXX_Ok,                        //成功
	EGGERRID_PostGtConQzhSsFail,            //PostGt连接逻辑服务器失败 
	EGGERRID_PostGtQzhSsConDis,             //PostGt和QzhSs服务器的连接被断开
	EGGERRID_DelXUser_InPostGt,             //PostGt上完成了用户的删除 
	EGGERRID_NoXUser_InPostGt,              //PostGt上没有这个用户对象，应该有的
	EGGERRID_CcConCtMax_InPostGt,           //已经最大链接数了,客户端现在的链接到PreGt的数目已经最大了
	EGGERRID_GmStop_ThisPreGt,              //Cs上,Gm停止了这个服务器的使用


	EGGERRID_QUERY_TIMEOUT			= 1001,
	EGGERRID_QUERY_NO_POSTGT,				//没有可以使用的PostGt
	EGGERRID_QUERY_NO_POSTGT_B,             //cs分配PostGt的时候，发现没有我可以使用的，可能提供给这个端口的逻辑服务器没有 

	EGGERRID_QUERY_SENDMSG_TOCS_FAILA,      //发送到cs询问消息失败(AskCfLookupPostGt)

	//客户端的参数错误
	EGGERRID_QUERY_DATA_FORMAT_ERRA =1100,  //数据格式错误
	EGGERRID_QUERY_PARAM_DEVNUM_ERRA,       //DevNum参数格式错误，为为空
	EGGERRID_QUERY_CS_NOINIT_OK,            //Cs服务器还没有启动完成


	//下面的错误，不应该发生，内存各种系统错误
	EGGERRID_QUERY_NOFACE			= 1200,	//没有这个接口
	EGGERRID_QUERY_ALLOCMEM_FAIL_A,			//内存分配失败
	EGGERRID_QUERY_ALLOCMEM_FAIL_B,         //内存分配失败(TCfUserPostGt)
	EGGERRID_QUERY_BUF_LITTLE_A,			//内存太小，拷贝不下
	EGGERRID_QUERY_LOGICERR_NODEFA,         //默认的一些错误，内存拷贝失败，等等


	EGGERRID_CCCON_DEVNUM_FORMATERR	= 3001,	//设备号为空，或者长度错误
	EGGERRID_CCCON_HAVE_CONNECT,			//发现已经有老的连接，会引起关闭老的连接

};
//////////////////////////////////////////////////////////////////////////////////////
#pragma pack()

#endif