//#ifdef WIN32
#include "stdafx.h"
//#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#ifndef WIN32
#include <unistd.h>
#include <sys/time.h>
#endif


#include "Log.h"
// �ڴ���ͷ�ļ� [3/4/2006]
#include "Memory.h"


#ifdef HAVE_GCCVISIBILITYPATCH
	#define DLLEXPORT __attribute__ ((visibility("default")))
	#define DLLLOCAL __attribute__ ((visibility("hidden")))
#else
	#define DLLEXPORT
	#define DLLLOCAL
#endif


//int	pf = -1;
//Level������
char DTLevelName[5][10] = {"NOLOG", "DEBUG", "INFO", "WARNING", "ERROR"};

//ʵ��ʹ�õ�Level
//int  DTUseLevel[5] = {DT_NO_LOG_LEVEL, DT_DEBUG_LEVEL, DT_INFO_LEVEL, DT_WARNING_LEVEL, DT_ERROR_LEVEL};
DLLEXPORT int  DTUseLevel[5] = {DT_NO_LOG_LEVEL, DT_DEBUG_LEVEL, DT_INFO_LEVEL, DT_WARNING_LEVEL, DT_ERROR_LEVEL};

static int DT_Log_Error_GetCurTime(char* strTime)
{
	struct tm*		tmTime = NULL;
	size_t			timeLen = 0;
	time_t			tTime = 0;	
	
	tTime = time(NULL);
	tmTime = localtime(&tTime);
	timeLen = strftime(strTime, 33, "%Y-%m-%d %H:%M:%S", tmTime);
	
	return timeLen;
}


static int DT_Log_Error_OpenFile(FILE** pf)
{
	char	fileName[100];
	
	memset(fileName, 0, 100);

	char m_szDir[512] = {0};
	memset(m_szDir, 0x00, sizeof(m_szDir));
	
	GetModuleFileNameA(AfxGetStaticModuleState()->m_hCurrentInstanceHandle, m_szDir, 512);
	for (int i = strlen(m_szDir) -1; i>=0; i--)
	{
		if (m_szDir[i] == '\\')
		{
			m_szDir[i] = '\0';
			break;
		}
	}

	
#ifdef	_DEFAULT_DEBUG_FILE_ 
	sprintf(fileName, "%s\\%s", m_szDir, _DEFAULT_DEBUG_FILE_);

#else
	sprintf(fileName, "%s/log/%s", getenv("HOME"), "DebugMessage.log");
#endif
    
    *pf = fopen(fileName, "a+");

	struct   stat   st;
	int   iFileSize;
	stat(fileName, &st);
	iFileSize = st.st_size;     //��ȡ�ļ���С   
	if (iFileSize > 30700000)
	{
		if (*pf != NULL)  
			fclose(*pf);

		char	fileName1[100];
		memset(fileName1, 0, 100);
		while (true)
		{
			srand((unsigned)time(NULL));
			int nRand = rand();
			sprintf(fileName1, "%s\\%d_%s", m_szDir, nRand, _DEFAULT_DEBUG_FILE_);
			FILE *pf1 = 0;
			pf1 = fopen(fileName1, "rb");
			if (pf1 == NULL)
			{
				break;
			}
		}
		Sleep(2000);

		CString cs1(fileName);
		CString cs2(fileName1);
		MoveFile(cs1, cs2);
		Sleep(2000);

		*pf = fopen(fileName, "a+");
	}

    if(*pf  == NULL)
    {
        return-1;
    }
	
	return 0;
}


/*static void DT_Log_Error_WriteFile(const char *str)
{
	int rv = -1;
	
	//�ж��ļ��Ƿ��Ѿ���
	if(pf < 0)
	{
		if(DT_Log_Error_OpenFile())
		{
			return ;
		}
	}
	
	//��LOG����д���ļ���
	rv = write(pf, str, strlen(str));
	if(rv <= 0)
	{
		if(DT_Log_Error_OpenFile())
		{
			return ;
		}
		write(pf, str, strlen(str));
	}
	
	return ;
}*/


static void DT_Log_Error_Core(const char *file, int line, int level, int status, const char *fmt, va_list args)
{
    char str[DT_MAX_STRING_LEN];
    int	 strLen = 0;
    char tmpStr[64];
    int	 tmpStrLen = 0;
    FILE*  pf = 0;
    
    //��ʼ��
    memset(str, 0, DT_MAX_STRING_LEN);
    memset(tmpStr, 0, 64);
    
    //����LOGʱ��
    tmpStrLen = DT_Log_Error_GetCurTime(tmpStr);
    tmpStrLen = sprintf(str, "[%s] ", tmpStr);
    strLen = tmpStrLen;

    //����LOG�ȼ�
    tmpStrLen = sprintf(str+strLen, "[%s] ", DTLevelName[level]);
    strLen += tmpStrLen;
    
    //����LOG״̬
    if (status != 0) 
    {
        tmpStrLen = sprintf(str+strLen, "[ERRNO is %d] ", status);
    }
    else
    {
    	tmpStrLen = sprintf(str+strLen, "[SUCCESS] ");
    }
    strLen += tmpStrLen;

    //����LOG��Ϣ
    tmpStrLen = vsprintf(str+strLen, fmt, args);
    strLen += tmpStrLen;

    //����LOG�����ļ�
    tmpStrLen = sprintf(str+strLen, " [%s]", file);
    strLen += tmpStrLen;

    //����LOG��������
    tmpStrLen = sprintf(str+strLen, " [%d]\n", line);
    strLen += tmpStrLen;
    
	tmpStrLen = sprintf(str+strLen, "\n","" );
    strLen += tmpStrLen;

    //��LOG�ļ�
    if(DT_Log_Error_OpenFile(&pf))
	{
		return ;
	}
	
    //д��LOG�ļ�
    fwrite(str, 1, strLen, pf);
    //DT_Log_Error_WriteFile(str);
    
    //�ر��ļ�
    fclose(pf);
    
    return ;
}


void DT_Log_Error(const char *file, int line, int level, int status, const char *fmt, ...)
{
    va_list args;
	//�ж��Ƿ���ҪдLOG
//	if(level!=DT_DEBUG_LEVEL && level!=DT_INFO_LEVEL && level!=DT_WARNING_LEVEL && level!=DT_ERROR_LEVEL)
if(level == DT_NO_LOG_LEVEL)
	{
		return ;
	}
	
	//���ú��ĵ�дLOG����
// va_start(args, fmt);
	va_start(args, fmt);
	DT_Log_Error_Core(file, line, level, status, fmt, args);
    va_end(args);
    
    return ;
}
