
// CameraMonitorDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"

typedef struct tagLOCAL_ALARM_INFO
{
	int iDeviceIndex;
	LONG lCommand;
	char sDeviceIP[128];    			/* IP��ַ */
	DWORD dwBufLen;
	tagLOCAL_ALARM_INFO()
	{
		iDeviceIndex = -1;
		lCommand = -1;
		memset(&sDeviceIP, 0, 128);
		dwBufLen = 0;
	}
}LOCAL_ALARM_INFO, *LPLOCAL_ALARM_INFO;

typedef struct STRU_CAMERA_INFO
{
	int nArrayIndex;
	int nCameraIndex;
	int nDeviceIndex;
	

	CString csCameraIp;
	CString csCameraAddr;
	CString csCameraState;
	CString csCameraType;
	CString csCameraAddrPath;


	int nPort;
	LONG lUserID; //��¼ID
	LONG lFortifyHandle;//����id
	LONG lPlayHandle;//����id
	CString csCameraName;
	CString csCameraPwd;

	HTREEITEM hCamera;

	STRU_CAMERA_INFO()
	{
		nCameraIndex = -1;
		nDeviceIndex = -1;
		nArrayIndex = -1;
		lUserID = -1;
		lFortifyHandle = -1;
		lPlayHandle = -1;
		nPort = 8000;

		csCameraAddrPath = "";
		csCameraIp = "";
		csCameraAddr = "";
		csCameraState = "";
		csCameraType = "";
		csCameraName = "admin";
		csCameraPwd = "unisib1000";
	}

}CAMERA_INFO, *pCAMERA_INFO;;


// CCameraMonitorDlg �Ի���
class CCameraMonitorDlg : public CDialogEx
{
// ����
public:
	CCameraMonitorDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CAMERAMONITOR_DIALOG };

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
	CImageList m_TreeImage;
	CTreeCtrl	m_wndTreeCamera;
	//�������ñ���
	CComboBox	m_comFaceAlarmType;
	CComboBox   m_comDeployType;
	CComboBox	m_comAlarmLevel;
	int			m_iTaskNo;
	BOOL		m_bChkBKN;
	BOOL		m_bChkPDC;
	BOOL		m_bChkHeatMap;
	BOOL		m_bConfirmMechanism;
	BOOL		m_bChkFaceSnap;
	BOOL		m_bChkJsonURL;
	BOOL		m_bFaceMatch;
	BOOL		m_bCopilotFace;
	BOOL		m_bIsPlaying;
	//end
	
	CString m_csDatabaseIp;
	CString m_csDatabaseTableName;
	CString m_csDatabaseName;
	CString m_csDatabasePwd;

	int			m_nPlayingIndex;
	int			m_nCameraIndex;


	BOOL LogIn(int nCameraInfoIndex);
	BOOL StartPlay(int nCameraInfoIndex);
	BOOL StopPlay(int nCameraInfoIndex);
	BOOL DoLogout(int nCameraInfoIndex);

	LRESULT OnWMProcAlarm(WPARAM wParam, LPARAM lParam);

	void ProcFaceSnapAlarm(WPARAM wParam, LPARAM lParam);

	BOOL SetupAlarmChan(int nCameraInfoIndex);
	BOOL CloseAlarmChan(int nCameraInfoIndex);

	BOOL GetParameterIni();
	BOOL InitDatabase();
	BOOL InitDeviceInfo();
	void CreateTree();
	void LoadTreeImage();

	void SetListOut(CString csName, CString csCameIp, CString csResults);

	afx_msg void OnNMRClickTreeCamera(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_wndList;
	afx_msg void OnNMDblclkTreeCamera(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButStop();
	afx_msg void OnLvnGetInfoTipList1(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL CreateImgFile();
	CString GetDirectoryPath();

	afx_msg void OnBnClickedButtSavepath();
	void ProgramStart();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSetalaem();
	afx_msg void OnClosealaem();
	afx_msg void OnSetattribute();
	afx_msg void OnMenureboot();
};
