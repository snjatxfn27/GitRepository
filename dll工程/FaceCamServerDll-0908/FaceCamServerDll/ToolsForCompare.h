#pragma once
//特征比对特征
int FeatureMatch(CString strIP, CString strSession, CString strFeature1, CString strFeature2, char*chJson, int jsonlen);
//特征比对人脸库
int MatchPerson(CString strIP, CString strSession, CString strDBs, double sim, CString strFeature, char*chJson, int jsonlen);