#pragma once


// MornitorSet �Ի���

class MornitorSet : public CDialog
{
	DECLARE_DYNAMIC(MornitorSet)

public:
	MornitorSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MornitorSet();

// �Ի�������
	enum { IDD = IDD_DLG_MONITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
