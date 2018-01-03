#pragma once


// CAddServer 对话框

class CAddServer : public CDialog
{
	DECLARE_DYNAMIC(CAddServer)

public:
	CAddServer(CWnd* pParent = NULL,int nType = 0);   // 标准构造函数
	virtual ~CAddServer();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADDSERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
