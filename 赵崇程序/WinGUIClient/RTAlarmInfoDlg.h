#pragma once


// CRTAlarmInfoDlg �Ի���

class CRTAlarmInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRTAlarmInfoDlg)

public:
	CRTAlarmInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRTAlarmInfoDlg();

	//�Ի���λ��
	CRect m_rClient;
	// �Ի�������
	enum { IDD = IDD_DIALOG_RTALARMINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
