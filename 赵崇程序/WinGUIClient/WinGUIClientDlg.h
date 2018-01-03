
// WinGUIClientDlg.h : 头文件
//

#pragma once
#include "TopDlg.h"
#include "VideoDlg.h"
#include "DeviceDlg.h"
#include "BottomDlg.h"
#include "RTAlarmDlg.h"
#include "RTAlarmInfoDlg.h"
#include "HistoryDlg.h"

// CWinGUIClientDlg 对话框
class CWinGUIClientDlg : public CDialogEx
{
// 构造
public:
	CWinGUIClientDlg(CWnd* pParent = NULL);	// 标准构造函数

	//显示区域划分
	CRect m_rClient, m_rTop, m_rVideo, m_rDevice, m_rBottom, m_rRTAlarm, m_rRTAlarmInfo, m_rHistory;
	CTopDlg *m_dlgTop;
	CVideoDlg *m_dlgVideo;
	CDeviceDlg *m_dlgDevice;
	CBottomDlg *m_dlgBottom;
	CRTAlarmDlg *m_dlgRTAlarm;
	CRTAlarmInfoDlg *m_dlgRTAlarmInfo;
	CHistoryDlg *m_dlgHistory;
	void InitClients();
	void RunDialogs();

// 对话框数据
	enum { IDD = IDD_WINGUICLIENT_DIALOG };

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
};
