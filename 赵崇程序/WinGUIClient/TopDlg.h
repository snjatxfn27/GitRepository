#pragma once


// CTopDlg �Ի���

class CTopDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTopDlg)

public:
	CTopDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTopDlg();

	//�Ի���λ��
	CRect m_rClient;

// �Ի�������
	enum { IDD = IDD_DIALOG_TOP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
