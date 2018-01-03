#pragma once


// CRTAlarmDlg 对话框

class CRTAlarmDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRTAlarmDlg)

public:
	CRTAlarmDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRTAlarmDlg();

	//对话框位置
	CRect m_rClient;

	// 对话框数据
	enum { IDD = IDD_DIALOG_RTALARM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
