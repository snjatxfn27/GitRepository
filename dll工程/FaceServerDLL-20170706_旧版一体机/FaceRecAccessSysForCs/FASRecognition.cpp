// FASRecognition.cpp: implementation of the FASRecognition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FaceRecAccessSys.h"
#include "FASRecognition.h"
#include "ToolsForRecognition.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFASRecognition::CFASRecognition()
{

}

CFASRecognition::~CFASRecognition()
{

}
//人脸提取特征
int CFASRecognition::FASFeatureDetect(char * pIP,char * pSession,char * pImage,char *pEImage,char *pEFeature)
{	
	CString strIP;
	CString strSession;
	CString strImage;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strImage.Format("%s",pImage);
	int m_result = FeatureDetect(strIP,strSession,strImage,pEImage,pEFeature);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}

//人脸识别
int CFASRecognition::FASImgRecongnition(char * pIP,char * pSession,char * pFacedb,char *pImage,char *pTop,char * pSimilarity, int * plistsize,struct StructRecognitionList * pStructRecognitionlist)
{	
	CString strIP;
	CString strSession;
	CString strFacedb;
	CString strImage;
	CString strTop;
	CString strSimilarity;

	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strFacedb.Format("%s",pFacedb);
	strImage.Format("%s",pImage);
	strTop.Format("%s",pTop);
	strSimilarity.Format("%s",pSimilarity);
	int m_result = ImgRecognition(strIP,strSession,strFacedb,strImage,strTop,strSimilarity,plistsize,pStructRecognitionlist);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}

//图片特征对比条件查询
//人脸识别
int CFASRecognition::FASImgRecongnitionForSelect(char * pIP,char * pSession,char * pFacedb,char *pImage,char *pTop,char * pSimilarity,char * pAge,char * pSex,char * pNative_place,char * pAddr,char * pName,char * pPhone,char * pId_card, int * plistsize,struct StructRecognitionList * pStructRecognitionlist)
{	
	CString strIP;
	CString strSession;
	CString strFacedb;
	CString strImage;
	CString strTop;
	CString strSimilarity;
	CString strAge;
	CString strSex;
	CString strNative_place;
	CString strAddr;
	CString strName;
	CString strPhone;
	CString strId_card;
	
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strFacedb.Format("%s",pFacedb);
	strImage.Format("%s",pImage);
	strTop.Format("%s",pTop);
	strSimilarity.Format("%s",pSimilarity);
	strAge.Format("%s",pAge);
	strSex.Format("%s",pSex);
	strNative_place.Format("%s",pNative_place);
	strAddr.Format("%s",pAddr);
	strName.Format("%s",pName);
	strPhone.Format("%s",pPhone);
	strId_card.Format("%s",pId_card);
	int m_result = ImgRecognitionForSelect(strIP,strSession,strFacedb,strImage,strTop,strSimilarity, strAge,strSex, strNative_place, strAddr, strName, strPhone, strId_card,plistsize,pStructRecognitionlist );
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}

//特征识别FASFeatureMatching
int CFASRecognition::FASFeatureMatching(char * pIP,char * pSession,char * pFacedb,char *pFeature,char *pTop,char * pSimilarity, int * plistsize,struct StructRecognitionList * pStructRecognitionlist)
{	
	CString strIP;
	CString strSession;
	CString strFacedb;
	CString strFeature;
	CString strTop;
	CString strSimilarity;
	
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strFacedb.Format("%s",pFacedb);
	strFeature.Format("%s",pFeature);
	strTop.Format("%s",pTop);
	strSimilarity.Format("%s",pSimilarity);
	int m_result = FeatureMatching(strIP,strSession,strFacedb,strFeature,strTop,strSimilarity,plistsize,pStructRecognitionlist);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}

//年龄性别识别
int CFASRecognition::FASImgAgeSexAnalysis(char * pIP,char * pSession,struct StructRecognitionList * pStructRecognitionlist,char * pImage)
{	
	CString strIP;
	CString strSession;
	CString strImage;
	strIP.Format("%s",pIP);
	strSession.Format("%s",pSession);
	strImage.Format("%s",pImage);
	int m_result = ImgAgeSexAnalysis(strIP,strSession,pStructRecognitionlist,strImage);
	if(m_result != 0)
	{
		return m_result;
	}
	return 0;
}
