#pragma once
//�����ȶ�����
int FeatureMatch(CString strIP, CString strSession, CString strFeature1, CString strFeature2, char*chJson, int jsonlen);
//�����ȶ�������
int MatchPerson(CString strIP, CString strSession, CString strDBs, double sim, CString strFeature, char*chJson, int jsonlen);