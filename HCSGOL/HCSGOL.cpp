// HCSGOL.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "HCSGOL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ��  ����ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CHCSGOLApp
// #define USER_MSG_CREAT_MAIN_WINDOW		(WM_USER + 100)
// ON_MESSAGE(USER_MSG_CREAT_MAIN_WINDOW, OnCreatMainWindow)

BEGIN_MESSAGE_MAP(CHCSGOLApp, CWinApp)	
END_MESSAGE_MAP()


// CHCSGOLApp ����

CHCSGOLApp::CHCSGOLApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CHCSGOLApp ����
CLog *g_PCLog = NULL;
/*static HWND g_shWnd;*/
HHOOK g_hhkGame = NULL;
CMainDlg *PMainDialog = NULL;
HINSTANCE g_hMyInstance = NULL;

#pragma data_seg(".Shared")
HWND g_hWnd = NULL;
#pragma data_seg()
#pragma comment(linker, "/Section:.Shared,RWS")

CHCSGOLApp theApp;

DWORD WINAPI ShowDialog(LPARAM lpData)
{
	//�����ʾ���ڵĴ���
// 	CMainDlg  m_MainWnd;
// 	m_MainWnd.CreateEx(WS_EX_NOPARENTNOTIFY, NULL, _T("hcsgol"), WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS,
// 		0, 0, 0, 0, g_hWnd, NULL, NULL);

	//m_MainWnd.SetWindowPos();
	SLog("ShowDialog");
	PMainDialog = new CMainDlg;
	SLog("ShowDialog");
	PMainDialog->DoModal();//�����ķ�ʽ 
	//�رմ˴��ں�
	SLog("��Դ�ͷ�");
	delete PMainDialog;//�ͷ�new����Ŀռ�
	PMainDialog = NULL;
	FreeLibraryAndExitThread(theApp.m_hInstance, 1);//�ͷ�mfc_dll.dll //FreeLibrary (GetModuleHandle("mfc_dll.dll")

	return TRUE;
}

DWORD WINAPI CreatDialog(LPARAM lpData)
{
	//�����ʾ���ڵĴ���
	PMainDialog = new CMainDlg;
	PMainDialog->Create(IDD_DIALOG_MAIN);
	PMainDialog->ShowWindow(TRUE);//SW_SHOW
	//PMainDialog->DoModal();//�����ķ�ʽ
	//�رմ˴��ں�
	//delete PMainDialog;//�ͷ�new����Ŀռ�
	//FreeLibraryAndExitThread(theApp.m_hInstance, 1);//�ͷ�mfc_dll.dll //FreeLibrary (GetModuleHandle("mfc_dll.dll")

	return TRUE;
}

LRESULT CHCSGOLApp::OnCreatMainWindow(WPARAM wParam, LPARAM lParam)
{
// 	int nCount = *((int*)lParam);
// 	CString strCount;
// 	strCount.Format("%d", nCount);
// 	m_CStaticCount.SetWindowTextA(strCount);
	::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CreatDialog, NULL, NULL, NULL);
	return 0;
}

// CHCSGOLApp ��ʼ��

BOOL CHCSGOLApp::InitInstance()
{
	CWinApp::InitInstance();
	g_hMyInstance = AfxGetInstanceHandle();	
	
/*	::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CreatDialog, NULL, NULL, NULL);*/

	return TRUE;
}

LRESULT CALLBACK KeyBoardHookWndProc(
	int code,       // hook code
	WPARAM wParam,  //�������� =VK_F12 VK_HOME
	LPARAM lParam   // 31λΪ0 ���Ǳ�����
	)
{
	CWPSTRUCT *p = (CWPSTRUCT *)lParam;
// 	if (g_hWnd != p->hwnd)
// 		return  CallNextHookEx(0, code, wParam, lParam);
	if (PMainDialog != NULL)
	{
		return CallNextHookEx(0, code, wParam, lParam);
	}
	
	if (g_PCLog == NULL)
	{
		g_PCLog = new CLog();
		g_PCLog->Open();
		SLog("��־�����ʼ����ɣ�");
	}

	//����˵ ����VK_HOME ����Ҫ��ʲô
	SLog("wParam = %d, lParam =%d", wParam, lParam);
	//g_hWnd = g_shWnd;
	SLog("Game g_hWnd: %d, g_hMyInstance: %d", g_hWnd, g_hMyInstance);
	if ((wParam == VK_INSERT) && ((lParam&(1 << 31)) == 0))
	{
		SLog("����Insert��");
		if (PMainDialog == NULL)
		{
			SLog("����һ���߳�������Ҵ���");
			::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ShowDialog, NULL, NULL, NULL);
		}
	}
	return CallNextHookEx(0, code, wParam, lParam);
}

LRESULT CALLBACK CbtHookWndProc(
	int code,       // hook code
	WPARAM wParam,
	LPARAM lParam
	)
{
	CWPSTRUCT *p = (CWPSTRUCT *)lParam;
// 	if (g_hWnd != p->hwnd)
// 		return  CallNextHookEx(0, code, wParam, lParam);

	if (g_PCLog == NULL)
	{
		g_PCLog = new CLog();
		g_PCLog->Open();
		SLog("��־�����ʼ����ɣ�");
	}

	
	SLog("p->hwnd: %d, p->lParam: %d, p->wParam: %d, p->message: %d", p->hwnd, p->lParam, p->wParam, p->message);
	SLog("wParam = %d, lParam =%d", wParam, lParam);

	return CallNextHookEx(0, code, wParam, lParam);
}

LRESULT CALLBACK MouseHookWndProc(
	int code,       // hook code
	WPARAM wParam,
	LPARAM lParam
	)
{
	CWPSTRUCT *p = (CWPSTRUCT *)lParam;
// 	if (g_hWnd != p->hwnd)
// 		return  CallNextHookEx(0, code, wParam, lParam);

	if (g_PCLog == NULL)
	{
		g_PCLog = new CLog();
		g_PCLog->Open();
		SLog("��־�����ʼ����ɣ�");
	}

	SLog("p->hwnd: %d, p->lParam: %d, p->wParam: %d, p->message: %d", p->hwnd, p->lParam, p->wParam, p->message);
	SLog("wParam = %d, lParam =%d", wParam, lParam);

	return CallNextHookEx(0, code, wParam, lParam);
}

LRESULT CALLBACK GameWndProc_1(
	int code,       // hook code
	WPARAM wParam,  //�������� =VK_F12 VK_HOME
	LPARAM lParam   // 31λΪ0 ���Ǳ�����
	)
{
	CWPSTRUCT *p = (CWPSTRUCT *)lParam;
// 	if (g_hWnd != p->hwnd)
// 		return  CallNextHookEx(0, code, wParam, lParam);

	if (g_PCLog == NULL)
	{
		g_PCLog = new CLog();
		g_PCLog->Open();
		SLog("��־�����ʼ����ɣ�");
	}

	SLog("p->hwnd: %d, p->lParam: %d, p->wParam: %d, p->message: %d", p->hwnd, p->lParam, p->wParam, p->message);
	//����˵ ����VK_HOME ����Ҫ��ʲô
	if (p->message == WM_KEYDOWN)
	{
		SLog("wParam = %d, lParam =%d", wParam, lParam);
		if ((wParam == VK_HOME) && ((lParam&(1 << 31)) == 0))
		{
			SLog("����Home��");
			if (PMainDialog == NULL)
			{
				SLog("����һ���߳�������Ҵ���");
				::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ShowDialog, NULL, NULL, NULL);
			}
		}
	}
	
	return CallNextHookEx(0, code, wParam, lParam);
}

LRESULT CALLBACK CallWndProcHookWndProc(
	int code,       // hook code
	WPARAM wParam,
	LPARAM lParam
	)
{
	CWPSTRUCT *p = (CWPSTRUCT *)lParam;
// 	if (g_hWnd != p->hwnd)
// 		return  CallNextHookEx(0, code, wParam, lParam);

	if (g_PCLog == NULL)
	{
		g_PCLog = new CLog();
		g_PCLog->Open();
		SLog("��־�����ʼ����ɣ�");
	}

	DWORD ndThreadId = GetCurrentThreadId();
	SLog("p->hwnd: %d, ndThreadId: %d, p->lParam: %d, p->wParam: %d, p->message: %d", p->hwnd, ndThreadId, p->lParam, p->wParam, p->message);
	//SLog("wParam = %d, lParam =%d", wParam, lParam);

	return CallNextHookEx(0, code, wParam, lParam);
}

//��װ���ӵĺ���
//�����������GamePlug����գ�ȫ�ֱ�����û�д��ݸ�dll
void SetHook(HWND hSelWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (g_PCLog == NULL)
	{
		g_PCLog = new CLog();
		//CString PID;
		//PID.Format("PID-%d", GetCurrentProcessId());
		//g_PCLog->Open(PID);
		g_PCLog->Open();
		SLog("��־�����ʼ����ɣ�");
	}

	g_hWnd = hSelWnd;
	//g_shWnd = hSelWnd;
	SLog("hook��Ϸ�����̣����� g_hWnd = %d", g_hWnd);
	DWORD ndThreadId = GetWindowThreadProcessId(g_hWnd, NULL);
	if (ndThreadId == 0)
		return;
	SLog("Game g_hWnd: %d, ndThreadId: %d�� g_hMyInstance: %d", g_hWnd, ndThreadId, g_hMyInstance);
	// ����ҹ�ֻ���߳�����������SGOLGamePlug�����˳�Ҳ��û����
	// ���еĹ��Ӵ�������Ҫ�ж��¼���Դ�Ƿ�������Ϸ����
	::SetWindowsHookEx(WH_KEYBOARD, &KeyBoardHookWndProc, g_hMyInstance, ndThreadId);  // ��������
	//::SetWindowsHookEx(WH_CBT, &CbtHookWndProc, g_hMyInstance, ndThreadId);  // �����¼�����
	//::SetWindowsHookEx(WH_MOUSE, &MouseHookWndProc, g_hMyInstance, ndThreadId);  // ��깳��
	//::SetWindowsHookEx(WH_CALLWNDPROC, &CallWndProcHookWndProc, g_hMyInstance, ndThreadId);  // ʲô���ܹ�
}


// �˳�
int	CHCSGOLApp::ExitInstance()
{
// 	if (PMainDialog != NULL) 
// 	{
// 		SLog("��Դ�ͷ�");
// 		delete PMainDialog;//�ͷ���Ӧ�ڴ�ռ�
// 		PMainDialog = NULL;
// 	}
	SLog("DLL�˳�");
	return CWinApp::ExitInstance();//winApp���ຯ��
}
