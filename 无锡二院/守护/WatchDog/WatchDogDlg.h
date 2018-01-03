
// WatchDogDlg.h : ͷ�ļ�
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
	BOOL  bFlag;  //�Ƿ���
	int	  nTimeIdex;
	int	  nTimeMax;
	CString csPath;
	CString csName;
	HANDLE hProgramHandle;
};

// CWatchDogDlg �Ի���
class CWatchDogDlg : public CDialogEx
{
// ����
public:
	CWatchDogDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WATCHDOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	//����
	CDC *pMemDC;
	CBitmap *pBitmap;
	CRect m_rClient;	//�ܽ���
	CRect m_rIIS, m_rMYSQL, m_rSBRJ, m_rXTRJ, m_rYPLXJ, m_rZPRJ;//�м�6����ͼ����ռλ��
	CRect m_rClose, m_rMin;//�رպ���С����ť��ռλ��
	BOOL m_bIIS, m_bMYSQL, m_bSBRJ, m_bXTRJ, m_bYPLXJ, m_bZPRJ;//false���������У�true������������
	void SetIISImage(BOOL bIIS);//������������ͼƬ������
	void SetMYSQLImage(BOOL bMYSQL);//����MySQL����ͼƬ������
	void SetSBRJImage(BOOL bSBRJ);//����ʶ���������ͼƬ������
	void SetXTRJImage(BOOL bXTRJ);//���������������ͼƬ������
	void SetYPLXJImage(BOOL bYPLXJ);//����Ӳ��¼���״̬ͼƬ������
	void SetZPRJImage(BOOL bZPRJ);//����ץ���������ͼƬ������
	int m_iDiskPerUse, m_iMemPerUse;//Ӳ�̺��ڴ�ʹ�������ٷ�֮��
	void SetDiskPerUse(int iDPU);//����Ӳ��ʹ����
	void SetMemPerUse(int iMPU);//�����ڴ�ʹ����



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
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	CListCtrl m_wndListLog;
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
