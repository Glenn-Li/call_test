
// SGOLGamePlug.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSGOLGamePlugApp: 
// �йش����ʵ�֣������ SGOLGamePlug.cpp
//

class CSGOLGamePlugApp : public CWinApp
{
public:
	CSGOLGamePlugApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSGOLGamePlugApp theApp;