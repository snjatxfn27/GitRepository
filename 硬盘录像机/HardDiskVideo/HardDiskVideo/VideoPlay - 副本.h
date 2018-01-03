#pragma once
#include "GeneralDef.h"


#define DLLexprot __declspec(dllexport)

class DLLexprot VideoPlay
{
public:
	VideoPlay();
	~VideoPlay();


	void InitVideoPlay();
	void CloseVideoPlay();

	BOOL  IsLogin();
	BOOL IsPlaying();


	BOOL LoginVideoPlay(CString lDevIp, CString lDevPort,
						CString lDevUser, CString lDevPwd);
	BOOL LogoutVideoPlay();

	void DoGetDeviceResoureCfg();
	int GetDevCameraSum(CStringArray &arCameraId, BOOL bEnable = FALSE);
	BOOL GetDevCameraNumData(int nIndex, CString &csIP, BOOL & bOnline);

	BOOL StartPlay(int iCurChanIndex, HWND PlayWnd, LONG &PlayHandle);
	void StopPlay(LONG PlayHandle);

	BOOL StartRecord(int nCurChanIndex, LONG PlayHandle,
		CString csFilPath,CString csFileName = NULL);
	BOOL StopRecord(LONG PlayHandle);
	BOOL CapturePictureBmp(int nCurChanIndex, LONG PlayHandle, CString csFilPath, CString csFileNam = NULL);

private:

	void SetPlay(HWND PlayWnd);


	LOCAL_DEVICE_INFO m_struDeviceInfo;
	HWND m_hPlayWnd; // satic hwnd
	LONG m_lPlayHandle; //µÇÂ¼ºóÐÅÏ¢
	BOOL m_bIsRecording; //Â¼Ïñ
	BOOL m_bIsLogin; //µÇÂ¼
};

