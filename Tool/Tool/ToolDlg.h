
// ToolDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CToolDlg 对话框
class CToolDlg : public CDialogEx
{
// 构造
public:
	CToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TOOL_DIALOG };

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
	afx_msg void OnBnClickedButLogin();
	BOOL m_bLogin;
	BOOL m_bConnect;
	BOOL m_bWork;
	char pSession[37];
	CString m_csDeviceIp;
	CString m_csLibraryId;
	CString m_csPath;

	BOOL GetConfigIni();

	void BayesCategoryTest(CString tp);
	BOOL TableIsFull(_ConnectionPtr pConnect, CString csTable, int nMax);
	BOOL MoveImg(CString csImgPath, CString csImgName);
	void SetListData(CString csPath, CString csResults);
		
	afx_msg void OnBnClickedButAdd();
	afx_msg void OnBnClickedButConnect();
	CString m_csSqlIp;
	CString m_csSqlName;
	CString m_csMaxNum;
	CString m_csMaxNPeople;
	BOOL m_Bruku;
	afx_msg void OnBnClickedButDelete();
	void ClearDb();
	BOOL GetEmpIdcard(CString &csIdcard);

	CListCtrl m_wndList;
	BOOL m_bBlack;
};
