#pragma once


// CDTFXKDlg �Ի���

class CDTFXKDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDTFXKDlg)

public:
	CDTFXKDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDTFXKDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DTFXK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
