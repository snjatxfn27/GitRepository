// ToolsForRecognition.h: interface for the ToolsForRecognition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLSFORRECOGNITION_H__143B7D0A_EA6F_44BD_96A5_6C42EACCD339__INCLUDED_)
#define AFX_TOOLSFORRECOGNITION_H__143B7D0A_EA6F_44BD_96A5_6C42EACCD339__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

	//��ȡfeature
	int FeatureDetect(CString strIP,CString strSession,CString strEImage,char * pEImage,char *pEFeature);
	//ͼƬ�����Ա�
	int ImgRecognition(CString strIP,CString strSession,CString strFacedb,CString strImage,CString strTop,CString strSimilarity,int * plistsize,struct StructRecognitionList * pStructRecognitionlist);
	//ͼƬ�����Ա�������ѯ
	int ImgRecognitionForSelect(CString strIP,CString strSession,CString strFacedb,CString strImage,CString strTop,CString strSimilarity, CString strAge,CString strSex,CString strNative_place,CString strAddr,CString strName,CString strPhone,CString strId_card,int * plistsize,struct StructRecognitionList * pStructRecognitionlist);
	//�����Ա�
	int FeatureMatching(CString strIP,CString strSession,CString strFacedb,CString strFeature,CString strTop,CString strSimilarity, int * plistsize,struct StructRecognitionList * pStructRecognitionlist );
	//�Ա�ʶ��
	int ImgAgeSexAnalysis(CString strIP,CString strSession,struct StructRecognitionList * pStructRecognitionlist,CString strImage);

#endif // !defined(AFX_TOOLSFORRECOGNITION_H__143B7D0A_EA6F_44BD_96A5_6C42EACCD339__INCLUDED_)
