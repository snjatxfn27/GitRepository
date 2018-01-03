
// Dispatcher.cpp : 定义应用程序的类行为。
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


// CDispatcherApp 构造

CDispatcherApp::CDispatcherApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CDispatcherApp 对象

CDispatcherApp theApp;


// CDispatcherApp 初始化

BOOL CDispatcherApp::InitInstance()
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
	//数据库
	USES_CONVERSION;
	CFile file;
	if (!file.Open(L"settings.txt", CFile::modeRead))
	{
//		MessageBox(L"配置文件打开失败", L"提示", MB_OK | MB_ICONINFORMATION);
		AfxMessageBox(L"配置文件打开失败！\n请检查配置文件是否存在。");
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
		AfxMessageBox(L"配置文件读取错误！\n请检查配置文件的正确性。");
		return false;
	}

	CString strBox;
	//从数据库读出相似度和对比服务器人脸库id
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
		strBox.Format(L"数据库连接失败！\n数据库IP为：%s，库名为：%s，用户名为：%s。\n请检查连接属性正确性。", m_strDBIP, m_strDBName, m_strDBUsername);
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
		strBox.Format(L"初始从数据库读取相似度和人脸库ID失败！\n请检查parameterini和device表正确性。", m_strDBIP, m_strDBName, m_strDBUsername);
		AfxMessageBox(strBox);
		return false;
	}
	conn->Close();
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

	CDispatcherDlg dlg;
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


int CDispatcherApp::ExitInstance()
{
	// TODO:  在此添加专用代码和/或调用基类
	WSACleanup();

	return CWinApp::ExitInstance();
}
