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
//人脸库人员分页查询
int GetFaceListPageInDb(CString strIP, CString strSession, CString strFaceDbId, CString IdCard, char*chJson, int jsonlen);
//给一个人脸库添加一条人信息
int AddPersoninfo(CString strIP, CString strSession, CString strName, CString IdCard, int intGender, long longFaceDbId, CString strFeatures, CString ImageData, CString strBirthday, CString strValidTo, char * pPossonId, int nPossonLen);
// 人脸库编辑一个人脸信息
//int UpdatePersoninfo(CString strIP, CString strSession, CString strPersonid, CString strName, CString IdCard, int intGender, long longFaceDbId, CString strFeatures, CString strImageData, CString strBirthday, CString strValidTo);
int UpdatePersoninfo(CString strIP, CString strSession, CString strPersonid, CString IdCard, long longFaceDbId);
//通过人脸库personid删除一条Person信息
int DELETEPersoninfo(CString strIP, CString strSession, CString strPersonid);
//通过人脸库personid获取人脸库中人员的的特征和面部照片信息
int GetFaceInDb(CString strIP, CString strSession, CString strPersonid, char*chJson, int jsonlen);
//给人脸库添加多个人脸信息
int BatchAddPersoninfo(CString strIP, CString strSession, struct batchPerson1 *sbatchPersonlist, int nNumber);
