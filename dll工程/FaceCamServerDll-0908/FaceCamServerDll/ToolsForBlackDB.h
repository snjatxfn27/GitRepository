#pragma once
//�������������ѯ
int CheckBlack(CString strIP, CString strSession, CString strName, char*chJson, int jsonlen);
//���������������
int AddBlack(CString strIP, CString strSession, CString strName, char*chJson, int jsonlen);
//�������������޸�
int EditBlack(CString strIP, CString strSession, CString strID, CString strName, char*chJson, int jsonlen);
//������������ɾ��
int DeleteBlack(CString strIP, CString strSession, CString strID, char*chJson, int jsonlen);