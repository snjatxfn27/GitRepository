
// GuardServerDlg.h : ͷ�ļ�
//

#pragma once
#include "Log.h"
#include "afxcmn.h"
struct SProgramData
{
	PROCESS_INFORMATION Process;//������������
	STARTUPINFO si;
	COleDateTime cLastTime; //��¼�ϴ�ʱ��
	BOOL  bExist;  //�Ƿ���
	int	  nTimeIdex;
	int	  nTimeMax;
	CString csPath;
	CString csName;
	HANDLE hProgramHandle;
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
