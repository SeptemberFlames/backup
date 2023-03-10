#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//请求命令
#define DBR_GP_SAVE_USER_DATA			1								//同步数据

//检查长度
struct DBR_GR_CheckReidsLen
{

};

//同步数据
struct DBR_GR_SaveUserData
{
	DWORD							dwDataBase;							//数据库名
	DWORD							dwType;								//执行类型
	TCHAR							szSql[2048];						//执行语句
};

//加载机器
struct DBR_GR_AndriodUserInfo
{
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	WORD							wFaceID;							//头像标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	BYTE							cbGender;							//用户性别
	TCHAR							szFaceUrl[256];						//网络头像
	DWORD							dwIsYouke;							//是否游客
};

#endif