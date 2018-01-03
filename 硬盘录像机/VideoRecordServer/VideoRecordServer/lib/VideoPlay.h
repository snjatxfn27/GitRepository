#pragma once
#include "lib/头文件/GeneralDef.h"
#define  DOWNLOADEND  (WM_USER+100)
#define  DOWNLOADSTORP  (WM_USER+101)
#define  DOWNLOADERROR  (WM_USER+102)

typedef struct
{
	int nCurChanIndex;
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

	LONG m_lPlayHandle; //在线播放句柄
	BOOL m_IsPause;
	BOOL m_bIsRecording; //录像
	BOOL m_bIsLogin; //登录
	HWND m_DlgMessageHwnd;

};

