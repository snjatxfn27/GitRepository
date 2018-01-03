
// VideoRecordServerDlg.h : ͷ�ļ�
//

#pragma once
#include "Tray.h"
#define WM_SHOW_TASK WM_USER+200
struct ThandScheduling
{
	BOOL				 bWorkState;//�Ƿ��ڹ���
	HANDLE				 HEvent; //Event �ź���
	
	CWinThread *		 HThand;//�߳̾��
	int					 nDifferenceTime;

	CString				csSnapshotid;
	CString				csWarningid;
	CString				csVideoTime;
	CString				csCameraIp;
	CString				csFile;
};
// CVideoRecordServerDlg �Ի���
class CVideoRecordServerDlg : public CDialogEx
{
	// ����
public:
	CVideoRecordServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

	// �Ի�������
	enum { IDD = IDD_VIDEORECORDSERVER_DIALOG };

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
