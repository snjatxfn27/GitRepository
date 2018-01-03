#pragma once


// CameraSet 对话框

class CameraSet : public CDialog
{
	DECLARE_DYNAMIC(CameraSet)

public:
	CameraSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CameraSet();

// 对话框数据
	enum { IDD = IDD_DLG_VCA_FACE_SNAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButSet();

	CComboBox	m_cmbChannel;
	CComboBox	m_comROI;
	CComboBox	m_comFaceEnable;
	CComboBox	m_comBackgroundPic;
	CComboBox	m_comboMatchType;
	CComboBox	m_comboSensitive;
	CComboBox	m_comboSnapTimes;
	CComboBox	m_comboRuleNum;
	CComboBox	m_comboPicQuality;
	CComboBox	m_comboPicResolution;
	BOOL	m_bChkFilterActive;
	BOOL	m_bDrawPolygon;
	BOOL	m_bChkEnable;
	BYTE	m_byRate;
	BOOL	m_bSetMiniFilter;
	BOOL	m_bSetMaxFilter;
	BYTE	m_bySnapInterval;
	BYTE	m_bySnapThreshold;
	BYTE	m_byReferBright;
	BYTE	m_byMatchThreshold;
	DWORD	m_dwFaceExposureMinDuration;
	DWORD	m_dwTerminalNo;
	DWORD	m_dwUploadInterval;
	DWORD	m_dwValidFaceMinDuration;
	DWORD	m_FaceFilteringTime;

	LONG	 m_lUerID;
	CString m_csCamename;

	NET_VCA_FACESNAPCFG m_struFaceSnap;
	LONG    m_lPlayHandle;
	CRect   m_rcWnd;
	BOOL    m_bCloseIn;
	BOOL    m_bMouseMove;
	BOOL    m_bNeedRedraw;
	DWORD   m_dwPosNum;
	DWORD   m_dwRuleNum;
	BOOL GetFaceSnap();
	void SetFaceSnapToWnd(NET_VCA_FACESNAPCFG const &struFaceSnap);
	void GetFaceSnapFromWnd(NET_VCA_FACESNAPCFG& struFaceSnap);
	BOOL SetFaceSnap();
	BOOL StartPlay();

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckFilterActive();
};
