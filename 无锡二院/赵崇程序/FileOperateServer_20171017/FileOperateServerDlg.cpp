
// FileOperateServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FileOperateServer.h"
#include "FileOperateServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define UDP_SEND_PORT 30000  //udp�˿�
#define  PROGRAM_INDEX   1		//����ID 
#define  SLEEP_TIME   30000		//����ID
UINT HeartbeatThread(LPVOID lpam);

UINT MainThread(LPVOID pParam);
UINT RecvThread(LPVOID pParam);

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileOperateServerDlg �Ի���



CFileOperateServerDlg::CFileOperateServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileOperateServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileOperateServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFileOperateServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()

LONG CallBackCrashHandler(EXCEPTION_POINTERS *pException)
{

	return EXCEPTION_EXECUTE_HANDLER;
}

// CFileOperateServerDlg ��Ϣ�������

BOOL CFileOperateServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)CallBackCrashHandler);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	int re;
	sockSrv = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(29001);
	re = bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	re = listen(sockSrv, SOMAXCONN);

	AfxBeginThread((AFX_THREADPROC)MainThread, (void*)this);
	AfxBeginThread((AFX_THREADPROC)HeartbeatThread, (void*)this);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CFileOperateServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFileOperateServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFileOperateServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT MainThread(LPVOID pParam)
{
	CFileOperateServerDlg *pWnd = (CFileOperateServerDlg*)pParam;
	int len = sizeof(SOCKADDR_IN);
	SOCKADDR_IN addrClient;
	while (1)
	{
		pWnd->sockConn = accept(pWnd->sockSrv, (SOCKADDR*)&addrClient, &len);// ���ܿͻ�������,��ȡ�ͻ��˵�ip��ַ
		pWnd->CreateRecvThread();
	}

	return 1;
}

UINT RecvThread(LPVOID pParam)
{
	CFileOperateServerDlg *pWnd = (CFileOperateServerDlg*)pParam;
	SOCKET sock = pWnd->sockConn;
	char chHead[14];
	memset(chHead, 0, 14);
	int revlen = recv(sock, chHead, 13, 0);
	CString strHead;
	strHead = chHead;
	TRACE(L"strHead = %s\n", strHead);
	CString strCom, strFilename;
	strCom = strHead.Mid(0, 3);
	strFilename = strHead.Mid(3, strHead.GetLength() - 3);
	CString strAllPath = theApp.m_strFilePath + "/Data/bx/" + strFilename + ".jpg";
	if (strCom == L"del")
	{
		DeleteFile(strAllPath);
	}
	else if (strCom == L"add")
	{
		int flen;
		CString strflen;
		char chflen[11];
		memset(chflen, 0, 11);
		int revlen = recv(sock, chflen, 10, 0);
		//		strflen = chflen;
		flen = atoi(chflen);
		TRACE(L"filelen = %d\n", flen);
		char * chFile = new char[flen+1];
		memset(chFile, 0, flen+1);
		int allRevlen = 0;
		while (true)
		{
			revlen = recv(sock, chFile+allRevlen, flen-allRevlen, 0);
			TRACE(L"recvlen = %d\n", revlen);
			if (revlen == 0 || revlen == -1)
				break;
			allRevlen += revlen;
		}
		TRACE(L"allrecvlen = %d\n", allRevlen);
		CFile file;
		file.Open(strAllPath, CFile::modeCreate | CFile::modeWrite);
		file.Write(chFile, flen);
		file.Close();
		delete[] chFile;
	}
	return 1;
}
void CFileOperateServerDlg::CreateRecvThread()
{
	AfxBeginThread((AFX_THREADPROC)RecvThread, (void*)this);
}

UINT HeartbeatThread(LPVOID lpam)
{
	USES_CONVERSION;
	while (true)
	{
		try
		{
			SOCKET sock;
			SOCKADDR_IN sin;
			sock = socket(AF_INET, SOCK_DGRAM, 0);
			if (sock == SOCKET_ERROR)
			{
				continue;;
			}
			sin.sin_family = AF_INET;
			sin.sin_addr.s_addr = inet_addr("127.0.0.1");
			sin.sin_port = htons(UDP_SEND_PORT);

			CString strSend;
			strSend.Format(L"Heartbeat_%d", PROGRAM_INDEX);
			int length;
			length = sendto(sock, T2A(strSend.GetBuffer(0)), strSend.GetLength(), 0, (SOCKADDR*)&sin, sizeof(sin));
			Sleep(SLEEP_TIME);
		}
		catch (CException* e)
		{
		}
	}
	return 0;
}