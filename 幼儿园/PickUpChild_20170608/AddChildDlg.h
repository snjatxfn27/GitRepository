#pragma once
#include "afxwin.h"


// CAddChildDlg �Ի���

class CAddChildDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddChildDlg)

public:
	CAddChildDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddChildDlg();

//�Լ��ı����ͺ���
	CString m_strCurSelPhotoPathName;
	void InitComboClass();
// �Ի�������
	enum { IDD = IDD_DIALOG_ADDCHILD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strChildName;
	CComboBox m_comboChildClass;
	CStatic m_static_photo;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
};
