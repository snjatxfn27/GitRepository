#pragma once


// MornitorSet 对话框

class MornitorSet : public CDialog
{
	DECLARE_DYNAMIC(MornitorSet)

public:
	MornitorSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MornitorSet();

// 对话框数据
	enum { IDD = IDD_DLG_MONITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
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
	afx_msg void OnBnClickedButSave();
};
