#pragma once


// CHistoryDlg �Ի���

class CHistoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistoryDlg)

public:
	CHistoryDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHistoryDlg();

	//�Ի���λ��
	CRect m_rClient;
	// �Ի�������
	enum { IDD = IDD_DIALOG_HISTORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
