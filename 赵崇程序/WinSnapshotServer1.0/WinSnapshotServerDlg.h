
// WinSnapshotServerDlg.h : ͷ�ļ�
//

#pragma once
#include "Tray.h"
#define WM_SHOW_TASK WM_USER+100

// CWinSnapshotServerDlg �Ի���
class CWinSnapshotServerDlg : public CDialogEx
{
// ����
public:
	CWinSnapshotServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WINSNAPSHOTSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	void PutSnapInfoToDB(CString strIP, char*cJson);
	BOOL GetSnapshotFaceImage(CString strIP, int iHostID, CString strFaceName, int iID,CString strImgpath);
	BOOL GetSnapshotPhotoImage(CString strIP, int iHostID, CString strPhotoName, int iID, CString strImgpath);
	int Login(CString strIP, CString &strSession);
	void ToTray();
	afx_msg LRESULT OnShowNotify(WPARAM wParam, LPARAM lParam);

	CTray m_tray;
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
	afx_msg void OnBnClickedTest();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnExit();
};
