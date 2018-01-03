#pragma once
/****************************************
登录管理头文件
*****************************************/
//登录一体机
extern "C" __declspec(dllexport) int DeviceLogin(const char * pIP, char *pSession);
//出登一体机
extern "C" __declspec(dllexport) int DeviceLogout(const char *pIP, const char *pSession);
/****************************************
特征提取头文件
*****************************************/
//获取人脸特征
extern "C" __declspec(dllexport) int   FASFeatureDetect(const char *pIP, const char * pSession, const char * pImageb64, char * chJson, int jsonlen);
//获取人脸特征
extern "C" __declspec(dllexport) int   FASFeatureDetectforImageFile(const char * pIP, const char * pSession, const char * pFilePath, char* chJson, int jsonlen);
/****************************************
特征比对头文件
*****************************************/
//特征比对特征
extern "C" __declspec(dllexport)int FASFeatureMatch(const char * pIP, const char * pSession, const char * pFeature1, const char * pFeature2, char*chJson, int jsonlen);
//特征比对人脸库
extern "C" __declspec(dllexport)int FASMatchPerson(const char * pIP, const char * pSession, const char * pDBs, double sim, const char * pFeature, char*chJson, int jsonlen);
/****************************************
黑名单人脸库管理头文件
*****************************************/
//黑名单人脸库查询
extern "C" __declspec(dllexport)int  FASCheckBlack(const char * pIP, const char * pSession, const char * pName, char*chJson, int jsonlen);
//黑名单人脸库添加
extern "C" __declspec(dllexport)int  FASAddBlack(const char * pIP, const char * pSession, const char * pName, char*chJson, int jsonlen);
//const char *
extern "C" __declspec(dllexport)int  FASEditBlack(const char * pIP, const char * pSession, const char * pID, const char * pName, char*chJson, int jsonlen);
//黑名单人脸库删除
extern "C" __declspec(dllexport)int  FASDeleteBlack(const char * pIP, const char * pSession, const char * pID, char*chJson, int jsonlen);
/****************************************
白名单人脸库管理头文件
*****************************************/
//白名单人脸库查询
extern "C" __declspec(dllexport)int  FASCheckWhite(const char * pIP, const char * pSession, const char * pName, char*chJson, int jsonlen);
//白名单人脸库添加
extern "C" __declspec(dllexport)int  FASAddWhite(const char * pIP, const char * pSession, const char * pName, char*chJson, int jsonlen);
//白名单人脸库修改
extern "C" __declspec(dllexport)int  FASEditWhite(const char * pIP, const char * pSession, const char * pID, const char * pName, char*chJson, int jsonlen);
//白名单人脸库删除
extern "C" __declspec(dllexport)int  FASDeleteWhite(const char * pIP, const char * pSession, const char * pID, char*chJson, int jsonlen);
/****************************************
人脸管理头文件
*****************************************/
//人脸库人员分页查询
extern "C" __declspec(dllexport) int  FASGetFaceListPageInDb(const char *  pIP, const char *  pSession, const char *  pFaceDbId, const char *  pIdCard,  char*chJson, int jsonlen);
//给一个人脸库添加一条人信息
extern "C" __declspec(dllexport) int  FASAddPersoninfo(const char *  pIP, const char *  pSession, const char *  pName, const char *  IdCard, int intGender, long longFaceDbId, const char *  pFeatures, const char *  ImageData, const char *  pBirthday, const char *  pValidTo , char * pPossonId, int nPossonLen);
// 人脸库编辑一个人脸信息
extern "C" __declspec(dllexport) int  FASUpdatePersoninfo(const char *  pIP, const char *  pSession, const char *  pPersonid, const char *  IdCard, long longFaceDbId);
//通过人脸库personid删除一条Person信息
extern "C" __declspec(dllexport) int  FASDELETEPersoninfo(const char *  pIP, const char *  pSession, const char *  pPsersonid);
//通过人脸库personid获取人脸库中人员的的特征信息
extern "C" __declspec(dllexport) int  FASGetFaceInDb(const char *  pIP, const char *  pSession, const char *  pPsersonid, char*chJson, int jsonlen);
//给人脸库添加多个人脸信息
extern "C" __declspec(dllexport) int  FASBatchAddPersoninfo(const char * pIP, const char * pSession, struct batchPerson *sbatchPersonlist);

/****************************************
摄像机管理头文件
*****************************************/
//查询摄像机
extern "C" __declspec(dllexport)int  FASCheckCamera(const char * pIP, const char * pSession, char*chJson, int jsonlen);