
// CrowdAnalyz.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "CrowdAnalyz.h"
#include "CrowdAnalyzDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCrowdAnalyzApp

BEGIN_MESSAGE_MAP(CCrowdAnalyzApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCrowdAnalyzApp ����

CCrowdAnalyzApp::CCrowdAnalyzApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CCrowdAnalyzApp ����

CCrowdAnalyzApp theApp;


// CCrowdAnalyzApp ��ʼ��

BOOL CCrowdAnalyzApp::InitInstance()
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
	//���ݿ�
	USES_CONVERSION;
	CFile file;
	file.Open(L"settings.txt", CFile::modeRead);
	int flen = file.GetLength();
	char *fchar = new char[flen + 1];
	memset(fchar, 0, flen);
	file.Read(fchar, flen);
	CString strFile;
	strFile = A2T(fchar);
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
	AfxExtractSubString(m_strServerIP, strTemp, 1, ' ');
	delete[] fchar;

	//����4�����ID
	_ConnectionPtr conn;
	conn.CreateInstance(__uuidof(Connection));
	_RecordsetPtr pSet;
	pSet.CreateInstance(__uuidof(Recordset));
	CString strConn;
	strConn.Format(_T("Driver={MySQL ODBC 5.1 Driver}; Server=%s;Database=%s; uid=%s; pwd=%s;"), theApp.m_strDBIP, theApp.m_strDBName, theApp.m_strDBUsername, theApp.m_strDBPassword);
	try
	{
		conn->Open(_bstr_t(strConn), "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		return false;
	}
	CString strSQL;
	strSQL.Format(_T("select facedbs from device"));
	pSet->Open(_variant_t(strSQL), conn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	if (!pSet->adoEOF)
	{
		CString strFaceDB;
		strFaceDB = (char*)_bstr_t(pSet->GetCollect(_variant_t("facedbs")));
		int b(0), e(0);
		e = strFaceDB.Find(L"+", b);
		m_strBID = strFaceDB.Mid(b, e - b);
		b = e + 1;
		e = strFaceDB.Find(L"+", b);
		m_strWID = strFaceDB.Mid(b, e - b);
		b = e + 1;
		e = strFaceDB.Find(L"+", b);
		m_strTBID = strFaceDB.Mid(b, e - b);
		m_strTEID = strFaceDB.Mid(e + 1, strFaceDB.GetLength() - e - 1);
	}
	else
	{
		pSet->Close();
		conn->Close();
		return false;
	}
	pSet->Close();
	conn->Close();
	
	//	//װ��GDI+
	GdiplusStartupInput m_gdiplusStartupInput;
	GdiplusStartup(&m_pGdiToken, &m_gdiplusStartupInput, NULL);
	
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

	CCrowdAnalyzDlg dlg;
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



int CCrowdAnalyzApp::ExitInstance()
{
	// TODO:  �ڴ����ר�ô����/����û���

	//ж��GDI+
	GdiplusShutdown(m_pGdiToken);
	_CrtDumpMemoryLeaks();
	return CWinApp::ExitInstance();
}
