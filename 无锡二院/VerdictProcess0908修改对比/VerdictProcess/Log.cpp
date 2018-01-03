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
// 内存检测头文件 [3/4/2006]
#include "Memory.h"


#ifdef HAVE_GCCVISIBILITYPATCH
	#define DLLEXPORT __attribute__ ((visibility("default")))
	#define DLLLOCAL __attribute__ ((visibility("hidden")))
#else
	#define DLLEXPORT
	#define DLLLOCAL
#endif


//int	pf = -1;
//Level的名称
char DTLevelName[5][10] = {"NOLOG", "DEBUG", "INFO", "WARNING", "ERROR"};

//实际使用的Level
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
	iFileSize = st.st_size;     //获取文件大小   
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
	
	//判断文件是否已经打开
	if(pf < 0)
	{
		if(DT_Log_Error_OpenFile())
		{
			return ;
		}
	}
	
	//将LOG内容写入文件中
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
    
    //初始化
    memset(str, 0, DT_MAX_STRING_LEN);
    memset(tmpStr, 0, 64);
    
    //加入LOG时间
    tmpStrLen = DT_Log_Error_GetCurTime(tmpStr);
    tmpStrLen = sprintf(str, "[%s] ", tmpStr);
    strLen = tmpStrLen;

    //加入LOG等级
    tmpStrLen = sprintf(str+strLen, "[%s] ", DTLevelName[level]);
    strLen += tmpStrLen;
    
    //加入LOG状态
    if (status != 0) 
    {
        tmpStrLen = sprintf(str+strLen, "[ERRNO is %d] ", status);
    }
    else
    {
    	tmpStrLen = sprintf(str+strLen, "[SUCCESS] ");
    }
    strLen += tmpStrLen;

    //加入LOG信息
    tmpStrLen = vsprintf(str+strLen, fmt, args);
    strLen += tmpStrLen;

    //加入LOG发生文件
    tmpStrLen = sprintf(str+strLen, " [%s]", file);
    strLen += tmpStrLen;

    //加入LOG发生行数
    tmpStrLen = sprintf(str+strLen, " [%d]\n", line);
    strLen += tmpStrLen;
    
	tmpStrLen = sprintf(str+strLen, "\n","" );
    strLen += tmpStrLen;

    //打开LOG文件
    if(DT_Log_Error_OpenFile(&pf))
	{
		return ;
	}
	
    //写入LOG文件
    fwrite(str, 1, strLen, pf);
    //DT_Log_Error_WriteFile(str);
    
    //关闭文件
    fclose(pf);
    
    return ;
}


void DT_Log_Error(const char *file, int line, int level, int status, const char *fmt, ...)
{
    va_list args;
	//判断是否需要写LOG
//	if(level!=DT_DEBUG_LEVEL && level!=DT_INFO_LEVEL && level!=DT_WARNING_LEVEL && level!=DT_ERROR_LEVEL)
if(level == DT_NO_LOG_LEVEL)
	{
		return ;
	}
	
	//调用核心的写LOG函数
// va_start(args, fmt);
	va_start(args, fmt);
	DT_Log_Error_Core(file, line, level, status, fmt, args);
    va_end(args);
    
    return ;
}
