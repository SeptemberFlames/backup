#include "Stdafx.h"
#include "Resource.h"
#include "DataServerDlg.h"

#define IDI_UPDATE_TIMER		1
//http回调消息
#define HTTP_CALL_BACK_MSG			100
//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDataServerDlg, CDialog)
	ON_WM_QUERYENDSESSION()
	ON_BN_CLICKED(IDC_STOP_SERVICE, OnBnClickedStopService)
	ON_BN_CLICKED(IDC_START_SERVICE, OnBnClickedStartService)
	
	ON_MESSAGE(WM_PROCESS_CMD_LINE,OnMessageProcessCmdLine)
	ON_MESSAGE(WM_PROCESS_GAME_QUIT,OnMessageProcessQuit)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDataServerDlg::CDataServerDlg() : CDialog(IDD_DLG_LOGON_SERVER)
{
	int aa = 0;
}

//析构函数
CDataServerDlg::~CDataServerDlg()
{
	int aa = 0;
}

//控件绑定
VOID CDataServerDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRACE_MESSAGE, m_TraceServiceControl);
}

//初始化函数
BOOL CDataServerDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("数据服务器 -- [ 停止 ]"));

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE); 

	//设置组件
	m_ServiceUnits.SetServiceUnitsSink(this);
	//m_FlashAccredit.SetServiceParameter(843);
	//m_FlashAccredit.StartService();
	//获取HTTP服务器配置
	TCHAR module_name[MAX_PATH];
	GetModuleFileName(NULL, module_name, MAX_PATH);
	CString strDir;
	strDir.Format(TEXT("%s"),module_name);
	int nSlipIndex = strDir.ReverseFind('\\');
	CString strWorkDir = strDir.Left(nSlipIndex);
	TCHAR SyncSvrHttpServerSetting[256] = {0};
	_stprintf(SyncSvrHttpServerSetting, TEXT("%s\\SyncSvrHttpServerSet.ini"), strWorkDir.GetBuffer());

	//命令处理
	LPCTSTR pszCmdLine=AfxGetApp()->m_lpCmdLine;
	if (pszCmdLine[0]!=0) PostMessage(WM_PROCESS_CMD_LINE,0,(LPARAM)pszCmdLine);

	return TRUE;
}

//确定消息
VOID CDataServerDlg::OnOK()
{
	return;
}

//取消函数
VOID CDataServerDlg::OnCancel()
{

	//关闭询问
	if (m_ServiceUnits.GetServiceStatus()!=ServiceStatus_Stop)
	{
		LPCTSTR pszQuestion=TEXT("数据服务器正在运行中，您确实要关闭服务器吗？");
		if (AfxMessageBox(pszQuestion,MB_YESNO|MB_DEFBUTTON2|MB_ICONQUESTION)!=IDYES) return;
	}
	//停止服务
	//每秒定时刷新
	KillTimer(IDI_UPDATE_TIMER);
	m_ServiceUnits.ConcludeService();

	__super::OnCancel();
}

//消息解释
BOOL CDataServerDlg::PreTranslateMessage(MSG * pMsg)
{
	//按键过滤
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//服务状态
VOID CDataServerDlg::OnServiceUnitsStatus(enServiceStatus ServiceStatus)
{
	//状态设置
	switch (ServiceStatus)
	{
	case ServiceStatus_Stop:	//停止状态
		{
			//每秒定时刷新
			KillTimer(IDI_UPDATE_TIMER);
			//设置标题
			SetWindowText(TEXT("数据服务器 -- [ 停止 ]"));

			//设置按钮
			GetDlgItem(IDC_STOP_SERVICE)->EnableWindow(FALSE);
			GetDlgItem(IDC_START_SERVICE)->EnableWindow(TRUE);
			
			//提示信息
			LPCTSTR pszDescribe=TEXT("服务停止成功");
			CTraceService::TraceString(pszDescribe,TraceLevel_Normal);

			break;
		}
	case ServiceStatus_Config:	//配置状态
		{
			//设置标题
			SetWindowText(TEXT("数据服务器 -- [ 初始化 ]"));

			//设置按钮
			GetDlgItem(IDC_STOP_SERVICE)->EnableWindow(TRUE);
			GetDlgItem(IDC_START_SERVICE)->EnableWindow(FALSE);
			
			//提示信息
			LPCTSTR pszDescribe=TEXT("正在初始化组件...");
			CTraceService::TraceString(pszDescribe,TraceLevel_Normal);

			break;
		}
	case ServiceStatus_Service:	//服务状态
		{
			CString  str;
			str.Format(TEXT("数据服务器 端口：%d-- [ 运行 ]"),m_ServiceUnits.m_wPort);
			SetWindowText(str);

			//设置按钮
			GetDlgItem(IDC_STOP_SERVICE)->EnableWindow(TRUE);
			GetDlgItem(IDC_START_SERVICE)->EnableWindow(FALSE);
			
			//提示信息
			LPCTSTR pszDescribe=TEXT("服务启动成功");
			CTraceService::TraceString(pszDescribe,TraceLevel_Normal);

			break;
		}
	}

	return;
}

//启动服务
VOID CDataServerDlg::OnBnClickedStartService()
{
	//启动服务
	try
	{
		m_ServiceUnits.StartService();
		//每秒定时刷新
		SetTimer(IDI_UPDATE_TIMER,30000,NULL);
	}
	catch (...)
	{
		ASSERT(FALSE);
	}

	return;
}

//停止服务
VOID CDataServerDlg::OnBnClickedStopService()
{
	//停止服务
	try
	{
		//每秒定时刷新
		KillTimer(IDI_UPDATE_TIMER);
		m_ServiceUnits.ConcludeService();
	}
	catch (...)
	{
		ASSERT(FALSE);
	}

	return;
}

//关闭询问
BOOL CDataServerDlg::OnQueryEndSession()
{
	//提示消息
	if (m_ServiceUnits.GetServiceStatus()!=ServiceStatus_Stop)
	{
		CTraceService::TraceString(TEXT("服务正在运行中，系统要求注销回话请求失败"),TraceLevel_Warning);
		return FALSE;
	}

	return TRUE;
}

//网络事件
LRESULT CDataServerDlg::OnNetworkEvent(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

//命令处理
LRESULT CDataServerDlg::OnMessageProcessCmdLine(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	CWHCommandLine CommandLine;
	LPCTSTR pszCommandLine=(LPCTSTR)(lParam);

	//房间标识
	TCHAR szSrverID[32]=TEXT("");
	if (CommandLine.SearchCommandItem(pszCommandLine,TEXT("/ServerID:"),szSrverID,CountArray(szSrverID))==true)
	{
		//获取房间
		WORD wServerID=(WORD)(_tstol(szSrverID));
		m_ServiceUnits.m_wPort = wServerID;

		//端口设置
		OnBnClickedStartService();
	}

	return 0L;
}

//进程退出
LRESULT CDataServerDlg::OnMessageProcessQuit(WPARAM wParam, LPARAM lParam)
{
	//停止服务
	m_ServiceUnits.ConcludeService();

	__super::OnCancel();

	return 0L;
}
