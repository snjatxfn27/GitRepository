// VideoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WinGUIClient.h"
#include "VideoDlg.h"
#include "afxdialogex.h"

CEvent _WaitConnect[MAX_VIDEO_THREAD];

UINT PlayerThread(void* param)
{
	THREAD_PARAM * pParam = (THREAD_PARAM *)param;
	int nSelfID = pParam->nID;
	CVideoDlg *pParent = (CVideoDlg*)(pParam->pParent);

	pParent->m_aThreadDataArray[nSelfID].nPlayerStatus = VIDEO_CONNECTING;
	pParent->ShowImage(pParent->m_matBlank, nSelfID, 0.5,
		"Loading...... please wait\n" + pParent->m_aThreadDataArray[nSelfID].sAddress);

	cv::Mat matFrame;
	pParent->m_aThreadDataArray[nSelfID].capture.open(pParent->m_aThreadDataArray[nSelfID].sAddress);
	//cv::VideoCapture capture("rtsp://218.204.223.237:554/live/1/66251FC11353191F/e7ooqwcfbqjoo80j.sdp");
	//cv::VideoCapture capture(0);
	if (pParent->m_aThreadDataArray[nSelfID].capture.isOpened())
	{
		pParent->m_aThreadDataArray[nSelfID].nPlayerStatus = VIDEO_PLAYING;
		_WaitConnect[nSelfID].SetEvent();

		while ((pParent->m_nActiveThreadID == nSelfID) && (!pParent->m_bStopFlag))
		{
			if (pParent->m_aThreadDataArray[nSelfID].capture.read(matFrame))
			{
				pParent->m_aThreadDataArray[nSelfID].nFrameCount++;
				if (pParent->m_aThreadDataArray[nSelfID].matFrame.empty())
					pParent->m_aThreadDataArray[nSelfID].matFrame = matFrame.clone();
			}
			Sleep(5);
		}
		//pParent->ShowImage(pParent->m_matBlank, nSelfID, "play finished");
	}
	else
	{
		pParent->ShowImage(pParent->m_matBlank, nSelfID, 0.5, "Can not connect to carama!!");
		pParent->m_aThreadDataArray[nSelfID].nPlayerStatus = VIDEO_STOPED;
		_WaitConnect[nSelfID].SetEvent();
		return -1;
	}

	return 0;
}
UINT ShowThread(void* param)
{
	THREAD_PARAM * pParam = (THREAD_PARAM *)param;
	int nSelfID = pParam->nID;
	CVideoDlg *pParent = (CVideoDlg*)(pParam->pParent);

	cv::Mat matFrame;
	//////////
	std::string sOutputstr;
	std::stringstream ss;
	ss << nSelfID;
	ss >> sOutputstr;
	sOutputstr = pParent->m_aThreadDataArray[nSelfID].sAddress + "\nCurrent Thread ID:" + sOutputstr;
	///////////

	while ((pParent->m_nActiveThreadID == nSelfID) && (!pParent->m_bStopFlag))
	{
		if (pParent->m_aThreadDataArray[nSelfID].matFrame.data)
		{
			matFrame = pParent->m_aThreadDataArray[nSelfID].matFrame.clone();
			pParent->m_aThreadDataArray[nSelfID].matFrame.release();

			// if face detect
//			if (pParent->m_bDetectFace) pParent->DetectFace(matFrame);

			// if erase background
			if (pParent->m_bEraseBkg) pParent->EraseBkg(matFrame);

			if (pParent->m_aThreadDataArray[nSelfID].nFrameCount < 80) pParent->ShowImage(matFrame, nSelfID, 0.5, sOutputstr);
			else pParent->ShowImage(matFrame, nSelfID);
		}
		Sleep(1);
	}

	if (pParent->m_bStopFlag)
	{
//		pParent->m_pTarget->Invalidate();
		pParent->InvalidateRect(pParent->m_rShowRect, false);
	}

	return 0;
}

UINT WatcherThread(void* param)
{
	THREAD_PARAM * pParam = (THREAD_PARAM *)param;
	int nSelfID = pParam->nID;
	CVideoDlg *pParent = (CVideoDlg*)(pParam->pParent);
	int nConnectTimeOut = pParent->m_nConnectTimeOut;
	int nStreamTimeOut = pParent->m_nStreamTimeOut;

	int nLastFrameCount = 0;
	BOOL bStreamStoped = false;
	BOOL bStreamLost = false;

	_WaitConnect[nSelfID].ResetEvent();
	WaitForSingleObject(_WaitConnect[nSelfID], nConnectTimeOut);

	if (pParent->m_aThreadDataArray[nSelfID].nPlayerStatus == VIDEO_CONNECTING)
	{
		pParent->ShowImage(pParent->m_matBlank, nSelfID, 0.6, "Can not connect to carama!");
		TerminateThread(pParent->m_aThreadDataArray[nSelfID].hPlayer, 0);
		pParent->m_aThreadDataArray[nSelfID].nPlayerStatus = VIDEO_STOPED;
		pParent->m_aThreadDataArray[nSelfID].hPlayer = 0;
		pParent->m_aThreadDataArray[nSelfID].hWatcher = 0;
		pParent->m_aThreadDataArray[nSelfID].nFrameCount = 0;
		pParent->m_aThreadDataArray[nSelfID].sAddress = "";
		return 0;
	}

	while ((pParent->m_nActiveThreadID == nSelfID) && (!pParent->m_bStopFlag))
	{
		Sleep(nStreamTimeOut);
		if (nLastFrameCount == pParent->m_aThreadDataArray[nSelfID].nFrameCount)
		{
			if (bStreamStoped)
			{
				pParent->ShowImage(pParent->m_matBlank, nSelfID, 0.6, "Video stream lost!");
				bStreamLost = true;
			}
			else
			{
				bStreamStoped = true;
			}
		}
		else
		{
			bStreamLost = false;
			bStreamStoped = false;
			nLastFrameCount = pParent->m_aThreadDataArray[nSelfID].nFrameCount;
		}
	}
	if (bStreamLost) TerminateThread(pParent->m_aThreadDataArray[nSelfID].hPlayer, 0);
	pParent->m_aThreadDataArray[nSelfID].nPlayerStatus = VIDEO_STOPED;
	pParent->m_aThreadDataArray[nSelfID].hPlayer = 0;
	pParent->m_aThreadDataArray[nSelfID].hWatcher = 0;
	pParent->m_aThreadDataArray[nSelfID].nFrameCount = 0;
	pParent->m_aThreadDataArray[nSelfID].sAddress = "";

	return 0;
}

// CVideoDlg 对话框

IMPLEMENT_DYNAMIC(CVideoDlg, CDialogEx)

CVideoDlg::CVideoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoDlg::IDD, pParent)
{

}

CVideoDlg::~CVideoDlg()
{
}

void CVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVideoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_TEST, &CVideoDlg::OnBnClickedTest)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CVideoDlg 消息处理程序
int CVideoDlg::Mat2CImage(Mat *mat, CImage &img)
{
	if (!mat || mat->empty()) return -1;

	//重建CImage  
	img.Destroy();
	int nBPP = mat->channels() * 8;
	img.Create(mat->cols, mat->rows, nBPP);
	if (nBPP == 8)
	{
		static RGBQUAD pRGB[256];
		for (int i = 0; i < 256; i++)
			pRGB[i].rgbBlue = pRGB[i].rgbGreen = pRGB[i].rgbRed = i;
		img.SetColorTable(0, 256, pRGB);
	}
	uchar* psrc = mat->data;
	uchar* pdst = (uchar*)img.GetBits();
	int imgPitch = img.GetPitch();
	for (int y = 0; y < mat->rows; y++)
	{
		memcpy(pdst, psrc, mat->cols*mat->channels());//mat->step is incorrect for those images created by roi (sub-images!)  
		psrc += mat->step;
		pdst += imgPitch;
	}
	return 0;
}
void CVideoDlg::ShowImage(Mat& matSource, int nID)
{
//	if ((nID != m_nActiveThreadID) || m_bStopFlag) return;

	CRect rect;
	CImage imgDst;

	// Get Display area rect
	cv::Size winSize(m_rShowRect.right, m_rShowRect.bottom);

	// Resize the source to the size of the destination image if necessary
	//cv::Mat frame(winSize, CV_8UC3);
	Mat frame;
	if (matSource.size() != winSize)
		cv::resize(matSource, frame, winSize);
	else
		frame = matSource.clone();

//	m_pTarget->GetClientRect(&rect);
	SetRect(rect, m_rShowRect.left, m_rShowRect.top, m_rShowRect.right, m_rShowRect.bottom);

	if (Mat2CImage(&frame, imgDst) == 0)
	{
		//Mat2CImage(&frame, imgDst);
//		CDC * pDC = m_pTarget->GetDC();
		CDC * pDC = GetDC();

		imgDst.Draw(pDC->GetSafeHdc(), rect);
		frame.release();
		ReleaseDC(pDC);
		imgDst.Destroy();
	}
}

void CVideoDlg::ShowImage(cv::Mat& matSource, int nID, float fFontSize, std::string sSubtitle)
{
	if ((nID != m_nActiveThreadID) || m_bStopFlag) return;

	CRect rect;
	CImage imgDst;

	// Get Display area rect
	cv::Size winSize(m_rShowRect.right, m_rShowRect.bottom);

	// Resize the source to the size of the destination image if necessary
	cv::Mat frame;
	if (matSource.size() != winSize) cv::resize(matSource, frame, winSize);
	else frame = matSource.clone();

	// if face detect
	//if (m_bDetectFace) DetectFace(frame);

	// if erase background
	//if (m_bEraseBkg) EraseBkg(frame);

	// Add subtitle to frame
	OutputSubtitle(frame, fFontSize, sSubtitle);

	// Convert to CImage for display
	Mat2CImage(&frame, imgDst);
	//	MatToCImage(frame, imgDst);

	// Display frame
	CDC *pDC = GetDC();
	imgDst.Draw(pDC->GetSafeHdc(), m_rShowRect);
	frame.release();
	ReleaseDC(pDC);
	imgDst.Destroy();
}

BOOL CVideoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
//	m_rShowRect.SetRect(0, 0, 300, 200);
	m_rShowRect.SetRect(0, theApp.m_rectVideo.Height() * 21 / 720, theApp.m_rectVideo.Width(), theApp.m_rectVideo.bottom);
	InitPlayEnviroment();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CVideoDlg::InitPlayEnviroment()
{
	m_bStopFlag = true;
//	m_pTarget = pTarget;
	CRect rect;
	cv::Size winSize(m_rShowRect.right, m_rShowRect.bottom);
	m_matBlank.release();
	m_matBlank = cv::Mat(winSize, CV_8UC3, cvScalar(200, 0, 0));

	//	SetSubtitleFont(1, 1, 1);
	int i;
	for (i = 0; i < MAX_VIDEO_THREAD; i++)
	{
		m_aThreadDataArray[i].hPlayer = 0;
		m_aThreadDataArray[i].hWatcher = 0;
		m_aThreadDataArray[i].hShow = 0;
		m_aThreadDataArray[i].nFrameCount = 0;
		m_aThreadDataArray[i].nPlayerStatus = VIDEO_STOPED;
		m_aThreadDataArray[i].sAddress = "";
	}
	m_nActiveThreadID = -1;

	// load face detect file
//	if (m_cascadeFace.load("d://haarcascade_frontalface_alt2.xml")) m_bCascadeLoaded = true;
//	else m_bCascadeLoaded = false;


	// for erase background
	m_mogBkg = cv::createBackgroundSubtractorMOG2(500, 16, true);
}

int CVideoDlg::Play(char * sAddress)
{
	int nID;

	if (m_nActiveThreadID > -1 && m_nActiveThreadID < MAX_VIDEO_THREAD)
	{
		if (m_aThreadDataArray[m_nActiveThreadID].sAddress.compare(sAddress) == 0
			&& m_aThreadDataArray[m_nActiveThreadID].nPlayerStatus != VIDEO_STOPED)  return 1; /*already in process*/
	}

	nID = FindAviableID();
	if (nID == -1) return 2; /*no more thread aviable*/

	m_nActiveThreadID = nID;

	m_bStopFlag = false;
	m_pThreadParam.nID = m_nActiveThreadID;
	m_pThreadParam.pParent = this;
	m_aThreadDataArray[m_nActiveThreadID].sAddress = sAddress;
	m_aThreadDataArray[m_nActiveThreadID].capture.release();
	m_aThreadDataArray[m_nActiveThreadID].matFrame.release();
	m_aThreadDataArray[m_nActiveThreadID].hPlayer = AfxBeginThread(PlayerThread, &m_pThreadParam);
	m_aThreadDataArray[m_nActiveThreadID].hWatcher = AfxBeginThread(WatcherThread, &m_pThreadParam);
	m_aThreadDataArray[m_nActiveThreadID].hShow = AfxBeginThread(ShowThread, &m_pThreadParam);

	return 0;
}
int CVideoDlg::FindAviableID()
{
	int i = 0;

	while (i < MAX_VIDEO_THREAD)
	{
		if (m_aThreadDataArray[i].hPlayer == 0
			&& m_aThreadDataArray[i].hWatcher == 0
			&& m_aThreadDataArray[i].nPlayerStatus == VIDEO_STOPED)
		{
			return i;
		}
		i++;
	}

	return -1;
}

void CVideoDlg::OutputSubtitle(cv::Mat& matOriginal, float fFontSize, std::string sSubtitle)
{
	int nLineCount = 0;
	std::string::size_type nFindPos = 0;
	std::string sText = sSubtitle;
	std::string sLines;

	// splite string by '\n' & add each line to frame
	nFindPos = sText.find('\n');
	while (nFindPos != std::string::npos)
	{
		sLines = sText.substr(0, nFindPos);
		putText(matOriginal, sLines, cvPoint(20, 30 + nLineCount * 60 * fFontSize),
			CV_FONT_HERSHEY_COMPLEX, fFontSize, cvScalar(0, 0, 0), 1, 8, false);
		putText(matOriginal, sLines, cvPoint(18, 30 + nLineCount * 60 * fFontSize - 2),
			CV_FONT_HERSHEY_COMPLEX, fFontSize, cvScalar(180, 180, 180), 1, 8, false);
		if (nFindPos < sText.length())
		{
			sLines.clear();
			sLines = sText.substr(nFindPos + 1, sText.length());
			sText = sLines;
			nFindPos = sText.find('\n');
		}
		else
		{
			sText.clear();
			break;
		}
		nLineCount++;
	}
	putText(matOriginal, sText, cvPoint(20, 30 + nLineCount * 60 * fFontSize),
		CV_FONT_HERSHEY_COMPLEX, fFontSize, cvScalar(0, 0, 0), 1, 8, false);
	putText(matOriginal, sText, cvPoint(18, 30 + nLineCount * 60 * fFontSize - 2),
		CV_FONT_HERSHEY_COMPLEX, fFontSize, cvScalar(180, 180, 180), 1, 8, false);
}

void CVideoDlg::EraseBkg(cv::Mat& matFrame)
{
	cv::Mat foreground;
	cv::Mat Original = matFrame.clone();
	matFrame.release();
	m_mogBkg->apply(Original, foreground, 0.001);
	cv::erode(foreground, foreground, cv::Mat());
	cv::dilate(foreground, foreground, cv::Mat());
	matFrame = foreground.clone();
}

void CVideoDlg::SetEraseBkg(BOOL bEraseBkg)
{
	m_bEraseBkg = bEraseBkg;
	m_mogBkg->clear();
}


void CVideoDlg::OnBnClickedTest()
{
	char * sAddr = "rtsp://admin:unisib1000@192.168.2.100:554/h264/ch1/main/av_stream";
	Play(sAddr);
}


void CVideoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	HDC hDC = GetDC()->m_hDC;
	Graphics g(hDC);
	Image img_bk(L"img/video_bk.jpg");
	g.DrawImage(&img_bk, 0, 0, m_rClient.Width(), m_rClient.Height());
	g.ReleaseHDC(hDC);
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


void CVideoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rClose(
		theApp.m_rectVideo.Width() * 910 / 960,
		0,
		theApp.m_rectVideo.Width(),
		theApp.m_rectVideo.Height() * 21 / 720
		);
	if (rClose.PtInRect(point))
	{
		Stop();
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CVideoDlg::Stop()
{
	m_bStopFlag = true;
	m_nActiveThreadID = -1;
}