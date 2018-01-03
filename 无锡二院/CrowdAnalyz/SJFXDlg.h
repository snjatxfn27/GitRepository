#pragma once
#include "afxwin.h"

#define  IMG_PORT     10000

// CSJFXDlg 对话框

class CSJFXDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSJFXDlg)

public:
	CSJFXDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSJFXDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SJFX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	//动态分析库统计
	struct ZHOUQIZONGLAN
	{
		CRect rBX;

		CRect rNum;
		CString strNum;//编号

		CRect rCount;//次数
		CString iCount;

		CRect rOperate;//操作
	} m_zhouqizhonglan[9];

	CDC *pMemDC;
	CBitmap *pBitmap;
	CRect m_rClient;//对话框总界面
	CRect m_rCurror;//zhouqi
	CRect m_rSelect;//chaxun
	CRect m_rOut;//chaxun
	CRect m_rUp;//chaxun
	CRect m_rNext;//chaxun
	CRect m_rPage;//chaxun
	CRect m_rMaxPage;//chaxun
	CRect m_rListData;//chaxun
	void SetComboxCurrent();
	void getListData();
	BOOL InitDatabaseMigration();
	CString GetSql();
	CString GetXiangqingSql(CString csIdcard);
	BOOL GetSnapIdImg(CString csImgName, CString csSnapshotid, CString csPath, CString &csTime, CString &csCameAdd);

	BOOL GetMaxPaga();
	void SetUp();
	void SetNext();
	BOOL OutList();
	BOOL GetIdcardImgOut(CString csImgName, CString csIdcard, CString csPath);
	BOOL OutParticulars(CString csIdcard, CString csDstImgPath, CString csPath);

	void SetNullJpg(CString csPath);
	BOOL GetIdcardImg(CString csImgName, CString csIdcard, CString csSnapshotid, int nIndex);
	void DisDatalist(Graphics *g);
	int m_iDataLinstCount;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CComboBox m_comCurrect;
	CString m_csCurrect;
	CString m_csPaga;
	int m_nPage;
	int m_nMaxPage;
	CTools m_cTools;

	CString m_csSqlIP;
	CString m_csSqlData;
	CString m_csSqlName;
	CString m_csSqlPwd;
	CString m_csImgIP;

};
