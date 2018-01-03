#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CChildManagerDlg �Ի���

class CChildManagerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChildManagerDlg)

public:
	CChildManagerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChildManagerDlg();

//�Լ��ı����ͺ���
	int m_iCurSelID;
	CString m_strCurSelPhotoPathName;
	void RefreshListChild();
	void InitComboChooseClass();
	void InitComboEditClass();
// �Ի�������
	enum { IDD = IDD_DIALOG_CHILDMANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboChooseClass;
	CListCtrl m_listChild;
	CComboBox m_comboEditClass;
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboChooseclass();
	afx_msg void OnPaint();
	afx_msg void OnClickListChild(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic m_static_photo;
	afx_msg void OnBnClickedButtonAddchild();
	afx_msg void OnBnClickedButtonChangephoto();
	afx_msg void OnBnClickedOk();
};
