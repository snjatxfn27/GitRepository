
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��







#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif
//ADO�������ݿ�
#import "c:\program files\common files\system\ado\msado15.dll" no_namespace \
	rename("EOF", "adoEOF")

//json
#ifdef _DEBUG
#pragma comment(lib,"json_vc71_libmtd.lib")
#endif
#ifndef _DEBUG
#pragma comment(lib,"json_vc71_libmt.lib")
#endif
#include "json\include\json.h"



//GDI+
#include <GdiPlus.h>
using namespace Gdiplus;
#pragma comment(lib, "GdiPlus.lib")
#include "Tools.h"

//FaceCamServerDll
#pragma comment(lib , "FaceCamServerDll.lib")
#include "FaceCamServerDll.h"



#include "GeneralDef.h"
#pragma comment(lib,"HCNetSDK.lib")

// #include "comboboxxp.h"


// #define _CRTDBG_MAP_ALLO
#define _CRTDBG_MAP_ALLOC
#include "stdlib.h"
#include <crtdbg.h>


extern  _ConnectionPtr  g_pConnect;


#include "Log.h"