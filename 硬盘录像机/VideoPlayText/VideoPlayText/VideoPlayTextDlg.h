
// VideoPlayTextDlg.h : ͷ�ļ�
//

#pragma once


// CVideoPlayTextDlg �Ի���
class CVideoPlayTextDlg : public CDialogEx
{
// ����
public:
	CVideoPlayTextDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_VIDEOPLAYTEXT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedOk2();
	afx_msg void OnBnClickedOk3();
	LRESULT OnMyMsg(WPARAM wParam, LPARAM lParam);
	LRESULT OnMyMsg1(WPARAM wParam, LPARAM lParam);

};
