#pragma once


// CXTSZDlg 对话框

class CXTSZDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CXTSZDlg)

public:
	CXTSZDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CXTSZDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_XTSZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();


	//界面
	CDC *pMemDC;
	CBitmap *pBitmap;
	CRect m_rClient;//对话框总界面
	CRect m_rJkzq;//
	CRect m_rBbsj;//
	CRect m_rXsd;//
	CRect m_rBC;//
	COLORREF m_colorEditText; // edit控件的字体颜色
	COLORREF m_colorEditBK; // edit控件的背景颜色
	CBrush* m_pEditBkBrush;
	CString m_csSim;

	BOOL GetConfig();
	BOOL SetConfig();
	BOOL PreTranslateMessage(MSG* pMsg);


	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString m_csJiankong;
	CString m_csShijian;
	CString m_csXiangsidu;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
