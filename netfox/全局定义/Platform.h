#ifndef PLATFORM_HEAD_FILE
#define PLATFORM_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//包含文件

//定义文件
#include "Macro.h"
#include "Define.h"
#include "datastream.h"
//结构文件
#include "Struct.h"
#include "Packet.h"
#include "Property.h"

//模板文件
#include "Map.h"
#include "Queue.h"
#include "Array.h"
#include "Module.h"
#include "PacketAide.h"
#include "ServerRule.h"
#include "RightDefine.h"

//标准模板
#include <vector>
#include <map>

//////////////////////////////////////////////////////////////////////////////////
//缓存定义
const char szxRedisHostAddr[] = "127.0.0.1";
const int  iRedisHostPort = 6379;
const char szxRedisPassword[] = "the_xRedis(Asia888)*pw";

//缓存类别
const char szRedisUserInfo[] = "RedisUserInfo";							//玩家信息

//缓存类型
#define XREDIS_CACHE_TYPE_1 1
#define XREDIS_CACHE_TYPE_2 2

//////////////////////////////////////////////////////////////////////////////////
//消息定义
#define WM_PROCESS_CMD_LINE			(WM_USER+100)						//处理命令
#define WM_PROCESS_GAME_QUIT		(WM_USER+101)						//游戏退出
#define WM_PROCESS_CORRESPOND_RESET	(WM_USER+102)						//协调重置

//////////////////////////////////////////////////////////////////////////////////
//大厅版本
#define VERSION_PLAZA1				PROCESS_VERSION(8,0,3)				//大厅版本

//程序版本
#define VERSION_FRAME				PROCESS_VERSION(6,0,3)				//框架版本
#define VERSION_PLAZA				PROCESS_VERSION(15,0,3)				//大厅版本
#define VERSION_MOBILE_ANDROID		PROCESS_VERSION(6,0,3)				//手机版本
#define VERSION_MOBILE_IOS			PROCESS_VERSION(6,0,3)				//手机版本

//版本定义
#define VERSION_EFFICACY			0									//效验版本
#define VERSION_FRAME_SDK			INTERFACE_VERSION(6,3)				//框架版本

//////////////////////////////////////////////////////////////////////////////////
//发布版本

enum LOG_Level
{
	LOG_DEBUG,  //所有都输出
	LOG_WARN,   //警告输出
	LOG_ERROR,  //错误输出
	LOG_OFF     //关闭
};

enum
{
	MatchMode_People = 0,
	MatchMode_Time = 1,
	MatchMode_Ladder = 2,
};

enum //比赛状态
{
	eMatchStatusWating = 0,		//报名等待中，未开始
	eMatchStatusPlaying,		//正在进行中
	eMatchStatusFinish			//结束
};

#ifndef _DEBUG

//平台常量
const TCHAR szProduct[] = TEXT("华商在线游戏中心");							//产品名字
const TCHAR szPlazaClass[] = TEXT("WHZJSYKTQPGamePlaza");						//广场类名
const TCHAR szProductKey[] = TEXT("WHZJSYKTQPGame");					        //产品主键

//地址定义
const TCHAR szCookieUrl[] = TEXT("http://game188.v.sdo.com");						//记录地址
const TCHAR szLogonServer[] =/*TEXT("http://www.game528.com")*/TEXT("14.17.94.62");					//登录地址
const TCHAR szPlatformLink[] = TEXT("http://game188.v.sdo.com");				//平台网站

#else

//////////////////////////////////////////////////////////////////////////////////
//内测版本

//平台常量
const TCHAR szProduct[] = TEXT("华商在线游戏中心");							//产品名字
const TCHAR szPlazaClass[] = TEXT("WHQPGamePlaza");						//广场类名
const TCHAR szProductKey[] = TEXT("WHQPGamePlatform");					//产品主键

//地址定义
const TCHAR szCookieUrl[] = TEXT("http://game188.v.sdo.com");						//记录地址
const TCHAR szLogonServer[] = TEXT("121.12.172.244");				  		//登录地址
const TCHAR szPlatformLink[] = TEXT("http://game188.v.sdo.com");				//平台网站

#endif

//////////////////////////////////////////////////////////////////////////////////

//数据库名

const TCHAR szAccountsDB[] = TEXT("QPAccountsDB");						//用户数据库
const TCHAR szQPControlDB[] = TEXT("QPControlDB");						//控制数据库
const TCHAR szTreasureDB[] = TEXT("QPTreasureDB");						//财富数据库
const TCHAR szTaskStoreDB[] = TEXT("QPTaskStoreDB");					//任务数据库
const TCHAR szPlatformDB[] = TEXT("QPPlatformDB");						//平台数据库

//////////////////////////////////////////////////////////////////////////////////

//授权信息
const TCHAR szCompilation[] = TEXT("A4E910E9-9D12-40DF-8ABF-9116A8630BFA");

//////////////////////////////////////////////////////////////////////////////////


class CGetGoldGameFace
{
public:

	//加载头像
	WORD GetGoldLevelFaceID(LONGLONG lScore, WORD wGen, LPCTSTR pszGameLevelFileName)
	{

		//等级积分
		LONG lLevelScore[20];
		ZeroMemory(&lLevelScore, sizeof(lLevelScore));

		//加载积分

		for (int i = 0; i < 20; i++)
		{
			CString str;
			str.Format(TEXT("LevelItem%d"), i + 1);

			lLevelScore[i] = GetPrivateProfileInt(TEXT("LevelDescribe"), str, 0, pszGameLevelFileName);

		}

		//获取头像

		for (WORD i = 0; i < 20; i++)
		{
			if (lScore >= lLevelScore[20 - i - 1])
			{

				if (wGen)
				{

					return 20 - i - 1;
				}
				else
				{

					return 20 - i - 1 + 20;
				}


			}
		}

		return 0;

	}
};
#endif