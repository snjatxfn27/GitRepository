
// ReplaceFileDlg.h : ͷ�ļ�
//

#pragma once


// CReplaceFileDlg �Ի���
class CReplaceFileDlg : public CDialogEx
{
// ����
public:
	CReplaceFileDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_REPLACEFILE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	BOOL m_bQuit;
	void ProcessOneFile(CString strPath,int iCameraID);
	//�����������������ļ��У�����ΪImgpath�����磺20170711
	BOOL CreateAllDirectory(CString strImgpath);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
