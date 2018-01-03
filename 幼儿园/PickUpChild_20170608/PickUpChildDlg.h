
// PickUpChildDlg.h : 头文件
//

#pragma once

#define MAX_VIDEO_THREAD 50

typedef struct _THREAD_PARAM
{
	int nID;
	void* pParent;
}THREAD_PARAM;

typedef struct _THREAD_DATA
{
	HANDLE hPlayer;
	HANDLE hShow;
	HANDLE hWatcher;
	std::string sAddress;
	int nPlayerStatus;
	int nFrameCount;
	cv::VideoCapture capture;
	cv::Mat matFrame;
}THREAD_DATA;

enum VIDEO_STATUS{ VIDEO_CONNECTING, VIDEO_PLAYING, VIDEO_STOPED, VIDEO_LOST, VIDEO_PAUSED };

// CPickUpChildDlg 对话框
class CPickUpChildDlg : public CDialogEx
{
// 构造
public:
	CPickUpChildDlg(CWnd* pParent = NULL);	// 标准构造函数

	//自己的变量
public:
	CRect m_rClient;
	CRect m_rChildManagerButton;
	CRect m_rParentManagerButton;
	CRect m_rConfirmButton;

	struct CURDISPARENT	//当前显示的家长结构体
	{
		CRect rCurDisParent;
		int iID;
		CString strName;
		CString strRelation;
		CString strPhone;
		CString strClassName;
		CString strPhotoName;
		BOOL isRight;
	} m_curDisParent[3];
	void InitDisParentArea();
	void RefreshDisParent(int iChildID);

	void InitInChildArea();
	struct CURINCHILD	//当前显示的在校学生结构体
	{
		CRect rInChildRect;
		int iCurInID;
		CString strName;
		CString strPhotoName;
	} m_curInChild[21];

	unsigned int m_uiCurInPage;	//当前显示第几页在校学生
	int m_iCurSelInNum;//当前选择的本页第几个学生，0-20共21个
	void RefreshInChildArea(unsigned int uiCurPage);

	void InitOutChildArea();
	struct CUROUTCHILD	//当前显示的离校学生结构体
	{
		CRect rOutChildRect;
		int iCurOutID;
		CString strName;
		CString strPhotoName;
	} m_curOutChild[21];
	unsigned int m_uiCurOutPage;	//当前显示第几页在校学生
	void RefreshOutChildArea(unsigned int uiCurPage);

	unsigned int m_uiTodayAllCount,m_uiEndCount,m_uiInCount,m_uiOutCount;
	void RefreshCount();
	void RefreshHistory();
	CString m_strHistory[9];
	void SetChildStatus(int iChildID, int iIsHere, int iTodayIsHere, int iIsEnd);
	void AddHistory(int iChildID, int iParentID);

	//opencv
	int Play(char * sAddress);
	void InitPlayEnviroment();

private:
	//显示视频
	// show video
	CRect m_rShowRect;
	cv::Mat m_matBlank;
	//CvFont m_fontSubtitle;
	THREAD_PARAM m_pThreadParam;
	THREAD_DATA m_aThreadDataArray[MAX_VIDEO_THREAD];
	int m_nStatus = VIDEO_STOPED;
	int m_nActiveThreadID = -1;
	int m_nConnectTimeOut = 10000;
	int m_nStreamTimeOut = 500;
	BOOL m_bStopFlag = true;  // the flag of PLAY(false) or STOP(ture) video, use for stop video play thread

	void ShowImage(cv::Mat& matSource, int nID);
	void ShowImage(cv::Mat& matSource, int nID, float fFontSize, std::string sSubtitle);
	void OutputSubtitle(cv::Mat& matOriginal, float fFontSize, std::string sSubTitle);
	int Mat2CImage(cv::Mat *mat, CImage &img);
	int FindAviableID();

	// erase background
	BOOL m_bEraseBkg = false;
	BOOL m_bBgFinded = false;
	cv::Mat m_matBkg;
	int m_nSameCount = 0;
	cv::Ptr<cv::BackgroundSubtractorMOG2> m_mogBkg;
	void EraseBkg(cv::Mat& matFrame);

	// Thread
	friend UINT PlayerThread(void* param);
	friend UINT ShowThread(void* param);
	friend UINT WatcherThread(void* param);
// 对话框数据
	enum { IDD = IDD_PICKUPCHILD_DIALOG };

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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
