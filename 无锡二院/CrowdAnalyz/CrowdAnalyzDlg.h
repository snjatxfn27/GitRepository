
// CrowdAnalyzDlg.h : 头文件
//

#pragma once
#include "stdafx.h"
#include "SSJKDlg.h"
#include "RLKGLDlg.h"
#include "XTSZDlg.h"
#include "SBGLDlg.h"
#include "LSJLDlg.h"
#include "SJFXDlg.h"
// CCrowdAnalyzDlg 对话框
class CCrowdAnalyzDlg : public CDialogEx
{
// 构造
public:
	CCrowdAnalyzDlg(CWnd* pParent = NULL);	// 标准构造函数

public:
	//界面
	CDC *pMemDC;
	CBitmap *pBitmap;
	CRect m_rClient;	//总界面
	CRect m_rClose, m_rMin;//关闭和最小化按钮所占位置
	CRect m_rToolBar;//工具栏总位置
	CRect m_rChildDialog;//子对话框位置
	CRect m_rSSJK, m_rRLKGL;//鼠标选择各功能位置
	CRect m_rXTSZ;
	CRect m_rSBGL;
	CRect m_rLSJL;
	CRect m_rSJFX;
	//当前显示的子对话框序号
	//0:实时监控	1:人脸库管理
	int m_iCurDlg;

	//子对话框对象
	CSSJKDlg *m_dlgSSJK;
	CRLKGLDlg *m_dlgRLKGL;
	CXTSZDlg *m_dlgXTSZ;
	CSBGLDlg *m_dlgSBGL;
	CLSJLDlg *m_dlgLSJL;
	CSJFXDlg *m_dlgSJLFX;


	void DisplayChildDialog(int iIndex);
	void DestoryCurDialog();
// 对话框数据
	enum { IDD = IDD_CROWDANALYZ_DIALOG };

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
