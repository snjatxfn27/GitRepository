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

// CVideoDlg 对话框

class CVideoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVideoDlg)

public:
	CVideoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVideoDlg();

	//对话框位置
	CRect m_rClient;

	// 对话框数据
	enum { IDD = IDD_DIALOG_VIDEO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void InitPlayEnviroment();
//	void SetFaceDetect(BOOL bDetect, BOOL bMilti, BOOL bTrace, cv::Size szMin);
	void SetEraseBkg(BOOL bEraseBkg);
	BOOL SetSubtitleFont(double HScale, double VScale, double Thick);
	int Play(char * sAddress);
	void Stop();
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

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedTest();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
