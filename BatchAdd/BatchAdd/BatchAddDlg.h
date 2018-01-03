
// BatchAddDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "AddFace.h"
typedef struct STRU_DEVICE_INFO
{
	BOOL    bIsonline;
	BOOL    bIsWork;
	BOOL    bIsStatic;//是否为静态服务器
	BOOL	bBlackOrWhite;
	CString csDeviceIp;
	CString DeviceName;
	CString csDescription;//描述
	CString csType;
	CString csDeviceId;
	CString csOnlyId;
	CString csAddr;
	CString csRemark;
	CString csIsFaceServer;
	CString csBlaekFacedbs;//黑名单 
	CString csWhiteFacedbs;//白名单
	int		nBlaekFacedbs;
	int		nWhiteFacedbs;
	int		nIndex;
	map<CString, CString> mapBlackFacedb;
	map<CString, CString> mapWhiteFacedb;

	CString csFacedb;
	CStringArray arFacePath;

	CWinThread * hThand;//线程句柄

	STRU_DEVICE_INFO()
	{
		bIsonline = FALSE;
		bIsWork = FALSE;
		bIsStatic = FALSE;
		bBlackOrWhite = FALSE;
		csType = "";
		csDeviceIp = "";
		DeviceName = "";
		csDescription = "";
		csBlaekFacedbs = "";
		csFacedb = "";
		csAddr = "";
		csRemark = "";
		csIsFaceServer = "";
		csOnlyId = "";
		csWhiteFacedbs = "";
		nBlaekFacedbs = 0;
		nWhiteFacedbs = 0;
		nIndex = 0;
		hThand = NULL;
	}

}DEVICE_INFO, *pDEVICE_INFO;


typedef struct STRU_TREE_INFO
{
	CString csDeviceIp;
	CString csDeviceId;
	CString csFaceID;
	CString DeviceName;
	CString csType;
	int		nFlag;
	int		nDeviceIndex;

	STRU_TREE_INFO()
	{
		csFaceID = "";
		csDeviceIp = "";
		DeviceName = "";
		csType = "";
		csDeviceId = "";
		nFlag = 0;
	}

}THREE_INFO, *pTHREE_INFO;

// CBatchAddDlg 对话框
class CBatchAddDlg : public CDialogEx
{
// 构造
public:
	CBatchAddDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BATCHADD_DIALOG };

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

	BOOL m_bInitDb;
	BOOL m_bFaceBorW;//ture 是黑名单
	BOOL m_bSelectPath;
	BOOL m_bAddFalg;
	CTreeCtrl m_wndBlackTree;
	CTreeCtrl m_wndWhiteTree;

	HTREEITEM m_hBlackRoot;
	HTREEITEM m_hBlackSelect;
	HTREEITEM m_hWhiteRoot;
	HTREEITEM m_hWhiteSelect;

	CString m_csContrastServerDB_Ip;
	CString m_csContrastServerDB_Name;
	CString m_csContrastServerDB_Pwd;
	CString m_csContrastServerDB_Database;
	CString m_csContrastServerDB_Capacity;
	int		m_nContrastServerDB_Capacity;

	CString m_csImgPath;
public:
	BOOL GetParameterIni();
	BOOL InitDatabase();
	BOOL InitDeviceInfo();
	BOOL GetDeviceFaceNmae(CString csDeviceId ,int nDeviceIndex, CString csFacedbs, int nFaceType);
	BOOL CreateTableToMySql(int nIndex, CString csDeviceId);
	BOOL CreateListAndImgToSqlServer(int nIndex, CString csDeviceId);
	BOOL ShowBalackTree();
	BOOL ShowWhiteTree();
	void SetListData(CString csPath, CString csResults, CString csIp);
	int TraversalBlackTree();
	int TraversalWhiteTree();
	void BayesCategoryTest(CString tp);
	BOOL VerdictCombox();
	BOOL SaveToSqlSrver(CString csFaceBdId, CString csFaceDbPersonId, CString csName, CString csGender, CString csBirthday, CString csAge, CString csNational, CString csPhone, CString csAddr, CString csDeviceID, CString csIdcard, CString csPresentId);

	BOOL GetPersonsListName(CString csDeviceId,int & PersonsIndex);
	void CreateImgFile();

	int GetTableSum(CString csTableName);

	BOOL MoveImg(CString csImgPath, int nType);

	afx_msg void OnBnClickedButtonConnectdb();
	BOOL InsertPersonsListCount(CString csTableName, int nIndex,CString csDeviceId);



	afx_msg void OnNMClickTreeBlack(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTreeWhite(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickTreeBlack(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAddface();
	afx_msg void OnDeleteface();
	afx_msg void OnEditface();
	afx_msg void OnUpdatatree();
	afx_msg void OnNMRClickTreeWhite(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_wndList;
	afx_msg void OnBnClickedButtonSelect();
	afx_msg void OnBnClickedButtonAdd();


	CComboBox m_wndComName;
	CString m_csComName;

	CComboBox m_wndComIdcard;
	CString m_csComIdcard;

	CComboBox m_wndComGender;
	CString m_csComGender;

	CComboBox m_wndComBitrhday; 
	CString m_csComBitrhday;

	CComboBox m_wndComNational;
	CString m_csComnational;

	CComboBox m_wndComAddress;
	CString m_csComAddress;

	CComboBox m_wndComPhone;
	CString m_csComPhone;

	CComboBox m_wndComSeparator;
	CString m_csComSeparator;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLvnGetInfoTipList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAddserver();
	afx_msg void OnEditserver();
	afx_msg void OnDeleteserver();
	BOOL ClearDeviceInFaceId(CString csDevicId, CString csFaceId);

	BOOL ClearDeviceFaceDbs(CString csDevicId, CString csFaceId);
};
