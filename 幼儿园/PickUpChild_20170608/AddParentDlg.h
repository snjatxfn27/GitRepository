#pragma once
#include "afxwin.h"


// CAddParentDlg �Ի���

class CAddParentDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddParentDlg)

public:
	CAddParentDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddParentDlg();

//�Լ��ı����ͺ���
	int m_iChildID;
	CString m_strCurSelPhotoPathName;
	void InitChildData();
// �Ի�������
	enum { IDD = IDD_DIALOG_ADDPARENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strChildName;
	CString m_strClassName;
	afx_msg void OnBnClickedButtonChoosephoto();
	CStatic m_static_photo;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	CString m_strParentName;
	CString m_strParentRelation;
};
