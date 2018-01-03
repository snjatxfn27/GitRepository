
// WinSnapshotServerDlg.h : 头文件
//

#pragma once
#include "Tray.h"
#define WM_SHOW_TASK WM_USER+100

// CWinSnapshotServerDlg 对话框
class CWinSnapshotServerDlg : public CDialogEx
{
// 构造
public:
	CWinSnapshotServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WINSNAPSHOTSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	void PutSnapInfoToDB(CString strIP, char*cJson);
	BOOL GetSnapshotFaceImage(CString strIP, int iHostID, CString strFaceName, int iID,CString strImgpath);
	BOOL GetSnapshotPhotoImage(CString strIP, int iHostID, CString strPhotoName, int iID, CString strImgpath);
	int Login(CString strIP, CString &strSession);
	void ToTray();
	afx_msg LRESULT OnShowNotify(WPARAM wParam, LPARAM lParam);

	CTray m_tray;
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
	afx_msg void OnBnClickedTest();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnExit();
};
