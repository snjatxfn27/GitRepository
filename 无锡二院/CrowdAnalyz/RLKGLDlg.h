#pragma once
#include "afxwin.h"


// CRLKGLDlg 对话框

class CRLKGLDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRLKGLDlg)

public:
	CRLKGLDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRLKGLDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_RLKGL };
public:
	//界面
	COLORREF m_colorEditText; // edit控件的字体颜色
	COLORREF m_colorEditBK; // edit控件的背景颜色
	CBrush* m_pEditBkBrush;
	CDC *pMemDC;
	CBitmap *pBitmap;
	CRect m_rClient;//对话框总界面
	CRect m_rJibenxinxi;//基本信息详情框
	CRect m_rBianji;//基本信息编辑框
	CRect m_rAdd;//基本信息添加框
	CRect m_rButtonAdd;//添加按钮位置
	int m_iCurMingdan;//当前是 0:本地黑名单 1:动态黑名单 2:白名单
	CRect m_rBDBlack, m_rDTBlack, m_rWhite;
	CRect m_rLeftBeginPage, m_rLeftPrePage, m_rLeftNextPage, m_rLeftEndPage, m_rLeftCurPage, m_rLeftTotalPage;//左侧列表的首页、上一页、下一页、末页、当前页码，总页码位置
	CRect m_rAlarmBeginPage, m_rAlarmPrePage, m_rAlarmNextPage, m_rAlarmEndPage, m_rAlarmCurPage;//报警列表的首页、上一页、下一页、末页、当前页码位置
	CRect m_rSearchName, m_rSearchSex, m_rSearchType, m_rSearchButton;//搜索 姓名、性别、类型、搜索按钮 位置
	struct ALARMLIST
	{
		CRect rZP, rBX;
		CRect rSim, rAddr, rTime;
	} m_alarmList[4];
	struct ALARMDATA
	{
		CString strSnapshotID;
		CString strSim;
		CString strAlarmTime;
		CString strAddr;
		CString strSrc;
	} m_alarmdata[40];
	int m_iTotalAlarm, m_iCurAlarmPage;//总共多少条报警记录，当前显示第几页（每页显示4条）
	struct MDLIST
	{
		BOOL bIsUse;
		CString strIDCard;
		CString strPersonID;
		CString strName;
		CRect rName;
		CString strSex;
		CRect rSex;
		CString strType;
		CRect rType;
		CRect rXQ, rDEL, rEDIT;
		CString strInsertTime, strBirth, strAddress,strPhone,strNativeplace,strBXPath;
	} m_mdList[21];

	CRect m_rJbxxxq[3];//显示基本信息详情，共3行
	CRect m_rJbbx;//显示人脸靶向的位置
	CRect m_rBjbx;//显示编辑人脸靶向的位置
	CRect m_rTjbx;//显示添加人脸靶向的位置
	CRect m_rBjbc, m_rTjbc;//添加和编辑两个保存按钮的位置
	//数据
	int m_iLeftTotalPage, m_iLeftCurPage;//总共多少条人脸，当前显示第几页（每页显示21条）
	int m_iJBXXXZIndex;
	int m_iDisWhat;//0 右侧不显示，1 显示详情，2 显示编辑，3 显示添加
	BOOL m_bIsHaveTJBX;//是否添加了照片
	BOOL m_bIsChangeBJBX;//是否更换的编辑靶向

	void DisLeftList(Graphics *g);
	void DisJBXXXQ(Graphics *g, int iIndex);
	void DisJBXXBJ(Graphics *g, int iIndex);
	void SetDataLeftList();
	void HideEditControls();
	void ShowEditControls();
	void HideAddControls();
	void ShowAddControls();
	void BianjiBaocun();
	void TianjiaBaocun();
	BOOL SelectPic();
	void ShanchuButton();
	void SetDataAlarmList();

	//封装添加人员
	BOOL ServerAddPerson(CString strName, CString strSex, CString strBirth, CString strLibraryID, CString strNativeplace, CString strPhone, CString strAddress, CString strType);
	//封装删除人员
	BOOL ServerDeletePerson(CString strPersonID,CString strIDCard);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CEdit m_editName;
	CEdit m_editBirth;
	CEdit m_editType;
	CEdit m_editPhone;
	CEdit m_editNativeplace;
	CEdit m_editAddress;
	CComboBox m_comboSex;
	CComboBox m_comboFacedb;
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);

	CEdit m_editSearchName;
	CEdit m_editSearchType;
	CComboBox m_comboSearchSex;
};
