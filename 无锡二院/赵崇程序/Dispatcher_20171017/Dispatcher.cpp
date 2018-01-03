
// Dispatcher.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Dispatcher.h"
#include "DispatcherDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDispatcherApp

BEGIN_MESSAGE_MAP(CDispatcherApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDispatcherApp ����

CDispatcherApp::CDispatcherApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDispatcherApp ����

CDispatcherApp theApp;


// CDispatcherApp ��ʼ��

BOOL CDispatcherApp::InitInstance()
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

	CString strBox;
	//�����ݿ�������ƶȺͶԱȷ�����������id
	_ConnectionPtr conn;
	conn.CreateInstance(__uuidof(Connection));
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	CString strConn;
	strConn.Format(_T("Driver={MySQL ODBC 5.1 Driver}; Server=%s;Database=%s; uid=%s; pwd=%s;"), theApp.m_strDBIP, theApp.m_strDBName, theApp.m_strDBUsername, theApp.m_strDBPassword);
	conn->ConnectionTimeout = 2;
	try{ conn->Open(_bstr_t(strConn), "", "", adModeUnknown); }
	catch (_com_error e)
	{
		strBox.Format(L"���ݿ�����ʧ�ܣ�\n���ݿ�IPΪ��%s������Ϊ��%s���û���Ϊ��%s��\n��������������ȷ�ԡ�", m_strDBIP, m_strDBName, m_strDBUsername);
		AfxMessageBox(strBox);
		return false;
	}
	try{
		//�����ƶ�
		CString strSQL = _T("select CameraSimilarity from parameterini where Parameter='computer'");
		pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		CString strSim;
		strSim = (char*)_bstr_t(pSet->GetCollect(_variant_t("CameraSimilarity")));
		m_dbTotalSim = _wtof(strSim);
		pSet->Close();
		//���Աȷ�����IP,������ID
		strSQL = _T("select ip,facedbs from device");
		pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		if (!pSet->adoEOF)
		{
			m_strDetectServerIP = (char*)_bstr_t(pSet->GetCollect(_variant_t("ip")));
			m_strTotalDBIDs = (char*)_bstr_t(pSet->GetCollect(_variant_t("facedbs")));
			int b(0), e(0);
			e = m_strTotalDBIDs.Find(L"+", b);
			m_strBID = m_strTotalDBIDs.Mid(b, e - b);
			b = e + 1;
			e = m_strTotalDBIDs.Find(L"+", b);
			m_strWID = m_strTotalDBIDs.Mid(b, e - b);
			b = e + 1;
			e = m_strTotalDBIDs.Find(L"+", b);
			m_strTBID = m_strTotalDBIDs.Mid(b, e - b);
			m_strTEID = m_strTotalDBIDs.Mid(e + 1, m_strTotalDBIDs.GetLength() - e - 1);
		}
		else
		{
			pSet->Close();
			conn->Close();
			return false;
		}
		pSet->Close();
	}
	catch (_com_error e){
		strBox.Format(L"��ʼ�����ݿ��ȡ���ƶȺ�������IDʧ�ܣ�\n����parameterini��device����ȷ�ԡ�", m_strDBIP, m_strDBName, m_strDBUsername);
		AfxMessageBox(strBox);
		return false;
	}
	conn->Close();

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

	CDispatcherDlg dlg;
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


int CDispatcherApp::ExitInstance()
{
	// TODO:  �ڴ����ר�ô����/����û���
	WSACleanup();
//	_CrtDumpMemoryLeaks();
	return CWinApp::ExitInstance();
}
