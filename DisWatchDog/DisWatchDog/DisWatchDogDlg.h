
// DisWatchDogDlg.h : ͷ�ļ�
//
#pragma once
#include "Log.h"

struct SProgramData
{
	PROCESS_INFORMATION Process;//������������
	STARTUPINFO si;
	COleDateTime cLastTime; //��¼�ϴ�ʱ��
	BOOL  bExist;  //�Ƿ���
	BOOL  bFlag;  //�Ƿ���
	int	  nTimeIdex;
	int	  nTimeMax;
	CString csPath;
	CString csName;
	HANDLE hProgramHandle;
};
// CDisWatchDogDlg �Ի���
class CDisWatchDogDlg : public CDialogEx
{
// ����
public:
	CDisWatchDogDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DISWATCHDOG_DIALOG };

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

	void InitProgram();
	BOOL GetProgramHandle(int nIndex);
	BOOL OpenProgram(int nIndex);
	BOOL CloseProgram(int nIndex);
	BOOL CompareTime(COleDateTime StartTime, int nInexe);
	BOOL IsFindProgram(int nIndex);
	void Log_Error(CString csLog);

};
