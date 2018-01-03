#pragma once
#include "GeneralDef.h"
#define  DOWNLOADEND  (WM_USER+100)
#define  DOWNLOADSTORP  (WM_USER+101)
#define  DOWNLOADERROR  (WM_USER+102)

typedef struct
{
	int nCurChanIndex;
	int nCameraIdex;
	HWND dlgHwnd;
	LONG LoginID;
	NET_DVR_TIME StartTime;
	NET_DVR_TIME StopTime;
	char csFileNam[100];
	char csFilPath[100];

}GETDOWNLOAD, *pGETDOWNLOAD;


#define DLLexprot __declspec(dllexport)

class DLLexprot VideoPlay
{
public:
	VideoPlay();
	~VideoPlay();


	void InitVideoPlay(HWND cwnHwnd);
	void CloseVideoPlay();

	BOOL  IsLogin();
	BOOL IsPlaying();


	BOOL LoginVideoPlay(CString lDevIp, CString lDevPort,
						CString lDevUser, CString lDevPwd);
	BOOL LogoutVideoPlay();

	void DoGetDeviceResoureCfg();
	int GetDevCameraSum(CStringArray &arCameraId, BOOL bEnable = FALSE);
	BOOL GetDevCameraNumDataToIP(int nIndex, CString &csIP, BOOL & bOnline);
	BOOL GetDevCameraNumDataToIndex(int &nIndex, CString csIP, BOOL & bOnline);

	BOOL StartPlay(int iCurChanIndex, HWND PlayWnd, LONG &PlayHandle);
	void StopPlay(LONG PlayHandle);

	BOOL StartRecord(int nCurChanIndex, LONG PlayHandle,
		CString csFilPath,CString csFileName = NULL);

	BOOL StopRecord(LONG PlayHandle);

	BOOL CapturePictureBmp(int nCurChanIndex, LONG PlayHandle,
		CString csFilPath, CString csFileNam = NULL);

	BOOL CapturePictureJpg(int nCurChanIndex, LONG PlayHandle, 
		int nSize, int nQuality, CString csFilPath,
		CString csFileNam = NULL);
	BOOL  GetDownloadPos(int nCurChanIndex, int  nCameraIdex, CString csStartTime,
		CString csEndTime, CString csFileNam, CString csFilPath = NULL);
	void StopDownloadPos();

	BOOL  PlayBackByTime(int nCurChanIndex, CString csStartTime,
		CString csEndTime, HWND playHwnd);
	void SetBackPlayByTime(int nTyp);

private:




	LOCAL_DEVICE_INFO m_struDeviceInfo;

	LONG m_lPlayHandle; //ÔÚÏß²¥·Å¾ä±ú
	BOOL m_IsPause;
	BOOL m_bIsRecording; //Â¼Ïñ
	BOOL m_bIsLogin; //µÇÂ¼
	HWND m_DlgMessageHwnd;

};

