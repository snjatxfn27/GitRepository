#include "stdafx.h"
#include "DLLMethodForCSharp.h"
#include "ToolsForFaceServerLogin.h"
#include "ToolsForRecognition.h"
#include "ToolsForFaceManage.h"
#include "ToolsForCompare.h"
#include "ToolsForBlackDB.h"
#include "ToolsForWhiteDB.h"
#include "ToolsForCamera.h"


/****************************************************
��¼�ǳ�
*****************************************************/
// ��¼һ���
extern "C" __declspec(dllexport) int  DeviceLogin(const char * pIP, char *pSession)
{
	USES_CONVERSION;
	int m_result = Login(A2T(pIP), pSession);
	return m_result;
}

// ��¼һ���
extern "C" __declspec(dllexport) int  DeviceLogout(const char *pIP, const char *pSession)
{
	USES_CONVERSION;
	int m_result = Logout(A2T(pIP), A2T(pSession));
	return m_result;
}
/****************************************
������ȡͷ�ļ�
*****************************************/

//��ȡ��������
extern "C" __declspec(dllexport) int   FASFeatureDetect(const char *pIP, const char * pSession, const char * pImageb64, char * chJson, int jsonlen)
{
	USES_CONVERSION;
	int m_result = FeatureDetect(A2T(pIP), A2T(pSession), A2T(pImageb64), chJson, jsonlen);
	return m_result;
}

//��ȡ��������
extern "C" __declspec(dllexport) int FASFeatureDetectforImageFile(const char * pIP, const char * pSession, const char * pFilePath, char* chJson, int jsonlen)
{
	USES_CONVERSION;
	int m_result = FeatureDetectforImageFile(A2T(pIP), A2T(pSession), A2T(pFilePath), chJson, jsonlen);
	return m_result;
}
//��ȡ����������ͼƬ(ASP.netʹ�ã�����ֵ��󲻳���100kb,dll�����еľֲ�����Ҫ��new��delete)
extern "C" __declspec(dllexport) int FASFeatureDetectforImageFileForNet(const char * pIP, const char * pSession, const char * pFilePath, char* chJson, int jsonlen)
{
	USES_CONVERSION;
	int m_result = FeatureDetectforImageFileForNet(A2T(pIP), A2T(pSession), A2T(pFilePath), chJson, jsonlen);
	return m_result;
}
/****************************************
��������
*****************************************/
//��������Ա��ҳ��ѯ
extern "C" __declspec(dllexport) int  FASGetFaceListPageInDb(const char *  pIP, const char *  pSession, const char *  pFaceDbId,  const char *  pIdCard, char*chJson, int jsonlen)
{
	USES_CONVERSION;
	int m_result = GetFaceListPageInDb(A2T(pIP), A2T(pSession), A2T(pFaceDbId), A2T(pIdCard), chJson, jsonlen);
	return m_result;
}
//��һ�����������һ������Ϣ
extern "C" __declspec(dllexport) int FASAddPersoninfo(const char *  pIP, const char *  pSession, const char *  pName, const char *  IdCard, int intGender, long longFaceDbId, const char *  pFeatures, const char *  ImageData, const char *  pBirthday, const char *  pValidTo, char * pPossonId, int nPossonLen)
{
	USES_CONVERSION;
	int m_result = AddPersoninfo(A2T(pIP), A2T(pSession), A2T(pName), A2T(IdCard), intGender, longFaceDbId, A2T(pFeatures), A2T(ImageData), A2T(pBirthday), A2T(pValidTo),pPossonId,nPossonLen);
	return m_result;
}
// ������༭һ��������Ϣ
extern "C" __declspec(dllexport) int  FASUpdatePersoninfo(const char *  pIP, const char *  pSession, const char *  pPersonid,  const char *  IdCard, long longFaceDbId)
{
	USES_CONVERSION;
	int m_result = UpdatePersoninfo(A2T(pIP), A2T(pSession),A2T(pPersonid), A2T(IdCard), longFaceDbId);
	return m_result;
}
//ͨ��������personidɾ��һ��Person��Ϣ
extern "C" __declspec(dllexport) int  FASDELETEPersoninfo(const char *  pIP, const char *  pSession, const char *  pPsersonid)
{
	USES_CONVERSION;
	int m_result = DELETEPersoninfo(A2T(pIP), A2T(pSession), A2T(pPsersonid));
	return m_result;
}
//ͨ��������personid��ȡ����������Ա�ĵ�������Ϣ
extern "C" __declspec(dllexport) int  FASGetFaceInDb(const char *  pIP, const char *  pSession, const char *  pPsersonid, char*chJson, int jsonlen)
{
	USES_CONVERSION;
	int m_result = GetFaceInDb(A2T(pIP), A2T(pSession), A2T(pPsersonid), chJson, jsonlen);
	return m_result;
}
//����������Ӷ��������Ϣ
extern "C" __declspec(dllexport) int FASBatchAddPersoninfo(const char * pIP, const char * pSession, struct batchPerson1 *sbatchPersonlist,int nNumber)
{
	USES_CONVERSION;
	int m_result = BatchAddPersoninfo(A2T(pIP), A2T(pSession), sbatchPersonlist, nNumber);
	return m_result;
}



/****************************************
�����ȶ�
*****************************************/
//�����ȶ�����
extern "C" __declspec(dllexport)int FASFeatureMatch(const char * pIP, const char * pSession, const char * pFeature1, const char * pFeature2, char*chJson, int jsonlen)
{
	USES_CONVERSION;
	int m_result = FeatureMatch(A2T(pIP), A2T(pSession), A2T(pFeature1), A2T(pFeature2),chJson,jsonlen);
	return m_result;
}
//�����ȶ�������
extern "C" __declspec(dllexport)int FASMatchPerson(const char * pIP, const char * pSession, const char * pDBs, double sim, const char * pFeature, char*chJson, int jsonlen)
{
	USES_CONVERSION;
	int m_result = MatchPerson(A2T(pIP), A2T(pSession), A2T(pDBs), sim, A2T(pFeature),chJson, jsonlen);
	return m_result;
}
/****************************************
���������������ͷ�ļ�
*****************************************/
//�������������ѯ
extern "C" __declspec(dllexport)int  FASCheckBlack(const char * pIP, const char * pSession, const char * pName, char*chJson, int jsonlen)
{
	USES_CONVERSION;
	int m_result = CheckBlack(A2T(pIP), A2T(pSession), A2T(pName), chJson, jsonlen);
	return m_result;
}
//���������������
extern "C" __declspec(dllexport)int  FASAddBlack(const char * pIP, const char * pSession, const char * pName, char*chJson, int jsonlen)
{
	USES_CONVERSION;
	int m_result = AddBlack(A2T(pIP), A2T(pSession), A2T(pName), chJson, jsonlen);
	return m_result;
}
//const char *
extern "C" __declspec(dllexport)int  FASEditBlack(const char * pIP, const char * pSession, const char * pID, const char * pName, char*chJson, int jsonlen)
{
	USES_CONVERSION;
	int m_result = EditBlack(A2T(pIP), A2T(pSession), A2T(pID), A2T(pName), chJson, jsonlen);
	return m_result;
}
//������������ɾ��
extern "C" __declspec(dllexport)int  FASDeleteBlack(const char * pIP, const char * pSession, const char * pID, char*chJson, int jsonlen)
{
	USES_CONVERSION;
	int m_result = DeleteBlack(A2T(pIP), A2T(pSession), A2T(pID), chJson, jsonlen);
	return m_result;
}
/****************************************
���������������ͷ�ļ�
*****************************************/
//�������������ѯ
extern "C" __declspec(dllexport)int  FASCheckWhite(const char * pIP, const char * pSession, const char * pName, char*chJson, int jsonlen)
{
	USES_CONVERSION;
	int m_result = CheckWhite(A2T(pIP), A2T(pSession), A2T(pName), chJson, jsonlen);
	return m_result;
}
//���������������
extern "C" __declspec(dllexport)int  FASAddWhite(const char * pIP, const char * pSession, const char * pName, char*chJson, int jsonlen)
{
	USES_CONVERSION;
	int m_result = AddWhite(A2T(pIP), A2T(pSession), A2T(pName), chJson, jsonlen);
	return m_result;
}
//�������������޸�
extern "C" __declspec(dllexport)int  FASEditWhite(const char * pIP, const char * pSession, const char * pID, const char * pName, char*chJson, int jsonlen)
{
	USES_CONVERSION;
	int m_result = EditWhite(A2T(pIP), A2T(pSession), A2T(pID), A2T(pName), chJson, jsonlen);
	return m_result;
}
//������������ɾ��
extern "C" __declspec(dllexport)int  FASDeleteWhite(const char * pIP, const char * pSession, const char * pID, char*chJson, int jsonlen)
{
	USES_CONVERSION;
	int m_result = DeleteWhite(A2T(pIP), A2T(pSession), A2T(pID),  chJson, jsonlen);
	return m_result;
}
/****************************************
���������ͷ�ļ�
*****************************************/
//��ѯ�����
extern "C" __declspec(dllexport)int  FASCheckCamera(const char * pIP, const char * pSession, char*chJson, int jsonlen)
{
	USES_CONVERSION;
	int m_result = CheckCamera(A2T(pIP), A2T(pSession), chJson, jsonlen);
	return m_result;
}