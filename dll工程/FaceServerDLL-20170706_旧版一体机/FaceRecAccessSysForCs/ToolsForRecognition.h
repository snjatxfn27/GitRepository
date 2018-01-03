// ToolsForRecognition.h: interface for the ToolsForRecognition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLSFORRECOGNITION_H__143B7D0A_EA6F_44BD_96A5_6C42EACCD339__INCLUDED_)
#define AFX_TOOLSFORRECOGNITION_H__143B7D0A_EA6F_44BD_96A5_6C42EACCD339__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

	//获取feature
	int FeatureDetect(CString strIP,CString strSession,CString strEImage,char * pEImage,char *pEFeature);
	//图片特征对比
	int ImgRecognition(CString strIP,CString strSession,CString strFacedb,CString strImage,CString strTop,CString strSimilarity,int * plistsize,struct StructRecognitionList * pStructRecognitionlist);
	//图片特征对比条件查询
	int ImgRecognitionForSelect(CString strIP,CString strSession,CString strFacedb,CString strImage,CString strTop,CString strSimilarity, CString strAge,CString strSex,CString strNative_place,CString strAddr,CString strName,CString strPhone,CString strId_card,int * plistsize,struct StructRecognitionList * pStructRecognitionlist);
	//特征对比
	int FeatureMatching(CString strIP,CString strSession,CString strFacedb,CString strFeature,CString strTop,CString strSimilarity, int * plistsize,struct StructRecognitionList * pStructRecognitionlist );
	//性别识别
	int ImgAgeSexAnalysis(CString strIP,CString strSession,struct StructRecognitionList * pStructRecognitionlist,CString strImage);

#endif // !defined(AFX_TOOLSFORRECOGNITION_H__143B7D0A_EA6F_44BD_96A5_6C42EACCD339__INCLUDED_)
