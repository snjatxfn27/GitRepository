#pragma once


// CRTAlarmDlg �Ի���

class CRTAlarmDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRTAlarmDlg)

public:
	CRTAlarmDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRTAlarmDlg();

	//�Ի���λ��
	CRect m_rClient;

	// �Ի�������
	enum { IDD = IDD_DIALOG_RTALARM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
