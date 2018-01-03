#pragma once


// CTopDlg 对话框

class CTopDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTopDlg)

public:
	CTopDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTopDlg();

	//对话框位置
	CRect m_rClient;

// 对话框数据
	enum { IDD = IDD_DIALOG_TOP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
