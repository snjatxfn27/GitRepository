
// GuardServerDlg.h : 头文件
//

#pragma once
#include "Log.h"
struct SProgramData
{
	PROCESS_INFORMATION Process;//启动程序配置
	CTime cLastTime; //记录上次时间
	BOOL  bExist;  //是否开启
	int	  nTimeIdex;
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
	BOOL CompareTime(CTime StartTime);
	BOOL IsFindProgram(CString strTitle, int nType);
	void Log_Error(CString csLog);

	CTime m_cTimeOldCollection;
	CTime m_cTimeOldChatter;
	CTime m_cTimeOldSnapshot;

	BOOL m_bCollection;
	BOOL m_bChatter;
	BOOL m_bSnapsho;
};
