
// FASDLLTestDlg.h : ͷ�ļ�
#pragma comment(lib , "../Debug/FaceCamServerDll.lib")
#include"FaceCamServerDll.h"
#include"DLLMethodForCSharp.h"
#pragma once
struct batchPerson1
{
	char* pName;
	char *pIdCard;
	int  intGender; //= 0;
	long longFaceDbId;// = 1;
	char *pFeatures;
	char *pImageData;
	char *pBirthday;
	char *pValidTo;
};

//csharp dll ����
struct batchPerson
{
	int ttt;
	//char pName[50];
	// 	char pIdCard[50];
	// 	int  intGender; //= 0;
	// 	long longFaceDbId;// = 1;
	// 	char pFeatures[1024];
	//char pImageData[1024 * 1024];
	//char pBirthday[50];
	//	char pValidTo[50];
};
// CFASDLLTestDlg �Ի���
class CFASDLLTestDlg : public CDialogEx
{
	// ����
public:
	CFASDLLTestDlg(CWnd* pParent = NULL);	// ��׼���캯��
	int EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen);
	int DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen);
	CString UrlEncode(CString szToEncode);

	// �Ի�������
	enum { IDD = IDD_FASDLLTEST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


	// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLogin();
	afx_msg void OnBnClickedBtnFeaturedetect();
	afx_msg void OnBnClickedBtnFeaturedetectforjpg();
	afx_msg void OnBnClickedBtnGetpersonface();
	afx_msg void OnBnClickedBtnAddpersoninfo();
	afx_msg void OnBnClickedBtnFeaturecompare();
	afx_msg void OnBnClickedBtnCreateblackdb();
	afx_msg void OnBnClickedBtnBlackdbquery();
	afx_msg void OnBnClickedBtnUpdateblackdb();
	afx_msg void OnBnClickedBtnDelblackdb();
	afx_msg void OnBnClickedBtnBatchaddperson();
	afx_msg void OnBnClickedBtnUpdateperson();
	afx_msg void OnBnClickedBtnQueryperson();
	afx_msg void OnBnClickedBtnDeleteperson();
	afx_msg void OnBnClickedBtnAspnetgetfeature();
	afx_msg void OnBnClickedBtnImagefilecompare();
	void sssss(CString csip, char pSession[37]);

	afx_msg void OnBnClickedBtnImagefilecompare2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedBtnCreatewdb();
};
