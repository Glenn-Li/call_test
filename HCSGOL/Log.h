#pragma once

//定义日志等级
#define LFatal	1
#define LError	2
#define LWarn	3
#define LInfo	4
#define LDebug	5

class CLog
{
public:
	CLog();
	~CLog();

	void Log(CString Msg);
	void Log(char *strFunc,
		int iLine,
		char* cFormat,
		...);

	BOOL Open();
	BOOL Open(CString FileName);
	void Close();
	void CLog::SetLogLevel(int nLevel);

private:
	int nLogLevel;
	CString LogDirPath;
	FILE *fplog;
	void CLog::SaveLogs(CString csmsg);
};

extern CLog *g_PCLog;

#define SLog(cFormat, ...) \
	do { \
			if (g_PCLog != NULL) \
				g_PCLog->Log(__FUNCTION__, __LINE__, cFormat, __VA_ARGS__); \
		}while(0);
   

