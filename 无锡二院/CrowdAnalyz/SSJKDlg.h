#pragma once
#include "afxwin.h"

// CSSJKDlg 对话框
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
	CSSJKDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSSJKDlg();

	//界面
	CDC *pMemDC;
	CBitmap *pBitmap;
	CRect m_rClient;//对话框总界面
	CRect m_rNum;//抓拍量框
	CRect m_rSnap;//实时抓拍图像框
	CRect m_rTongji;//动态分析库统计框
	CRect m_rAlarm;//黑名单报警框
	CRect m_rToday;//今日部分框
	CRect m_rCameraVideo;//
	CRect m_rCameraVideoClose;//
	CRect m_rZhuapailiang[8],m_rBendiHei[8],m_rDongtaiHei[8],m_rDongtaiCun[8];//总抓拍量、本地黑名单报警、动态黑名单报警、动态分析库存每个数字位置
	CRect m_rTodayZhuapai[8], m_rTodayAlarn[8];//今日抓拍、今日报警 每个数字位置
	CRect m_rSnapImage[10];//每个实时抓拍图像的显示位置
	CRect m_rCamera, m_rCameraStatus, m_rCameraPos;
	CString m_csCameraStatus, m_csCameraPos;
	//动态分析库统计
	struct DONGTAIFENXI
	{
		CRect rZP;
		CRect rBX;

		CRect rSim;
		double dbSim;

		CRect rNum;
		CString strNum;//编号

		CRect rCount;//次数
		int iCount;

		CRect rPos;//位置
		CString strPos;

		CRect rTime;//时间
		CString strTime;

		CRect rOperate;//操作
	} m_dongtaifenxi[5];

	int m_iDisTongjiCount;

	//黑名单报警
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
	void DisTongji(Graphics *g);//动态分析
	void DisHeimingdan(Graphics *g);//黑名单
	void DisToday(Graphics *g);//设置相机抓拍值

	//封装函数
	//改变4个数量区域的值
	void SetNumArea(int iZP, int iBH, int iDH, int iDC);
	//请把新来的抓拍图片用modeCreate|modeWrite的方法写入data/realtimesnap_10.jpg文件，然后再调用此函数
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
// 对话框数据
	enum { IDD = IDD_DIALOG_SSJK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
