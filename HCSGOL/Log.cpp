#include "stdafx.h"
#include "Log.h"
#include <share.h>


CLog::CLog()
{
	CString CurPath = "";
	::GetCurrentDirectory(1024, CurPath.GetBuffer(1024));
	CurPath.ReleaseBuffer();

	LogDirPath = CurPath;
	fplog = NULL;
	nLogLevel = LDebug;
}


CLog::~CLog()
{
	if (fplog != NULL)
		fclose(fplog);
}

BOOL CLog::Open()
{
	char buffline[256];

	memset(buffline, 0, 256);
	// Need fix bug
	CTime m_time = CTime::GetCurrentTime();
	CString ctime = m_time.Format(_T("%Y%m%d"));
	CreateDirectory(LogDirPath + "\\Logs", NULL);
	sprintf_s(buffline, "%s\\Logs\\hcsgol_%s.log", (LPSTR)(LPCTSTR)LogDirPath, (LPSTR)(LPCTSTR)ctime);
	fplog = _fsopen(buffline, "a+", _SH_DENYWR);
	if (fplog == NULL){
		CString strTemp;
		strTemp.Format("打开日志文件路径写错误! buffline: %s", buffline);
		AfxMessageBox(strTemp);
		return FALSE;
	}

	return TRUE;
}

BOOL CLog::Open(CString FileName)
{
	char buffline[256];

	memset(buffline, 0, 256);
	// Need fix bug
	CTime m_time = CTime::GetCurrentTime();
	CString ctime = m_time.Format(_T("%Y%m%d"));
	CreateDirectory(LogDirPath + "\\Logs", NULL);
	sprintf_s(buffline, "%s\\Logs\\%s_%s.log", LogDirPath, (LPSTR)(LPCTSTR)FileName, (LPSTR)(LPCTSTR)ctime);
	fplog = _fsopen(buffline, "a+", _SH_DENYWR);
	if (fplog == NULL){
		CString strTemp;
		strTemp.Format("打开日志文件路径写错误! buffline: %s", buffline);
		AfxMessageBox(strTemp);
		return FALSE;
	}

	return TRUE;
}

void CLog::Close()
{
	if (fplog != NULL)
		fclose(fplog);

}

void CLog::SetLogLevel(int nLevel)
{
	nLogLevel = nLevel;
}

void CLog::Log(CString Msg)
{
	CString strFmtMag;
	CTime m_time;

	m_time = CTime::GetCurrentTime();
	strFmtMag = m_time.Format(_T("[%Y-%m-%d %H:%M:%S] "));
	strFmtMag += Msg + "\n";

	SaveLogs(strFmtMag);
}

void CLog::Log(char *strFunc,
	int iLine,
	char* cFormat,
	...)
{

	CString strTimeStamp;
	CString strFmtMag;
	CTime m_time;

	m_time = CTime::GetCurrentTime();
	strTimeStamp = m_time.Format(_T("[%Y-%m-%d %H:%M:%S] "));
	strFmtMag.Format("%s [%s][%d] ", strTimeStamp, strFunc, iLine);

	char acBuffer[255];
	va_list args;
	va_start(args, cFormat);
	vsprintf_s(acBuffer, cFormat, args);

	strFmtMag += acBuffer;
	strFmtMag += "\n";

	va_end(args);

	SaveLogs(strFmtMag);
}

void CLog::SaveLogs(CString csMsg)
{
	if (fplog != NULL)
	{
		fputs((LPSTR)(LPCTSTR)csMsg, fplog);
		fflush(fplog);
	}
}