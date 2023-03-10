﻿#pragma once

//////////////////////////////////////////////////////////////////////////////////
//MFC 文件

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif						

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0501
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0501
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#define _AFX_ALL_WARNINGS

#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxDisp.h>
#include <AfxDtctl.h>
#include "afxinet.h"


#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <AfxCmn.h>
#endif

//////////////////////////////////////////////////////////////////////////////////
//包含文件

//平台定义
#include "..\..\全局定义\Platform.h"
#include "..\..\消息定义\CMD_Correspond.h"
#include "..\..\消息定义\CMD_ControlServer.h"

//组件定义
#include "..\..\公共组件\服务核心\ServiceCoreHead.h"
#include "..\..\服务器组件\游戏服务\GameServiceHead.h"
#include "..\..\服务器组件\内核引擎\KernelEngineHead.h"
//////////////////////////////////////////////////////////////////////////////////
//链接代码

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../链接库/Ansi/ServiceCore.lib")
#pragma comment (lib,"../../链接库/Ansi/CacheService.lib")
#pragma comment (lib,"../../链接库/Ansi/GameService.lib")
#pragma comment (lib,"../../链接库/Ansi/KernelEngine.lib")
#else
#pragma comment (lib,"../../链接库/Unicode/ServiceCore.lib")
#pragma comment (lib,"../../链接库/Unicode/GameService.lib")
#pragma comment (lib,"../../链接库/Unicode/KernelEngine.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../链接库/Ansi/ServiceCoreD.lib")
#pragma comment (lib,"../../链接库/Ansi/GameServiceD.lib")
#pragma comment (lib,"../../链接库/Ansi/KernelEngineD.lib")
#else
#pragma comment (lib,"../../链接库/Unicode/ServiceCoreD.lib")
#pragma comment (lib,"../../链接库/Unicode/GameServiceD.lib")
#pragma comment (lib,"../../链接库/Unicode/KernelEngineD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////
