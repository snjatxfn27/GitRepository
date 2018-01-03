
// CrowdAnalyzDlg.h : ͷ�ļ�
//

#pragma once
#include "stdafx.h"
#include "SSJKDlg.h"
#include "RLKGLDlg.h"
#include "XTSZDlg.h"
#include "SBGLDlg.h"
#include "LSJLDlg.h"
#include "SJFXDlg.h"
// CCrowdAnalyzDlg �Ի���
class CCrowdAnalyzDlg : public CDialogEx
{
// ����
public:
	CCrowdAnalyzDlg(CWnd* pParent = NULL);	// ��׼���캯��

public:
	//����
	CDC *pMemDC;
	CBitmap *pBitmap;
	CRect m_rClient;	//�ܽ���
	CRect m_rClose, m_rMin;//�رպ���С����ť��ռλ��
	CRect m_rToolBar;//��������λ��
	CRect m_rChildDialog;//�ӶԻ���λ��
	CRect m_rSSJK, m_rRLKGL;//���ѡ�������λ��
	CRect m_rXTSZ;
	CRect m_rSBGL;
	CRect m_rLSJL;
	CRect m_rSJFX;
	//��ǰ��ʾ���ӶԻ������
	//0:ʵʱ���	1:���������
	int m_iCurDlg;

	//�ӶԻ������
	CSSJKDlg *m_dlgSSJK;
	CRLKGLDlg *m_dlgRLKGL;
	CXTSZDlg *m_dlgXTSZ;
	CSBGLDlg *m_dlgSBGL;
	CLSJLDlg *m_dlgLSJL;
	CSJFXDlg *m_dlgSJLFX;


	void DisplayChildDialog(int iIndex);
	void DestoryCurDialog();
// �Ի�������
	enum { IDD = IDD_CROWDANALYZ_DIALOG };

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
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void SetLsjl();
	CString m_csLSJLSql;
	CString m_csLSJLIdcard;
	BOOL InitDatabase();
	CString GetiniPath();
	CString m_csSqlIP;
	CString m_csSqlData;
	CString m_csSqlName;
	CString m_csSqlPwd;
};
