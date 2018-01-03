#pragma once


// CRTAlarmInfoDlg 对话框

class CRTAlarmInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRTAlarmInfoDlg)

public:
	CRTAlarmInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRTAlarmInfoDlg();

	//对话框位置
	CRect m_rClient;
	// 对话框数据
	enum { IDD = IDD_DIALOG_RTALARMINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
