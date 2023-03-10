#include "Stdafx.h"
#include "AfxDllx.h"
#include "KernelEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
static AFX_EXTENSION_MODULE KernelEngineDLL = { NULL,NULL };

//////////////////////////////////////////////////////////////////////////////////

//导出函数
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
			{
				if (!AfxInitExtensionModule(KernelEngineDLL, hInstance)) return 0;
				new CDynLinkLibrary(KernelEngineDLL);

				//加载 COM
				CoInitialize(NULL);

				//加载网络
				WSADATA WSAData;
				if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) return FALSE;

				break;
			}
		case DLL_THREAD_ATTACH:
			{
				//加载 COM
				CoInitialize(NULL);

				break;
			}
		case DLL_THREAD_DETACH:
			{
				//释放 COM
				CoUninitialize();

				break;
			}
		case DLL_PROCESS_DETACH:
			{
				//释放网络
				WSACleanup();

				//释放 COM
				CoUninitialize();

				AfxTermExtensionModule(KernelEngineDLL);

				break;
			}
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////
