#ifndef STRUCT_HEAD_FILE
#define STRUCT_HEAD_FILE

#include "datastream.h"

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////

//游戏类型
struct tagGameType
{
	WORD							wJoinID;							//挂接索引
	WORD							wSortID;							//排序索引
	WORD							wTypeID;							//类型索引
	TCHAR							szTypeName[LEN_TYPE];				//种类名字
};

//游戏种类
struct tagGameKind
{
	WORD							wTypeID;							//类型索引
	WORD							wJoinID;							//挂接索引
	WORD							wSortID;							//排序索引
	WORD							wKindID;							//类型索引
	WORD							wGameID;							//模块索引
	DWORD							dwOnLineCount;						//在线人数
	WORD                            wPlazaType;							//模块索引
	WORD                            wDiff;
	DWORD							dwFullCount;						//满员人数
	TCHAR							szKindName[LEN_KIND];				//游戏名字
	TCHAR							szProcessName[LEN_PROCESS];			//进程名字

	TCHAR                           szMBDownloadUrl[LEN_DWELLING_PLACE];//更新包大小
	TCHAR                           szGameVerison[LEN_MD5];             //MD5码
};

//游戏节点
struct tagGameNode
{
	WORD							wKindID;							//名称索引
	WORD							wJoinID;							//挂接索引
	WORD							wSortID;							//排序索引
	WORD							wNodeID;							//节点索引
	TCHAR							szNodeName[LEN_NODE];				//节点名称
};

//定制类型
struct tagGamePage
{
	WORD							wPageID;							//页面索引
	WORD							wKindID;							//名称索引
	WORD							wNodeID;							//节点索引
	WORD							wSortID;							//排序索引
	WORD							wOperateType;						//控制类型
	TCHAR							szDisplayName[LEN_PAGE];			//显示名称
};

//游戏房间
struct tagGameServer
{
	WORD							wKindID;							//名称索引
	WORD							wNodeID;							//节点索引
	WORD							wSortID;							//排序索引
	WORD							wServerID;							//房间索引
	WORD							wServerPort;						//房间端口
	WORD							wServerType;						//房间类型
	DWORD							dwOnLineCount;						//在线人数
	DWORD							dwFullCount;						//满员人数
	TCHAR							szServerAddr[32];					//房间名称
	TCHAR							szServerName[LEN_SERVER];			//房间名称
	TCHAR							szServerNameShow[LEN_SERVER];		//房间名称
	LONGLONG						lMaxEnterScore;						//最大进入分数
	LONGLONG						lMinEnterScore;						//最小进入分数
};

//视频配置
struct tagAVServerOption
{
	WORD							wAVServerPort;						//视频端口
	DWORD							dwAVServerAddr;						//视频地址
};

//////////////////////////////////////////////////////////////////////////////////
//用户信息

//桌子状态
struct tagTableStatus
{
	BYTE							cbTableLock;						//锁定标志
	BYTE							cbPlayStatus;						//游戏标志
};

//用户状态
struct tagUserStatus
{
	WORD							wTableID;							//桌子索引
	WORD							wChairID;							//椅子位置
	BYTE							cbUserStatus;						//用户状态
};

//用户属性
struct tagUserAttrib
{
	BYTE							cbCompanion;						//用户关系
};

//用户积分
struct tagUserScore
{
	//积分信息
	SCORE							lScore;								//用户分数
	SCORE							lInsure;							//用户银行

	//输赢信息
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数
};

//用户积分
struct tagMobileUserScore
{
	//积分信息
	SCORE							lScore;								//用户分数

	//输赢信息
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数

	//全局信息
	DWORD							dwExperience;						//用户经验
};


//道具使用
struct tagUsePropertyInfo
{
	WORD                            wPropertyCount;                     //道具数目
	WORD                            dwValidNum;						    //有效数字
	DWORD                           dwEffectTime;                       //生效时间
};


//用户道具
struct tagUserProperty
{
	WORD                            wPropertyUseMark;                   //道具标示
	tagUsePropertyInfo              PropertyInfo[MAX_PT_MARK];			//使用信息   
};

//道具包裹
struct tagPropertyPackage
{
	WORD                            wTrumpetCount;                     //小喇叭数
	WORD                            wTyphonCount;                      //大喇叭数
};

//时间信息
struct tagTimeInfo
{
	DWORD							dwEnterTableTimer;					//进出桌子时间
	DWORD							dwLeaveTableTimer;					//离开桌子时间
	DWORD							dwStartGameTimer;					//开始游戏时间
	DWORD							dwEndGameTimer;						//离开游戏时间
};

//包裹信息
struct tagPropItemInfo
{
	WORD							wItemID;							//道具标识
	DWORD							dwItemCount;						//道具数量
};

//用户包裹
struct tagUserPackage
{
	tagPropItemInfo					PropItemInfo[MAX_PROPERTY];			//道具信息
};


//用户信息
struct tagUserInfo
{
	//基本属性
	DWORD							dwUserID;							//用户 I D
	DWORD							dwGameID;							//游戏 I D

	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名

	//头像信息
	WORD							wFaceID;							//头像索引
	TCHAR						    szFaceUrl[256];						//网络头像

	//用户资料
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级

	//用户状态
	WORD							wTableID;							//桌子索引
	WORD							wChairID;							//椅子索引
	BYTE							cbUserStatus;						//用户状态

	//积分信息
	SCORE							lScore;								//用户分数
	SCORE							lInsure;							//用户银行

	SCORE							lRoomCards;							//房卡数量

	//游戏信息
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数

	//时间信息
	tagTimeInfo						TimerInfo;							//时间信息

	//比赛信息
	BYTE							cbEnlistStatus;						//报名状态（开宝箱的次数）

	//扩展标识
	DWORD							dwExpand;							//扩展标致

	//机器标志
	bool							bIsAndroidUser;						//机器标志

	//移动标志
	bool							bIsMobileUser;						//移动标志

	//渠道标志
	int								intChannel;							//渠道标志
};

//用户信息
struct tagUserInfoHead
{
	//用户属性
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwUserID;							//用户 I D

	//头像信息
	WORD							wFaceID;							//头像索引
	TCHAR						    szFaceUrl[256];						//用户头像

	//字符信息
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//个性签名

	//用户属性
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级

	//用户状态
	WORD							wTableID;							//桌子索引
	WORD							wChairID;							//椅子索引
	BYTE							cbUserStatus;						//用户状态

	//积分信息
	SCORE							lScore;								//用户分数
	SCORE							lInsure;							//用户银行
	SCORE							lRoomCards;							//房卡数量

	//游戏信息
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数

	bool							bIsAndroidUser;						//是否机器人
	bool							bIsMobileUser;						//移动标志

	WORD                            dwLogForm;                          //平台类型
};

struct tagMatchUserData :public tagUserInfo
{
	WORD							wMatchRank;							//比赛排名
	SCORE                           lMatchScore;						//比赛积分
	BYTE							cbIsOut;							//比赛出局
	int                             cbMatchID;
	BYTE                            cbOffLineCount;                     //断线次数
	BYTE                            cbUserLadder;                       //用户的层次
};

//头像信息
struct tagCustomFaceInfo
{
	DWORD							dwDataSize;							//数据大小
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//图片信息
};

//用户信息
struct tagUserRemoteInfo
{
	//用户信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称

	//等级信息
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级
	BYTE							cbMasterOrder;						//管理等级

	//位置信息
	WORD							wKindID;							//类型标识
	WORD							wServerID;							//房间标识
	TCHAR							szGameServer[LEN_SERVER];			//房间位置
};

//////////////////////////////////////////////////////////////////////////////////

//广场子项
struct tagGamePlaza
{
	WORD							wPlazaID;							//广场标识
	TCHAR							szServerAddr[32];					//服务地址
	TCHAR							szServerName[32];					//服务器名
};

//级别子项
struct tagLevelItem
{
	LONG							lLevelScore;						//级别积分
	TCHAR							szLevelName[16];					//级别描述
};

//会员子项
struct tagMemberItem
{
	BYTE							cbMemberOrder;						//等级标识
	TCHAR							szMemberName[16];					//等级名字
};

//管理子项
struct tagMasterItem
{
	BYTE							cbMasterOrder;						//等级标识
	TCHAR							szMasterName[16];					//等级名字
};

//列表子项
struct tagColumnItem
{
	BYTE							cbColumnWidth;						//列表宽度
	BYTE							cbDataDescribe;						//字段类型
	TCHAR							szColumnName[16];					//列表名字
};

//地址信息
struct tagAddressInfo
{
	TCHAR							szAddress[32];						//服务地址
};

//数据信息
struct tagDataBaseParameter
{
	WORD							wDataBasePort;						//数据库端口
	TCHAR							szDataBaseAddr[32];					//数据库地址
	TCHAR							szDataBaseSlaveAddr[32];			//数据库地址
	TCHAR							szDataBaseUser[32];					//数据库用户
	TCHAR							szDataBasePass[32];					//数据库密码
	TCHAR							szDataBaseName[32];					//数据库名字
};

//房间配置
struct tagServerOptionInfo
{
	//挂接属性
	WORD							wKindID;							//挂接类型
	WORD							wNodeID;							//挂接节点
	WORD							wSortID;							//排列标识

	//税收配置
	WORD							wRevenueRatio;						//税收比例
	SCORE							lServiceScore;						//服务费用

	//房间配置
	SCORE							lRestrictScore;						//限制积分
	SCORE							lMinTableScore;						//最低积分
	SCORE							lMinEnterScore;						//最低积分
	SCORE							lMaxEnterScore;						//最高积分

	//会员限制
	BYTE							cbMinEnterMember;					//最低会员
	BYTE							cbMaxEnterMember;					//最高会员

	//房间属性
	DWORD							dwServerRule;						//房间规则
	TCHAR							szServerName[LEN_SERVER];			//房间名称
};

//用户信息
struct tagMobileUserInfoHead
{
	//用户属性
	DWORD							dwGameID;							//游戏 I D
	DWORD							dwUserID;							//用户 I D

	//头像信息
	WORD							wFaceID;							//头像索引

	//用户属性
	BYTE							cbGender;							//用户性别
	BYTE							cbMemberOrder;						//会员等级

	//用户状态
	WORD							wTableID;							//桌子索引
	WORD							wChairID;							//椅子索引
	BYTE							cbUserStatus;						//用户状态

	//积分信息
	SCORE							lScore;								//用户分数

	//游戏信息
	DWORD							dwWinCount;							//胜利盘数
	DWORD							dwLostCount;						//失败盘数
	DWORD							dwDrawCount;						//和局盘数
	DWORD							dwFleeCount;						//逃跑盘数
};
//////////////////////////////////////////////////////////////////////////////////


//比赛信息
typedef struct CMD_GT_MatchInfo
{
	WORD	                    wKindID;					//类型ID
	int                         cbMatchID;
	WORD                        wRegistMinUser;               //最少开赛条件
	BYTE						cbMatchType;				//比赛类型   人数赛or时间赛
	int                         MatchGold;                 //报名费用
	int                         virtualMatchScole;          //初始化虚拟金币
	BYTE                        DefaultMark;                 //内部标识
	TCHAR                       szRoomName[64];			       //房间名字 
	TCHAR                       szWinScore[128];               //奖励金币
	BYTE                        cbMatchStatus;               //状态
	WORD	                    wCurrentSignUp;				//当前报名人数
	int                         wConfigID;                  //配置ID
}tagMatchInfo, CMD_GT_CreateMatch;

//用户一盘输赢
struct tagPrivateRandRecordChild
{
	DWORD						dwKindID;
	DWORD						dwVersion;
	int							iRecordID;
	int							iRecordChildID;
	std::vector<int>			kScore;
	SYSTEMTIME					kPlayTime;
	datastream					kRecordGame;
	std::string					kUserDefine;
	std::vector<int>			kUserSex;		//add 性别
	std::vector<std::string>	kUserWXFaceID;	//微信头像
	DWORD						GameRuleIdex;	//游戏规则
	DWORD						GameTypeIdex;	//游戏规则
	tagPrivateRandRecordChild()
	{
		dwKindID = 0;
		dwVersion = 0;
		iRecordID = 0;
		iRecordChildID = 0;
		GameRuleIdex = 0;
		GameTypeIdex = 0;
	}
	void StreamValue(datastream& kData, bool bSend)
	{
		Stream_VALUE(iRecordID);
		Stream_VALUE(iRecordChildID);
		Stream_VALUE(kScore);
		Stream_VALUE(kRecordGame);
		Stream_VALUE_SYSTEMTIME(kPlayTime);
		Stream_VALUE(kUserDefine);
		Stream_VALUE(dwKindID);
		Stream_VALUE(dwVersion);
		Stream_VALUE(GameRuleIdex);
		Stream_VALUE(GameTypeIdex);
		Stream_VALUE(kUserSex);
		Stream_VALUE(kUserWXFaceID);
	}
};

//代开记录查询
struct tagQueryHelpRoomRecord
{
	DWORD     dwRoomNum;
	DWORD     dwKindID;
	TCHAR	  szRoomName[32];
	TCHAR     szUserName[512];
	TCHAR	  szUserScore[512];
	TCHAR	  szUserChest[512];
};

//用户一轮总输赢
struct tagPrivateRandTotalRecord
{
	tagPrivateRandTotalRecord()
	{
		dwKindID = 0;
		dwVersion = 0;
		iRoomNum = 0;
		iRecordID = 0;
	}
	DWORD						dwKindID;
	DWORD						dwVersion;
	int							iRoomNum;
	int							iRecordID;
	std::vector<int>			kScore;
	std::vector<int>			kUserID;
	std::vector<std::string>	kNickName;
	SYSTEMTIME					kPlayTime;
	std::string					kUserDefine;

	std::vector<tagPrivateRandRecordChild>	kRecordChild;

	void StreamValue(datastream& kData, bool bSend)
	{
		Stream_VALUE(iRoomNum);
		Stream_VALUE(kUserID);
		Stream_VALUE(kNickName);
		Stream_VALUE(kScore);
		Stream_VALUE_SYSTEMTIME(kPlayTime);
		StructVecotrMember(tagPrivateRandRecordChild, kRecordChild);
		Stream_VALUE(iRecordID);
		Stream_VALUE(kUserDefine);
		Stream_VALUE(dwKindID);
		Stream_VALUE(dwVersion);
	}
};

struct tagPrivateRandTotalRecordList
{
	DWORD		dwUserID;
	std::vector<tagPrivateRandTotalRecord> kList;

	void StreamValue(datastream& kData, bool bSend)
	{
		Stream_VALUE(dwUserID);
		StructVecotrMember(tagPrivateRandTotalRecord, kList);
	}
};

//私人场信息
struct tagPrivateRoomInfo
{
	DWORD			dwCreaterUserID;								//一桌游戏创建者
	DWORD		    dwHelpCreateUserID;							    //房间代开用户
	BOOL		    bHelpCreateRoom;								//是否代开房间
	DWORD			dwRoomNum;										//房间编号
	DWORD			dwPlayCout;										//玩家局数
	DWORD			dwPlayCost;										//消耗点数
	bool			bStart;											//是否开局
	bool			bInEnd;											//是否结束
	float			fAgainPastTime;									//重新开始等待时间

	BYTE			cbRoomType;										//房间类型（0私有房间，1公有房间)
	LONGLONG		m_llStartScore;									//开始带入金币

	BYTE			cbPlayCnts;										//游戏人数
	DWORD			dwStartPlayCout;								//游戏开局数
	DWORD			dwFinishPlayCout;								//开房间后游戏完成局数

	BYTE			bPlayCoutIdex;									//玩家局数
	BYTE			bGameTypeIdex;									//游戏类型
	DWORD			bGameRuleIdex;									//游戏规则
	BYTE			bIsAApayment;									//是否AA付房卡
	BYTE			bIsChest;										//如何开宝箱 大赢家 & 人人分
	DWORD           dBetCards;										//房卡数量	
	bool			bGameStatusIsChest;								//进入开宝箱阶段
	DWORD			Nextchair;										//下一局的庄
};


// 游戏库存
struct tagGameStock
{
	WORD							wNumber;							// 索引号
	SCORE							lCurrentValue;						// 当前值
	SCORE							lBetMin;							// 投注最小值
	SCORE							lBetMax;							// 投注最大值
	SCORE							lStockValue;						// 库存值
	SCORE							lStockMax;							// 最大值
	SCORE							lStockMin;							// 最小值
	bool							bInit;								// 初始化
	DWORD							dPercent;							// 抽水比例
};

// 服务库存
struct tagServerStock
{
	DWORD							dwServerID;
	TCHAR							szGameName[50];
	tagGameStock					GameStock;
	WORD							wUserCount;
};

//缓存机器
struct tagAndriodInfo
{
	//基本信息
	DWORD							dwUserID;							//用户标识
	DWORD							dwGameID;							//游戏标识
	WORD							wFaceID;							//头像标识
	BYTE                            cbLoginType;                         //登录类型
	TCHAR							szNickName[LEN_NICKNAME];			//用户昵称
	BYTE							cbGender;							//用户性别
	TCHAR							szFaceUrl[256];						//网络头像
	BYTE							cbIsYouke;							//是否游客

	//扩展信息
	WORD							wServerID;							//房间信息
};

//房间库存
struct tagRoomHandsel
{
	//基本属性
	WORD							wServerId;							//房间信息
	WORD							wKindID;							//房间类型
	TCHAR							szServerName[LEN_SERVER];			//房间名称
	FLOAT							fDegree;							//房间难度
	LONGLONG						llHandselLv1;						//房间彩金1
	LONGLONG						llHandselLv2;						//房间彩金2
	LONGLONG						llHandselLv3;						//房间彩金3
	LONGLONG						llHandselLv4;						//房间彩金4
	LONGLONG						llHandselLv5;						//房间彩金5
	LONGLONG						llHandselLv6;						//房间彩金6
	LONGLONG						llHandselLv7;						//房间彩金7
	LONGLONG						llHandselLv8;						//房间彩金8

	//是否变动
	BOOL							bIsChange;							//是否变动
};

//桌子彩金
struct tagTableHandsel
{
	//基本属性
	WORD							wTableID;							//桌子信息
	LONGLONG						llTableHandsel;						//彩金数值
	FLOAT							fTableDegree;						//房间难度

	//是否变动
	BOOL							bIsChange;							//是否变动
};

//幸运数值
struct tagTableLuckReward
{
	//基本属性
	WORD							wTableID;							//桌子信息
	SCORE							llLuckValue;						//幸运数值
	SCORE							llLuckScore;						//幸运奖励

	//是否变动
	BOOL							bIsChange;							//是否变动
};

//游戏活动
struct tagTableGameActive
{
	//基本属性
	WORD							wTableID;							//桌子信息
	CHAR							szGameActive[MAX_GAME_ACTIVE];		//游戏活动

	//是否变动
	BOOL							bIsChange;							//是否变动
};

#pragma pack()

#endif