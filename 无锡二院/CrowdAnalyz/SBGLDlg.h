#pragma once


// CSBGLDlg �Ի���

class CSBGLDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSBGLDlg)

public:
	CSBGLDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSBGLDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SBGL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);

	//��̬������ͳ��
	struct DATA_LIST
	{
		CRect rXQ;
		CRect rSC;
		CRect rBJ;

		CRect rIP;
		CString csIP;//ip

		CRect rPos;//λ��
		CString strPos;

		CRect rState;//״̬
		CString csState;

		CString csFaceDbs;
	} m_DataList[21];

	int m_iDisTongjiCount;



	//����
	COLORREF m_colorEditText; // edit�ؼ���������ɫ
	COLORREF m_colorEditBK; // edit�ؼ��ı�����ɫ
	CBrush* m_pEditBkBrush;


	CDC *pMemDC;
	CBitmap *pBitmap;
	CRect m_rClient;//�Ի����ܽ���
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
