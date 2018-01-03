
// ReplaceFile.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ReplaceFile.h"
#include "ReplaceFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CReplaceFileApp

BEGIN_MESSAGE_MAP(CReplaceFileApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CReplaceFileApp ����

CReplaceFileApp::CReplaceFileApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CReplaceFileApp ����

CReplaceFileApp theApp;


// CReplaceFileApp ��ʼ��

BOOL CReplaceFileApp::InitInstance()
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
	AfxOleInit();
	//Socket
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

	//���ݿ�
	USES_CONVERSION;
	GetPrivateProfileStringW(_T("SqlConfiguration"), L"Sql_ServerAdd", _T(""), m_strDBIP.GetBuffer(MAX_PATH), MAX_PATH, L"./Parameter.ini");
	GetPrivateProfileStringW(_T("SqlConfiguration"), L"Sql_Name", _T(""), m_strDBUsername.GetBuffer(MAX_PATH), MAX_PATH, L"./Parameter.ini");
	GetPrivateProfileStringW(_T("SqlConfiguration"), L"Sql_Pwd", _T(""), m_strDBPassword.GetBuffer(MAX_PATH), MAX_PATH, L"./Parameter.ini");
	GetPrivateProfileStringW(_T("SqlConfiguration"), L"Sql_Database", _T(""), m_strDBName.GetBuffer(MAX_PATH), MAX_PATH, L"./Parameter.ini");
	GetPrivateProfileStringW(_T("Path"), L"Img_Start_Path", _T(""), m_strSrcPath.GetBuffer(MAX_PATH), MAX_PATH, L"./Parameter.ini");
	GetPrivateProfileStringW(_T("Path"), L"Img_End_Path", _T(""), m_strDstPath.GetBuffer(MAX_PATH), MAX_PATH, L"./Parameter.ini");
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

	CReplaceFileDlg dlg;
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



int CReplaceFileApp::ExitInstance()
{
	// TODO:  �ڴ����ר�ô����/����û���

	return CWinApp::ExitInstance();
}
