
// VideoRecordServerDlg.h : 头文件
//

#pragma once
#include "Tray.h"
#define WM_SHOW_TASK WM_USER+200
struct ThandScheduling
{
	BOOL				 bWorkState;//是否在工作
	HANDLE				 HEvent; //Event 信号量
	
	CWinThread *		 HThand;//线程句柄
	int					 nDifferenceTime;

	CString				csSnapshotid;
	CString				csWarningid;
	CString				csVideoTime;
	CString				csCameraIp;
	CString				csFile;
};
// CVideoRecordServerDlg 对话框
class CVideoRecordServerDlg : public CDialogEx
{
	// 构造
public:
	CVideoRecordServerDlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_VIDEORECORDSERVER_DIALOG };

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
	BOOL   m_bIsLogIn;


	CString m_csDifferenceTime;
	CString m_csThreadSum;
	CString m_cslDevIp;
	CString m_cslDevPort;
	CString m_cslDevUser;
	CString m_cslDevPwd;

	CTray m_tray;

	int m_nThreadSum;

	CString GetiniPath();
	CString GetVideoPath();

	BOOL InitDbParameter();

	BOOL  GetDBParameterini();

	int InitThreadToSum();

	BOOL GetVideoTime(CString csTime, CString &csStartTime, CString &csEndTime, int nTime);
	LRESULT OnDownLoadMessageEnd(WPARAM wParam, LPARAM lParam);
	LRESULT OnDownLoadMessageStorp(WPARAM wParam, LPARAM lParam);
	LRESULT OnDownLoadMessageError(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowNotify(WPARAM wParam, LPARAM lParam);

	
	
	afx_msg void OnBnClickedButStart();
	afx_msg void OnBnClickedButStart2();
	afx_msg void OnExit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButTesting();
};
