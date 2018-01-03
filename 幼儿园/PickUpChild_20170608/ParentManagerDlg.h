#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CParentManagerDlg �Ի���

class CParentManagerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CParentManagerDlg)

public:
	CParentManagerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CParentManagerDlg();

//�Լ��ı����ͺ���
	int m_iCurSelID;
	int m_iCurSelParentID;
	void RefreshListChild();
	void RefreshListParent(int iChildID);
	void InitComboChooseClass();

// �Ի�������
	enum { IDD = IDD_DIALOG_PARENTMANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboChooseClass;
	CListCtrl m_listChild;
	virtual BOOL OnInitDialog();
//	afx_msg void OnNMClickListChild(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickListChild(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	CStatic m_static_childphoto;
	CStatic m_static_parentphoto;
	CListCtrl m_listParent;
	afx_msg void OnClickListParent(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDelparent();
	afx_msg void OnBnClickedButtonAddparent();
	afx_msg void OnSelchangeComboChooseclass();
};
