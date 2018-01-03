#pragma once
#include "afxwin.h"

#define  IMG_PORT     10000

// CSJFXDlg �Ի���

class CSJFXDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSJFXDlg)

public:
	CSJFXDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSJFXDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SJFX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	//��̬������ͳ��
	struct ZHOUQIZONGLAN
	{
		CRect rBX;

		CRect rNum;
		CString strNum;//���

		CRect rCount;//����
		CString iCount;

		CRect rOperate;//����
	} m_zhouqizhonglan[9];

	CDC *pMemDC;
	CBitmap *pBitmap;
	CRect m_rClient;//�Ի����ܽ���
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
