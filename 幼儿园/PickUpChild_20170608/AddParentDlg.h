#pragma once
#include "afxwin.h"


// CAddParentDlg 对话框

class CAddParentDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddParentDlg)

public:
	CAddParentDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddParentDlg();

//自己的变量和函数
	int m_iChildID;
	CString m_strCurSelPhotoPathName;
	void InitChildData();
// 对话框数据
	enum { IDD = IDD_DIALOG_ADDPARENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
