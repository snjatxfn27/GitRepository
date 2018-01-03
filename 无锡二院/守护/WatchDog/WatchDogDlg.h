
// WatchDogDlg.h : 头文件
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
	BOOL  bFlag;  //是否开启
	int	  nTimeIdex;
	int	  nTimeMax;
	CString csPath;
	CString csName;
	HANDLE hProgramHandle;
};

// CWatchDogDlg 对话框
class CWatchDogDlg : public CDialogEx
{
// 构造
public:
	CWatchDogDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WATCHDOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	//界面
	CDC *pMemDC;
	CBitmap *pBitmap;
	CRect m_rClient;	//总界面
	CRect m_rIIS, m_rMYSQL, m_rSBRJ, m_rXTRJ, m_rYPLXJ, m_rZPRJ;//中间6个大图标所占位置
	CRect m_rClose, m_rMin;//关闭和最小化按钮所占位置
	BOOL m_bIIS, m_bMYSQL, m_bSBRJ, m_bXTRJ, m_bYPLXJ, m_bZPRJ;//false代表不在运行，true代表运行正常
	void SetIISImage(BOOL bIIS);//设置心跳运行图片及文字
	void SetMYSQLImage(BOOL bMYSQL);//设置MySQL运行图片及文字
	void SetSBRJImage(BOOL bSBRJ);//设置识别软件运行图片及文字
	void SetXTRJImage(BOOL bXTRJ);//设置心跳软件运行图片及文字
	void SetYPLXJImage(BOOL bYPLXJ);//设置硬盘录像机状态图片及文字
	void SetZPRJImage(BOOL bZPRJ);//设置抓拍软件运行图片及文字
	int m_iDiskPerUse, m_iMemPerUse;//硬盘和内存使用量，百分之几
	void SetDiskPerUse(int iDPU);//设置硬盘使用量
	void SetMemPerUse(int iMPU);//设置内存使用量



	void InitProgram();
	BOOL GetProgramHandle(int nIndex);
	BOOL OpenProgram(int nIndex);
	BOOL CloseProgram(int nIndex);
	BOOL CompareTime(COleDateTime StartTime, int nInexe);
	BOOL IsFindProgram(int nIndex);
	void SetListData(CString cname, CString cevent, COleDateTime ctime);
	bool IsSvcRun(LPCTSTR lpszSvcName);
	void SetProgramIndex(int nInexe, BOOL bFlag);
	void Log_Error(CString csLog);

	int GetDiskCapacity(CStringArray &arNmae, CStringArray &arTotal, CStringArray &arCurrent);
	BOOL GetMySqlState();
	DWORD getMemUsage();
	double getDrive(CString csDrive);

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
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	CListCtrl m_wndListLog;
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
