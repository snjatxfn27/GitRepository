
// WinSnapshotServer.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "WinSnapshotServer.h"
#include "WinSnapshotServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWinSnapshotServerApp

BEGIN_MESSAGE_MAP(CWinSnapshotServerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CWinSnapshotServerApp ����

CWinSnapshotServerApp::CWinSnapshotServerApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CWinSnapshotServerApp ����

CWinSnapshotServerApp theApp;


// CWinSnapshotServerApp ��ʼ��

BOOL CWinSnapshotServerApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	//�Լ��Ĵ���
	WORD wVersionRequested; WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return false;
	}
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return false;
	}
	//
	CString strIp;
	TCHAR pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);

	CString csFullPath(pFileName);
	csFullPath += "\\Parameter.ini";
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Sql_ServerAdd"), _T(""), m_strDBIP.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Sql_Database"), _T(""), m_strDBName.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Sql_Name"), _T(""), m_strDBUsername.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	GetPrivateProfileString(_T("SqlConfiguration"), _T("Sql_Pwd"), _T(""), m_strDBPassword.GetBuffer(MAX_PATH), MAX_PATH, csFullPath);
	if (m_strDBIP.IsEmpty() || m_strDBName.IsEmpty() || m_strDBUsername.IsEmpty() || m_strDBPassword.IsEmpty())
	{
		
	}


// 	m_strDBIP = L"192.168.1.145";
// 	m_strDBName = L"facedetectsys";
// 	m_strDBUsername = L"root";
// 	m_strDBPassword = L"cr";

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CWinSnapshotServerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}



int CWinSnapshotServerApp::ExitInstance()
{
	// TODO:  �ڴ����ר�ô����/����û���
	WSACleanup();

	return CWinApp::ExitInstance();
}
