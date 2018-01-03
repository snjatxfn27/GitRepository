
// FaceComparDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "Tools.h"
#define  MAX_LEN  1024 * 1024 * 6
// CFaceComparDlg 对话框
class CFaceComparDlg : public CDialogEx
{
// 构造
public:
	CFaceComparDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FACECOMPAR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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

	HTREEITEM m_hRoot;//一体机树根节点
	HTREEITEM m_hComputer;//一体机单击时记录树节点，用于添加数据库时与根节点判断获取一体机ip
	CListCtrl m_wndComparList;


	CFaceAccessSys m_FaceAccess; //下发注册
	CFASStatusQuery m_CFASStatusQuery;//人脸库管理
	CFASLoginOrLogout m_CFASLoginOrLogout;//登录与登出
	CFASRecognition m_CFASRecognition;

	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButOpen();
	afx_msg void OnBnClickedButcompar();
	CString m_csName;
	CString m_csAge;
	CString m_csGender;
	// 籍贯
	CString m_csOrigin;
	CString m_csAddress;
	CString m_csTelephone;
	// 证件号
	CString m_csCertificates;
	CString m_csSimilar;
	CString m_csMaxSum;
};
