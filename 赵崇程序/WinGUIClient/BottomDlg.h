#pragma once


// CBottomDlg �Ի���

class CBottomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBottomDlg)

public:
	CBottomDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBottomDlg();

	CRect m_rClient;
// �Ի�������
	enum { IDD = IDD_DIALOG_BOTTOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
