#pragma once


// CHistoryDlg 对话框

class CHistoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistoryDlg)

public:
	CHistoryDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHistoryDlg();

	//对话框位置
	CRect m_rClient;
	// 对话框数据
	enum { IDD = IDD_DIALOG_HISTORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
