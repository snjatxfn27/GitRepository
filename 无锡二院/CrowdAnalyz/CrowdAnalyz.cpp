
// CrowdAnalyz.cpp : 定义应用程序的类行为。
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


// CCrowdAnalyzApp 构造

CCrowdAnalyzApp::CCrowdAnalyzApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CCrowdAnalyzApp 对象

CCrowdAnalyzApp theApp;


// CCrowdAnalyzApp 初始化

BOOL CCrowdAnalyzApp::InitInstance()
{


	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	//自己的代码
	AfxOleInit();
	//数据库
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

	//读出4个库的ID
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
	
	//	//装载GDI+
	GdiplusStartupInput m_gdiplusStartupInput;
	GdiplusStartup(&m_pGdiToken, &m_gdiplusStartupInput, NULL);
	
	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO:  应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CCrowdAnalyzDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}



int CCrowdAnalyzApp::ExitInstance()
{
	// TODO:  在此添加专用代码和/或调用基类

	//卸载GDI+
	GdiplusShutdown(m_pGdiToken);
	_CrtDumpMemoryLeaks();
	return CWinApp::ExitInstance();
}
