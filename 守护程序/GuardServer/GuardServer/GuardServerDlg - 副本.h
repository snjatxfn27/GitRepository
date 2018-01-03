
// GuardServerDlg.h : ͷ�ļ�
//

#pragma once
#include "Log.h"
struct SProgramData
{
	PROCESS_INFORMATION Process;//������������
	CTime cLastTime; //��¼�ϴ�ʱ��
	BOOL  bExist;  //�Ƿ���
	int	  nTimeIdex;
};
// CGuardServerDlg �Ի���
class CGuardServerDlg : public CDialogEx
{
// ����
public:
	CGuardServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GUARDSERVER_DIALOG };

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
