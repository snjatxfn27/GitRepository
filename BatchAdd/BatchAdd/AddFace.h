#pragma once


// AddFace �Ի���

class AddFace : public CDialog
{
	DECLARE_DYNAMIC(AddFace)

public:
	AddFace(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~AddFace();

// �Ի�������
	enum { IDD = IDD_DIALOG_ADDFACENAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_csFaceName;
};
