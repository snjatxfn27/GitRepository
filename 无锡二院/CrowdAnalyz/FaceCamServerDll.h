#pragma once
/****************************************
��¼����ͷ�ļ�
*****************************************/
//��¼һ���
extern "C" __declspec(dllexport) int DeviceLogin(const char * pIP, char *pSession);
//����һ���
extern "C" __declspec(dllexport) int DeviceLogout(const char *pIP, const char *pSession);
/****************************************
������ȡͷ�ļ�
*****************************************/
//��ȡ��������
extern "C" __declspec(dllexport) int   FASFeatureDetect(const char *pIP, const char * pSession, const char * pImageb64, char * chJson, int jsonlen);
//��ȡ��������
extern "C" __declspec(dllexport) int   FASFeatureDetectforImageFile(const char * pIP, const char * pSession, const char * pFilePath, char* chJson, int jsonlen);
/****************************************
�����ȶ�ͷ�ļ�
*****************************************/
//�����ȶ�����
extern "C" __declspec(dllexport)int FASFeatureMatch(const char * pIP, const char * pSession, const char * pFeature1, const char * pFeature2, char*chJson, int jsonlen);
//�����ȶ�������
extern "C" __declspec(dllexport)int FASMatchPerson(const char * pIP, const char * pSession, const char * pDBs, double sim, const char * pFeature, char*chJson, int jsonlen);
/****************************************
���������������ͷ�ļ�
*****************************************/
//�������������ѯ
extern "C" __declspec(dllexport)int  FASCheckBlack(const char * pIP, const char * pSession, const char * pName, char*chJson, int jsonlen);
//���������������
extern "C" __declspec(dllexport)int  FASAddBlack(const char * pIP, const char * pSession, const char * pName, char*chJson, int jsonlen);
//const char *
extern "C" __declspec(dllexport)int  FASEditBlack(const char * pIP, const char * pSession, const char * pID, const char * pName, char*chJson, int jsonlen);
//������������ɾ��
extern "C" __declspec(dllexport)int  FASDeleteBlack(const char * pIP, const char * pSession, const char * pID, char*chJson, int jsonlen);
/****************************************
���������������ͷ�ļ�
*****************************************/
//�������������ѯ
extern "C" __declspec(dllexport)int  FASCheckWhite(const char * pIP, const char * pSession, const char * pName, char*chJson, int jsonlen);
//���������������
extern "C" __declspec(dllexport)int  FASAddWhite(const char * pIP, const char * pSession, const char * pName, char*chJson, int jsonlen);
//�������������޸�
extern "C" __declspec(dllexport)int  FASEditWhite(const char * pIP, const char * pSession, const char * pID, const char * pName, char*chJson, int jsonlen);
//������������ɾ��
extern "C" __declspec(dllexport)int  FASDeleteWhite(const char * pIP, const char * pSession, const char * pID, char*chJson, int jsonlen);
/****************************************
��������ͷ�ļ�
*****************************************/
//��������Ա��ҳ��ѯ
extern "C" __declspec(dllexport) int  FASGetFaceListPageInDb(const char *  pIP, const char *  pSession, const char *  pFaceDbId, const char *  pIdCard,  char*chJson, int jsonlen);
//��һ�����������һ������Ϣ
extern "C" __declspec(dllexport) int  FASAddPersoninfo(const char *  pIP, const char *  pSession, const char *  pName, const char *  IdCard, int intGender, long longFaceDbId, const char *  pFeatures, const char *  ImageData, const char *  pBirthday, const char *  pValidTo , char * pPossonId, int nPossonLen);
// ������༭һ��������Ϣ
extern "C" __declspec(dllexport) int  FASUpdatePersoninfo(const char *  pIP, const char *  pSession, const char *  pPersonid, const char *  IdCard, long longFaceDbId);
//ͨ��������personidɾ��һ��Person��Ϣ
extern "C" __declspec(dllexport) int  FASDELETEPersoninfo(const char *  pIP, const char *  pSession, const char *  pPsersonid);
//ͨ��������personid��ȡ����������Ա�ĵ�������Ϣ
extern "C" __declspec(dllexport) int  FASGetFaceInDb(const char *  pIP, const char *  pSession, const char *  pPsersonid, char*chJson, int jsonlen);
//����������Ӷ��������Ϣ
extern "C" __declspec(dllexport) int  FASBatchAddPersoninfo(const char * pIP, const char * pSession, struct batchPerson *sbatchPersonlist);

/****************************************
���������ͷ�ļ�
*****************************************/
//��ѯ�����
extern "C" __declspec(dllexport)int  FASCheckCamera(const char * pIP, const char * pSession, char*chJson, int jsonlen);