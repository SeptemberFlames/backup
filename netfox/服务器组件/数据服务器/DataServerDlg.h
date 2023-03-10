#ifndef LOGON_SERVER_SERVER_DLG_HEAD_FILE
#define LOGON_SERVER_SERVER_DLG_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ServiceUnits.h"
#include ".\flashaccredit.h"

//////////////////////////////////////////////////////////////////////////////////
//http控制信息配置
struct stuHttpSettingInfo
{
	bool			bConWin;
	int				nConRate;
	LONGLONG		llConScore;
};

//主对话框
class CDataServerDlg : public CDialog, public IServiceUnitsSink
{
	DECLARE_MESSAGE_MAP()

	//组件变量
protected:
	CServiceUnits					m_ServiceUnits;						//服务单元
	CTraceServiceControl			m_TraceServiceControl;				//追踪窗口
	CFlashAccredit                  m_FlashAccredit;                    //网络

	//函数定义
public:
	//构造函数
	CDataServerDlg();
	//析构函数
	virtual ~CDataServerDlg();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定消息
	virtual VOID OnOK();
	//取消函数
	virtual VOID OnCancel();
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//服务接口
public:
	//服务状态
	virtual VOID OnServiceUnitsStatus(enServiceStatus ServiceStatus);

	//按钮消息
protected:
	//启动服务
	VOID OnBnClickedStartService();
	//停止服务
	VOID OnBnClickedStopService();

	//消息映射
public:
	//关闭询问
	BOOL OnQueryEndSession();
	//网络事件
	LRESULT OnNetworkEvent(WPARAM wparam, LPARAM lparam);
	//命令处理
	LRESULT OnMessageProcessCmdLine(WPARAM wParam, LPARAM lParam);
	//进程退出
	LRESULT OnMessageProcessQuit(WPARAM wParam, LPARAM lParam);
};

//////////////////////////////////////////////////////////////////////////////////

#endif