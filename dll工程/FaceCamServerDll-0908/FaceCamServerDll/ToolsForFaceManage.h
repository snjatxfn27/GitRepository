#pragma once
struct batchPerson1
{
	char* pName;
	char *pIdCard;
	int  intGender; //= 0;
	long longFaceDbId;// = 1;
	char *pFeatures;
	char *pImageData;
	char *pBirthday;
	char *pValidTo;
};
//��������Ա��ҳ��ѯ
int GetFaceListPageInDb(CString strIP, CString strSession, CString strFaceDbId, CString IdCard, char*chJson, int jsonlen);
//��һ�����������һ������Ϣ
int AddPersoninfo(CString strIP, CString strSession, CString strName, CString IdCard, int intGender, long longFaceDbId, CString strFeatures, CString ImageData, CString strBirthday, CString strValidTo, char * pPossonId, int nPossonLen);
// ������༭һ��������Ϣ
//int UpdatePersoninfo(CString strIP, CString strSession, CString strPersonid, CString strName, CString IdCard, int intGender, long longFaceDbId, CString strFeatures, CString strImageData, CString strBirthday, CString strValidTo);
int UpdatePersoninfo(CString strIP, CString strSession, CString strPersonid, CString IdCard, long longFaceDbId);
//ͨ��������personidɾ��һ��Person��Ϣ
int DELETEPersoninfo(CString strIP, CString strSession, CString strPersonid);
//ͨ��������personid��ȡ����������Ա�ĵ��������沿��Ƭ��Ϣ
int GetFaceInDb(CString strIP, CString strSession, CString strPersonid, char*chJson, int jsonlen);
//����������Ӷ��������Ϣ
int BatchAddPersoninfo(CString strIP, CString strSession, struct batchPerson1 *sbatchPersonlist, int nNumber);
