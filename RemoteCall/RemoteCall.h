
// RemoteCall.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRemoteCallApp: 
// �йش����ʵ�֣������ RemoteCall.cpp
//

class CRemoteCallApp : public CWinApp
{
public:
	CRemoteCallApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRemoteCallApp theApp;