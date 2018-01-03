#pragma once
#include "afxwin.h"
#include "Tools.h"

// CLSJLDlg 对话框

class CLSJLDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLSJLDlg)

public:
	CLSJLDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLSJLDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_LSJL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();


	//界面
	COLORREF m_colorEditText; // edit控件的字体颜色
	COLORREF m_colorEditBK; // edit控件的背景颜色
	CBrush* m_pEditBkBrush;
	void DisDatalist(Graphics *g);
	void DisXiangqing(Graphics *g);
	void InitRet();

	struct DATA_LIST
	{
		CRect rZP;
		CRect rBX;
		CRect rCZ;

		CRect rXiangsidu;
		CString csXiangsidu;

		CRect rIdcard;
		CString csIdcard;

		CRect rFenlei;
		CString csFenlei;

		CRect rBaojingyuan;
		CString csBaojingyuan;

		CRect rPos;
		CString csPos;

		CRect rTime;
		CString csTime;

	} m_DataList[9];

	int m_iDataLinstCount;

	CDC *pMemDC;
	CBitmap *pBitmap;
	CRect m_rClient;//对话框总界面
	CRect m_rDataList;//对话框总界面

	CRect m_rXingming;
	CRect m_rXingbie;
	CRect m_rWeizhi;

	CRect m_rFanwei1;
	CRect m_rFanwei2;
	CRect m_rXiangsidu;
	
	CRect m_rSousuo;

	CRect m_rUp;
	CRect m_rNext;
	CRect m_rPaga;
	CRect m_rTuXiang;
	CRect m_rRenLian;
	CRect m_rBaxiang;
	BOOL m_bShowJpg;

	CString m_csSqlIP;
	CString m_csSqlData;
	CString m_csSqlName;
	CString m_csSqlPwd;
	CString GetiniPath();
	BOOL InitDatabaseMigration();
	BOOL GetDlgInfo();
	BOOL GetDataList();
	BOOL SetDataList(int nIndex);

	CString GetCurrectSql(CString csTable, CString csStartTime, CString csEditTime, int nType);
	void SetNullJpg(CString csPath);
	BOOL GetempInfo(CString csIdcard, int nIndex);

	BOOL GetMaxCurrent(CString csMaxCurrnet, CString &csStartTime, CString &csEndTime);
	BOOL GetMaxPaga();
	void SetUp();
	void SetNext();
	BOOL ShowXiangxi();

	CString m_csName;
	CString m_csImgIP;
	CString m_csXiangsidu;
	CComboBox m_comXingbie;
	CComboBox m_comWeizhi;
	CComboBox m_comFanwei1;
	CComboBox m_comFanwei2;

// 	CComboBoxXP m_comXingbie;
// 	CComboBoxXP m_comWeizhi;
// 	CComboBoxXP m_comFanwei1;
// 	CComboBoxXP m_comFanwei2;

	CString m_csXingbie;
	CString m_csWeizhi;
	CString m_csFanwei1;
	CString m_csFanwei2;
	CString m_csEndSql;
	CTools m_cTools;

	BOOL GetIdcardImg(CString csImgName, CString csIdcard, CString csSnapshotid, int nIndex);

	int m_nPagaNum;
	int m_nPagasMax;
	CString m_csPaga;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
