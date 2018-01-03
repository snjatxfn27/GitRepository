
// DisWatchDogDlg.h : 头文件
//
#pragma once
#include "Log.h"

struct SProgramData
{
	PROCESS_INFORMATION Process;//启动程序配置
	STARTUPINFO si;
	COleDateTime cLastTime; //记录上次时间
	BOOL  bExist;  //是否开启
	BOOL  bFlag;  //是否开启
	int	  nTimeIdex;
	int	  nTimeMax;
	CString csPath;
	CString csName;
	HANDLE hProgramHandle;
};
// CDisWatchDogDlg 对话框
class CDisWatchDogDlg : public CDialogEx
{
// 构造
public:
	CDisWatchDogDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DISWATCHDOG_DIALOG };

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

	void InitProgram();
	BOOL GetProgramHandle(int nIndex);
	BOOL OpenProgram(int nIndex);
	BOOL CloseProgram(int nIndex);
	BOOL CompareTime(COleDateTime StartTime, int nInexe);
	BOOL IsFindProgram(int nIndex);
	void Log_Error(CString csLog);

};
