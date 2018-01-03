
// GuardServerDlg.h : 头文件
//

#pragma once
#include "Log.h"
#include "afxcmn.h"
struct SProgramData
{
	PROCESS_INFORMATION Process;//启动程序配置
	STARTUPINFO si;
	COleDateTime cLastTime; //记录上次时间
	BOOL  bExist;  //是否开启
	int	  nTimeIdex;
	int	  nTimeMax;
	CString csPath;
	CString csName;
	HANDLE hProgramHandle;
};
// CGuardServerDlg 对话框
class CGuardServerDlg : public CDialogEx
{
// 构造
public:
	CGuardServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GUARDSERVER_DIALOG };

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
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void InitProgram();
	BOOL GetProgramHandle(int nIndex);
	BOOL OpenProgram(int nIndex);
	BOOL CloseProgram(int nIndex);
	BOOL CompareTime(COleDateTime StartTime, int nInexe);
	BOOL IsFindProgram(int nIndex);
	void Log_Error(CString csLog);

	void SetListData(CString cname, CString cstate, CString cevent, COleDateTime ctime);

	int GetDiskCapacity(CStringArray &arNmae, CStringArray &arTotal, CStringArray &arCurrent);
	BOOL GetMySqlState();
	CListCtrl m_wndListLog;
};
