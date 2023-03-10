#pragma once

//////////////////////////////////////////////////////////////////////////////////

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

#define _ATL_ATTRIBUTES
#define _AFX_ALL_WARNINGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

//////////////////////////////////////////////////////////////////////////////////

//MFC 文件
#include <AfxWin.h> 
#include <AfxExt.h>
#include <AfxCmn.h>
#include <AfxDisp.h>

#include <Shlwapi.h>
#include <comutil.h>
#include <winsock2.h>
#include <WS2tcpip.h>

//////////////////////////////////////////////////////////////////////////////////

//链接代码
#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../链接库/Ansi/ServiceCore.lib")
#else
#pragma comment (lib,"../../链接库/Unicode/ServiceCore.lib")
#pragma comment (lib,"../../链接库/Unicode/libzmq.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../链接库/Ansi/ServiceCoreD.lib")
#else
#pragma comment (lib,"../../链接库/Unicode/ServiceCoreD.lib")
#pragma comment (lib,"../../链接库/Unicode/libzmqD.lib")
#endif
#endif

//位置索引
#define SOCKET_INDEX(dwSocketID)	LOWORD(dwSocketID)
//循环索引
#define SOCKET_ROUNTID(dwSocketID)	HIWORD(dwSocketID)


#ifdef __cplusplus
#if _MSC_VER < 1600 && !defined(__cplusplus_cli)
namespace std
{
	struct nullptr_t
	{
		nullptr_t(void *_ = NULL) :$(_) {}
		operator bool()const { return NULL != $; }
		template <typename T>
		operator T *()const { return (T*)$; }
	private:
		void *$;
	};
}
#define nullptr static_cast<::std::nullptr_t>(NULL)
#endif
#else
#define nullptr NULL;
#endif


//数组维数
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

//无效地址
#define INVALID_IP_ADDRESS(IPAddress) (((IPAddress==0L)||(IPAddress==INADDR_NONE)))

//输出定义
#define  KernelEngineLog				TEXT("KernelEngine")
#define  KernelEngineError				TEXT("KernelEngineError")  
#define  KernelEngineExecuteProcessLog  TEXT("KernelEngineExecuteProcessLog")

//存储长度
#ifdef _UNICODE
#define CountStringBuffer CountStringBufferW
#else
#define CountStringBuffer CountStringBufferA
#endif

//存储长度
#define CountStringBufferA(String) ((UINT)((lstrlenA(String)+1)*sizeof(CHAR)))
#define CountStringBufferW(String) ((UINT)((lstrlenW(String)+1)*sizeof(WCHAR)))

#ifdef UNICODE
#define TCharToUnicode(wzSrc, wzDest, cchSize)   UnicodeToUnicode(wzSrc, wzDest, cchSize)
#define TCharToAnsi(wzSrc, szDest, cchSize)      UnicodeToAnsi(wzSrc, szDest, cchSize)
#define UnicodeToTChar(wzSrc, wzDest, cchSize)   UnicodeToUnicode(wzSrc, wzDest, cchSize)
#define AnsiToTChar(szSrc, wzDest, cchSize)		 AnsiToUnicode(szSrc, wzDest, cchSize)
#else
#define TCharToUnicode(szSrc, wzDest, cchSize)	 AnsiToUnicode(szSrc, wzDest, cchSize)
#define TCharToAnsi(szSrc, szDest, cchSize)      AnsiToAnsi(szSrc, szDest, cchSize)
#define UnicodeToTChar(wzSrc, szDest, cchSize)   UnicodeToAnsi(wzSrc, szDest, cchSize)
#define AnsiToTChar(szSrc, szDest, cchSize)      AnsiToAnsi(szSrc, szDest, cchSize)
#endif




//////////////////////////////////////////////////////////////////////////////////
