// CollocateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Collocate.h"
#include "CollocateDlg.h"
#include "collocatedlg.h"

#include <locale.h>


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCollocateDlg 对话框



CCollocateDlg::CCollocateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCollocateDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCollocateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCollocateDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTONLOADCONFIG, OnBnClickedButtonloadconfig)
	ON_BN_CLICKED(IDC_BUTTONSAVECONFIG, OnBnClickedButtonsaveconfig)
END_MESSAGE_MAP()


// CCollocateDlg 消息处理程序

BOOL CCollocateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//获取机器
	LoadCurrentMachineID();

	//获取路径
	TCHAR szWorkDir[MAX_PATH] = TEXT("");
	CWHService::GetWorkDirectory(szWorkDir, CountArray(szWorkDir));

	//构造路径
	TCHAR szFileName[MAX_PATH] = TEXT("");
	_sntprintf_s(szFileName, _TRUNCATE, TEXT("%s\\ServerParameter.ini"), szWorkDir);

	//加载配置
	LoadCollocateConfig(szFileName);

	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CCollocateDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCollocateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CCollocateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString	CCollocateDlg::IntToCString(int integer)
{
	CString cString;
	cString.Format(TEXT("%d"), integer);
	return cString;
}

void CCollocateDlg::LoadCurrentMachineID()
{
	CWHService::GetMachineID(m_szServiceMachine);
	GetDlgItem(IDC_EDIT17)->SetWindowText(m_szServiceMachine);
}

void CCollocateDlg::LoadCollocateConfig(CString fileName)
{
	CWHIniData IniData;
	IniData.SetIniFilePath(fileName);

	// ================================= 服务器配置 ==========================================

	TCHAR	szServiceName[LEN_SERVER];										// 服务器名称
	TCHAR	szServiceAddr[LEN_SERVER];										// 服务器地址
	TCHAR	szCorrespondAddr[LEN_SERVER];									// 协调服地址

	IniData.ReadEncryptString(TEXT("ServerInfo"), TEXT("ServiceName"), NULL, szServiceName, CountArray(szServiceName));
	IniData.ReadEncryptString(TEXT("ServerInfo"), TEXT("ServiceAddr"), NULL, szServiceAddr, CountArray(szServiceAddr));
	IniData.ReadEncryptString(TEXT("ServerInfo"), TEXT("CorrespondAddr"), NULL, szCorrespondAddr, CountArray(szCorrespondAddr));

	GetDlgItem(IDC_EDIT1)->SetWindowText(szServiceName);					// 设置服务器名称
	GetDlgItem(IDC_IPADDRESS1)->SetWindowText(szServiceAddr);				// 设置服务器地址
	GetDlgItem(IDC_IPADDRESS2)->SetWindowText(szCorrespondAddr);			// 设置协调服地址

	// ================================= 平台数据库配置 ==========================================
	TCHAR	PlatformDBAddr[LEN_SERVER];										// 平台数据库地址
	TCHAR	PlatformDBSlaveAddr[LEN_SERVER];								// 平台数据库地址
	DWORD	PlatformDBPort;													// 平台数据库端口
	TCHAR	PlatformDBUser[LEN_SERVER];										// 平台数据库用户
	TCHAR	PlatformDBPass[LEN_SERVER];										// 平台数据库密码

	IniData.ReadEncryptString(TEXT("PlatformDB"), TEXT("DBAddr"), NULL, PlatformDBAddr, CountArray(PlatformDBAddr));
	IniData.ReadEncryptString(TEXT("PlatformDB"), TEXT("DBSlaveAddr"), NULL, PlatformDBSlaveAddr, CountArray(PlatformDBSlaveAddr));
	PlatformDBPort = (DWORD)IniData.ReadInt(TEXT("PlatformDB"), TEXT("DBPort"), 0);
	IniData.ReadEncryptString(TEXT("PlatformDB"), TEXT("DBUser"), NULL, PlatformDBUser, CountArray(PlatformDBUser));
	IniData.ReadEncryptString(TEXT("PlatformDB"), TEXT("DBPass"), NULL, PlatformDBPass, CountArray(PlatformDBPass));

	GetDlgItem(IDC_STATIC1)->SetWindowText(TEXT("QPPlatformDB"));			// 设置平台数据库名称
	GetDlgItem(IDC_IPADDRESS3)->SetWindowText(PlatformDBAddr);				// 设置平台数据库地址
	GetDlgItem(IDC_IPADDRESS3_1)->SetWindowText(PlatformDBSlaveAddr);		// 设置平台数据库地址
	GetDlgItem(IDC_EDIT2)->SetWindowText(IntToCString(PlatformDBPort));		// 设置平台数据库端口
	GetDlgItem(IDC_EDIT3)->SetWindowText(PlatformDBUser);					// 设置平台数据库用户
	GetDlgItem(IDC_EDIT4)->SetWindowText(PlatformDBPass);					// 设置平台数据库密码


	// ================================= 金币数据库配置 ==========================================

	TCHAR	TreasurdDBAddr[LEN_SERVER];										// 金币数据库地址
	TCHAR	TreasurdDBSlaveAddr[LEN_SERVER];								// 金币数据库地址
	DWORD	TreasurdDBPort;													// 金币数据库端口
	TCHAR	TreasurdDBUser[LEN_SERVER];										// 金币数据库用户
	TCHAR	TreasurdDBPass[LEN_SERVER];										// 金币数据库密码

	IniData.ReadEncryptString(TEXT("TreasureDB"), TEXT("DBAddr"), NULL, TreasurdDBAddr, CountArray(TreasurdDBAddr));
	IniData.ReadEncryptString(TEXT("TreasureDB"), TEXT("DBSlaveAddr"), NULL, TreasurdDBSlaveAddr, CountArray(TreasurdDBSlaveAddr));
	TreasurdDBPort = (DWORD)IniData.ReadInt(TEXT("TreasureDB"), TEXT("DBPort"), 0);
	IniData.ReadEncryptString(TEXT("TreasureDB"), TEXT("DBUser"), NULL, TreasurdDBUser, CountArray(TreasurdDBUser));
	IniData.ReadEncryptString(TEXT("TreasureDB"), TEXT("DBPass"), NULL, TreasurdDBPass, CountArray(TreasurdDBPass));

	GetDlgItem(IDC_STATIC2)->SetWindowText(TEXT("QPTreasureDB"));			// 设置金币数据库名称
	GetDlgItem(IDC_IPADDRESS4)->SetWindowText(TreasurdDBAddr);				// 设置金币数据库地址
	GetDlgItem(IDC_IPADDRESS4_1)->SetWindowText(TreasurdDBSlaveAddr);		// 设置金币数据库地址
	GetDlgItem(IDC_EDIT5)->SetWindowText(IntToCString(TreasurdDBPort));		// 设置金币数据库端口
	GetDlgItem(IDC_EDIT6)->SetWindowText(TreasurdDBUser);					// 设置金币数据库用户
	GetDlgItem(IDC_EDIT7)->SetWindowText(TreasurdDBPass);					// 设置金币数据库密码

	// ================================= 用户数据库配置 ==========================================

	TCHAR	AccountsDBAddr[LEN_SERVER];										// 用户数据库地址
	TCHAR	AccountsDBSlaveAddr[LEN_SERVER];								// 用户数据库地址
	DWORD	AccountsDBPort;													// 用户数据库端口
	TCHAR	AccountsDBUser[LEN_SERVER];										// 用户数据库用户
	TCHAR	AccountsDBPass[LEN_SERVER];										// 用户数据库密码

	IniData.ReadEncryptString(TEXT("AccountsDB"), TEXT("DBAddr"), NULL, AccountsDBAddr, CountArray(AccountsDBAddr));
	IniData.ReadEncryptString(TEXT("AccountsDB"), TEXT("DBSlaveAddr"), NULL, AccountsDBSlaveAddr, CountArray(AccountsDBSlaveAddr));
	AccountsDBPort = (DWORD)IniData.ReadInt(TEXT("AccountsDB"), TEXT("DBPort"), 0);
	IniData.ReadEncryptString(TEXT("AccountsDB"), TEXT("DBUser"), NULL, AccountsDBUser, CountArray(AccountsDBUser));
	IniData.ReadEncryptString(TEXT("AccountsDB"), TEXT("DBPass"), NULL, AccountsDBPass, CountArray(AccountsDBPass));

	GetDlgItem(IDC_STATIC3)->SetWindowText(TEXT("QPAccountsDB"));			// 设置用户数据库名称
	GetDlgItem(IDC_IPADDRESS5)->SetWindowText(AccountsDBAddr);				// 设置用户数据库地址
	GetDlgItem(IDC_IPADDRESS5_1)->SetWindowText(AccountsDBSlaveAddr);		// 设置用户数据库地址
	GetDlgItem(IDC_EDIT8)->SetWindowText(IntToCString(AccountsDBPort));		// 设置用户数据库端口
	GetDlgItem(IDC_EDIT9)->SetWindowText(AccountsDBUser);					// 设置用户数据库用户
	GetDlgItem(IDC_EDIT10)->SetWindowText(AccountsDBPass);					// 设置用户数据库密码

	// ================================= 控制数据库配置 ==========================================

	TCHAR	ControlDBAddr[LEN_SERVER];										// 控制数据库地址
	TCHAR	ControlDBSlaveAddr[LEN_SERVER];									// 控制数据库地址
	DWORD	ControlDBPort;													// 控制数据库端口
	TCHAR	ControlDBUser[LEN_SERVER];										// 控制数据库用户
	TCHAR	ControlDBPass[LEN_SERVER];										// 控制数据库密码

	IniData.ReadEncryptString(TEXT("QPControlDB"), TEXT("DBAddr"), NULL, ControlDBAddr, CountArray(ControlDBAddr));
	IniData.ReadEncryptString(TEXT("QPControlDB"), TEXT("DBSlaveAddr"), NULL, ControlDBSlaveAddr, CountArray(ControlDBSlaveAddr));
	ControlDBPort = (DWORD)IniData.ReadInt(TEXT("QPControlDB"), TEXT("DBPort"), 0);
	IniData.ReadEncryptString(TEXT("QPControlDB"), TEXT("DBUser"), NULL, ControlDBUser, CountArray(ControlDBUser));
	IniData.ReadEncryptString(TEXT("QPControlDB"), TEXT("DBPass"), NULL, ControlDBPass, CountArray(ControlDBPass));

	GetDlgItem(IDC_STATIC5)->SetWindowText(TEXT("QPControlDB"));			// 设置控制数据库名称
	GetDlgItem(IDC_IPADDRESS7)->SetWindowText(ControlDBAddr);				// 设置控制数据库地址
	GetDlgItem(IDC_IPADDRESS7_1)->SetWindowText(ControlDBSlaveAddr);		// 设置控制数据库地址
	GetDlgItem(IDC_EDIT14)->SetWindowText(IntToCString(ControlDBPort));		// 设置控制数据库端口
	GetDlgItem(IDC_EDIT15)->SetWindowText(ControlDBUser);					// 设置控制数据库用户
	GetDlgItem(IDC_EDIT16)->SetWindowText(ControlDBPass);					// 设置控制数据库密码

	return;
}

void CCollocateDlg::SaveCollocateConfig(CString fileName)
{
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");

	CStdioFile file;

	CString szFormatString;

	CFileException fileException;

	// CFile::modeNoTruncate|
	if (!file.Open(fileName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeText))
	{
		MessageBox(TEXT("打开文件错误"), TEXT("警告"), MB_OK);
		return;
	}
	file.SeekToEnd();

	// ======================================== 配置文件头 ===========================================

	file.WriteString(TEXT("//////////////////////////////////////////////////////////////////////////////////\n"));
	file.WriteString(TEXT("\n"));
	file.WriteString(TEXT("                          Net Fox 6603 Configuration Files                        \n"));
	file.WriteString(TEXT("\n"));

	// ======================================== 服务器配置 ===========================================

	file.WriteString(TEXT("//////////////////////////////////////////////////////////////////////////////////\n"));
	file.WriteString(TEXT("\n"));

	CString	szServiceName;
	CString	szServiceAddr;
	CString	szCorrespondAddr;

	GetDlgItem(IDC_EDIT1)->GetWindowText(szServiceName);					// 设置服务器名称
	GetDlgItem(IDC_IPADDRESS1)->GetWindowText(szServiceAddr);				// 设置服务器地址
	GetDlgItem(IDC_IPADDRESS2)->GetWindowText(szCorrespondAddr);			// 设置协调服地址

	// 写入ServerInfo信息
	file.WriteString(TEXT("[ServerInfo]\n"));
	szFormatString.Format(TEXT("ServiceName=%s\n"), XorEnCryptString(szServiceName));
	file.WriteString(szFormatString);
	szFormatString.Format(TEXT("ServiceAddr=%s\n"), XorEnCryptString(szServiceAddr));
	file.WriteString(szFormatString);
	szFormatString.Format(TEXT("CorrespondAddr=%s\n"), XorEnCryptString(szCorrespondAddr));
	file.WriteString(szFormatString);

	// 文本格式控制
	file.WriteString(TEXT("\n"));
	file.WriteString(TEXT("//////////////////////////////////////////////////////////////////////////////////\n"));
	file.WriteString(TEXT("\n"));

	// ======================================== 平台数据库配置 ===========================================

	CString	szPlatformDBAddr;													// 平台数据库地址
	CString	szPlatformDBSlaveAddr;												// 平台数据库地址
	CString	szPlatformDBPort;													// 平台数据库端口
	CString	szPlatformDBUser;													// 平台数据库用户
	CString	szPlatformDBPass;													// 平台数据库密码

	// 更新控件显示内容
	GetDlgItem(IDC_IPADDRESS3)->GetWindowText(szPlatformDBAddr);				// 设置平台数据库地址
	GetDlgItem(IDC_IPADDRESS3_1)->GetWindowText(szPlatformDBSlaveAddr);			// 设置平台数据库地址
	GetDlgItem(IDC_EDIT2)->GetWindowText(szPlatformDBPort);						// 设置平台数据库端口
	GetDlgItem(IDC_EDIT3)->GetWindowText(szPlatformDBUser);						// 设置平台数据库用户
	GetDlgItem(IDC_EDIT4)->GetWindowText(szPlatformDBPass);						// 设置平台数据库密码

	// 写入PlatformDB信息
	file.WriteString(TEXT("[PlatformDB]\n"));
	szFormatString.Format(TEXT("DBPort=%s\n"), szPlatformDBPort);
	file.WriteString(szFormatString);
	szFormatString.Format(TEXT("DBAddr=%s\n"), XorEnCryptString(szPlatformDBAddr));
	file.WriteString(szFormatString);
	szFormatString.Format(TEXT("DBSlaveAddr=%s\n"), XorEnCryptString(szPlatformDBSlaveAddr));
	file.WriteString(szFormatString);
	szFormatString.Format(TEXT("DBUser=%s\n"), XorEnCryptString(szPlatformDBUser));
	file.WriteString(szFormatString);
	szFormatString.Format(TEXT("DBPass=%s\n"), XorEnCryptString(szPlatformDBPass));
	file.WriteString(szFormatString);

	// 文本格式控制
	file.WriteString(TEXT("\n"));
	file.WriteString(TEXT("//////////////////////////////////////////////////////////////////////////////////\n"));
	file.WriteString(TEXT("\n"));

	// ======================================== 金币数据库配置 ===========================================

	CString	szTreasurdDBAddr;													// 金币数据库地址
	CString	szTreasurdDBSlaveAddr;												// 金币数据库地址
	CString	szTreasurdDBPort;													// 金币数据库端口
	CString	szTreasurdDBUser;													// 金币数据库用户
	CString	szTreasurdDBPass;													// 金币数据库密码

	GetDlgItem(IDC_IPADDRESS4)->GetWindowText(szTreasurdDBAddr);				// 设置金币数据库地址
	GetDlgItem(IDC_IPADDRESS4_1)->GetWindowText(szTreasurdDBSlaveAddr);			// 设置金币数据库地址
	GetDlgItem(IDC_EDIT5)->GetWindowText(szTreasurdDBPort);						// 设置金币数据库端口
	GetDlgItem(IDC_EDIT6)->GetWindowText(szTreasurdDBUser);						// 设置金币数据库用户
	GetDlgItem(IDC_EDIT7)->GetWindowText(szTreasurdDBPass);

	// 写入TreasureDB信息
	file.WriteString(TEXT("[TreasureDB]\n"));
	szFormatString.Format(TEXT("DBPort=%s\n"), szTreasurdDBPort);
	file.WriteString(szFormatString);
	szFormatString.Format(TEXT("DBAddr=%s\n"), XorEnCryptString(szTreasurdDBAddr));
	file.WriteString(szFormatString);
	szFormatString.Format(TEXT("DBSlaveAddr=%s\n"), XorEnCryptString(szTreasurdDBSlaveAddr));
	file.WriteString(szFormatString);
	szFormatString.Format(TEXT("DBUser=%s\n"), XorEnCryptString(szTreasurdDBUser));
	file.WriteString(szFormatString);
	szFormatString.Format(TEXT("DBPass=%s\n"), XorEnCryptString(szTreasurdDBPass));
	file.WriteString(szFormatString);

	// 文本格式控制
	file.WriteString(TEXT("\n"));
	file.WriteString(TEXT("//////////////////////////////////////////////////////////////////////////////////\n"));
	file.WriteString(TEXT("\n"));

	// ======================================== 用户数据库配置 ===========================================

	CString	szAccountsDBAddr;													// 用户数据库地址
	CString	szAccountsDBSlaveAddr;												// 用户数据库地址
	CString	szAccountsDBPort;													// 用户数据库端口
	CString	szAccountsDBUser;													// 用户数据库用户
	CString	szAccountsDBPass;													// 用户数据库密码

	GetDlgItem(IDC_IPADDRESS5)->GetWindowText(szAccountsDBAddr);				// 设置用户数据库地址
	GetDlgItem(IDC_IPADDRESS5_1)->GetWindowText(szAccountsDBSlaveAddr);			// 设置用户数据库地址
	GetDlgItem(IDC_EDIT8)->GetWindowText(szAccountsDBPort);						// 设置用户数据库端口
	GetDlgItem(IDC_EDIT9)->GetWindowText(szAccountsDBUser);						// 设置用户数据库用户
	GetDlgItem(IDC_EDIT10)->GetWindowText(szAccountsDBPass);					// 设置用户数据库密码

	// 写入AccountsDB信息
	file.WriteString(TEXT("[AccountsDB]\n"));
	szFormatString.Format(TEXT("DBPort=%s\n"), szAccountsDBPort);
	file.WriteString(szFormatString);
	szFormatString.Format(TEXT("DBAddr=%s\n"), XorEnCryptString(szAccountsDBAddr));
	file.WriteString(szFormatString);
	szFormatString.Format(TEXT("DBSlaveAddr=%s\n"), XorEnCryptString(szAccountsDBSlaveAddr));
	file.WriteString(szFormatString);
	szFormatString.Format(TEXT("DBUser=%s\n"), XorEnCryptString(szAccountsDBUser));
	file.WriteString(szFormatString);
	szFormatString.Format(TEXT("DBPass=%s\n"), XorEnCryptString(szAccountsDBPass));
	file.WriteString(szFormatString);

	// 文本格式控制
	file.WriteString(TEXT("\n"));
	file.WriteString(TEXT("//////////////////////////////////////////////////////////////////////////////////\n"));
	file.WriteString(TEXT("\n"));

	// ======================================== 控制数据库配置 ===========================================

	CString	szControlDBAddr;														// 控制数据库地址
	CString	szControlDBSlaveAddr;													// 控制数据库地址
	CString	szControlDBPort;														// 控制数据库端口
	CString	szControlDBUser;														// 控制数据库用户
	CString	szControlDBPass;														// 控制数据库密码

	GetDlgItem(IDC_IPADDRESS7)->GetWindowText(szControlDBAddr);						// 设置控制数据库地址
	GetDlgItem(IDC_IPADDRESS7_1)->GetWindowText(szControlDBSlaveAddr);				// 设置控制数据库地址
	GetDlgItem(IDC_EDIT14)->GetWindowText(szControlDBPort);							// 设置控制数据库端口
	GetDlgItem(IDC_EDIT15)->GetWindowText(szControlDBUser);							// 设置控制数据库用户
	GetDlgItem(IDC_EDIT16)->GetWindowText(szControlDBPass);							// 设置控制数据库密码

	// 写入AccountsDB信息
	file.WriteString(TEXT("[QPControlDB]\n"));
	szFormatString.Format(TEXT("DBPort=%s\n"), szControlDBPort);
	file.WriteString(szFormatString);
	szFormatString.Format(TEXT("DBAddr=%s\n"), XorEnCryptString(szControlDBAddr));
	file.WriteString(szFormatString);
	szFormatString.Format(TEXT("DBSlaveAddr=%s\n"), XorEnCryptString(szControlDBSlaveAddr));
	file.WriteString(szFormatString);
	szFormatString.Format(TEXT("DBUser=%s\n"), XorEnCryptString(szControlDBUser));
	file.WriteString(szFormatString);
	szFormatString.Format(TEXT("DBPass=%s\n"), XorEnCryptString(szControlDBPass));
	file.WriteString(szFormatString);

	// 文本格式控制
	file.WriteString(TEXT("\n"));
	file.WriteString(TEXT("//////////////////////////////////////////////////////////////////////////////////\n"));
	file.WriteString(TEXT("\n"));

	file.Flush();
	file.Close();

	setlocale(LC_CTYPE, old_locale);
	free(old_locale);
	return;
}

CString	CCollocateDlg::XorEnCryptString(CString inString)
{
	TCHAR szTestSourceData[512];
	TCHAR outChar[512];

	_tcscpy_s(szTestSourceData, inString);

	CWHEncrypt::XorEncrypt(szTestSourceData, outChar, CountArray(szTestSourceData));

	CString  outString;
	outString.Format(TEXT("%s"), outChar);

	return outString;
}

void CCollocateDlg::OnBnClickedButtonloadconfig()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, TEXT(""), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("(*.ini)|*.ini||"));
	if (dlg.DoModal() == IDOK)
	{
		CString fileName = dlg.GetPathName();
		LoadCollocateConfig(fileName);
	}
}

void CCollocateDlg::OnBnClickedButtonsaveconfig()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, TEXT(""), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("(*.ini)|*.ini||"));
	if (dlg.DoModal() == IDOK)
	{
		CString fileName = dlg.GetPathName();
		SaveCollocateConfig(fileName);
	}
}
