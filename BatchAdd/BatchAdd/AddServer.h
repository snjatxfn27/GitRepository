#pragma once


// CAddServer �Ի���

class CAddServer : public CDialog
{
	DECLARE_DYNAMIC(CAddServer)

public:
	CAddServer(CWnd* pParent = NULL,int nType = 0);   // ��׼���캯��
	virtual ~CAddServer();

// �Ի�������
	enum { IDD = IDD_DIALOG_ADDSERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_csIp;
	CString m_csName;
	CString m_csDescription;
	CString m_csAddr;
	CString m_csMemark;
	CString m_csOnlyId;
	BOOL m_bInorOut;
	BOOL m_bStatic;
	int m_nInOrOut;
	int m_nStatic;
	int m_nType;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
