
// WinChatterServerDlg.h : ͷ�ļ�
//

#pragma once
#include "Tray.h"
#define WM_SHOW_TASK WM_USER+100

// CWinChatterServerDlg �Ի���
class CWinChatterServerDlg : public CDialogEx
{
// ����
public:
	CWinChatterServerDlg(CWnd* pParent = NULL);	// ��׼���캯��
	BOOL CompareTime(CString strNowTime, CString strLastTime);
	void PutJumpTimeToDB(CString strOnlyID, CString strJumpTime);
	void ToTray();
	afx_msg LRESULT OnShowNotify(WPARAM wParam, LPARAM lParam);

	CTray m_tray;

// �Ի�������
	enum { IDD = IDD_WINCHATTERSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnExit();
};
