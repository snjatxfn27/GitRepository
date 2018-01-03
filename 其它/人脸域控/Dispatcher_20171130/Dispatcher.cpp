
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
	CFile file;
	if (!file.Open(L"settings.txt", CFile::modeRead))
	{
//		MessageBox(L"�����ļ���ʧ��", L"��ʾ", MB_OK | MB_ICONINFORMATION);
		AfxMessageBox(L"�����ļ���ʧ�ܣ�\n���������ļ��Ƿ���ڡ�");
		return false;
	}
	int flen = file.GetLength();
	char *fchar = new char[flen + 1];
	memset(fchar, 0, flen);
	file.Read(fchar, flen);
	CString strFile;
	strFile = fchar;
	file.Close();
	CString strTemp;
	AfxExtractSubString(strTemp, strFile, 0, '\r');
	AfxExtractSubString(m_strDBIP, strTemp, 1, ' ');
	AfxExtractSubString(strTemp, strFile, 1, '\r');
	AfxExtractSubString(m_strDBName, strTemp, 1, ' ');
	AfxExtractSubString(strTemp, strFile, 2, '\r');
	AfxExtractSubString(m_strDBUsername, strTemp, 1, ' ');
	AfxExtractSubString(strTemp, strFile, 3, '\r');
	AfxExtractSubString(m_strDBPassword, strTemp, 1, ' ');
	AfxExtractSubString(strTemp, strFile, 4, '\r');
	AfxExtractSubString(m_strDetectServerIP, strTemp, 1, ' ');
	delete[] fchar;
	if (m_strDBIP.IsEmpty() || m_strDBName.IsEmpty() || m_strDBUsername.IsEmpty() || m_strDetectServerIP.IsEmpty())
	{
		AfxMessageBox(L"�����ļ���ȡ����\n���������ļ�����ȷ�ԡ�");
		return false;
	}

	CString strBox;
	//�����ݿ�������ƶȺͶԱȷ�����������id
	_ConnectionPtr conn;
	conn.CreateInstance(__uuidof(Connection));
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	CString strConn;
	strConn.Format(_T("Provider=SQLOLEDB; Server=%s;Database=%s; uid=%s; pwd=%s;"), theApp.m_strDBIP, theApp.m_strDBName, theApp.m_strDBUsername, theApp.m_strDBPassword);
	conn->ConnectionTimeout = 2;
	try{conn->Open(_bstr_t(strConn), "", "", adModeUnknown);}
	catch (_com_error e)
	{
		strBox.Format(L"���ݿ�����ʧ�ܣ�\n���ݿ�IPΪ��%s������Ϊ��%s���û���Ϊ��%s��\n��������������ȷ�ԡ�", m_strDBIP, m_strDBName, m_strDBUsername);
		AfxMessageBox(strBox);
		return false;
	}
	try{
		CString strSQL = _T("select CameraSimilarity from parameterini where Parameter='computer'");
		pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		CString strSim;
		strSim = (char*)_bstr_t(pSet->GetCollect(_variant_t("CameraSimilarity")));
		m_dbTotalSim = atof(T2A(strSim));
		pSet->Close();
		strSQL = _T("select BlackFaceDbs,WhiteFaceDbs from device where delflag=0 and IsFaceServer=1");
		pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		m_strTotalBlackDBIDs = (char*)_bstr_t(pSet->GetCollect(_variant_t("BlackFaceDbs")));
		m_strTotalWhiteDBIDs = (char*)_bstr_t(pSet->GetCollect(_variant_t("WhiteFaceDbs")));
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

	return CWinApp::ExitInstance();
}
