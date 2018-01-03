#pragma once
//获取人脸特征
int FeatureDetect(CString strIP, CString strSession, CString strImageb64, char * chJson, int jsonlen);
//获取人脸特征传图片
int FeatureDetectforImageFile(CString strIP, CString strSession, CString strFilePath, char*chJson, int jsonlen);
//获取人脸特征传图片(ASP.net使用，返回值最大不超过100kb,dll方法中的局部变量要用new，delete)
int FeatureDetectforImageFileForNet(CString strIP, CString strSession, CString strFilePath, char*chJson, int jsonlen);