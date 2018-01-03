#include "stdafx.h"
#include "VideoPlay.h"
#include "Log.h"
GETDOWNLOAD m_GetDownload;
BOOL g_bDownload = FALSE;

VideoPlay::VideoPlay()
{
	m_lPlayHandle = NULL;//���߲��ž��
	m_bIsRecording = FALSE; //¼��
	m_bIsLogin = FALSE; //��¼
	m_IsPause = FALSE;//���߲���
}


VideoPlay::~VideoPlay()
{
}



void VideoPlay::InitVideoPlay(HWND cwnHwnd)
{
	NET_DVR_Init();          //Init SDK
	m_DlgMessageHwnd = cwnHwnd;
}

void VideoPlay::CloseVideoPlay()
{
	NET_DVR_Cleanup();
}
/*************************************************
������:    	LoginVideoPlay
��������:	��¼Ӳ��¼���
�������:
�������:
����ֵ:
**************************************************/

BOOL VideoPlay::LoginVideoPlay(CString lDevIp, CString lDevPort, CString lDevUser, CString lDevPwd)
{
	NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;
	memset(&DeviceInfoTmp, 0, sizeof(NET_DVR_DEVICEINFO_V30));

	LONG lLoginID = NET_DVR_Login_V30(lDevIp.GetBuffer(lDevIp.GetLength()), atoi(lDevPort), \
		lDevUser.GetBuffer(lDevUser.GetLength()), lDevPwd.GetBuffer(lDevPwd.GetLength()), &DeviceInfoTmp);
	if (lLoginID == -1)
	{
		//NET_DVR_GetLastError() == NET_DVR_PASSWORD_ERROR;
		m_bIsLogin = FALSE;
		return FALSE;
	}
	m_struDeviceInfo.lLoginID = lLoginID;
	m_struDeviceInfo.iDeviceChanNum = DeviceInfoTmp.byChanNum;
	m_struDeviceInfo.iIPChanNum = DeviceInfoTmp.byIPChanNum;
	m_struDeviceInfo.iStartChan = DeviceInfoTmp.byStartChan;
	m_struDeviceInfo.iIPStartChan = DeviceInfoTmp.byStartDChan;

	m_bIsLogin = TRUE;
	return TRUE;
}
/*************************************************
������:    	LogoutVideoPlay
��������:	�ǳ�Ӳ��¼���
�������:
�������:
����ֵ:
**************************************************/
BOOL VideoPlay::LogoutVideoPlay()
{
	return NET_DVR_Logout_V30(m_struDeviceInfo.lLoginID);
}
/*************************************************
������:    	DoGetDeviceResoureCfg
��������:	��ȡ��������õ��豸״̬
�������:
�������:
����ֵ:
**************************************************/
void VideoPlay::DoGetDeviceResoureCfg()
{
	NET_DVR_IPPARACFG_V40 IpAccessCfg;
	memset(&IpAccessCfg, 0, sizeof(IpAccessCfg));
	DWORD  dwReturned;

	m_struDeviceInfo.bIPRet = NET_DVR_GetDVRConfig(m_struDeviceInfo.lLoginID, NET_DVR_GET_IPPARACFG_V40, 0, &IpAccessCfg, sizeof(NET_DVR_IPPARACFG_V40), &dwReturned);

	int i;
	if (!m_struDeviceInfo.bIPRet)   //��֧��ip����,9000�����豸��֧�ֽ���ģ��ͨ��
	{
		for (i = 0; i < MAX_ANALOG_CHANNUM; i++)
		{
			if (i < m_struDeviceInfo.iDeviceChanNum)
			{
				//sprintf(m_struDeviceInfo.struChanInfo[i].chChanName, "camera%d", i + m_struDeviceInfo.iStartChan);
				sprintf(m_struDeviceInfo.struChanInfo[i + MAX_ANALOG_CHANNUM].chChanName, IpAccessCfg.struIPDevInfo->struIP.sIpV4);
				m_struDeviceInfo.struChanInfo[i].iChanIndex = i + m_struDeviceInfo.iStartChan;  //ͨ����
				m_struDeviceInfo.struChanInfo[i].bEnable = TRUE;

			}
			else
			{
				m_struDeviceInfo.struChanInfo[i].iChanIndex = -1;
				m_struDeviceInfo.struChanInfo[i].bEnable = FALSE;
				sprintf(m_struDeviceInfo.struChanInfo[i].chChanName, "");
			}
		}
	}
	else        //֧��IP���룬9000�豸
	{
		for (i = 0; i < MAX_ANALOG_CHANNUM; i++)  //ģ��ͨ��
		{
			if (i < m_struDeviceInfo.iDeviceChanNum)
			{
				//	sprintf(m_struDeviceInfo.struChanInfo[i].chChanName, "camera%d", i + m_struDeviceInfo.iStartChan);
				sprintf(m_struDeviceInfo.struChanInfo[i + MAX_ANALOG_CHANNUM].chChanName, IpAccessCfg.struIPDevInfo->struIP.sIpV4);
				m_struDeviceInfo.struChanInfo[i].iChanIndex = i + m_struDeviceInfo.iStartChan;
				if (IpAccessCfg.byAnalogChanEnable[i])
				{
					m_struDeviceInfo.struChanInfo[i].bEnable = TRUE;
				}
				else
				{
					m_struDeviceInfo.struChanInfo[i].bEnable = FALSE;
				}

			}
			else//clear the state of other channel
			{
				m_struDeviceInfo.struChanInfo[i].iChanIndex = -1;
				m_struDeviceInfo.struChanInfo[i].bEnable = FALSE;
				sprintf(m_struDeviceInfo.struChanInfo[i].chChanName, "");
			}
		}

		//����ͨ��
		for (i = 0; i < MAX_IP_CHANNEL; i++)
		{

			if (IpAccessCfg.struIPDevInfo[i].byEnable)
			{
				m_struDeviceInfo.struChanInfo[i + MAX_ANALOG_CHANNUM].bEnableOnline = TRUE;

				if (IpAccessCfg.struStreamMode[i].uGetStream.struChanInfo.byEnable)  //ipͨ������
				{
					m_struDeviceInfo.struChanInfo[i + MAX_ANALOG_CHANNUM].bEnable = TRUE;
					m_struDeviceInfo.struChanInfo[i + MAX_ANALOG_CHANNUM].iChanIndex = i + IpAccessCfg.dwStartDChan;
					sprintf(m_struDeviceInfo.struChanInfo[i + MAX_ANALOG_CHANNUM].chChanName, IpAccessCfg.struIPDevInfo[i].struIP.sIpV4);
				}
				else
				{
					m_struDeviceInfo.struChanInfo[i + MAX_ANALOG_CHANNUM].bEnable = FALSE;
					m_struDeviceInfo.struChanInfo[i + MAX_ANALOG_CHANNUM].iChanIndex = i + IpAccessCfg.dwStartDChan;
					sprintf(m_struDeviceInfo.struChanInfo[i + MAX_ANALOG_CHANNUM].chChanName, IpAccessCfg.struIPDevInfo[i].struIP.sIpV4);
				}
			}
		}
	}

}
/*************************************************
������:    	GetDevCameraSum
��������:	��ȡ�����Ӹ���
�������:bEnable Ϊtrueʱ��ȡ���ߵ��豸 falseʱ��ȡ˵���豸
�������:arCameraId �����豸ͨ��
����ֵ:
**************************************************/
int VideoPlay::GetDevCameraSum(CStringArray &arCameraId, BOOL bEnable)
{
	int nSum = 0;
	CString csTemp;
	for (int i = 0; i < MAX_CHANNUM_V30; i++)
	{
		if (m_struDeviceInfo.struChanInfo[i].bEnableOnline)
		{
			if (bEnable)
			{
				if (m_struDeviceInfo.struChanInfo[i].bEnable)
				{
					csTemp.Format("%d", m_struDeviceInfo.struChanInfo[i].iChanIndex  - 1);
					arCameraId.Add(csTemp);
				}
				else
				{
					continue;
				}
			}
			csTemp.Format("%d", m_struDeviceInfo.struChanInfo[i].iChanIndex  - 1);
			arCameraId.Add(csTemp);
			nSum++;
		}
	}
	return nSum;
}
/*************************************************
������:    	GetDevCameraNumData
��������:	ͨ���豸ͨ����ȡIP ������״̬
�������:
�������:
����ֵ:
**************************************************/
BOOL VideoPlay::GetDevCameraNumDataToIP(int nIndex, CString &csIP, BOOL & bOnline)
{
	if (m_struDeviceInfo.struChanInfo[nIndex].bEnableOnline)
	{
		csIP = m_struDeviceInfo.struChanInfo[nIndex].chChanName;

		bOnline = m_struDeviceInfo.struChanInfo[nIndex].bEnable;
		return TRUE;
	}
	return FALSE;
}


BOOL VideoPlay::GetDevCameraNumDataToIndex(int &nIndex, CString csIP, BOOL & bOnline)
{
	for (int i = 0; i < MAX_IP_CHANNEL; i++)
	{

		if (m_struDeviceInfo.struChanInfo[i + MAX_ANALOG_CHANNUM].bEnableOnline)
		{
			if (m_struDeviceInfo.struChanInfo[i + MAX_ANALOG_CHANNUM].chChanName == csIP)
			{
				nIndex = i + MAX_ANALOG_CHANNUM + 1;
				bOnline = m_struDeviceInfo.struChanInfo[i + MAX_ANALOG_CHANNUM].bEnable;
				return TRUE;
			}
		}
	}
	return FALSE;
}

/*************************************************
������:    	StartPlay
��������:	��ʼ������Ƶ
�������: iCurChanIndex �豸��ͨ��ֵ��PlayWnd ��Ҫ���ŵ�static�����
�������:PlayHandle ���ز��ųɹ�������ڹر�
����ֵ:
**************************************************/
BOOL VideoPlay::StartPlay(int iCurChanIndex, HWND PlayWnd, LONG &PlayHandle)
{
	NET_DVR_CLIENTINFO ClientInfo;
	ClientInfo.hPlayWnd = PlayWnd;
	ClientInfo.lChannel = iCurChanIndex;
	ClientInfo.lLinkMode = 0;
	ClientInfo.sMultiCastIP = NULL;
	TRACE("Channel number:%d\n", ClientInfo.lChannel);
	PlayHandle = NET_DVR_RealPlay_V30(m_struDeviceInfo.lLoginID, &ClientInfo, NULL, NULL, TRUE);
	if (-1 == PlayHandle)
	{
		DWORD err = NET_DVR_GetLastError();
		return FALSE;
	}

	return TRUE;
}
/*************************************************
������:    	StopPlay
��������:	ͨ������رղ���
�������:PlayHandle ���ž��
�������:
����ֵ:
**************************************************/
void VideoPlay::StopPlay(LONG PlayHandle)
{
	if (PlayHandle != -1)
	{
		if (m_bIsRecording)  //����¼����ֹͣ
		{
			StopRecord(PlayHandle);
		}
		NET_DVR_StopRealPlay(PlayHandle);
		//GetDlgItem(IDC_STATIC_PLAY)->Invalidate();
	}
}
/*************************************************
������:    	StartRecord
��������:	¼��
�������: nCurChanIndex ����ͨ�� PlayHandle ���ž�� csFilPath �ļ�·�� csFileNam �ļ�����Ϊ��
�������:
����ֵ:
**************************************************/
BOOL VideoPlay::StartRecord(int nCurChanIndex, LONG PlayHandle, CString csFilPath, CString csFileNam)
{
	char RecName[256] = { 0 };
	if (csFileNam.IsEmpty())
	{
		CTime CurTime = CTime::GetCurrentTime();;
		sprintf(RecName, "%s%04d%02d%02d%02d%02d%02d_ch%02d.mp4", csFilPath.GetBuffer(csFilPath.GetLength()), CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(), \
			CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond(), m_struDeviceInfo.struChanInfo[nCurChanIndex].iChanIndex);
	}
	else
	{
		sprintf(RecName, "%s%s.mp4", csFilPath.GetBuffer(csFilPath.GetLength()), csFileNam.GetBuffer(csFileNam.GetLength()));
	}

	if (!NET_DVR_SaveRealData(PlayHandle, RecName))
	{
		m_bIsRecording = FALSE;
		return FALSE;
	}
	m_bIsRecording = TRUE;
	return TRUE;
}
/*************************************************
������:    	StopRecord
��������:	ֹͣ¼��
�������:
�������:
����ֵ:
**************************************************/
BOOL VideoPlay::StopRecord(LONG PlayHandle)
{
	if (!NET_DVR_StopSaveRealData(PlayHandle))
	{
		return FALSE;
	}
	m_bIsRecording = FALSE;

	return TRUE;
}


/*************************************************
������:    	CapturePictureBmp
��������:	 ץȡbmpͼƬ
�������:
�������:
����ֵ:
**************************************************/
BOOL VideoPlay::CapturePictureBmp(int nCurChanIndex, LONG PlayHandle, CString csFilPath, CString csFileNam)
{
	char PicName[256] = { 0 };

	if (csFileNam.IsEmpty())
	{

		CTime CurTime = CTime::GetCurrentTime();;
		sprintf(PicName, "%s%04d%02d%02d%02d%02d%02d_ch%02d.bmp", csFilPath.GetBuffer(csFilPath.GetLength()), CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(), \
			CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond(), m_struDeviceInfo.struChanInfo[nCurChanIndex].iChanIndex);
	}
	else
	{
		sprintf(PicName, "%s%s.bmp", csFilPath.GetBuffer(csFilPath.GetLength()), csFileNam.GetBuffer(csFileNam.GetLength()));
	}


	if (NET_DVR_CapturePicture(PlayHandle, PicName))
	{
		return TRUE;
	}

	return FALSE;
}


BOOL VideoPlay::CapturePictureJpg(int nCurChanIndex, LONG PlayHandle,int nSize,int nQuality, CString csFilPath, CString csFileNam)
{
	char PicName[256] = { 0 };

	if (csFileNam.IsEmpty())
	{

		CTime CurTime = CTime::GetCurrentTime();;
		sprintf(PicName, "%s%04d%02d%02d%02d%02d%02d_ch%02d.jpg", csFilPath.GetBuffer(csFilPath.GetLength()), CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(), \
			CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond(), m_struDeviceInfo.struChanInfo[nCurChanIndex].iChanIndex);
	}
	else
	{
		sprintf(PicName, "%s%s.jpg", csFilPath.GetBuffer(csFilPath.GetLength()), csFileNam.GetBuffer(csFileNam.GetLength()));
	}


	NET_DVR_JPEGPARA JpgPara = { 0 };
	JpgPara.wPicSize = (WORD)nSize;
	JpgPara.wPicQuality = (WORD)nQuality;

	LONG iCurChan = m_struDeviceInfo.struChanInfo[nCurChanIndex].iChanIndex;

	if (NET_DVR_CaptureJPEGPicture(m_struDeviceInfo.lLoginID, iCurChan, &JpgPara, PicName))
	{
		return TRUE;
	}

	return FALSE;
}
/*************************************************
������:    	IsLogin
��������:	�ж��Ƿ��¼
�������:
�������:
����ֵ:
**************************************************/
BOOL  VideoPlay::IsLogin()
{
	return m_bIsLogin;
}


UINT GetDownloadPosThread(LPVOID lpam)
{
	GETDOWNLOAD* GetDownload = (GETDOWNLOAD*)lpam;
	LONG lDownloadHandle;
	char fName[200] = { 0 };
	sprintf(fName, "%s%s.avi", GetDownload->csFilPath, GetDownload->csFileNam);

	lDownloadHandle = NET_DVR_GetFileByTime(GetDownload->LoginID, GetDownload->nCurChanIndex,
		&(GetDownload->StartTime), &(GetDownload->StopTime), fName);

	if (lDownloadHandle == -1)
	{

		SendMessage(GetDownload->dlgHwnd, DOWNLOADERROR, GetDownload->nCameraIdex, (LPARAM)GetDownload->csFileNam);
		return -1;
	}


	if (NET_DVR_PlayBackControl(lDownloadHandle, NET_DVR_PLAYSTART, GetDownload->nCameraIdex, NULL))
	{
		BOOL bWhile = TRUE;
		while (bWhile)
		{
			if (g_bDownload)
			{
				NET_DVR_StopGetFile(lDownloadHandle);
				SendMessage(GetDownload->dlgHwnd, DOWNLOADSTORP, GetDownload->nCameraIdex, (LPARAM)GetDownload->csFileNam);
				return 0;
			}
			DWORD pos;
			pos = NET_DVR_GetDownloadPos(lDownloadHandle);
			if (pos == 100)
			{
				bWhile = FALSE;

				NET_DVR_StopGetFile(lDownloadHandle);
				SendMessage(GetDownload->dlgHwnd, DOWNLOADEND, GetDownload->nCameraIdex, (LPARAM)GetDownload->csFileNam);
			}
			else if (pos == 200)
			{
				bWhile = FALSE;

				NET_DVR_StopGetFile(lDownloadHandle);
				SendMessage(GetDownload->dlgHwnd, DOWNLOADERROR, GetDownload->nCameraIdex, (LPARAM)GetDownload->csFileNam);
			}
		}
	}
	return 0;
}
void VideoPlay::StopDownloadPos()
{
	g_bDownload = TRUE;
}

BOOL  VideoPlay::GetDownloadPos(int nCurChanIndex,  int  nCameraIdex, CString csStartTime, CString csEndTime, CString csFileNam, CString csFilPath)
{
	if (csStartTime.GetLength() != 14 || csEndTime.GetLength() != 14 || csFileNam.GetLength() == 0 )
	{
		return FALSE;
	}

	ZeroMemory(&m_GetDownload, sizeof(m_GetDownload));

	g_bDownload = FALSE;

	m_GetDownload.StartTime.dwYear = atoi(csStartTime.Mid(0, 4));
	m_GetDownload.StartTime.dwMonth = atoi(csStartTime.Mid(4, 2));
	m_GetDownload.StartTime.dwDay = atoi(csStartTime.Mid(6, 2));
	m_GetDownload.StartTime.dwHour = atoi(csStartTime.Mid(8, 2));
	m_GetDownload.StartTime.dwMinute = atoi(csStartTime.Mid(10, 2));
	m_GetDownload.StartTime.dwSecond = atoi(csStartTime.Mid(12, 2));

	m_GetDownload.StopTime.dwYear = atoi(csEndTime.Mid(0, 4));
	m_GetDownload.StopTime.dwMonth = atoi(csEndTime.Mid(4, 2));
	m_GetDownload.StopTime.dwDay = atoi(csEndTime.Mid(6, 2));
	m_GetDownload.StopTime.dwHour = atoi(csEndTime.Mid(8, 2));
	m_GetDownload.StopTime.dwMinute = atoi(csEndTime.Mid(10, 2));
	m_GetDownload.StopTime.dwSecond = atoi(csEndTime.Mid(12, 2));

	m_GetDownload.nCurChanIndex = nCurChanIndex;
	m_GetDownload.nCameraIdex = nCameraIdex;

	m_GetDownload.dlgHwnd = m_DlgMessageHwnd;
	m_GetDownload.LoginID = m_struDeviceInfo.lLoginID;;
	sprintf(m_GetDownload.csFileNam, "%s", csFileNam);
	sprintf(m_GetDownload.csFilPath, "%s", csFilPath);

	AfxBeginThread(&GetDownloadPosThread, &m_GetDownload);

	return TRUE;
}

BOOL  VideoPlay::PlayBackByTime(int nCurChanIndex, CString csStartTime, CString csEndTime, HWND playHwnd)
{
	if (csStartTime.GetLength() != 14 || csEndTime.GetLength() != 14)
	{
		return FALSE;
	}


	NET_DVR_TIME StartTime;
	NET_DVR_TIME StopTime;

	StartTime.dwYear = atoi(csStartTime.Mid(0, 4));
	StartTime.dwMonth = atoi(csStartTime.Mid(4, 2));
	StartTime.dwDay = atoi(csStartTime.Mid(6, 2));
	StartTime.dwHour = atoi(csStartTime.Mid(8, 2));
	StartTime.dwMinute = atoi(csStartTime.Mid(10, 2));
	StartTime.dwSecond = atoi(csStartTime.Mid(12, 2));

	StopTime.dwYear = atoi(csEndTime.Mid(0, 4));
	StopTime.dwMonth = atoi(csEndTime.Mid(4, 2));
	StopTime.dwDay = atoi(csEndTime.Mid(6, 2));
	StopTime.dwHour = atoi(csEndTime.Mid(8, 2));
	StopTime.dwMinute = atoi(csEndTime.Mid(10, 2));
	StopTime.dwSecond = atoi(csEndTime.Mid(12, 2));

	m_lPlayHandle = NET_DVR_PlayBackByTime(m_struDeviceInfo.lLoginID, nCurChanIndex, &StartTime, &StopTime, playHwnd);
	if (-1 == m_lPlayHandle)
	{
		return FALSE;
	}

	if (!NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYSTART, 0, NULL))
	{
		return FALSE; 
	}
	m_IsPause = TRUE;

	return TRUE;
}


void VideoPlay::SetBackPlayByTime(int nTyp)
{
	if (m_lPlayHandle >= 0 && !m_IsPause)
	{
		switch (nTyp){
		case 1: 	NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYPAUSE, 0, NULL); break;
		case 2: 	NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYRESTART, 0, NULL); break;
		case 3: 	NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYFAST, 0, NULL); break;
		case 4: 	NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYSLOW, 0, NULL); break;
		case 5: 	NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYNORMAL, 0, NULL); break;
		case 6: 	NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYSTARTAUDIO, 0, NULL); break;
		case 7: 	NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYSTOPAUDIO, 0, NULL); break;
		case 8: 	NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYSTOP, 0, NULL); break;
			
		default: 	NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYNORMAL, 0, NULL); break;
		}
	
	}
}

