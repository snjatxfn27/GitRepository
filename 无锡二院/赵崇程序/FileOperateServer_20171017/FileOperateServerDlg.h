
// FileOperateServerDlg.h : ͷ�ļ�
//

#pragma once


// CFileOperateServerDlg �Ի���
class CFileOperateServerDlg : public CDialogEx
{
// ����
public:
	CFileOperateServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FILEOPERATESERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
public:
	SOCKET sockSrv;
	SOCKET sockConn;
	void CreateRecvThread();
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
