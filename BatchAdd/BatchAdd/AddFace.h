#pragma once


// AddFace 对话框

class AddFace : public CDialog
{
	DECLARE_DYNAMIC(AddFace)

public:
	AddFace(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~AddFace();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADDFACENAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_csFaceName;
};
