
// FaceComparDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "Tools.h"
#define  MAX_LEN  1024 * 1024 * 6
// CFaceComparDlg �Ի���
class CFaceComparDlg : public CDialogEx
{
// ����
public:
	CFaceComparDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FACECOMPAR_DIALOG };

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
	int OpenFileGetPath(CString &csPath);

	BOOL UpdatComputerTree();
	BOOL InitDbParameter();
	BOOL GetComputerTreeCheck(HTREEITEM  hFather);
	BOOL GetFaceTreeCheck(HTREEITEM  hFather);
	void ComparFace(CString csIP,CString csID);
	CTreeCtrl m_ComputerTree;
	CString   m_csImgPath;
	CString m_csFaceIp;
	char m_pSession[19];
	char m_pImage[MAX_LEN];
	char m_pFeature[MAX_LEN];

	_ConnectionPtr  m_pConnect;

	HTREEITEM m_hRoot;//һ��������ڵ�
	HTREEITEM m_hComputer;//һ�������ʱ��¼���ڵ㣬����������ݿ�ʱ����ڵ��жϻ�ȡһ���ip
	CListCtrl m_wndComparList;


	CFaceAccessSys m_FaceAccess; //�·�ע��
	CFASStatusQuery m_CFASStatusQuery;//���������
	CFASLoginOrLogout m_CFASLoginOrLogout;//��¼��ǳ�
	CFASRecognition m_CFASRecognition;

	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButOpen();
	afx_msg void OnBnClickedButcompar();
	CString m_csName;
	CString m_csAge;
	CString m_csGender;
	// ����
	CString m_csOrigin;
	CString m_csAddress;
	CString m_csTelephone;
	// ֤����
	CString m_csCertificates;
	CString m_csSimilar;
	CString m_csMaxSum;
};
