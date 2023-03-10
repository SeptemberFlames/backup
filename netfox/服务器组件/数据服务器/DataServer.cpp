#include "Stdafx.h"
#include "DataServer.h"
#include "DataServerDlg.h"

//////////////////////////////////////////////////////////////////////////////////

//程序对象
CLogonServerApp theApp;

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CLogonServerApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CLogonServerApp::CLogonServerApp()
{
}

//启动函数
BOOL CLogonServerApp::InitInstance()
{
	__super::InitInstance();

	//设置组件
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//设置注册表
	SetRegistryKey(szProduct);

	//显示窗口
	CDataServerDlg DataServerDlg;
	m_pMainWnd=&DataServerDlg;
	DataServerDlg.DoModal();

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////
