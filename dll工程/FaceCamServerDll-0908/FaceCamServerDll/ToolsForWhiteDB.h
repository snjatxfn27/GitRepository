#pragma once
//�������������ѯ
int CheckWhite(CString strIP, CString strSession, CString strName, char*chJson, int jsonlen);
//���������������
int AddWhite(CString strIP, CString strSession, CString strName, char*chJson, int jsonlen);
//�������������޸�
int EditWhite(CString strIP, CString strSession, CString strID, CString strName, char*chJson, int jsonlen);
//������������ɾ��
int DeleteWhite(CString strIP, CString strSession, CString strID, char*chJson, int jsonlen);