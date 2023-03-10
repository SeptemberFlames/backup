// CollocateDlg.h : 头文件
//

#pragma once


// CCollocateDlg 对话框
class CCollocateDlg : public CDialog
{
// 构造
public:
	CCollocateDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_COLLOCATE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonloadconfig();
	afx_msg void OnBnClickedButtonsaveconfig();

public:
	CString		IntToCString(int integer);	
	CString		XorEnCryptString(CString inString);

public:
	void		LoadCurrentMachineID();

public:
	void		LoadCollocateConfig(CString fileName);
	void		SaveCollocateConfig(CString fileName);	

public:
	TCHAR		m_szServiceMachine[LEN_MACHINE_ID];
};
