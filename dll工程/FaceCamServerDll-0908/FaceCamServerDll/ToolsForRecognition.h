#pragma once
//��ȡ��������
int FeatureDetect(CString strIP, CString strSession, CString strImageb64, char * chJson, int jsonlen);
//��ȡ����������ͼƬ
int FeatureDetectforImageFile(CString strIP, CString strSession, CString strFilePath, char*chJson, int jsonlen);
//��ȡ����������ͼƬ(ASP.netʹ�ã�����ֵ��󲻳���100kb,dll�����еľֲ�����Ҫ��new��delete)
int FeatureDetectforImageFileForNet(CString strIP, CString strSession, CString strFilePath, char*chJson, int jsonlen);