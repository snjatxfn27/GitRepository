#ifndef _LOG_H_
#define _LOG_H_

#ifndef _DEFAULT_DEBUG_FILE_
#define _DEFAULT_DEBUG_FILE_	"EooroInfo.log"
#endif

#define DT_MAX_STRING_LEN 		10240

//Level类别
#define DT_NO_LOG_LEVEL			0
#define DT_DEBUG_LEVEL			1
#define DT_INFO_LEVEL			2
#define DT_WARNING_LEVEL		3
#define DT_ERROR_LEVEL			4

//Level的名称
extern char DTLevelName[5][10];


//实际使用的Level
//extern int  DTUseLevel[5];
extern int  REQD_DTUseLevel[5];
#define DTUseLevel REQD_DTUseLevel


void DT_Log_Error(const char *file, int line, int level, int status, const char *fmt, ...);
// void DT_Log_Error(const char *file, int line, int level, int status,CString csTemp, ...);

#endif
