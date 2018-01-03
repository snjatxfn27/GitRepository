#pragma once
#include "afxwin.h"


// CRLKGLDlg �Ի���

class CRLKGLDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRLKGLDlg)

public:
	CRLKGLDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRLKGLDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RLKGL };
public:
	//����
	COLORREF m_colorEditText; // edit�ؼ���������ɫ
	COLORREF m_colorEditBK; // edit�ؼ��ı�����ɫ
	CBrush* m_pEditBkBrush;
	CDC *pMemDC;
	CBitmap *pBitmap;
	CRect m_rClient;//�Ի����ܽ���
	CRect m_rJibenxinxi;//������Ϣ�����
	CRect m_rBianji;//������Ϣ�༭��
	CRect m_rAdd;//������Ϣ��ӿ�
	CRect m_rButtonAdd;//��Ӱ�ťλ��
	int m_iCurMingdan;//��ǰ�� 0:���غ����� 1:��̬������ 2:������
	CRect m_rBDBlack, m_rDTBlack, m_rWhite;
	CRect m_rLeftBeginPage, m_rLeftPrePage, m_rLeftNextPage, m_rLeftEndPage, m_rLeftCurPage, m_rLeftTotalPage;//����б����ҳ����һҳ����һҳ��ĩҳ����ǰҳ�룬��ҳ��λ��
	CRect m_rAlarmBeginPage, m_rAlarmPrePage, m_rAlarmNextPage, m_rAlarmEndPage, m_rAlarmCurPage;//�����б����ҳ����һҳ����һҳ��ĩҳ����ǰҳ��λ��
	CRect m_rSearchName, m_rSearchSex, m_rSearchType, m_rSearchButton;//���� �������Ա����͡�������ť λ��
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
	int m_iTotalAlarm, m_iCurAlarmPage;//�ܹ�������������¼����ǰ��ʾ�ڼ�ҳ��ÿҳ��ʾ4����
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

	CRect m_rJbxxxq[3];//��ʾ������Ϣ���飬��3��
	CRect m_rJbbx;//��ʾ���������λ��
	CRect m_rBjbx;//��ʾ�༭���������λ��
	CRect m_rTjbx;//��ʾ������������λ��
	CRect m_rBjbc, m_rTjbc;//��Ӻͱ༭�������水ť��λ��
	//����
	int m_iLeftTotalPage, m_iLeftCurPage;//�ܹ���������������ǰ��ʾ�ڼ�ҳ��ÿҳ��ʾ21����
	int m_iJBXXXZIndex;
	int m_iDisWhat;//0 �Ҳ಻��ʾ��1 ��ʾ���飬2 ��ʾ�༭��3 ��ʾ���
	BOOL m_bIsHaveTJBX;//�Ƿ��������Ƭ
	BOOL m_bIsChangeBJBX;//�Ƿ�����ı༭����

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

	//��װ�����Ա
	BOOL ServerAddPerson(CString strName, CString strSex, CString strBirth, CString strLibraryID, CString strNativeplace, CString strPhone, CString strAddress, CString strType);
	//��װɾ����Ա
	BOOL ServerDeletePerson(CString strPersonID,CString strIDCard);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
