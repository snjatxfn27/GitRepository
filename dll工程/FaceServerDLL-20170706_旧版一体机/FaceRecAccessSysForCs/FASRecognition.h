// FASRecognition.h: interface for the FASRecognition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FASRECOGNITION_H__C1642042_8ECF_4901_93E6_E8DBF71C6C75__INCLUDED_)
#define AFX_FASRECOGNITION_H__C1642042_8ECF_4901_93E6_E8DBF71C6C75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//定义策略列表结构体
struct StructRecognitionList
{
	char aId_card[50];//身份证号
	char aSimilarity[5];//相似度
// 	char aPserson_id[10];//persion_Id
// 	char aName[50];//姓名
// 	char aBirth[50];//出生日期
// 	char aSex[50];//性别
// 	char aCard_type[50];//证件类型
// 	char aNative_place[50];//籍贯
// 	char aAddr[50];//地址
// 	char aPhone[50];//手机号	
// 	char aDb_id[6];//所在人脸库;
// 	char aCreate_date[6];//人员信息地址
// 	char aDefaut_face_image_url[6];//face 图片地址

};
class __declspec(dllexport) CFASRecognition
{
public:
	CFASRecognition();
	~CFASRecognition();
	//人脸提取特征
	int FASFeatureDetect(char * pIP,char * pSession,char * pImage,char *pEImage,char *pEFeature);
	//人脸识别
	int FASImgRecongnition(char * pIP,char * pSession,char * pFacedb,char *pImage,char *pTop,char *pSimilarity,int * plistsize,struct StructRecognitionList * pStructRecognitionlist);
	//图片特征对比条件查询
	//人脸识别
	int FASImgRecongnitionForSelect(char * pIP,char * pSession,char * pFacedb,char *pImage,char *pTop,char * pSimilarity,char * pAge,char * pSex,char * pNative_place,char * pAddr,char * pName,char * pPhone,char * pId_card, int * plistsize,struct StructRecognitionList * pStructRecognitionlist);
	//特征识别
	int FASFeatureMatching(char * pIP,char * pSession,char * pFacedb,char *pFeature,char *pTop,char *pSimilarity,int * plistsize,struct StructRecognitionList * pStructRecognitionlist);
	//性别识别
	int FASImgAgeSexAnalysis(char * pIP,char * pSession,struct StructRecognitionList * pStructRecognitionlist,char * pImage);
};
#endif // !defined(AFX_FASRECOGNITION_H__C1642042_8ECF_4901_93E6_E8DBF71C6C75__INCLUDED_)
