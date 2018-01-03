#include "stdafx.h"
#include "OperationTool.h"


int _GetExecuteResults(_RecordsetPtr & pSet, _ConnectionPtr pConnect, CString csSql)
{
	try
	{
		_RecordsetPtr  pSet1;
		VARIANT vTmp;
		vTmp.vt = VT_BSTR;
		vTmp.bstrVal = NULL;
		pSet = pConnect->Execute(_bstr_t(csSql), &vTmp, adCmdText);

		//CString csNumber(vTmp.bstrVal);
		//	return atoi(csNumber);
		return pSet->GetRecordCount();
	}
	catch (CException* e)
	{
		return -1;
	}
}

int __BreakCString(CString csData, CStringArray &arOutCString, CString csFindFlag)
{
	int iPos = -1;
	CString csTemp;
	while ((iPos = csData.Find(csFindFlag)) != -1)
	{
		csTemp = csData.Left(iPos);
		csData.Delete(0, iPos + 1);
		arOutCString.Add(csTemp);
	}
	arOutCString.Add(csData);

	return arOutCString.GetCount();
}

void __ExpandAllNode(HTREEITEM hItem, CTreeCtrl& m_treeShow)
{

	HTREEITEM hChild = m_treeShow.GetChildItem(hItem);

	while (hChild)
	{
		m_treeShow.Expand(hItem, TVE_EXPAND);

		__ExpandAllNode(hChild, m_treeShow);

		hChild = m_treeShow.GetNextSiblingItem(hChild);
	}

}


int __EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen)
{
	char EnBase64Tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	unsigned char c1, c2, c3;    // ���뻺��������3���ֽ�
	int nDstLen = 0;             // ������ַ�����
	int nLineLen = 0;            // ������г��ȼ���
	int nDiv = nSrcLen / 3;      // �������ݳ��ȳ���3�õ��ı���
	int nMod = nSrcLen % 3;      // �������ݳ��ȳ���3�õ�������

	// ÿ��ȡ3���ֽڣ������4���ַ�
	for (int i = 0; i < nDiv; i++)
	{
		// ȡ3���ֽ�
		c1 = *pSrc++;
		c2 = *pSrc++;
		c3 = *pSrc++;

		// �����4���ַ�
		*pDst++ = EnBase64Tab[c1 >> 2];
		*pDst++ = EnBase64Tab[((c1 << 4) | (c2 >> 4)) & 0x3f];
		*pDst++ = EnBase64Tab[((c2 << 2) | (c3 >> 6)) & 0x3f];
		*pDst++ = EnBase64Tab[c3 & 0x3f];
		nLineLen += 4;
		nDstLen += 4;

		// ������У�
		if (nLineLen > nMaxLineLen - 4)
		{
			*pDst++ = '\r';
			*pDst++ = '\n';
			nLineLen = 0;
			nDstLen += 2;
		}
	}

	// �������µ��ֽ�
	if (nMod == 1)
	{
		c1 = *pSrc++;
		*pDst++ = EnBase64Tab[(c1 & 0xfc) >> 2];
		*pDst++ = EnBase64Tab[((c1 & 0x03) << 4)];
		*pDst++ = '=';
		*pDst++ = '=';
		nLineLen += 4;
		nDstLen += 4;
	}
	else if (nMod == 2)
	{
		c1 = *pSrc++;
		c2 = *pSrc++;
		*pDst++ = EnBase64Tab[(c1 & 0xfc) >> 2];
		*pDst++ = EnBase64Tab[((c1 & 0x03) << 4) | ((c2 & 0xf0) >> 4)];
		*pDst++ = EnBase64Tab[((c2 & 0x0f) << 2)];
		*pDst++ = '=';
		nDstLen += 4;
	}
	// ����Ӹ�������
	*pDst = '\0';

	return nDstLen;
}
//base64����
int __DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen)
{
	char DeBase64Tab[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		62,        // '+'
		0, 0, 0,
		63,        // '/'
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61,        // '0'-'9'
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,        // 'A'-'Z'
		0, 0, 0, 0, 0, 0,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,        // 'a'-'z'
	};

	int nDstLen;            // ������ַ�����
	int nValue;             // �����õ��ĳ�����
	int i;

	i = 0;
	nDstLen = 0;

	// ȡ4���ַ������뵽һ�����������پ�����λ�õ�3���ֽ�
	while (i < nSrcLen)
	{
		if (*pSrc != '\r' && *pSrc != '\n')
		{
			nValue = DeBase64Tab[*pSrc++] << 18;
			nValue += DeBase64Tab[*pSrc++] << 12;
			*pDst++ = (nValue & 0x00ff0000) >> 16;
			nDstLen++;

			if (*pSrc != '=')
			{
				nValue += DeBase64Tab[*pSrc++] << 6;
				*pDst++ = (nValue & 0x0000ff00) >> 8;
				nDstLen++;

				if (*pSrc != '=')
				{
					nValue += DeBase64Tab[*pSrc++];
					*pDst++ = nValue & 0x000000ff;
					nDstLen++;
				}
			}

			i += 4;
		}
		else        // �س����У�����
		{
			pSrc++;
			i++;
		}
	}
	// ����Ӹ�������
	*pDst = '\0';

	return nDstLen;
}

/*********************************************************
Function:	g_StringLanType
Desc:		get the current operation language string type
Input:	szLanCn, Chinese string; szLanEn, English string;
Output:	szDstLan, current string
Return:	none
**********************************************************/
void g_StringLanType(char *szDstLan, char *szLanCn, char *szLanEn)
{
#ifdef DEMO_LAN_CN
	sprintf(szDstLan, "%s", szLanCn);
#else	
	sprintf(szDstLan, "%s", szLanEn);
#endif
	return;
	WORD wLangID = (WORD)(PRIMARYLANGID(::GetSystemDefaultLangID()));
	switch (wLangID)
	{
	case LANG_ENGLISH:
		sprintf(szDstLan, "%s", szLanEn);
		break;
	case LANG_CHINESE:
		sprintf(szDstLan, "%s", szLanCn);
		break;
	default:
		sprintf(szDstLan, "%s", szLanEn);
		break;
	}
}

