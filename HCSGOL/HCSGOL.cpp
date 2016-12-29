// HCSGOL.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "HCSGOL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CHCSGOLApp
// #define USER_MSG_CREAT_MAIN_WINDOW		(WM_USER + 100)
// ON_MESSAGE(USER_MSG_CREAT_MAIN_WINDOW, OnCreatMainWindow)

BEGIN_MESSAGE_MAP(CHCSGOLApp, CWinApp)	
END_MESSAGE_MAP()


// CHCSGOLApp 构造

CHCSGOLApp::CHCSGOLApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CHCSGOLApp 对象
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
	//添加显示窗口的代码
// 	CMainDlg  m_MainWnd;
// 	m_MainWnd.CreateEx(WS_EX_NOPARENTNOTIFY, NULL, _T("hcsgol"), WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS,
// 		0, 0, 0, 0, g_hWnd, NULL, NULL);

	//m_MainWnd.SetWindowPos();
	SLog("ShowDialog");
	PMainDialog = new CMainDlg;
	SLog("ShowDialog");
	PMainDialog->DoModal();//阻塞的方式 
	//关闭此窗口后
	SLog("资源释放");
	delete PMainDialog;//释放new分配的空间
	PMainDialog = NULL;
	FreeLibraryAndExitThread(theApp.m_hInstance, 1);//释放mfc_dll.dll //FreeLibrary (GetModuleHandle("mfc_dll.dll")

	return TRUE;
}

DWORD WINAPI CreatDialog(LPARAM lpData)
{
	//添加显示窗口的代码
	PMainDialog = new CMainDlg;
	PMainDialog->Create(IDD_DIALOG_MAIN);
	PMainDialog->ShowWindow(TRUE);//SW_SHOW
	//PMainDialog->DoModal();//阻塞的方式
	//关闭此窗口后
	//delete PMainDialog;//释放new分配的空间
	//FreeLibraryAndExitThread(theApp.m_hInstance, 1);//释放mfc_dll.dll //FreeLibrary (GetModuleHandle("mfc_dll.dll")

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

// CHCSGOLApp 初始化

BOOL CHCSGOLApp::InitInstance()
{
	CWinApp::InitInstance();
	g_hMyInstance = AfxGetInstanceHandle();	
	
/*	::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CreatDialog, NULL, NULL, NULL);*/

	return TRUE;
}

LRESULT CALLBACK KeyBoardHookWndProc(
	int code,       // hook code
	WPARAM wParam,  //按键代码 =VK_F12 VK_HOME
	LPARAM lParam   // 31位为0 则是被按下
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
		SLog("日志服务初始化完成！");
	}

	//比如说 按下VK_HOME 我们要做什么
	SLog("wParam = %d, lParam =%d", wParam, lParam);
	//g_hWnd = g_shWnd;
	SLog("Game g_hWnd: %d, g_hMyInstance: %d", g_hWnd, g_hMyInstance);
	if ((wParam == VK_INSERT) && ((lParam&(1 << 31)) == 0))
	{
		SLog("按下Insert键");
		if (PMainDialog == NULL)
		{
			SLog("创建一个线程来打开外挂窗口");
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
		SLog("日志服务初始化完成！");
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
		SLog("日志服务初始化完成！");
	}

	SLog("p->hwnd: %d, p->lParam: %d, p->wParam: %d, p->message: %d", p->hwnd, p->lParam, p->wParam, p->message);
	SLog("wParam = %d, lParam =%d", wParam, lParam);

	return CallNextHookEx(0, code, wParam, lParam);
}

LRESULT CALLBACK GameWndProc_1(
	int code,       // hook code
	WPARAM wParam,  //按键代码 =VK_F12 VK_HOME
	LPARAM lParam   // 31位为0 则是被按下
	)
{
	CWPSTRUCT *p = (CWPSTRUCT *)lParam;
// 	if (g_hWnd != p->hwnd)
// 		return  CallNextHookEx(0, code, wParam, lParam);

	if (g_PCLog == NULL)
	{
		g_PCLog = new CLog();
		g_PCLog->Open();
		SLog("日志服务初始化完成！");
	}

	SLog("p->hwnd: %d, p->lParam: %d, p->wParam: %d, p->message: %d", p->hwnd, p->lParam, p->wParam, p->message);
	//比如说 按下VK_HOME 我们要做什么
	if (p->message == WM_KEYDOWN)
	{
		SLog("wParam = %d, lParam =%d", wParam, lParam);
		if ((wParam == VK_HOME) && ((lParam&(1 << 31)) == 0))
		{
			SLog("按下Home键");
			if (PMainDialog == NULL)
			{
				SLog("创建一个线程来打开外挂窗口");
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
		SLog("日志服务初始化完成！");
	}

	DWORD ndThreadId = GetCurrentThreadId();
	SLog("p->hwnd: %d, ndThreadId: %d, p->lParam: %d, p->wParam: %d, p->message: %d", p->hwnd, ndThreadId, p->lParam, p->wParam, p->message);
	//SLog("wParam = %d, lParam =%d", wParam, lParam);

	return CallNextHookEx(0, code, wParam, lParam);
}

//安装勾子的函数
//这个函数是在GamePlug的领空，全局变量都没有传递给dll
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
		SLog("日志服务初始化完成！");
	}

	g_hWnd = hSelWnd;
	//g_shWnd = hSelWnd;
	SLog("hook游戏主进程！！！ g_hWnd = %d", g_hWnd);
	DWORD ndThreadId = GetWindowThreadProcessId(g_hWnd, NULL);
	if (ndThreadId == 0)
		return;
	SLog("Game g_hWnd: %d, ndThreadId: %d， g_hMyInstance: %d", g_hWnd, ndThreadId, g_hMyInstance);
	// 这个挂钩只与线程相关联，如果SGOLGamePlug进程退出也就没用了
	// 所有的钩子处理函数需要判断事件来源是否来自游戏进程
	::SetWindowsHookEx(WH_KEYBOARD, &KeyBoardHookWndProc, g_hMyInstance, ndThreadId);  // 按键钩子
	//::SetWindowsHookEx(WH_CBT, &CbtHookWndProc, g_hMyInstance, ndThreadId);  // 窗口事件钩子
	//::SetWindowsHookEx(WH_MOUSE, &MouseHookWndProc, g_hMyInstance, ndThreadId);  // 鼠标钩子
	//::SetWindowsHookEx(WH_CALLWNDPROC, &CallWndProcHookWndProc, g_hMyInstance, ndThreadId);  // 什么都能钩
}


// 退出
int	CHCSGOLApp::ExitInstance()
{
// 	if (PMainDialog != NULL) 
// 	{
// 		SLog("资源释放");
// 		delete PMainDialog;//释放相应内存空间
// 		PMainDialog = NULL;
// 	}
	SLog("DLL退出");
	return CWinApp::ExitInstance();//winApp基类函数
}
