#pragma once
#include "afxwin.h"

// CSSJKDlg �Ի���
struct CameraInfo
{
	CString csCameraId;
	CString csCameraIp;
	CString csCameraAddr;
	CString csCameraRtsp;
	int     nCameraState;
	int     nCameraType;
};
class CSSJKDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSSJKDlg)

public:
	CSSJKDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSSJKDlg();

	//����
	CDC *pMemDC;
	CBitmap *pBitmap;
	CRect m_rClient;//�Ի����ܽ���
	CRect m_rNum;//ץ������
	CRect m_rSnap;//ʵʱץ��ͼ���
	CRect m_rTongji;//��̬������ͳ�ƿ�
	CRect m_rAlarm;//������������
	CRect m_rToday;//���ղ��ֿ�
	CRect m_rCameraVideo;//
	CRect m_rCameraVideoClose;//
	CRect m_rZhuapailiang[8],m_rBendiHei[8],m_rDongtaiHei[8],m_rDongtaiCun[8];//��ץ���������غ�������������̬��������������̬�������ÿ������λ��
	CRect m_rTodayZhuapai[8], m_rTodayAlarn[8];//����ץ�ġ����ձ��� ÿ������λ��
	CRect m_rSnapImage[10];//ÿ��ʵʱץ��ͼ�����ʾλ��
	CRect m_rCamera, m_rCameraStatus, m_rCameraPos;
	CString m_csCameraStatus, m_csCameraPos;
	//��̬������ͳ��
	struct DONGTAIFENXI
	{
		CRect rZP;
		CRect rBX;

		CRect rSim;
		double dbSim;

		CRect rNum;
		CString strNum;//���

		CRect rCount;//����
		int iCount;

		CRect rPos;//λ��
		CString strPos;

		CRect rTime;//ʱ��
		CString strTime;

		CRect rOperate;//����
	} m_dongtaifenxi[5];

	int m_iDisTongjiCount;

	//����������
	struct HEIMINGDAN
	{
		CRect rZP;
		CRect rBX;

		CRect rSim;
		double dbSim;

		CRect rNum;
		CString strNum;

		CRect rCount;
		int iCount;

		CRect rPos;
		CString strPos;

		CRect rTime;
		CString strTime;

		CRect rOperate;
	} m_heimingdan[5];

	int m_iDisHeimingdanCount;


	int m_nCameraSum;


	int m_iDrawZhuapailiang,m_iDrawBendiHei,m_iDrawDongtaiHei,m_iDrawDongtaiCun;
	int m_iDrawTodayZhuapai, m_iDrawTodayAlarm;
	int m_iSnapImageBegin;

	void DisNum(Graphics *g);
	void DisSnap(Graphics *g);
	void DisTongji(Graphics *g);//��̬����
	void DisHeimingdan(Graphics *g);//������
	void DisToday(Graphics *g);//�������ץ��ֵ

	//��װ����
	//�ı�4�����������ֵ
	void SetNumArea(int iZP, int iBH, int iDH, int iDC);
	//���������ץ��ͼƬ��modeCreate|modeWrite�ķ���д��data/realtimesnap_10.jpg�ļ���Ȼ���ٵ��ô˺���
	void SetSnapArea();

	void SetNullJpg(CString csPath);

	/////////


	BOOL GetMaxCurrent(CString &csMaxCurrnet, CString &csStartTime, CString &csEndTime);

	BOOL GetSnapshotInfoCameraSum(CString csCameraID, CString &csMaxNum);

	int GetDbTableMaxSum(CString csTable, CString csStartTime = NULL, CString csEndTime = NULL);

	BOOL GetCameraWarningSum(CString csTable, CString csCameraID, CString &csMaxNum);
	int GetCameraToCombox(CComboBox &CameraCombox);

	void SetWarningNum();
	void SetCameraNum(int nCameraIndex);

	BOOL GetIdcardImg(CString csImgName, CString csIdcard, CString csSnapshotid, int nIndex);
	BOOL SetTongjiInfo(int nIndex);
	BOOL GetSSXS();
	void SetSSXS();
	BOOL SetHeimingdan(int nIndex);
	BOOL GetHeimingdan(int nIndex);
	BOOL IsBlacklist(CString csIdcard);
	BOOL Start();
	BOOL InitParameter();
	CString GetiniPath();

	BOOL GetTempsimilarity(CString &csSimilarity, CString csIdcard, CString csTime);

	BOOL GetTempinfo(int nIndex);

	void SetNullJpgAll();

	BOOL DoLogin(CString csIp);
	BOOL DoLogout();
	BOOL StopPlay();
	BOOL StartPlay();
	LOCAL_DEVICE_INFO m_struDeviceInfo;
	BOOL m_bIsPlaying;
	LONG m_lPlayHandle;
	CTools m_cTools;
	CString m_csSSXSID;
	CString m_csImgIP;
	int  m_nTongjiIndexMax;
	int  m_nHeimingdanIndexMax;

	int  m_nTongjiIndex;
	int  m_nHeimingdanIndex;
	int m_nComboxIndex;
// �Ի�������
	enum { IDD = IDD_DIALOG_SSJK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	CComboBoxXP m_wndCameraCom;
	CComboBox m_wndCameraCom;
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeComboCamera();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
