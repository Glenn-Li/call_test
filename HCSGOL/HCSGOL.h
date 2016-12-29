// HCSGOL.h : HCSGOL DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "Log.h"
#include "MainDlg.h"

extern HWND g_hWnd;
extern HINSTANCE g_hMyInstance;
extern CMainDlg *PMainDialog;


// CHCSGOLApp
// 有关此类实现的信息，请参阅 HCSGOL.cpp
//

class CHCSGOLApp : public CWinApp
{
public:
	CHCSGOLApp();

// 重写
public:
	LRESULT OnCreatMainWindow(WPARAM wParam, LPARAM lParam);
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
