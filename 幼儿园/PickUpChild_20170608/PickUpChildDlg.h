
// PickUpChildDlg.h : ͷ�ļ�
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

// CPickUpChildDlg �Ի���
class CPickUpChildDlg : public CDialogEx
{
// ����
public:
	CPickUpChildDlg(CWnd* pParent = NULL);	// ��׼���캯��

	//�Լ��ı���
public:
	CRect m_rClient;
	CRect m_rChildManagerButton;
	CRect m_rParentManagerButton;
	CRect m_rConfirmButton;

	struct CURDISPARENT	//��ǰ��ʾ�ļҳ��ṹ��
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
	struct CURINCHILD	//��ǰ��ʾ����Уѧ���ṹ��
	{
		CRect rInChildRect;
		int iCurInID;
		CString strName;
		CString strPhotoName;
	} m_curInChild[21];

	unsigned int m_uiCurInPage;	//��ǰ��ʾ�ڼ�ҳ��Уѧ��
	int m_iCurSelInNum;//��ǰѡ��ı�ҳ�ڼ���ѧ����0-20��21��
	void RefreshInChildArea(unsigned int uiCurPage);

	void InitOutChildArea();
	struct CUROUTCHILD	//��ǰ��ʾ����Уѧ���ṹ��
	{
		CRect rOutChildRect;
		int iCurOutID;
		CString strName;
		CString strPhotoName;
	} m_curOutChild[21];
	unsigned int m_uiCurOutPage;	//��ǰ��ʾ�ڼ�ҳ��Уѧ��
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
	//��ʾ��Ƶ
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
// �Ի�������
	enum { IDD = IDD_PICKUPCHILD_DIALOG };

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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
