// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 标头中排除不常使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER
#define WINVER 0x0501		//为 Windows xp 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501		//为 Windows xp 及更新版本改变为适当的值。
#endif						

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0501 //为 Windows xp 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0600	//为 IE 6.0 及更新版本改变为适当的值。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常被安全忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心和标准组件
#include <afxext.h>         // MFC 扩展
#include <afxdisp.h>        // MFC 自动化类

#include <afxdtctl.h>		// Internet Explorer 4 公共控件的 MFC 支持
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows 公共控件的 MFC 支持
#endif // _AFX_NO_AFXCMN_SUPPORT

// #include "Define.h"
// #include "Macro.h"
// #include "ProductKey.h"

//////////////////////////////////////////////////////////////////////////////////
//包含文件

//平台定义
#include "..\..\全局定义\Platform.h"

//组件定义
#include "..\..\公共组件\服务核心\ServiceCoreHead.h"

//////////////////////////////////////////////////////////////////////////////////
//链接代码

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../链接库/Ansi/ServiceCore.lib")
#else
#pragma comment (lib,"../../链接库/Unicode/ServiceCore.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../链接库/Ansi/ServiceCoreD.lib")
#else
#pragma comment (lib,"../../链接库/Unicode/ServiceCoreD.lib")
#endif
#endif
