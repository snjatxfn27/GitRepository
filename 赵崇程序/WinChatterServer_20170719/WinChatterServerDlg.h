
// WinChatterServerDlg.h : 头文件
//

#pragma once
#include "Tray.h"
#define WM_SHOW_TASK WM_USER+100

// CWinChatterServerDlg 对话框
class CWinChatterServerDlg : public CDialogEx
{
// 构造
public:
	CWinChatterServerDlg(CWnd* pParent = NULL);	// 标准构造函数
	BOOL CompareTime(CString strNowTime, CString strLastTime);
	void PutJumpTimeToDB(CString strOnlyID, CString strJumpTime);
	void ToTray();
	afx_msg LRESULT OnShowNotify(WPARAM wParam, LPARAM lParam);

	CTray m_tray;

// 对话框数据
	enum { IDD = IDD_WINCHATTERSERVER_DIALOG };

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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnExit();
};
