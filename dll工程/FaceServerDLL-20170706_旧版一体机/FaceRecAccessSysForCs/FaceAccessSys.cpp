#include "stdafx.h"
#include "FaceAccessSys.h"
#include "ToolsForFaceManage.h"

CFaceAccessSys::CFaceAccessSys(void)
{
}

CFaceAccessSys::~CFaceAccessSys(void)
{
}
//注册到一体机，传入参数：本地人脸库id，本地数据库ip，数据库名称，用户名，密码，注册一体机ip，一体机人脸库id，返回注册成功的人脸记录条数。
int CFaceAccessSys::BatchInfoPublish(char * pLocalDbId,char * pLocalDbIp,char * pDbName,char * pDbUser,char * pDbPwd,char * pFaceDbIp,char * pFaceDbId,int * pi_Count_Success)
{
	CString strLocalDbIp = pLocalDbIp;
	CString strLocalDbId = pLocalDbId;
	CString strDbName = pDbName;
	CString strDbUser = pDbUser;
	CString strDbPwd = pDbPwd;
	CString strFaceDbIp = pFaceDbIp;//一体机ip
	char pSession[19];
	* pi_Count_Success =0;
	CString strFaceDbId = pFaceDbId;//一体机人脸库id
	CString strPersonName;//姓名
	CString strSex="male";//性别
	CString strBirth ="";//出生日期
	CString strDepartment="";//部门
	CString strNativeplace="";//籍贯
	CString strPhone="";//手机号
	CString strAddress = "";
	CString strIdcard="-!@2";//卡号
	CString strIdcard_mem="-?^1";//上一个卡号
	char pPersonid[19];
	CString strPersonid = "0";
	CString strImage;
	CString strFeature;
	//连接本地数据库获
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
		AfxMessageBox("数据库连接失败！");
	}
	try
	{
	//登录在线的一体机。
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
// 		m_result = Logout(strFaceDbIp,pSession);//登出一个一体机。
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
