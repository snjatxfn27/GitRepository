#pragma once


// CXTSZDlg �Ի���

class CXTSZDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CXTSZDlg)

public:
	CXTSZDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CXTSZDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_XTSZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();


	//����
	CDC *pMemDC;
	CBitmap *pBitmap;
	CRect m_rClient;//�Ի����ܽ���
	CRect m_rJkzq;//
	CRect m_rBbsj;//
	CRect m_rXsd;//
	CRect m_rBC;//
	COLORREF m_colorEditText; // edit�ؼ���������ɫ
	COLORREF m_colorEditBK; // edit�ؼ��ı�����ɫ
	CBrush* m_pEditBkBrush;
	CString m_csSim;

	BOOL GetConfig();
	BOOL SetConfig();
	BOOL PreTranslateMessage(MSG* pMsg);


	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString m_csJiankong;
	CString m_csShijian;
	CString m_csXiangsidu;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
