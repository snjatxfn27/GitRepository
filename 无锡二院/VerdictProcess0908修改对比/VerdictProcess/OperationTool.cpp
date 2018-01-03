#include "stdafx.h"
#include "OperationTool.h"


int _GetExecuteResults(_RecordsetPtr & pSet, _ConnectionPtr pConnect, CString csSql)
{
	try
	{
		VARIANT vTmp;
		vTmp.vt = VT_BSTR;
		vTmp.bstrVal = NULL;
		CString csql = _T("select * from ParameterIni ");
		pSet = pConnect->Execute(_bstr_t(csql), &vTmp, adCmdText);
		CString csNumber(vTmp.bstrVal);

		return atoi(csNumber);
	}
	catch (CException* e)
	{
		return -1;
	}
}