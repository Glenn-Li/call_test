// HCSGOL.h : HCSGOL DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "Log.h"
#include "MainDlg.h"

extern HWND g_hWnd;
extern HINSTANCE g_hMyInstance;
extern CMainDlg *PMainDialog;


// CHCSGOLApp
// �йش���ʵ�ֵ���Ϣ������� HCSGOL.cpp
//

class CHCSGOLApp : public CWinApp
{
public:
	CHCSGOLApp();

// ��д
public:
	LRESULT OnCreatMainWindow(WPARAM wParam, LPARAM lParam);
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
