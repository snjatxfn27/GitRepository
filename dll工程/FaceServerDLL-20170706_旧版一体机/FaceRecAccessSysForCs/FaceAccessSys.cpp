#include "stdafx.h"
#include "FaceAccessSys.h"
#include "ToolsForFaceManage.h"

CFaceAccessSys::CFaceAccessSys(void)
{
}

CFaceAccessSys::~CFaceAccessSys(void)
{
}
//ע�ᵽһ������������������������id���������ݿ�ip�����ݿ����ƣ��û��������룬ע��һ���ip��һ���������id������ע��ɹ���������¼������
int CFaceAccessSys::BatchInfoPublish(char * pLocalDbId,char * pLocalDbIp,char * pDbName,char * pDbUser,char * pDbPwd,char * pFaceDbIp,char * pFaceDbId,int * pi_Count_Success)
{
	CString strLocalDbIp = pLocalDbIp;
	CString strLocalDbId = pLocalDbId;
	CString strDbName = pDbName;
	CString strDbUser = pDbUser;
	CString strDbPwd = pDbPwd;
	CString strFaceDbIp = pFaceDbIp;//һ���ip
	char pSession[19];
	* pi_Count_Success =0;
	CString strFaceDbId = pFaceDbId;//һ���������id
	CString strPersonName;//����
	CString strSex="male";//�Ա�
	CString strBirth ="";//��������
	CString strDepartment="";//����
	CString strNativeplace="";//����
	CString strPhone="";//�ֻ���
	CString strAddress = "";
	CString strIdcard="-!@2";//����
	CString strIdcard_mem="-?^1";//��һ������
	char pPersonid[19];
	CString strPersonid = "0";
	CString strImage;
	CString strFeature;
	//���ӱ������ݿ��
	_ConnectionPtr conn;
	conn.CreateInstance(__uuidof(Connection));
	CString csSqlConnect;
	csSqlConnect.Format("Driver={MySQL ODBC 5.1 Driver};Server=%s;Database=%s;uid=%s;pwd=%s;",strLocalDbIp,strDbName,strDbUser,strDbPwd);
	try
	{
		_bstr_t strConnect = csSqlConnect;
		conn->Open(strConnect, "", "", adModeUnknown);
	}
	catch(_com_error e)
	{
		AfxMessageBox("���ݿ�����ʧ�ܣ�");
	}
	try
	{
	//��¼���ߵ�һ�����
		int m_result = Login(pFaceDbIp,pSession);
		if(m_result != 0)
		{
			TRACE("login fault!\n");
			return m_result;
		}
		_RecordsetPtr pSet_Employee_Table;
		pSet_Employee_Table.CreateInstance(__uuidof(Recordset));
		CString l_strSQL = "select *, employeeinfo.idcard as employeeidcard ,imginfo.idcard as imgidcard from employeeinfo left join imginfo on employeeinfo.idcard=imginfo.idcard where  isregister=0 and isdelete = 0 and imginfo.idcard is NOT NULL and libraryid="+strLocalDbId;
		pSet_Employee_Table->Open(_variant_t(l_strSQL),conn.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
		while(!pSet_Employee_Table->adoEOF)
		{
			//Sleep(1);
			strPersonName= getsqlstr_employee("name");
			strSex = getsqlstr_employee("sex");
			strBirth = getsqlstr_employee("birth");
			strDepartment = getsqlstr_employee("department");
			strNativeplace = getsqlstr_employee("nativeplace");
			strPhone = getsqlstr_employee("phone");
			strAddress = getsqlstr_employee("address");
			strIdcard = getsqlstr_employee("employeeidcard");
			_variant_t var;
			var = pSet_Employee_Table->GetCollect("img");
			if(var.vt != VT_NULL)
			{
				strImage = getsqlstr_employee("img");
				strFeature = getsqlstr_employee("feature");
			}else
			{
				pSet_Employee_Table->MoveNext();
				continue;
			}

			if (strIdcard_mem==strIdcard)
			{	
				UpdatePersonInfo(strFaceDbIp,pSession,strPersonid,strFaceDbId,strPersonName,strSex,strBirth,strNativeplace,strPhone,strAddress,strIdcard);
				AddFaceInfo(strFaceDbIp,pSession,strImage,strFeature.GetBuffer(strFeature.GetLength()),strFaceDbId,strPersonid);
			}else
			{	
				DeletePersonInfobyIdcard(strFaceDbIp,pSession,strFaceDbId,strIdcard);	
				AddPersonInfo(strFaceDbIp,pSession,strFaceDbId,strPersonName,strSex,strBirth,strNativeplace,strPhone,strAddress,strIdcard,pPersonid);
				strPersonid = pPersonid;
				AddFaceInfo(strFaceDbIp,pSession,strImage,strFeature.GetBuffer(strFeature.GetLength()),strFaceDbId,strPersonid);
				strIdcard_mem = strIdcard;
				(* pi_Count_Success) ++;
			}
			pSet_Employee_Table->MoveNext();
		}
		pSet_Employee_Table->Close();
// 		m_result = Logout(strFaceDbIp,pSession);//�ǳ�һ��һ�����
// 		if(m_result != 0)
// 		{
// 			TRACE("logout fault!\n");
// 			return m_result;
// 		}
	conn->Close();
	}catch(_com_error e)
	{
	return -1;
	}
	return 0;
}
