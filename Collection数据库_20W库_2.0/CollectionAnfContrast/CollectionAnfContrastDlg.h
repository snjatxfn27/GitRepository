
// CollectionAnfContrastDlg.h : 头文件
//
#include "Tray.h"
#define WM_SHOW_TASK WM_USER+100

#pragma once
struct ThandScheduling
{
	BOOL					 bWorkState;//是否在工作
	BOOL					 bIsOnline;//心跳
	HANDLE				 HEvent; //Event 信号量
	CWinThread *		 HThand;//线程句柄
	CString					 csFaceDbIdex;//一体机对比人脸库
	CString					 csComputerIp;//一体机IP
	CString					 csMaxSum;//最大返回个数
	CString					 csSimilarity;//对比相似度
	//CStringArray
	CStringArray					csImgName;//图片名字
	CStringArray					csImgFile;//图片文件夹
	int							 nWarningSum;
};

// CCollectionAnfContrastDlg 对话框
class CCollectionAnfContrastDlg : public CDialogEx
{
// 构造
public:
	CCollectionAnfContrastDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_COLLECTIONANFCONTRAST_DIALOG };

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

	CFaceAccessSys m_FaceAccess;
	CFASStatusQuery m_CFASStatusQuery;
	CFASLoginOrLogout m_CFASLoginOrLogout;
	CFASCameraManage m_CFASCameraManage;
	CFASPolicyManage m_CFASPolicyManage;
	CFASRecognition m_CFASRecognition;
	CTray m_tray;

	int m_nSleepTime;
	int m_nBideTime;
	int m_nIsOnlineTime;
	int m_nwarningsum;

	CString m_csCameraSimilarity;
	CString m_csSleepTime;
	CString m_csBideTime;
	CString m_csIsOnlineTime;
	CString m_cswarningsum;

	CString GetiniPath();
	void GetInITime();
	bool GetParameterToDb();
	CString GetImagePath(CString csImageName, CString csFile);
	CString GetWarningPath(CString csImageName, CString csFile);
	BOOL WriteWarningb64(CString csWarning,CString csWarningPath);
	BOOL InitDbParameter();
	int InitComputerThread();
	int UpdataComputerOline();

	afx_msg LRESULT OnShowNotify(WPARAM wParam, LPARAM lParam);


	afx_msg void OnBnClickedButDetectiondb();
	afx_msg void OnBnClickedButStartthand();
	afx_msg void OnBnClickedButSetevent();
	afx_msg void OnDeletetary();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButSetvaluetodb();
	afx_msg void OnBnClickedCancel2();
};
