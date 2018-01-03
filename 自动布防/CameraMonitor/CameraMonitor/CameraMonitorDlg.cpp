
// CameraMonitorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CameraMonitor.h"
#include "CameraMonitorDlg.h"
#include "afxdialogex.h"
#include "CameraSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define  TREE_ROOT				0
#define  TREE_INIT				1
#define  TREE_OK				2
#define  TREE_NO				3

#define  PROGRAM_NAME					"CameraMonitorError.log" 
#define  RETURN_OK						"成功"
#define  RETURN_NO						"失败"
static CCameraMonitorDlg * g_pMainDlg = NULL;	//

static _ConnectionPtr  g_pConnect;
static CAMERA_INFO * g_CameraArrInfo;
static int g_CameraSum = 0;
static CString g_csPath;// = "C:\\Picture";
UINT ThreadSetupAlarmChan(LPVOID lparam);

/*********************************************************
Function:	g_ExceptionCallBack
Desc:		exception callback function
Input:	dwType: message type;lUserID: log-in device ID;lHandle, handle of connected channel ; pUser, user data；
Output:
Return:
**********************************************************/
void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
	UNREFERENCED_PARAMETER(pUser);

	int i = 0, j = 0;
	int iDeviceIndex = -1;
	CString sTemp;

	for (i = 0; i < MAX_DEVICES; i++)
	{
		if (g_CameraArrInfo[i].lUserID == lUserID)
		{
			iDeviceIndex = i;
			break;
		}
	}
	if (dwType == EXCEPTION_RELOGIN)
	{
		//g_pMainDlg->SetListOut("重启", g_CameraArrInfo[m_nCameraIndex].csCameraIp, "正在重启");
	}
	else if (dwType == RELOGIN_SUCCESS)
	{
		g_pMainDlg->SetListOut("重启", g_CameraArrInfo[g_pMainDlg->m_nCameraIndex].csCameraIp, "重启完成");
	}

	return;
}

void CALLBACK MessageCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void* pUser)
{
	UNREFERENCED_PARAMETER(pUser);
	UINT iDeviceIndex = 0xffff;

	for (int i = 0; i < MAX_DEVICES; i++)
	{
		if (strcmp(g_CameraArrInfo[i].csCameraIp, pAlarmer->sDeviceIP) == 0)
		{
			iDeviceIndex = i;
			break;
		}
	}

	if (iDeviceIndex >= MAX_DEVICES || iDeviceIndex < 0)
		return;
	try
	{
		char *pAlarmMsg = NULL;
		if (lCommand == COMM_ALARM_WALL_CONFERNECE)
		{
			pAlarmMsg = new char[sizeof(DWORD)+dwBufLen];
			if (pAlarmMsg == NULL)
			{
				return;
			}
			memset(pAlarmMsg, 0, sizeof(DWORD)+dwBufLen);
			memcpy(pAlarmMsg, &dwBufLen, sizeof(DWORD));
			memcpy(pAlarmMsg + sizeof(DWORD), pAlarmInfo, dwBufLen);
		}
		else
		{
			pAlarmMsg = new char[dwBufLen];
			if (pAlarmMsg == NULL)
			{
				return;
			}
			memset(pAlarmMsg, 0, dwBufLen);
			memcpy(pAlarmMsg, pAlarmInfo, dwBufLen);
		}

		if (lCommand == COMM_ALARM_RULE)
		{
			((LPNET_VCA_RULE_ALARM)pAlarmMsg)->pImage = new BYTE[((LPNET_VCA_RULE_ALARM)pAlarmMsg)->dwPicDataLen];
			memset(((LPNET_VCA_RULE_ALARM)pAlarmMsg)->pImage, 0, ((LPNET_VCA_RULE_ALARM)pAlarmMsg)->dwPicDataLen);
			memcpy(((LPNET_VCA_RULE_ALARM)pAlarmMsg)->pImage, ((LPNET_VCA_RULE_ALARM)pAlarmInfo)->pImage, ((LPNET_VCA_RULE_ALARM)pAlarmMsg)->dwPicDataLen);
		}
		else if (COMM_ALARM_AID == lCommand)
		{
			((LPNET_DVR_AID_ALARM)pAlarmMsg)->pImage = new BYTE[((LPNET_DVR_AID_ALARM)pAlarmMsg)->dwPicDataLen];
			memset(((LPNET_DVR_AID_ALARM)pAlarmMsg)->pImage, 0, ((LPNET_DVR_AID_ALARM)pAlarmMsg)->dwPicDataLen);
			memcpy(((LPNET_DVR_AID_ALARM)pAlarmMsg)->pImage, ((LPNET_DVR_AID_ALARM)pAlarmInfo)->pImage, ((LPNET_DVR_AID_ALARM)pAlarmMsg)->dwPicDataLen);
		}
		else if (COMM_ALARM_FACE == lCommand)
		{
			((LPNET_DVR_FACEDETECT_ALARM)pAlarmMsg)->pImage = new BYTE[((LPNET_DVR_FACEDETECT_ALARM)pAlarmMsg)->dwPicDataLen];
			memset(((LPNET_DVR_FACEDETECT_ALARM)pAlarmMsg)->pImage, 0, ((LPNET_DVR_FACEDETECT_ALARM)pAlarmMsg)->dwPicDataLen);
			memcpy(((LPNET_DVR_FACEDETECT_ALARM)pAlarmMsg)->pImage, ((LPNET_DVR_FACEDETECT_ALARM)pAlarmInfo)->pImage, ((LPNET_DVR_FACEDETECT_ALARM)pAlarmMsg)->dwPicDataLen);

			((LPNET_DVR_FACEDETECT_ALARM)pAlarmMsg)->pFaceImage = new BYTE[((LPNET_DVR_FACEDETECT_ALARM)pAlarmMsg)->dwFacePicDataLen];
			memset(((LPNET_DVR_FACEDETECT_ALARM)pAlarmMsg)->pFaceImage, 0, ((LPNET_DVR_FACEDETECT_ALARM)pAlarmMsg)->dwFacePicDataLen);
			memcpy(((LPNET_DVR_FACEDETECT_ALARM)pAlarmMsg)->pFaceImage, ((LPNET_DVR_FACEDETECT_ALARM)pAlarmInfo)->pFaceImage, ((LPNET_DVR_FACEDETECT_ALARM)pAlarmMsg)->dwFacePicDataLen);

		}
		else if (COMM_UPLOAD_FACESNAP_RESULT == lCommand)
		{
			((LPNET_VCA_FACESNAP_RESULT)pAlarmMsg)->pBuffer1 = new BYTE[((LPNET_VCA_FACESNAP_RESULT)pAlarmMsg)->dwFacePicLen];
			memset(((LPNET_VCA_FACESNAP_RESULT)pAlarmMsg)->pBuffer1, 0, ((LPNET_VCA_FACESNAP_RESULT)pAlarmMsg)->dwFacePicLen);
			memcpy(((LPNET_VCA_FACESNAP_RESULT)pAlarmMsg)->pBuffer1, ((LPNET_VCA_FACESNAP_RESULT)pAlarmInfo)->pBuffer1, ((LPNET_VCA_FACESNAP_RESULT)pAlarmMsg)->dwFacePicLen);

			((LPNET_VCA_FACESNAP_RESULT)pAlarmMsg)->pBuffer2 = new BYTE[((LPNET_VCA_FACESNAP_RESULT)pAlarmMsg)->dwBackgroundPicLen];
			memset(((LPNET_VCA_FACESNAP_RESULT)pAlarmMsg)->pBuffer2, 0, ((LPNET_VCA_FACESNAP_RESULT)pAlarmMsg)->dwBackgroundPicLen);
			memcpy(((LPNET_VCA_FACESNAP_RESULT)pAlarmMsg)->pBuffer2, ((LPNET_VCA_FACESNAP_RESULT)pAlarmInfo)->pBuffer2, ((LPNET_VCA_FACESNAP_RESULT)pAlarmMsg)->dwBackgroundPicLen);

		}
		else if (COMM_ALARM_FACE_DETECTION == lCommand)
		{
			((LPNET_DVR_FACE_DETECTION)pAlarmMsg)->pBackgroundPicpBuffer = new BYTE[((LPNET_DVR_FACE_DETECTION)pAlarmMsg)->dwBackgroundPicLen];
			memset(((LPNET_DVR_FACE_DETECTION)pAlarmMsg)->pBackgroundPicpBuffer, 0, ((LPNET_DVR_FACE_DETECTION)pAlarmMsg)->dwBackgroundPicLen);
			memcpy(((LPNET_DVR_FACE_DETECTION)pAlarmMsg)->pBackgroundPicpBuffer, ((LPNET_DVR_FACE_DETECTION)pAlarmInfo)->pBackgroundPicpBuffer, \
				((LPNET_DVR_FACE_DETECTION)pAlarmMsg)->dwBackgroundPicLen);
		}
		else if (COMM_SNAP_MATCH_ALARM == lCommand)
		{
			((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struSnapInfo.pBuffer1 = new BYTE[((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struSnapInfo.dwSnapFacePicLen];
			memset(((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struSnapInfo.pBuffer1, 0, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struSnapInfo.dwSnapFacePicLen);
			memcpy(((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struSnapInfo.pBuffer1, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmInfo)->struSnapInfo.pBuffer1, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struSnapInfo.dwSnapFacePicLen);

			((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.pBuffer1 = new BYTE[((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.dwBlackListPicLen];
			memset(((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.pBuffer1, 0, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.dwBlackListPicLen);
			memcpy(((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.pBuffer1, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmInfo)->struBlackListInfo.pBuffer1, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.dwBlackListPicLen);

			((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->pSnapPicBuffer = new BYTE[((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->dwSnapPicLen];
			memset(((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->pSnapPicBuffer, 0, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->dwSnapPicLen);
			memcpy(((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->pSnapPicBuffer, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmInfo)->pSnapPicBuffer, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->dwSnapPicLen);


			((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->pModelDataBuffer = new BYTE[((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->dwModelDataLen];
			memset(((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->pModelDataBuffer, 0, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->dwModelDataLen);
			memcpy(((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->pModelDataBuffer, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmInfo)->pModelDataBuffer, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->dwModelDataLen);

			((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.struBlackListInfo.struAttribute.pPersonInfoExtend = new BYTE[((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.struBlackListInfo.struAttribute.dwPersonInfoExtendLen];
			memset(((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.struBlackListInfo.struAttribute.pPersonInfoExtend, 0, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.struBlackListInfo.struAttribute.dwPersonInfoExtendLen);
			memcpy(((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.struBlackListInfo.struAttribute.pPersonInfoExtend, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmInfo)->struBlackListInfo.struBlackListInfo.struAttribute.pPersonInfoExtend, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.struBlackListInfo.struAttribute.dwPersonInfoExtendLen);

			((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struSnapInfo.pUIDBuffer = new BYTE[((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struSnapInfo.dwUIDLen];
			memset(((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struSnapInfo.pUIDBuffer, 0, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struSnapInfo.dwUIDLen);
			memcpy(((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struSnapInfo.pUIDBuffer, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmInfo)->struSnapInfo.pUIDBuffer, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struSnapInfo.dwUIDLen);

			((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.pFDID = new BYTE[((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.dwFDIDLen];
			memset(((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.pFDID, 0, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.dwFDIDLen);
			memcpy(((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.pFDID, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmInfo)->struBlackListInfo.pFDID, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.dwFDIDLen);

			((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.pPID = new BYTE[((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.dwPIDLen];
			memset(((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.pPID, 0, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.dwPIDLen);
			memcpy(((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.pPID, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmInfo)->struBlackListInfo.pPID, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.dwPIDLen);

			((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.struBlackListInfo.pFDDescriptionBuffer = new BYTE[((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.struBlackListInfo.dwFDDescriptionLen];
			memset(((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.struBlackListInfo.pFDDescriptionBuffer, 0, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.struBlackListInfo.dwFDDescriptionLen);
			memcpy(((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.struBlackListInfo.pFDDescriptionBuffer, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmInfo)->struBlackListInfo.struBlackListInfo.pFDDescriptionBuffer, ((LPNET_VCA_FACESNAP_MATCH_ALARM)pAlarmMsg)->struBlackListInfo.struBlackListInfo.dwFDDescriptionLen);
		}
		else if (COMM_ALARM_CAPTURE_UPLOAD == lCommand)
		{
			((LPNET_DVR_CAPTURE_UPLOAD)pAlarmMsg)->pBuffer = new BYTE[((LPNET_DVR_CAPTURE_UPLOAD)pAlarmMsg)->dwPicLen];
			memset(((LPNET_DVR_CAPTURE_UPLOAD)pAlarmMsg)->pBuffer, 0, ((LPNET_DVR_CAPTURE_UPLOAD)pAlarmMsg)->dwPicLen);
			memcpy(((LPNET_DVR_CAPTURE_UPLOAD)pAlarmMsg)->pBuffer, ((LPNET_DVR_CAPTURE_UPLOAD)pAlarmInfo)->pBuffer, ((LPNET_DVR_CAPTURE_UPLOAD)pAlarmMsg)->dwPicLen);
		}
		else if (COMM_FACESNAP_RAWDATA_ALARM == lCommand)
		{
			((LPNET_VCA_FACESNAP_RAWDATA_ALARM)pAlarmMsg)->pJsonBuff = new BYTE[((LPNET_VCA_FACESNAP_RAWDATA_ALARM)pAlarmMsg)->dwJsonDataLen];
			memset(((LPNET_VCA_FACESNAP_RAWDATA_ALARM)pAlarmMsg)->pJsonBuff, 0, ((LPNET_VCA_FACESNAP_RAWDATA_ALARM)pAlarmMsg)->dwJsonDataLen);
			memcpy(((LPNET_VCA_FACESNAP_RAWDATA_ALARM)pAlarmMsg)->pJsonBuff, ((LPNET_VCA_FACESNAP_RAWDATA_ALARM)pAlarmInfo)->pJsonBuff, ((LPNET_VCA_FACESNAP_RAWDATA_ALARM)pAlarmMsg)->dwJsonDataLen);
		}
		else if (COMM_FRAMES_PEOPLE_COUNTING_ALARM == lCommand)
		{
			((LPNET_DVR_FRAMES_PEOPLE_COUNTING)pAlarmMsg)->pPicBuffer = new BYTE[((LPNET_DVR_FRAMES_PEOPLE_COUNTING)pAlarmMsg)->dwPicLen];
			memset(((LPNET_DVR_FRAMES_PEOPLE_COUNTING)pAlarmMsg)->pPicBuffer, 0, ((LPNET_DVR_FRAMES_PEOPLE_COUNTING)pAlarmMsg)->dwPicLen);
			memcpy(((LPNET_DVR_FRAMES_PEOPLE_COUNTING)pAlarmMsg)->pPicBuffer, ((LPNET_DVR_FRAMES_PEOPLE_COUNTING)pAlarmInfo)->pPicBuffer, ((LPNET_DVR_FRAMES_PEOPLE_COUNTING)pAlarmMsg)->dwPicLen);
		}
		else if (COMM_FIREDETECTION_ALARM == lCommand)
		{
			((LPNET_DVR_FIREDETECTION_ALARM)pAlarmMsg)->pBuffer = new BYTE[((LPNET_DVR_FIREDETECTION_ALARM)pAlarmMsg)->dwPicDataLen];
			memset(((LPNET_DVR_FIREDETECTION_ALARM)pAlarmMsg)->pBuffer, 0, ((LPNET_DVR_FIREDETECTION_ALARM)pAlarmMsg)->dwPicDataLen);
			memcpy(((LPNET_DVR_FIREDETECTION_ALARM)pAlarmMsg)->pBuffer, ((LPNET_DVR_FIREDETECTION_ALARM)pAlarmInfo)->pBuffer, ((LPNET_DVR_FIREDETECTION_ALARM)pAlarmMsg)->dwPicDataLen);

			((LPNET_DVR_FIREDETECTION_ALARM)pAlarmMsg)->pVisiblePicBuf = new BYTE[((LPNET_DVR_FIREDETECTION_ALARM)pAlarmMsg)->dwVisiblePicLen];
			memset(((LPNET_DVR_FIREDETECTION_ALARM)pAlarmMsg)->pVisiblePicBuf, 0, ((LPNET_DVR_FIREDETECTION_ALARM)pAlarmMsg)->dwVisiblePicLen);
			memcpy(((LPNET_DVR_FIREDETECTION_ALARM)pAlarmMsg)->pVisiblePicBuf, ((LPNET_DVR_FIREDETECTION_ALARM)pAlarmInfo)->pVisiblePicBuf, ((LPNET_DVR_FIREDETECTION_ALARM)pAlarmMsg)->dwVisiblePicLen);
			if (((LPNET_DVR_FIREDETECTION_ALARM)pAlarmInfo)->byAlarmSubType == 1 || ((LPNET_DVR_FIREDETECTION_ALARM)pAlarmInfo)->byAlarmSubType == 2)
			{
				((LPNET_DVR_FIREDETECTION_ALARM)pAlarmMsg)->pSmokeBuf = new BYTE[sizeof(NET_DVR_SMOKEDETECTION_ALARM)];
				memset(((LPNET_DVR_FIREDETECTION_ALARM)pAlarmMsg)->pSmokeBuf, 0, sizeof(NET_DVR_SMOKEDETECTION_ALARM));
				memcpy(((LPNET_DVR_FIREDETECTION_ALARM)pAlarmMsg)->pSmokeBuf, ((LPNET_DVR_FIREDETECTION_ALARM)pAlarmInfo)->pSmokeBuf, sizeof(NET_DVR_SMOKEDETECTION_ALARM));
			}
		}
		else if (COMM_ALARM_SHIPSDETECTION == lCommand)
		{
			((LPNET_DVR_SHIPSDETECTION_ALARM)pAlarmMsg)->pPicBuffer = new BYTE[((LPNET_DVR_SHIPSDETECTION_ALARM)pAlarmMsg)->dwPicLen];
			memset(((LPNET_DVR_SHIPSDETECTION_ALARM)pAlarmMsg)->pPicBuffer, 0, ((LPNET_DVR_SHIPSDETECTION_ALARM)pAlarmMsg)->dwPicLen);
			memcpy(((LPNET_DVR_SHIPSDETECTION_ALARM)pAlarmMsg)->pPicBuffer, ((LPNET_DVR_SHIPSDETECTION_ALARM)pAlarmInfo)->pPicBuffer, ((LPNET_DVR_SHIPSDETECTION_ALARM)pAlarmMsg)->dwPicLen);

			((LPNET_DVR_SHIPSDETECTION_ALARM)pAlarmMsg)->pThermalPicBuffer = new BYTE[((LPNET_DVR_SHIPSDETECTION_ALARM)pAlarmMsg)->dwThermalPicLen];
			memset(((LPNET_DVR_SHIPSDETECTION_ALARM)pAlarmMsg)->pThermalPicBuffer, 0, ((LPNET_DVR_SHIPSDETECTION_ALARM)pAlarmMsg)->dwThermalPicLen);
			memcpy(((LPNET_DVR_SHIPSDETECTION_ALARM)pAlarmMsg)->pThermalPicBuffer, ((LPNET_DVR_SHIPSDETECTION_ALARM)pAlarmInfo)->pThermalPicBuffer, ((LPNET_DVR_SHIPSDETECTION_ALARM)pAlarmMsg)->dwThermalPicLen);
		}
		else if (COMM_VEHICLE_CONTROL_ALARM == lCommand)
		{
			((LPNET_DVR_VEHICLE_CONTROL_ALARM)pAlarmMsg)->pPicData = new char[((LPNET_DVR_VEHICLE_CONTROL_ALARM)pAlarmMsg)->dwPicDataLen];
			memset(((LPNET_DVR_VEHICLE_CONTROL_ALARM)pAlarmMsg)->pPicData, 0, ((LPNET_DVR_VEHICLE_CONTROL_ALARM)pAlarmMsg)->dwPicDataLen);
			memcpy(((LPNET_DVR_VEHICLE_CONTROL_ALARM)pAlarmMsg)->pPicData, ((LPNET_DVR_VEHICLE_CONTROL_ALARM)pAlarmInfo)->pPicData, ((LPNET_DVR_VEHICLE_CONTROL_ALARM)pAlarmMsg)->dwPicDataLen);
		}
		else if (COMM_THERMOMETRY_ALARM == lCommand)
		{
			((LPNET_DVR_THERMOMETRY_ALARM)pAlarmMsg)->pPicBuff = new char[((LPNET_DVR_THERMOMETRY_ALARM)pAlarmMsg)->dwPicLen];
			memset(((LPNET_DVR_THERMOMETRY_ALARM)pAlarmMsg)->pPicBuff, 0, ((LPNET_DVR_THERMOMETRY_ALARM)pAlarmMsg)->dwPicLen);
			memcpy(((LPNET_DVR_THERMOMETRY_ALARM)pAlarmMsg)->pPicBuff, ((LPNET_DVR_THERMOMETRY_ALARM)pAlarmInfo)->pPicBuff, ((LPNET_DVR_THERMOMETRY_ALARM)pAlarmMsg)->dwPicLen);

			((LPNET_DVR_THERMOMETRY_ALARM)pAlarmMsg)->pThermalPicBuff = new char[((LPNET_DVR_THERMOMETRY_ALARM)pAlarmMsg)->dwThermalPicLen];
			memset(((LPNET_DVR_THERMOMETRY_ALARM)pAlarmMsg)->pThermalPicBuff, 0, ((LPNET_DVR_THERMOMETRY_ALARM)pAlarmMsg)->dwThermalPicLen);
			memcpy(((LPNET_DVR_THERMOMETRY_ALARM)pAlarmMsg)->pThermalPicBuff, ((LPNET_DVR_THERMOMETRY_ALARM)pAlarmInfo)->pThermalPicBuff, ((LPNET_DVR_THERMOMETRY_ALARM)pAlarmMsg)->dwThermalPicLen);

			((LPNET_DVR_THERMOMETRY_ALARM)pAlarmMsg)->pThermalInfoBuff = new char[((LPNET_DVR_THERMOMETRY_ALARM)pAlarmMsg)->dwThermalInfoLen];
			memset(((LPNET_DVR_THERMOMETRY_ALARM)pAlarmMsg)->pThermalInfoBuff, 0, ((LPNET_DVR_THERMOMETRY_ALARM)pAlarmMsg)->dwThermalInfoLen);
			memcpy(((LPNET_DVR_THERMOMETRY_ALARM)pAlarmMsg)->pThermalInfoBuff, ((LPNET_DVR_THERMOMETRY_ALARM)pAlarmInfo)->pThermalInfoBuff, ((LPNET_DVR_THERMOMETRY_ALARM)pAlarmMsg)->dwThermalInfoLen);
		}
		else if (COMM_THERMOMETRY_DIFF_ALARM == lCommand)
		{
			((LPNET_DVR_THERMOMETRY_DIFF_ALARM)pAlarmMsg)->pPicBuff = new char[((LPNET_DVR_THERMOMETRY_DIFF_ALARM)pAlarmMsg)->dwPicLen];
			memset(((LPNET_DVR_THERMOMETRY_DIFF_ALARM)pAlarmMsg)->pPicBuff, 0, ((LPNET_DVR_THERMOMETRY_DIFF_ALARM)pAlarmMsg)->dwPicLen);
			memcpy(((LPNET_DVR_THERMOMETRY_DIFF_ALARM)pAlarmMsg)->pPicBuff, ((LPNET_DVR_THERMOMETRY_DIFF_ALARM)pAlarmInfo)->pPicBuff, ((LPNET_DVR_THERMOMETRY_DIFF_ALARM)pAlarmMsg)->dwPicLen);

			((LPNET_DVR_THERMOMETRY_DIFF_ALARM)pAlarmMsg)->pThermalPicBuff = new char[((LPNET_DVR_THERMOMETRY_DIFF_ALARM)pAlarmMsg)->dwThermalPicLen];
			memset(((LPNET_DVR_THERMOMETRY_DIFF_ALARM)pAlarmMsg)->pThermalPicBuff, 0, ((LPNET_DVR_THERMOMETRY_DIFF_ALARM)pAlarmMsg)->dwThermalPicLen);
			memcpy(((LPNET_DVR_THERMOMETRY_DIFF_ALARM)pAlarmMsg)->pThermalPicBuff, ((LPNET_DVR_THERMOMETRY_DIFF_ALARM)pAlarmInfo)->pThermalPicBuff, ((LPNET_DVR_THERMOMETRY_DIFF_ALARM)pAlarmMsg)->dwThermalPicLen);

			((LPNET_DVR_THERMOMETRY_DIFF_ALARM)pAlarmMsg)->pThermalInfoBuff = new char[((LPNET_DVR_THERMOMETRY_DIFF_ALARM)pAlarmMsg)->dwThermalInfoLen];
			memset(((LPNET_DVR_THERMOMETRY_DIFF_ALARM)pAlarmMsg)->pThermalInfoBuff, 0, ((LPNET_DVR_THERMOMETRY_DIFF_ALARM)pAlarmMsg)->dwThermalInfoLen);
			memcpy(((LPNET_DVR_THERMOMETRY_DIFF_ALARM)pAlarmMsg)->pThermalInfoBuff, ((LPNET_DVR_THERMOMETRY_DIFF_ALARM)pAlarmInfo)->pThermalInfoBuff, ((LPNET_DVR_THERMOMETRY_DIFF_ALARM)pAlarmMsg)->dwThermalInfoLen);
		}
		else if (COMM_VEHICLE_RECOG_RESULT == lCommand)
		{
			//车辆
			((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->pVehicleBuffer = new BYTE[((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->dwVehicleBufferLen];
			memset(((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->pVehicleBuffer, 0, ((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->dwVehicleBufferLen);
			memcpy(((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->pVehicleBuffer, \
				((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmInfo)->pVehicleBuffer, ((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->dwVehicleBufferLen);
			//车牌
			((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->pPlateBuffer = new BYTE[((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->dwPlateBufferLen];
			memset(((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->pPlateBuffer, 0, ((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->dwPlateBufferLen);
			memcpy(((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->pPlateBuffer, \
				((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmInfo)->pPlateBuffer, ((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->dwPlateBufferLen);
			//人脸（主驾驶)
			((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->pPilotFaceBuffer = new BYTE[((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->dwPilotFaceBufferLen];
			memset(((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->pPilotFaceBuffer, 0, ((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->dwPilotFaceBufferLen);
			memcpy(((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->pPilotFaceBuffer, \
				((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmInfo)->pPilotFaceBuffer, ((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->dwPilotFaceBufferLen);
			//人脸（副驾驶）
			((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->pCopilotFaceBuffer = new BYTE[((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->dwCopilotFaceBufferLen];
			memset(((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->pCopilotFaceBuffer, 0, ((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->dwCopilotFaceBufferLen);
			memcpy(((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->pCopilotFaceBuffer, \
				((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmInfo)->pCopilotFaceBuffer, ((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->dwCopilotFaceBufferLen);
			//安全带（主驾驶）
			((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->pPilotSafebeltBuffer = new BYTE[((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->dwPilotSafebeltBufferLen];
			memset(((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->pPilotSafebeltBuffer, 0, ((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->dwPilotSafebeltBufferLen);
			memcpy(((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->pPilotSafebeltBuffer, \
				((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmInfo)->pPilotSafebeltBuffer, ((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->dwPilotSafebeltBufferLen);
			//安全带（副驾驶）
			((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->pCopilotSafebeltBuffer = new BYTE[((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->dwCopilotSafebeltBufferLen];
			memset(((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->pCopilotSafebeltBuffer, 0, ((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->dwCopilotSafebeltBufferLen);
			memcpy(((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->pCopilotSafebeltBuffer, \
				((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmInfo)->pCopilotSafebeltBuffer, ((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->dwCopilotSafebeltBufferLen);
			//json
			((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->pJsonBuffer = new BYTE[((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->dwJsonBufferLen];
			memset(((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->pJsonBuffer, 0, ((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->dwJsonBufferLen);
			memcpy(((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->pJsonBuffer, \
				((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmInfo)->pJsonBuffer, ((LPNET_DVR_VEHICLE_RECOG_RESULT)pAlarmMsg)->dwJsonBufferLen);

		}
		else if (COMM_ITS_PLATE_RESULT == lCommand)
		{
			int i = 0;
			DWORD dwPrePicLen = 0;
			for (i = 0; i < 6; i++)
			{
				if (((LPNET_ITS_PLATE_RESULT)pAlarmMsg)->struPicInfo[i].dwDataLen > 0)
				{
					((LPNET_ITS_PLATE_RESULT)pAlarmMsg)->struPicInfo[i].pBuffer = (BYTE*)pAlarmMsg + sizeof(NET_ITS_PLATE_RESULT)+dwPrePicLen;
					dwPrePicLen += ((LPNET_ITS_PLATE_RESULT)pAlarmMsg)->struPicInfo[i].dwDataLen;
				}
			}

			if (1 == ((LPNET_ITS_PLATE_RESULT)pAlarmMsg)->byIllegalFromatType)
			{
				((LPNET_ITS_PLATE_RESULT)pAlarmMsg)->pIllegalInfoBuf = new BYTE[sizeof(NET_ITS_ILLEGAL_INFO)];
				memset(((LPNET_ITS_PLATE_RESULT)pAlarmMsg)->pIllegalInfoBuf, 0, sizeof(NET_ITS_ILLEGAL_INFO));
				memcpy(((LPNET_ITS_PLATE_RESULT)pAlarmMsg)->pIllegalInfoBuf, ((LPNET_ITS_PLATE_RESULT)pAlarmInfo)->pIllegalInfoBuf, \
					sizeof(NET_ITS_ILLEGAL_INFO));
			}
		}
		else if (COMM_PEOPLE_DETECTION_UPLOAD == lCommand)
		{
			int i = 0;
			DWORD dwPrePicLen = 0;
			for (i = 0; i < MAX_PEOPLE_DETECTION_NUM; i++)
			{
				if (((LPNET_DVR_PEOPLE_DETECTION_RESULT)pAlarmMsg)->struPeopleRegion[i].dwPicLen > 0)
				{
					((LPNET_DVR_PEOPLE_DETECTION_RESULT)pAlarmMsg)->struPeopleRegion[i].pPicBuffer = (char*)pAlarmMsg + sizeof(NET_DVR_PEOPLE_DETECTION_RESULT)+dwPrePicLen;
					dwPrePicLen += ((LPNET_DVR_PEOPLE_DETECTION_RESULT)pAlarmMsg)->struPeopleRegion[i].dwPicLen;
				}
			}
		}
		else if (COMM_TME_VEHICLE_INDENTIFICATION == lCommand)
		{
			int i = 0;
			DWORD dwPrePicLen = 0;
			for (i = 0; i < 4; i++)
			{
				if (((LPNET_DVR_TME_VEHICLE_RESULT)pAlarmMsg)->struPicInfo[i].dwDataLen > 0)
				{
					((LPNET_DVR_TME_VEHICLE_RESULT)pAlarmMsg)->struPicInfo[i].pBuffer = (BYTE*)pAlarmMsg + sizeof(NET_DVR_TME_VEHICLE_RESULT)+dwPrePicLen;
					dwPrePicLen += ((LPNET_DVR_TME_VEHICLE_RESULT)pAlarmMsg)->struPicInfo[i].dwDataLen;
				}
			}
		}
		else if (COMM_PLATE_RESULT_V50 == lCommand)
		{
			int i = 0;
			DWORD dwPrePicLen = 0;
			for (i = 0; i < 6; i++)
			{
				if (((LPNET_DVR_PLATE_RESULT_V50)pAlarmMsg)->struPicInfo[i].dwDataLen > 0)
				{
					((LPNET_DVR_PLATE_RESULT_V50)pAlarmMsg)->struPicInfo[i].pBuffer = (BYTE*)pAlarmMsg + sizeof(NET_DVR_PLATE_RESULT_V50)+dwPrePicLen;
					dwPrePicLen += ((LPNET_DVR_PLATE_RESULT_V50)pAlarmMsg)->struPicInfo[i].dwDataLen;
				}
			}
		}
		else if (COMM_ITS_GATE_VEHICLE == lCommand)
		{
			int i = 0;
			DWORD dwPrePicLen = 0;
			for (i = 0; i < 4; i++)
			{
				if (((LPNET_ITS_GATE_VEHICLE)pAlarmMsg)->struPicInfo[i].dwDataLen > 0)
				{
					((LPNET_ITS_GATE_VEHICLE)pAlarmMsg)->struPicInfo[i].pBuffer = (BYTE*)pAlarmMsg + sizeof(NET_ITS_GATE_VEHICLE)+dwPrePicLen;
					dwPrePicLen += ((LPNET_ITS_GATE_VEHICLE)pAlarmMsg)->struPicInfo[i].dwDataLen;
				}
			}
		}
		else if (COMM_ITS_GATE_FACE == lCommand)
		{
			((LPNET_ITS_GATE_FACE)pAlarmMsg)->struFaceInfo.pBuffer1 = new BYTE[((LPNET_ITS_GATE_FACE)pAlarmMsg)->struFaceInfo.dwFacePicLen];
			memset(((LPNET_ITS_GATE_FACE)pAlarmMsg)->struFaceInfo.pBuffer1, 0, ((LPNET_ITS_GATE_FACE)pAlarmMsg)->struFaceInfo.dwFacePicLen);
			memcpy(((LPNET_ITS_GATE_FACE)pAlarmMsg)->struFaceInfo.pBuffer1, ((LPNET_ITS_GATE_FACE)pAlarmInfo)->struFaceInfo.pBuffer1, \
				((LPNET_ITS_GATE_FACE)pAlarmMsg)->struFaceInfo.dwFacePicLen);

			((LPNET_ITS_GATE_FACE)pAlarmMsg)->struFaceInfo.pBuffer2 = new BYTE[((LPNET_ITS_GATE_FACE)pAlarmMsg)->struFaceInfo.dwBackgroundPicLen];
			memset(((LPNET_ITS_GATE_FACE)pAlarmMsg)->struFaceInfo.pBuffer2, 0, ((LPNET_ITS_GATE_FACE)pAlarmMsg)->struFaceInfo.dwBackgroundPicLen);
			memcpy(((LPNET_ITS_GATE_FACE)pAlarmMsg)->struFaceInfo.pBuffer2, ((LPNET_ITS_GATE_FACE)pAlarmInfo)->struFaceInfo.pBuffer2, \
				((LPNET_ITS_GATE_FACE)pAlarmMsg)->struFaceInfo.dwBackgroundPicLen);

		}
		else if (COMM_ITS_PARK_VEHICLE == lCommand)
		{
			int i = 0;
			DWORD dwPrePicLen = 0;
			for (i = 0; i < 2; i++)
			{
				if (((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->struPicInfo[i].dwDataLen > 0)
				{
					((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->struPicInfo[i].pBuffer = (BYTE*)pAlarmMsg + sizeof(NET_ITS_PARK_VEHICLE)+dwPrePicLen;
					dwPrePicLen += ((LPNET_ITS_PARK_VEHICLE)pAlarmMsg)->struPicInfo[i].dwDataLen;
				}
			}
		}
		else if (COMM_ALARM_AID_V41 == lCommand)
		{
			((LPNET_DVR_AID_ALARM_V41)pAlarmMsg)->pImage = new BYTE[((LPNET_DVR_AID_ALARM_V41)pAlarmMsg)->dwPicDataLen];
			memset(((LPNET_DVR_AID_ALARM_V41)pAlarmMsg)->pImage, 0, ((LPNET_DVR_AID_ALARM_V41)pAlarmMsg)->dwPicDataLen);
			memcpy(((LPNET_DVR_AID_ALARM_V41)pAlarmMsg)->pImage, ((LPNET_DVR_AID_ALARM_V41)pAlarmInfo)->pImage, ((LPNET_DVR_AID_ALARM_V41)pAlarmMsg)->dwPicDataLen);

			((LPNET_DVR_AID_ALARM_V41)pAlarmMsg)->pXmlBuf = new char[((LPNET_DVR_AID_ALARM_V41)pAlarmMsg)->dwXmlLen];
			memset(((LPNET_DVR_AID_ALARM_V41)pAlarmMsg)->pXmlBuf, 0, ((LPNET_DVR_AID_ALARM_V41)pAlarmMsg)->dwXmlLen);
			memcpy(((LPNET_DVR_AID_ALARM_V41)pAlarmMsg)->pXmlBuf, ((LPNET_DVR_AID_ALARM_V41)pAlarmInfo)->pXmlBuf, ((LPNET_DVR_AID_ALARM_V41)pAlarmMsg)->dwXmlLen);

		}
		else if (COMM_ALARM_TFS == lCommand)
		{
			int i = 0;
			DWORD dwPrePicLen = 0;
			for (i = 0; i < 8; i++)
			{
				if (((LPNET_DVR_TFS_ALARM)pAlarmMsg)->struPicInfo[i].dwDataLen > 0)
				{
					((LPNET_DVR_TFS_ALARM)pAlarmMsg)->struPicInfo[i].pBuffer = (BYTE*)pAlarmMsg + sizeof(NET_DVR_TFS_ALARM)+dwPrePicLen;
					dwPrePicLen += ((LPNET_DVR_TFS_ALARM)pAlarmMsg)->struPicInfo[i].dwDataLen;
				}
			}
		}
		else if (COMM_ALARM_VQD_EX == lCommand)
		{
			if (((LPNET_DVR_VQD_ALARM)pAlarmMsg)->dwPicDataLen > 0)
			{
				((LPNET_DVR_VQD_ALARM)pAlarmMsg)->pImage = new BYTE[((LPNET_DVR_VQD_ALARM)pAlarmMsg)->dwPicDataLen];
				memset(((LPNET_DVR_VQD_ALARM)pAlarmMsg)->pImage, 0, ((LPNET_DVR_VQD_ALARM)pAlarmMsg)->dwPicDataLen);
				memcpy(((LPNET_DVR_VQD_ALARM)pAlarmMsg)->pImage, ((LPNET_DVR_VQD_ALARM)pAlarmInfo)->pImage, ((LPNET_DVR_VQD_ALARM)pAlarmMsg)->dwPicDataLen);
			}
		}
		else if (COMM_UPLOAD_HEATMAP_RESULT == lCommand)
		{
			if ((((LPNET_DVR_HEATMAP_RESULT)pAlarmMsg)->wArrayColumn > 0) || (((LPNET_DVR_HEATMAP_RESULT)pAlarmMsg)->wArrayLine > 0))
			{
				int iLen = (((LPNET_DVR_HEATMAP_RESULT)pAlarmMsg)->wArrayColumn) * (((LPNET_DVR_HEATMAP_RESULT)pAlarmMsg)->wArrayLine);
				((LPNET_DVR_HEATMAP_RESULT)pAlarmMsg)->pBuffer = new BYTE[iLen];
				memset(((LPNET_DVR_HEATMAP_RESULT)pAlarmMsg)->pBuffer, 0, iLen);
				memcpy(((LPNET_DVR_HEATMAP_RESULT)pAlarmMsg)->pBuffer, ((LPNET_DVR_HEATMAP_RESULT)pAlarmInfo)->pBuffer, iLen);
			}
		}
		else if (COMM_ALARM_V40 == lCommand)
		{
			DWORD dwVerDataNum = 0;
			switch (((LPNET_DVR_ALARMINFO_V40)pAlarmMsg)->struAlarmFixedHeader.dwAlarmType)
			{
			case 0:
			case 23:
				dwVerDataNum = (((LPNET_DVR_ALARMINFO_V40)pAlarmMsg)->struAlarmFixedHeader.uStruAlarm.struIOAlarm.dwTrigerAlarmOutNum + \
					((LPNET_DVR_ALARMINFO_V40)pAlarmMsg)->struAlarmFixedHeader.uStruAlarm.struIOAlarm.dwTrigerRecordChanNum);
				break;
			case 1:
			case 4:
			case 5:
				dwVerDataNum = ((LPNET_DVR_ALARMINFO_V40)pAlarmMsg)->struAlarmFixedHeader.uStruAlarm.struAlarmHardDisk.dwAlarmHardDiskNum;
				break;
			case 2:
			case 3:
			case 6:
			case 9:
			case 10:
			case 11:
			case 13:
			case 15:
			case 28:
				dwVerDataNum = ((LPNET_DVR_ALARMINFO_V40)pAlarmMsg)->struAlarmFixedHeader.uStruAlarm.struAlarmChannel.dwAlarmChanNum;
				break;
			default:
				dwVerDataNum = 0;
				break;

			}
			if (dwVerDataNum != 0)
			{
				((LPNET_DVR_ALARMINFO_V40)pAlarmMsg)->pAlarmData = new DWORD[dwVerDataNum];
				memset(((LPNET_DVR_ALARMINFO_V40)pAlarmMsg)->pAlarmData, 0, dwVerDataNum * sizeof(DWORD));
				memcpy(((LPNET_DVR_ALARMINFO_V40)pAlarmMsg)->pAlarmData, ((LPNET_DVR_ALARMINFO_V40)pAlarmInfo)->pAlarmData, dwVerDataNum* sizeof(DWORD));

			}

		}
		else if (COMM_UPLOAD_VIDEO_INTERCOM_EVENT == lCommand)
		{
			LPNET_DVR_VIDEO_INTERCOM_EVENT pVideoIntercomEvent = (LPNET_DVR_VIDEO_INTERCOM_EVENT)pAlarmMsg;
			if (pVideoIntercomEvent->byEventType == 1 && pVideoIntercomEvent->uEventInfo.struUnlockRecord.dwPicDataLen > 0)
			{
				pVideoIntercomEvent->uEventInfo.struUnlockRecord.pImage = new BYTE[pVideoIntercomEvent->uEventInfo.struUnlockRecord.dwPicDataLen];
				memcpy(pVideoIntercomEvent->uEventInfo.struUnlockRecord.pImage, ((LPNET_DVR_VIDEO_INTERCOM_EVENT)pAlarmInfo)->uEventInfo.struUnlockRecord.pImage,
					pVideoIntercomEvent->uEventInfo.struUnlockRecord.dwPicDataLen);
			}
			if (pVideoIntercomEvent->byEventType == 3 && pVideoIntercomEvent->uEventInfo.struAuthInfo.dwPicDataLen > 0)
			{
				pVideoIntercomEvent->uEventInfo.struAuthInfo.pImage = new BYTE[pVideoIntercomEvent->uEventInfo.struAuthInfo.dwPicDataLen];
				memcpy(pVideoIntercomEvent->uEventInfo.struAuthInfo.pImage, ((LPNET_DVR_VIDEO_INTERCOM_EVENT)pAlarmInfo)->uEventInfo.struAuthInfo.pImage,
					pVideoIntercomEvent->uEventInfo.struAuthInfo.dwPicDataLen);
			}
		}
		else if (lCommand == COMM_SWITCH_LAMP_ALARM)
		{
			((LPNET_DVR_SWITCH_LAMP_ALARM)pAlarmMsg)->pPicData = new char[((LPNET_DVR_SWITCH_LAMP_ALARM)pAlarmMsg)->dwPicDataLen];
			memset(((LPNET_DVR_SWITCH_LAMP_ALARM)pAlarmMsg)->pPicData, 0, ((LPNET_DVR_SWITCH_LAMP_ALARM)pAlarmMsg)->dwPicDataLen);
			memcpy(((LPNET_DVR_SWITCH_LAMP_ALARM)pAlarmMsg)->pPicData, ((LPNET_DVR_SWITCH_LAMP_ALARM)pAlarmInfo)->pPicData, ((LPNET_DVR_SWITCH_LAMP_ALARM)pAlarmMsg)->dwPicDataLen);
		}

		else if (lCommand == COMM_ALARM_ACS)
		{
			((LPNET_DVR_ACS_ALARM_INFO)pAlarmMsg)->pPicData = new char[((LPNET_DVR_ACS_ALARM_INFO)pAlarmMsg)->dwPicDataLen];
			memset(((LPNET_DVR_ACS_ALARM_INFO)pAlarmMsg)->pPicData, 0, ((LPNET_DVR_ACS_ALARM_INFO)pAlarmMsg)->dwPicDataLen);
			memcpy(((LPNET_DVR_ACS_ALARM_INFO)pAlarmMsg)->pPicData, ((LPNET_DVR_ACS_ALARM_INFO)pAlarmInfo)->pPicData, ((LPNET_DVR_ACS_ALARM_INFO)pAlarmMsg)->dwPicDataLen);
		}
		else if (lCommand == COMM_ID_INFO_ALARM)
		{
			if (((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmMsg)->pPicData > 0 && ((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmInfo)->pPicData != NULL)
			{
				((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmMsg)->pPicData = new char[((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmMsg)->dwPicDataLen];
				memset(((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmMsg)->pPicData, 0, ((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmMsg)->dwPicDataLen);
				memcpy(((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmMsg)->pPicData, ((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmInfo)->pPicData, ((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmMsg)->dwPicDataLen);
			}
			if ((((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmMsg)->pFingerPrintData > 0) && (((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmInfo)->pFingerPrintData != NULL))
			{
				((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmMsg)->pFingerPrintData = new char[((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmMsg)->dwFingerPrintDataLen];
				memset(((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmMsg)->pFingerPrintData, 0, ((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmMsg)->dwFingerPrintDataLen);
				memcpy(((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmMsg)->pFingerPrintData, ((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmInfo)->pFingerPrintData, ((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmMsg)->dwFingerPrintDataLen);
			}
			if ((((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmMsg)->pCapturePicData > 0) && (((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmInfo)->pCapturePicData != NULL))
			{
				((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmMsg)->pCapturePicData = new char[((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmMsg)->dwCapturePicDataLen];
				memset(((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmMsg)->pCapturePicData, 0, ((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmMsg)->dwCapturePicDataLen);
				memcpy(((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmMsg)->pCapturePicData, ((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmInfo)->pCapturePicData, ((LPNET_DVR_ID_CARD_INFO_ALARM)pAlarmMsg)->dwCapturePicDataLen);
			}
		}
		else if (lCommand == COMM_PANORAMIC_LINKAGE_ALARM)
		{
			if (((LPNET_DVR_PANORAMIC_LINKAGE)pAlarmMsg)->pPicBuff > 0 && ((LPNET_DVR_PANORAMIC_LINKAGE)pAlarmInfo)->pPicBuff != NULL)
			{
				((LPNET_DVR_PANORAMIC_LINKAGE)pAlarmMsg)->pPicBuff = new char[((LPNET_DVR_PANORAMIC_LINKAGE)pAlarmMsg)->dwPicLen];
				memset(((LPNET_DVR_PANORAMIC_LINKAGE)pAlarmMsg)->pPicBuff, 0, ((LPNET_DVR_PANORAMIC_LINKAGE)pAlarmMsg)->dwPicLen);
				memcpy(((LPNET_DVR_PANORAMIC_LINKAGE)pAlarmMsg)->pPicBuff, ((LPNET_DVR_PANORAMIC_LINKAGE)pAlarmInfo)->pPicBuff, ((LPNET_DVR_PANORAMIC_LINKAGE)pAlarmMsg)->dwPicLen);
			}
		}

		LPLOCAL_ALARM_INFO pAlarmDev = new LOCAL_ALARM_INFO;
		if (pAlarmDev == NULL)
		{
			delete[] pAlarmMsg;
			pAlarmMsg = NULL;
			return;
		}
		pAlarmDev->iDeviceIndex = iDeviceIndex;
		pAlarmDev->lCommand = lCommand;
		memcpy(pAlarmDev->sDeviceIP, pAlarmer->sDeviceIP, 128);
		pAlarmDev->dwBufLen = dwBufLen;

		::PostMessage(g_pMainDlg->m_hWnd, WM_PROC_ALARM, (WPARAM)pAlarmDev, (LPARAM)pAlarmMsg);
	}
	catch (...)
	{
		OutputDebugString("New Alarm Exception!\n");
	}

	return;
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()
// CCameraMonitorDlg 对话框

CCameraMonitorDlg::CCameraMonitorDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CCameraMonitorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCameraMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_CAMERA, m_wndTreeCamera);

	DDX_Control(pDX, IDC_LIST1, m_wndList);
}

BEGIN_MESSAGE_MAP(CCameraMonitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_PROC_ALARM, OnWMProcAlarm)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_CAMERA, &CCameraMonitorDlg::OnNMRClickTreeCamera)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_CAMERA, &CCameraMonitorDlg::OnNMDblclkTreeCamera)
	ON_BN_CLICKED(IDC_BUT_STOP, &CCameraMonitorDlg::OnBnClickedButStop)
	ON_NOTIFY(LVN_GETINFOTIP, IDC_LIST1, &CCameraMonitorDlg::OnLvnGetInfoTipList1)
	ON_BN_CLICKED(IDC_BUTT_SAVEPATH, &CCameraMonitorDlg::OnBnClickedButtSavepath)
	ON_WM_TIMER()
	ON_COMMAND(ID_SETALAEM, &CCameraMonitorDlg::OnSetalaem)
	ON_COMMAND(ID_CLOSEALAEM, &CCameraMonitorDlg::OnClosealaem)
	ON_COMMAND(ID_SETATTRIBUTE, &CCameraMonitorDlg::OnSetattribute)
	ON_COMMAND(ID_MenuReboot, &CCameraMonitorDlg::OnMenureboot)
END_MESSAGE_MAP()
// CCameraMonitorDlg 消息处理程序

BOOL CCameraMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	//////////////////
	GetDlgItem(IDC_STATIC_VIDEO)->SetWindowText("");
	m_wndList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP);
	m_wndList.InsertColumn(0, _T("提示内容"), LVCFMT_LEFT, 210);
	m_wndList.InsertColumn(1, _T("相机"), LVCFMT_LEFT, 130);
	m_wndList.InsertColumn(2, _T("时间"), LVCFMT_LEFT, 130);
	m_wndList.InsertColumn(3, _T("结果"), LVCFMT_LEFT, 70);
	m_wndList.GetHeaderCtrl()->EnableWindow(FALSE);
	m_nPlayingIndex = -1;
	g_pMainDlg = this;

	ProgramStart();

	GetDlgItem(IDC_EDIT_PATH)->SetWindowText(g_csPath);
	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCameraMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCameraMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCameraMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//自动布放
UINT ThreadSetupAlarmChan(LPVOID lparam)
{
	try
	{
		CCameraMonitorDlg *pWnd = (CCameraMonitorDlg *)AfxGetApp()->GetMainWnd();
		for (int i = 0; i < g_CameraSum; i++)
		{
			if (pWnd->LogIn(i))
				pWnd->SetupAlarmChan(i);
			else
				pWnd->SetListOut("登录", g_CameraArrInfo[i].csCameraIp, RETURN_NO);
		}
	}
	catch (CException* e)
	{
		return -1;
	}

	return 0;
}
//照片回调函数
LRESULT CCameraMonitorDlg::OnWMProcAlarm(WPARAM wParam, LPARAM lParam)
{
	try
	{
		LPLOCAL_ALARM_INFO pAlarmDev = (LPLOCAL_ALARM_INFO)(wParam);
		char *pAlarmInfo = (char *)(lParam);
		int iDeviceIndex = pAlarmDev->iDeviceIndex;
		if (iDeviceIndex < 0 && iDeviceIndex >= MAX_DEVICES)
		{
			delete[] pAlarmInfo;
			delete pAlarmDev;
			return NULL;
		}
		char szLan[128] = { 0 };
		char szInfoBuf[1024] = { 0 };
		switch (pAlarmDev->lCommand)
		{

		case COMM_UPLOAD_FACESNAP_RESULT:
			ProcFaceSnapAlarm(wParam, lParam);
			break;
		}

		if (pAlarmInfo != NULL)
		{
			delete[] pAlarmInfo;
			pAlarmInfo = NULL;
		}
		if (pAlarmDev != NULL)
		{
			delete pAlarmDev;
			pAlarmDev = NULL;
		}
	}
	catch (...)
	{
	}

	return NULL;
}
//照片回调写
void CCameraMonitorDlg::ProcFaceSnapAlarm(WPARAM wParam, LPARAM lParam)
{
	try
	{
		//IDD_DLG_VCA_FACE_SNAP
		char szInfoBuf[1024] = { 0 };
		LPLOCAL_ALARM_INFO pAlarmDev = (LPLOCAL_ALARM_INFO)(wParam);
		char *pAlarmInfo = (char *)(lParam);
		int iDeviceIndex = pAlarmDev->iDeviceIndex;
		if (g_CameraArrInfo[iDeviceIndex].csCameraAddrPath.IsEmpty())
		{
			SetListOut("路径为空", g_CameraArrInfo[iDeviceIndex].csCameraIp, "");
			return;
		}

		NET_VCA_FACESNAP_RESULT struFaceSnapAlarm = { 0 };
		memcpy(&struFaceSnapAlarm, pAlarmInfo, sizeof(struFaceSnapAlarm));
		SYSTEMTIME t;
		GetLocalTime(&t);

		if (struFaceSnapAlarm.dwFacePicLen > 0 && struFaceSnapAlarm.pBuffer1 != NULL)
		{
			char cFilename[256] = { 0 };
			HANDLE hFile;
			DWORD dwReturn;

			char chTime[128];
			sprintf(chTime, "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d%3.3d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);

			if (struFaceSnapAlarm.byUploadEventDataType == 0)
			{
				sprintf(cFilename, "%s\\SnapPic[%s].jpg", g_CameraArrInfo[iDeviceIndex].csCameraAddrPath, chTime);
			}
			else
			{
				sprintf(cFilename, "%s\\SnapURL[%s].txt", g_CameraArrInfo[iDeviceIndex].csCameraAddrPath, chTime);
			}

			hFile = CreateFile(cFilename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				DWORD dError = GetLastError();
				return;
			}
			WriteFile(hFile, struFaceSnapAlarm.pBuffer1, struFaceSnapAlarm.dwFacePicLen, &dwReturn, NULL);
			CloseHandle(hFile);
			SetListOut(cFilename, g_CameraArrInfo[iDeviceIndex].csCameraIp, RETURN_OK);
			hFile = NULL;
		}

		if (struFaceSnapAlarm.dwBackgroundPicLen > 0 && struFaceSnapAlarm.pBuffer2 != NULL)
		{
			char cFilename[256] = { 0 };
			HANDLE hFile;
			DWORD dwReturn;

			char chTime[128];
			sprintf(chTime, "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d%3.3d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);

			if (struFaceSnapAlarm.byUploadEventDataType == 0)
			{
				sprintf(cFilename, "%s\\BackgroundPic[%s].jpg", g_CameraArrInfo[iDeviceIndex].csCameraAddrPath, chTime);
			}
			else
			{
				sprintf(cFilename, "%s\\BackgroundPic[%s].txt", g_CameraArrInfo[iDeviceIndex].csCameraAddrPath, chTime);
			}

			hFile = CreateFile(cFilename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				DWORD dError = GetLastError();
				return;
			}
			WriteFile(hFile, struFaceSnapAlarm.pBuffer2, struFaceSnapAlarm.dwBackgroundPicLen, &dwReturn, NULL);
			CloseHandle(hFile);
			SetListOut(cFilename, g_CameraArrInfo[iDeviceIndex].csCameraIp, RETURN_OK);
			hFile = NULL;
		}
	}
	catch (...)
	{
	}
}
//登录
BOOL CCameraMonitorDlg::LogIn(int nCameraInfoIndex)
{
	try
	{
		if (nCameraInfoIndex == -1)
		{
			return FALSE;
		}
		NET_DVR_DEVICEINFO_V30 struDeviceInfo;
		memset(&struDeviceInfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));

		NET_DVR_USER_LOGIN_INFO struLoginInfo = { 0 };
		NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = { 0 };
		struLoginInfo.bUseAsynLogin = false;
		struLoginInfo.wPort = g_CameraArrInfo[nCameraInfoIndex].nPort;
		memcpy(struLoginInfo.sDeviceAddress, g_CameraArrInfo[nCameraInfoIndex].csCameraIp, NET_DVR_DEV_ADDRESS_MAX_LEN);
		memcpy(struLoginInfo.sUserName, g_CameraArrInfo[nCameraInfoIndex].csCameraName, NAME_LEN);
		memcpy(struLoginInfo.sPassword, g_CameraArrInfo[nCameraInfoIndex].csCameraPwd, NAME_LEN);
		g_CameraArrInfo[nCameraInfoIndex].lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
		if (g_CameraArrInfo[nCameraInfoIndex].lUserID < 0)
			SetListOut("登录", g_CameraArrInfo[nCameraInfoIndex].csCameraIp, RETURN_NO);
		else
			SetListOut("登录", g_CameraArrInfo[nCameraInfoIndex].csCameraIp, RETURN_OK);
	}
	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}
//播放
BOOL CCameraMonitorDlg::StartPlay(int nCameraInfoIndex)
{
	try
	{
		if (nCameraInfoIndex == -1)
		{
			return FALSE;
		}
		NET_DVR_CLIENTINFO ClientInfo;
		ClientInfo.hPlayWnd = GetDlgItem(IDC_STATIC_VIDEO)->m_hWnd;
		ClientInfo.lChannel = 1;
		ClientInfo.lLinkMode = 0;
		ClientInfo.sMultiCastIP = NULL;
		g_CameraArrInfo[nCameraInfoIndex].lPlayHandle = NET_DVR_RealPlay_V30(g_CameraArrInfo[nCameraInfoIndex].lUserID, &ClientInfo, NULL, NULL, TRUE);
		if (-1 == g_CameraArrInfo[nCameraInfoIndex].lPlayHandle)
		{
			DWORD err = NET_DVR_GetLastError();
			CString m_csErr;
			m_csErr.Format(_T("播放出错，错误代码%d"), err);
			SetListOut(m_csErr, g_CameraArrInfo[nCameraInfoIndex].csCameraIp, RETURN_NO);
			return FALSE;
		}
		SetListOut("播放", g_CameraArrInfo[nCameraInfoIndex].csCameraIp, RETURN_OK);
		m_bIsPlaying = TRUE;
	}
	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}
//停止播放
BOOL CCameraMonitorDlg::StopPlay(int nCameraInfoIndex)
{
	try
	{
		if (nCameraInfoIndex == -1)
		{
			return FALSE;
		}
		if (g_CameraArrInfo[nCameraInfoIndex].lPlayHandle != -1)
		{
			if (NET_DVR_StopRealPlay(g_CameraArrInfo[nCameraInfoIndex].lPlayHandle))
			{
				g_CameraArrInfo[nCameraInfoIndex].lPlayHandle = -1;
				m_bIsPlaying = FALSE;
				SetListOut("停止播放", g_CameraArrInfo[nCameraInfoIndex].csCameraIp, RETURN_OK);
				return TRUE;
			}
			SetListOut("停止播放", g_CameraArrInfo[nCameraInfoIndex].csCameraIp, RETURN_NO);
		}
	}
	catch (...)
	{
		return FALSE;
	}
	return FALSE;
}
//登出
BOOL CCameraMonitorDlg::DoLogout(int nCameraInfoIndex)
{
	if (g_CameraArrInfo[nCameraInfoIndex].lPlayHandle != -1)
	{
		NET_DVR_Logout_V30(g_CameraArrInfo[nCameraInfoIndex].lUserID);
		g_CameraArrInfo[nCameraInfoIndex].lUserID = -1;
		g_CameraArrInfo[nCameraInfoIndex].lPlayHandle = -1;
		m_bIsPlaying = FALSE;
		//NET_DVR_Cleanup();//清理SDK所有的数据
		return TRUE;
	}
	return FALSE;
}
//布放
BOOL CCameraMonitorDlg::SetupAlarmChan(int nCameraInfoIndex)
{
	try
	{
		if (nCameraInfoIndex == -1)
		{
			return FALSE;
		}
		NET_DVR_SETUPALARM_PARAM struSetupAlarmParam = { 0 };
		struSetupAlarmParam.dwSize = sizeof(struSetupAlarmParam);
		struSetupAlarmParam.byRetVQDAlarmType = TRUE; //Prefer VQD Alarm type of NET_DVR_VQD_ALARM
		struSetupAlarmParam.byRetAlarmTypeV40 = TRUE;
		struSetupAlarmParam.byFaceAlarmDetection = 1;
		struSetupAlarmParam.wTaskNo = 0;
		struSetupAlarmParam.byBrokenNetHttp = 0;
		struSetupAlarmParam.byBrokenNetHttp |= (0 << 0);
		struSetupAlarmParam.byBrokenNetHttp |= (0 << 1);
		struSetupAlarmParam.byBrokenNetHttp |= (0 << 2);
		struSetupAlarmParam.byRetDevInfoVersion = TRUE;
		struSetupAlarmParam.byAlarmInfoType = 1;
		struSetupAlarmParam.bySupport |= (0 << 1);
		struSetupAlarmParam.byDeployType = 0;
		struSetupAlarmParam.bySupport |= (1 << 2);
		struSetupAlarmParam.byAlarmTypeURL |= (0 << 0);
		struSetupAlarmParam.byAlarmTypeURL |= (0 << 1);
		struSetupAlarmParam.byAlarmTypeURL |= (0 << 2);
		struSetupAlarmParam.byCustomCtrl = 0;
		struSetupAlarmParam.byLevel = 0;
		g_CameraArrInfo[nCameraInfoIndex].lFortifyHandle = NET_DVR_SetupAlarmChan_V41(g_CameraArrInfo[nCameraInfoIndex].lUserID, &struSetupAlarmParam);
		if (g_CameraArrInfo[nCameraInfoIndex].lFortifyHandle == -1)//报警设防失败
		{
			m_wndTreeCamera.SetItemImage(g_CameraArrInfo[nCameraInfoIndex].hCamera, TREE_NO, TREE_NO);
			SetListOut("布防", g_CameraArrInfo[nCameraInfoIndex].csCameraIp, RETURN_NO);
			return FALSE;
		}
		else
		{
			m_wndTreeCamera.SetItemImage(g_CameraArrInfo[nCameraInfoIndex].hCamera, TREE_OK, TREE_OK);
			SetListOut("布防", g_CameraArrInfo[nCameraInfoIndex].csCameraIp, RETURN_OK);
		}
	}
	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}
//撤防
BOOL CCameraMonitorDlg::CloseAlarmChan(int nCameraInfoIndex)
{
	try
	{
		if (nCameraInfoIndex == -1)
		{
			return FALSE;
		}

		if (NET_DVR_CloseAlarmChan_V30(g_CameraArrInfo[nCameraInfoIndex].lFortifyHandle))
		{
			SetListOut("撤防", g_CameraArrInfo[nCameraInfoIndex].csCameraIp, RETURN_NO);
		}
		else//报警撤防失败
		{
			SetListOut("撤防", g_CameraArrInfo[nCameraInfoIndex].csCameraIp, RETURN_NO);
			return FALSE;
		}
	}
	catch (...)
	{
		return FALSE;
	}


	return TRUE;
}
//获取配置文件
BOOL CCameraMonitorDlg::GetParameterIni()
{
	try
	{
		int nError;
		CString csIniPath;
		csIniPath = "./Parameter.ini";

		nError = GetPrivateProfileString(("SqlConfiguration"), "Sql_ServerAdd", (""), m_csDatabaseIp.GetBuffer(MAX_PATH), MAX_PATH, csIniPath);
		if (nError <= 0)
		{
			return FALSE;
		}
		nError = GetPrivateProfileString(("SqlConfiguration"), "Sql_Database", (""), m_csDatabaseTableName.GetBuffer(MAX_PATH), MAX_PATH, csIniPath);
		if (nError <= 0)
		{
			return FALSE;
		}
		nError = GetPrivateProfileString(("SqlConfiguration"), "Sql_Name", (""), m_csDatabaseName.GetBuffer(MAX_PATH), MAX_PATH, csIniPath);
		if (nError <= 0)
		{
			return FALSE;
		}

		nError = GetPrivateProfileString(("SqlConfiguration"), "Sql_Pwd", (""), m_csDatabasePwd.GetBuffer(MAX_PATH), MAX_PATH, csIniPath);
		if (nError <= 0)
		{
			return FALSE;
		}

		nError = GetPrivateProfileString(("Path"), "Addr_Path", (""), g_csPath.GetBuffer(MAX_PATH), MAX_PATH, csIniPath);
		if (nError <= 0)
		{
			return FALSE;
		}


	}
	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}
//初始化数据库
BOOL CCameraMonitorDlg::InitDatabase()
{
	try
	{
		CString csSqlConnect;
		csSqlConnect.Format("Driver={MySQL ODBC 5.1 Driver};Server=%s;Database=%s;uid=%s;pwd=%s;",
			m_csDatabaseIp, m_csDatabaseTableName, m_csDatabaseName, m_csDatabasePwd);

		g_pConnect.CreateInstance(__uuidof(Connection));
		g_pConnect->CursorLocation = adUseClient; //游标类型     
		_bstr_t strConnect = csSqlConnect;
		g_pConnect->Open(strConnect, "", "", adModeUnknown);
	}
	catch (_com_error &e)
	{
		CString csError = e.Description();
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 0, csError, PROGRAM_NAME);
		return FALSE;
	}
	return  TRUE;
}
//初始化device的基本信息
BOOL CCameraMonitorDlg::InitDeviceInfo()
{
	try
	{
		_RecordsetPtr pRecordset;
		if (g_CameraArrInfo != NULL)
		{
			delete[]g_CameraArrInfo;
		}

		int nArrayIndex = 0;
		CString csSql;
		csSql.Format("SELECT * FROM camera WHERE DelFlag = '0';");

		g_CameraSum = _GetExecuteResults(pRecordset, g_pConnect, csSql);
		if (g_CameraSum == 0)
			return FALSE;

		g_CameraArrInfo = new CAMERA_INFO[g_CameraSum];

		while (!pRecordset->adoEOF)
		{
			g_CameraArrInfo[nArrayIndex].nArrayIndex = nArrayIndex;
			g_CameraArrInfo[nArrayIndex].nCameraIndex = atoi((TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("CameraId")->Value);
			g_CameraArrInfo[nArrayIndex].nDeviceIndex = atoi((TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("DeviceId")->Value);
			g_CameraArrInfo[nArrayIndex].csCameraIp = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("IP")->Value;
			g_CameraArrInfo[nArrayIndex].csCameraAddr = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("Addr")->Value;
			g_CameraArrInfo[nArrayIndex].csCameraState = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("Camera_State")->Value;
			g_CameraArrInfo[nArrayIndex].csCameraType = (TCHAR *)(_bstr_t)pRecordset->GetFields()->GetItem("Camera_Type")->Value;


			nArrayIndex++;
			pRecordset->MoveNext();
		}
	}
	catch (_com_error &e)
	{
		delete[]g_CameraArrInfo;
		CString csError = e.Description();
		DT_Log_Error(__FILE__, __LINE__, DTUseLevel[4], 0, csError, PROGRAM_NAME);
		return FALSE;
	}

	return TRUE;
}
//
void CCameraMonitorDlg::LoadTreeImage()
{
	CBitmap bmp[5];
	m_TreeImage.Create(34, 23, ILC_COLOR32 | ILC_MASK, 1, 1);

	bmp[TREE_ROOT].LoadBitmap(IDB_BITMAP_ROOT);
	m_TreeImage.Add(&bmp[TREE_ROOT], RGB(1, 1, 1));

	bmp[TREE_INIT].LoadBitmap(IDB_BITMAP_INIT);
	m_TreeImage.Add(&bmp[TREE_INIT], RGB(1, 1, 1));

	bmp[TREE_OK].LoadBitmap(IDB_BITMAP_ok);
	m_TreeImage.Add(&bmp[TREE_OK], RGB(1, 1, 1));

	bmp[TREE_NO].LoadBitmap(IDB_BITMAP_NO);
	m_TreeImage.Add(&bmp[TREE_NO], RGB(1, 1, 1));

	m_wndTreeCamera.SetImageList(&m_TreeImage, LVSIL_NORMAL);
}
//创建树
void CCameraMonitorDlg::CreateTree()
{
	try
	{
		HTREEITEM hRoot = m_wndTreeCamera.InsertItem("摄像机列表", TREE_ROOT, TREE_ROOT);

		for (int i = 0; i < g_CameraSum; i++)
		{
			HTREEITEM hCamera = m_wndTreeCamera.InsertItem(g_CameraArrInfo[i].csCameraIp, TREE_INIT, TREE_INIT, hRoot);
			m_wndTreeCamera.SetItemData(hCamera, g_CameraArrInfo[i].nArrayIndex);
			g_CameraArrInfo[i].hCamera = hCamera;
		}
		__ExpandAllNode(hRoot, m_wndTreeCamera);
	}
	catch (...)
	{
	}
}
//
BOOL CCameraMonitorDlg::CreateImgFile()
{
	try
	{
		if (g_CameraSum == 0)
			return FALSE;

		if (!PathIsDirectory(g_csPath))
			CreateDirectory(g_csPath, NULL);

		CString csCameFile;
		for (int i = 0; i < g_CameraSum; i++)
		{
			csCameFile.Format("%s//%s[1]", g_csPath, g_CameraArrInfo[i].csCameraIp);
			if (!PathIsDirectory(csCameFile))
			{
				if (CreateDirectory(csCameFile, NULL))
					g_CameraArrInfo[i].csCameraAddrPath = csCameFile;
				else
					return FALSE;
			}
			else
			{
				g_CameraArrInfo[i].csCameraAddrPath = csCameFile;
			}
		}
	}
	catch (...)
	{
		return FALSE;
	}

	return TRUE;
}
//输出信息
void CCameraMonitorDlg::SetListOut(CString csName, CString csCameIp, CString csResults)
{
	COleDateTime coTime;
	coTime = COleDateTime::GetCurrentTime();
	CString strCurTime = coTime.Format("%Y-%m-%d %H:%M:%S");
	m_wndList.InsertItem(0, "");
	m_wndList.SetItemText(0, 0, csName);
	m_wndList.SetItemText(0, 1, csCameIp);
	m_wndList.SetItemText(0, 2, strCurTime);
	m_wndList.SetItemText(0, 3, csResults);
}
//树右键
void CCameraMonitorDlg::OnNMRClickTreeCamera(NMHDR *pNMHDR, LRESULT *pResult)
{
	try
	{
		CPoint pt;
		GetCursorPos(&pt);
		m_wndTreeCamera.ScreenToClient(&pt);
		UINT uFlags = 0;
		HTREEITEM hChildSelect = NULL;
		hChildSelect = m_wndTreeCamera.HitTest(pt, &uFlags);
		if (hChildSelect != NULL && hChildSelect != m_wndTreeCamera.GetRootItem())
		{
			m_nCameraIndex = m_wndTreeCamera.GetItemData(hChildSelect);
			m_wndTreeCamera.SelectItem(hChildSelect);
			m_wndTreeCamera.SetCheck(hChildSelect, TRUE);
			CMenu menu;
			menu.LoadMenu(IDR_MENU_TREE);
			CPoint point;
			GetCursorPos(&point);
			CMenu* pSubMenu = menu.GetSubMenu(0);
			pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
		}
		*pResult = 0;
	}
	catch (...)
	{
	}
}
//树双击
void CCameraMonitorDlg::OnNMDblclkTreeCamera(NMHDR *pNMHDR, LRESULT *pResult)
{
	try
	{
		CPoint pt;
		GetCursorPos(&pt);
		m_wndTreeCamera.ScreenToClient(&pt);
		UINT uFlags = 0;
		HTREEITEM hChildSelect = NULL;
		hChildSelect = m_wndTreeCamera.HitTest(pt, &uFlags);
		if (hChildSelect != NULL && hChildSelect != m_wndTreeCamera.GetRootItem())
		{
			int nCameraIndex = m_wndTreeCamera.GetItemData(hChildSelect);
			if (m_bIsPlaying && m_nPlayingIndex != -1)
			{
				if (StopPlay(m_nPlayingIndex))
				{
					if (nCameraIndex != m_nPlayingIndex)
					{
						if (StartPlay(nCameraIndex))
						{
							m_nPlayingIndex = nCameraIndex;
						}
					}
					else
					{
						m_nPlayingIndex = -1;
					}
				}
			}
			else
			{
				if (StartPlay(nCameraIndex))
				{
					m_nPlayingIndex = nCameraIndex;
				}
			}
		}
		*pResult = 0;
	}
	catch (...)
	{
	}
}
//停止播放
void CCameraMonitorDlg::OnBnClickedButStop()
{
	if (m_nPlayingIndex != -1)
		StopPlay(m_nPlayingIndex);
}
//提示
void CCameraMonitorDlg::OnLvnGetInfoTipList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVGETINFOTIP pGetInfoTip = reinterpret_cast<LPNMLVGETINFOTIP>(pNMHDR);
	CString csTip = m_wndList.GetItemText(pGetInfoTip->iItem, 0);
	_tcscpy(pGetInfoTip->pszText, csTip);
	*pResult = 0;
}
//打开选择路径
CString CCameraMonitorDlg::GetDirectoryPath()
{
	LPITEMIDLIST pidlRoot = NULL;
	SHGetSpecialFolderLocation(m_hWnd, CSIDL_DRIVES, &pidlRoot);
	BROWSEINFO bi;   //the input parameters needed
	CString strDisplayName;
	bi.hwndOwner = GetSafeHwnd();   //get handle value of parent window 
	bi.pidlRoot = pidlRoot;
	bi.pszDisplayName = strDisplayName.GetBuffer(MAX_PATH + 1);   //get buffer pointer
	char szLan[32] = { 0 };
	g_StringLanType(szLan, "文件夹", "Directory");
	bi.lpszTitle = szLan;   //set title
	bi.ulFlags = BIF_RETURNONLYFSDIRS;   //set flag
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;   //The above parameters are not related to the settings, better set up,
	LPITEMIDLIST lpIDList = SHBrowseForFolder(&bi);	//open dialog
	strDisplayName.ReleaseBuffer();   //correspond to GetBuffer()
	char pPath[MAX_PATH];
	CString Str;
	if (lpIDList)
	{
		SHGetPathFromIDList(lpIDList, pPath);
		Str = pPath;
	}
	return Str;
}
//设置路径
void CCameraMonitorDlg::OnBnClickedButtSavepath()
{
	CString csTemp = GetDirectoryPath();
	if (csTemp.IsEmpty())
	{
		MessageBox("路径错误", "提示");
		return;
	}

	if (MessageBox("是否保存?", "提示", MB_OKCANCEL) == IDOK)
	{
		int nError = 0;
		g_csPath = csTemp;
		CString csIniPath = "./Parameter.ini";
		nError = WritePrivateProfileString(("Path"), "Addr_Path", g_csPath.GetBuffer(MAX_PATH), csIniPath);
		if (nError > 0)
		{
			//IDC_EDIT_PATH
			GetDlgItem(IDC_EDIT_PATH)->SetWindowText(g_csPath);
			CreateImgFile();
		}
	}
}
//启动
void CCameraMonitorDlg::ProgramStart()
{
	NET_DVR_Init();
	NET_DVR_SetDVRMessageCallBack_V30(MessageCallback, NULL);
	NET_DVR_SetExceptionCallBack_V30(WM_NULL/*WM_MYCOMMAND*/, NULL/*this->m_hWnd*/, g_ExceptionCallBack, NULL);

	LoadTreeImage();
	while (true)
	{
		if (GetParameterIni())
			break;
	}

	while (true)
	{
		if (InitDatabase())
			break;
	}

	while (true)
	{
		if (InitDeviceInfo())
			break;
	}

	while (true)
	{
		if (CreateImgFile())
			break;
	}

	CreateTree();
	AfxBeginThread(&ThreadSetupAlarmChan, NULL);
	SetTimer(1, 30000, NULL);
}
//判断布放失败继续布放
void CCameraMonitorDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		BOOL bIsAllAlarm = TRUE;
		for (int i = 0; i < g_CameraSum; i++)
		{
			if (g_CameraArrInfo[i].lUserID == -1)
			{

				if (LogIn(i))
					SetupAlarmChan(i);
				else
				{
					SetListOut("登录", g_CameraArrInfo[i].csCameraIp, RETURN_NO);
					bIsAllAlarm = FALSE;
				}
			}
			else
			{
				if (g_CameraArrInfo[i].lFortifyHandle == -1)
				{
					if (!SetupAlarmChan(i))
						bIsAllAlarm = FALSE;
				}
			}
		}

		if (bIsAllAlarm)
		{
			//	KillTimer(1);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}
//布放
void CCameraMonitorDlg::OnSetalaem()
{
	if (m_nCameraIndex == -1)
		return;

	if (g_CameraArrInfo[m_nCameraIndex].lFortifyHandle == -1)
		SetupAlarmChan(m_nCameraIndex);
	else
		SetListOut("布防", g_CameraArrInfo[m_nCameraIndex].csCameraIp, "已布防");

}
//撤防
void CCameraMonitorDlg::OnClosealaem()
{
	if (m_nCameraIndex == -1)
		return;

	if (g_CameraArrInfo[m_nCameraIndex].lFortifyHandle != -1)
		CloseAlarmChan(m_nCameraIndex);
	else
		SetListOut("撤防", g_CameraArrInfo[m_nCameraIndex].csCameraIp, "未布防");
}
//设置
void CCameraMonitorDlg::OnSetattribute()
{
	CameraSet dlg;
	dlg.m_lUerID = g_CameraArrInfo[m_nCameraIndex].lUserID;
	dlg.m_csCamename = g_CameraArrInfo[m_nCameraIndex].csCameraIp;
	dlg.DoModal();
}
//重启
void CCameraMonitorDlg::OnMenureboot()
{
	if (NET_DVR_RebootDVR(g_CameraArrInfo[m_nCameraIndex].lUserID))
	{
		SetListOut("重启", g_CameraArrInfo[m_nCameraIndex].csCameraIp, "正在重启");
	}
	else
	{
		SetListOut("重启", g_CameraArrInfo[m_nCameraIndex].csCameraIp, "重启失败");
	}
}
