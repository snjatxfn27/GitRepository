#pragma once


// CSetValuToDb �Ի���

class CSetValuToDb : public CDialog
{
	DECLARE_DYNAMIC(CSetValuToDb)

public:
	CSetValuToDb(CWnd* pParent = NULL, int csSimilarity = 0, 
		int csSleeptime = 0, int csBideTime =0,
		int csOnlineTime = 0);   // ��׼���캯��
	virtual ~CSetValuToDb();

// �Ի�������
	enum { IDD = IDD_DLGSETVALUETODB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_CSSimilarity;
	CString m_CSSleep;
	CString m_CSBide;
	CString m_CSOnline;
	virtual BOOL OnInitDialog();
};
