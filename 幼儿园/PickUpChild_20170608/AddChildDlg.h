#pragma once
#include "afxwin.h"


// CAddChildDlg 对话框

class CAddChildDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddChildDlg)

public:
	CAddChildDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddChildDlg();

//自己的变量和函数
	CString m_strCurSelPhotoPathName;
	void InitComboClass();
// 对话框数据
	enum { IDD = IDD_DIALOG_ADDCHILD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
