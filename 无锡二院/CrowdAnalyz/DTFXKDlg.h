#pragma once


// CDTFXKDlg 对话框

class CDTFXKDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDTFXKDlg)

public:
	CDTFXKDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDTFXKDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DTFXK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
