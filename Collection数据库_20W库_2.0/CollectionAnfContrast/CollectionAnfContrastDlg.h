
// CollectionAnfContrastDlg.h : ͷ�ļ�
//
#include "Tray.h"
#define WM_SHOW_TASK WM_USER+100

#pragma once
struct ThandScheduling
{
	BOOL					 bWorkState;//�Ƿ��ڹ���
	BOOL					 bIsOnline;//����
	HANDLE				 HEvent; //Event �ź���
	CWinThread *		 HThand;//�߳̾��
	CString					 csFaceDbIdex;//һ����Ա�������
	CString					 csComputerIp;//һ���IP
	CString					 csMaxSum;//��󷵻ظ���
	CString					 csSimilarity;//�Ա����ƶ�
	//CStringArray
	CStringArray					csImgName;//ͼƬ����
	CStringArray					csImgFile;//ͼƬ�ļ���
	int							 nWarningSum;
};

// CCollectionAnfContrastDlg �Ի���
class CCollectionAnfContrastDlg : public CDialogEx
{
// ����
public:
	CCollectionAnfContrastDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_COLLECTIONANFCONTRAST_DIALOG };

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

	CFaceAccessSys m_FaceAccess;
	CFASStatusQuery m_CFASStatusQuery;
	CFASLoginOrLogout m_CFASLoginOrLogout;
	CFASCameraManage m_CFASCameraManage;
	CFASPolicyManage m_CFASPolicyManage;
	CFASRecognition m_CFASRecognition;
	CTray m_tray;

	int m_nSleepTime;
	int m_nBideTime;
	int m_nIsOnlineTime;
	int m_nwarningsum;

	CString m_csCameraSimilarity;
	CString m_csSleepTime;
	CString m_csBideTime;
	CString m_csIsOnlineTime;
	CString m_cswarningsum;

	CString GetiniPath();
	void GetInITime();
	bool GetParameterToDb();
	CString GetImagePath(CString csImageName, CString csFile);
	CString GetWarningPath(CString csImageName, CString csFile);
	BOOL WriteWarningb64(CString csWarning,CString csWarningPath);
	BOOL InitDbParameter();
	int InitComputerThread();
	int UpdataComputerOline();

	afx_msg LRESULT OnShowNotify(WPARAM wParam, LPARAM lParam);


	afx_msg void OnBnClickedButDetectiondb();
	afx_msg void OnBnClickedButStartthand();
	afx_msg void OnBnClickedButSetevent();
	afx_msg void OnDeletetary();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButSetvaluetodb();
	afx_msg void OnBnClickedCancel2();
};
