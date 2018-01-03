#pragma once


// CSBGLDlg 对话框

class CSBGLDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSBGLDlg)

public:
	CSBGLDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSBGLDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SBGL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);

	//动态分析库统计
	struct DATA_LIST
	{
		CRect rXQ;
		CRect rSC;
		CRect rBJ;

		CRect rIP;
		CString csIP;//ip

		CRect rPos;//位置
		CString strPos;

		CRect rState;//状态
		CString csState;

		CString csFaceDbs;
	} m_DataList[21];

	int m_iDisTongjiCount;



	//界面
	COLORREF m_colorEditText; // edit控件的字体颜色
	COLORREF m_colorEditBK; // edit控件的背景颜色
	CBrush* m_pEditBkBrush;


	CDC *pMemDC;
	CBitmap *pBitmap;
	CRect m_rClient;//对话框总界面
	CRect m_rTable;//
	CRect m_rData;//



	CRect m_rFuwuqi;//
	CRect m_rXiangji;//
	CRect m_rBaocun;//
	CRect m_rQuxiao;//
	CRect m_rTianjia;//

	CRect m_rShebei;//
	CRect m_rIp;//
	CRect m_rPos;//

	CRect m_rUp;//
	CRect m_rNext;//
	CRect m_rPaga;//
	int	m_nPagaNum;
	int m_nPagasMax;
	int m_nRedact;
	int m_nSelectIndex;
	void InitRet();
	BOOL GetDeviceInfo(int nIndex);
	BOOL SetDeviceInfo(int nIndex);
	BOOL SetCameraInfo(int nIndex);
	BOOL GetCameraInfo(int nIndex);
	BOOL SetShanchu(int nIndex);
	BOOL SetBianji(int nIndex);
	BOOL SetBaocun();
	BOOL SetQuxiao();
	BOOL SetTianjia();
	BOOL GetMaxPaga();
	void SetUp();
	void SetNext();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CString m_csIp;
	CString m_csIpOld;
	CString m_csPos;
	CString m_csFaceDbs;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString m_csPage;
};
