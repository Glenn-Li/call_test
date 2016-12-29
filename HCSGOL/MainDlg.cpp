// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HCSGOL.h"
#include "MainDlg.h"
#include "afxdialogex.h"

// CMainDlg 对话框

BOOL bIsMainWinShow = FALSE;
const int Keys[11] = { VK_TAB, VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, 
						WM_LBUTTONDOWN, WM_RBUTTONDOWN };

#define WM_HELPER_CTRL				(WM_USER+100)

BOOL bIsHelperRunning = FALSE;

IMPLEMENT_DYNAMIC(CMainDlg, CDialogEx)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainDlg::IDD, pParent)
{

}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_CTabCtrlMain);
	DDX_Control(pDX, IDC_COMBO_KEY, m_CComboBoxKey);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_WM_KILLFOCUS()
	ON_WM_ACTIVATE()
	ON_MESSAGE(WM_HELPER_CTRL, OnHelperCtrl)
	ON_CBN_SELCHANGE(IDC_COMBO_KEY, &CMainDlg::OnCbnSelchangeComboKey)
END_MESSAGE_MAP()


// CMainDlg 消息处理程序

BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SLog("注入成功！！！");
	CRect rtGameWnd, rtMyWnd;
	CWnd *pWnd = CWnd::FromHandle(g_hWnd);
	pWnd->GetWindowRect(&rtGameWnd);  // 获取游戏窗口尺寸
	GetWindowRect(&rtMyWnd);		// 获取本dll的窗口尺寸
	SLog("游戏窗口尺寸: left=%ld, top=%ld, right=%ld, bottom=%ld, height=%d, width=%d",
		rtGameWnd.left, rtGameWnd.top, rtGameWnd.right, rtGameWnd.bottom, rtGameWnd.Height(), rtGameWnd.Width());
	SLog("DLL窗口尺寸: left=%ld, top=%ld, right=%ld, bottom=%ld, height=%d, width=%d",
		rtMyWnd.left, rtMyWnd.top, rtMyWnd.right, rtMyWnd.bottom, rtMyWnd.Height(), rtMyWnd.Width());

	//SetWindowPos(g_hWnd, );
	MoveWindow((int)(rtGameWnd.left + (rtGameWnd.Width() - rtMyWnd.Width()) / 2), (int)rtGameWnd.top + 60, rtMyWnd.Width(), rtMyWnd.Height());
	//MoveWindow(20, 20, 0, 0);
	//SetWindowPos();

	m_CComboBoxKey.SetCurSel(0);

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 1);//任务栏隐藏
	MainDlgSetHook(g_hWnd);
	bIsMainWinShow = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

LRESULT CALLBACK MainDlgKeyBoardHookWndProc(
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

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//比如说 按下VK_HOME， 每次都会有连续的两次的命中
	SLog("code = %d, wParam = %ld, lParam =%ld", code, wParam, lParam);
	if ((code > 0) && (wParam == VK_INSERT) && (WM_KEYDOWN == wParam || WM_SYSKEYDOWN) && ((lParam&(1 << 31)) == 0))
	{
		SLog("按下Insert键");
		if (PMainDialog == NULL)
		{
			SLog("窗口未创建，反正就是出了不可预知的问题");
		}
		else
		{
			if (bIsMainWinShow == FALSE)
			{
				SLog("PMainDialog->ShowWindow(SW_SHOW); 显示");
				bIsMainWinShow = TRUE;
				PMainDialog->ShowWindow(SW_SHOW);
			}
		}
	}
	if ((code > 0) && (wParam == VK_DELETE) && (WM_KEYDOWN == wParam || WM_SYSKEYDOWN) && ((lParam&(1 << 31)) == 0))
	{
		SLog("按下DELETE键");
		if (PMainDialog == NULL)
		{
			SLog("窗口未创建，反正就是出了不可预知的问题");
		}
		else
		{
			if (bIsMainWinShow == TRUE)
			{
				SLog("PMainDialog->ShowWindow(SW_HIDE); 隐藏");
				bIsMainWinShow = FALSE;
				PMainDialog->ShowWindow(SW_HIDE);
			}
		}
	}
	if ((code > 0) && (wParam == VK_HOME) && (WM_KEYDOWN == wParam || WM_SYSKEYDOWN) && ((lParam&(1 << 31)) == 0))
	{
		SLog("按下Home键");
		if (PMainDialog == NULL)
		{
			SLog("窗口未创建，反正就是出了不可预知的问题");
		}
		else
		{
			if (bIsHelperRunning == TRUE)
			{
// 				SLog("按键辅助停止");
// 				bIsHelperRunning = FALSE;
// 				PMainDialog->SendMessage(WM_HELPER_CTRL, 0, (LPARAM)(&bIsHelperRunning));
			}
			else
			{
				SLog("按键辅助开始");
				bIsHelperRunning = TRUE;
				PMainDialog->SendMessage(WM_HELPER_CTRL, 0, (LPARAM)(&bIsHelperRunning));
				
			}
		}
	}
	return CallNextHookEx(0, code, wParam, lParam);
}

//安装勾子的函数
void CMainDlg::MainDlgSetHook(HWND hSelWnd)
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
	SLog("hook游戏主进程！！！ g_hWnd = %d", g_hWnd);
	DWORD ndThreadId = GetWindowThreadProcessId(g_hWnd, NULL);
	if (ndThreadId == 0)
		return;
	SLog("My hWnd:%d, Game g_hWnd: %d, ndThreadId: %lld", g_hWnd, ndThreadId);
	// 这个挂钩只与线程相关联，如果SGOLGamePlug进程退出也就没用了
	// 所有的钩子处理函数需要判断事件来源是否来自游戏进程
	HHOOK hhk = ::SetWindowsHookEx(WH_KEYBOARD, &MainDlgKeyBoardHookWndProc, g_hMyInstance, ndThreadId);  // 按键钩子
	//::SetWindowsHookEx(WH_CBT, &CbtHookWndProc, g_hMyInstance, ndThreadId);  // 窗口事件钩子
	//::SetWindowsHookEx(WH_MOUSE, &MouseHookWndProc, g_hMyInstance, ndThreadId);  // 鼠标钩子
	//::SetWindowsHookEx(WH_CALLWNDPROC, &CallWndProcHookWndProc, g_hMyInstance, ndThreadId);  // 什么都能钩
}



BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	//判断是否为键盘消息
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		//判断是否按下键盘Enter键
		if (pMsg->wParam == VK_INSERT)
		{
			//Do anything what you want to
			if (bIsMainWinShow == FALSE)
			{
				SLog("PMainDialog->ShowWindow(SW_SHOW); 显示");
				bIsMainWinShow = TRUE;
				PMainDialog->ShowWindow(SW_SHOW);
			}
			return TRUE;
		}
		if (pMsg->wParam == VK_DELETE)
		{
			//Do anything what you want to
			if (bIsMainWinShow == TRUE)
			{
				SLog("PMainDialog->ShowWindow(SW_HIDE); 隐藏");
				bIsMainWinShow = FALSE;
				PMainDialog->ShowWindow(SW_HIDE);
			}
			return TRUE;
		}

	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CMainDlg::OnKillFocus(CWnd* pNewWnd)
{
	CDialogEx::OnKillFocus(pNewWnd);

	// TODO:  在此处添加消息处理程序代码
	SLog("窗口失去焦点");
	if (bIsMainWinShow == TRUE)
	{
		SLog("PMainDialog->ShowWindow(SW_HIDE); 隐藏");
		bIsMainWinShow = FALSE;
		PMainDialog->ShowWindow(SW_HIDE);
	}
}


//当失去焦点时隐藏起来
void CMainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO:  在此处添加消息处理程序代码
	SLog("窗口激活状态改变");
	if (WA_INACTIVE == nState)
	{
		if (bIsMainWinShow == TRUE)
		{
			SLog("PMainDialog->ShowWindow(SW_HIDE); 隐藏");
			bIsMainWinShow = FALSE;
			PMainDialog->ShowWindow(SW_HIDE);
		}
	}
}

LPARAM VKBParam(int VirtKey, int flag)
{
	if (flag == 1) // 按下
	{
		return (0x1 | (MapVirtualKey(VirtKey, MAPVK_VK_TO_VSC) << 16));
	}
	else if (flag == 0)
	{
		return (0x1 | ((KF_REPEAT | KF_UP | MapVirtualKey(VirtKey, MAPVK_VK_TO_VSC)) << 16));
	}
}

LRESULT CMainDlg::OnHelperCtrl(WPARAM wParam, LPARAM lParam)
{
	BOOL bCtrl = *((BOOL*)lParam);
	SLog("响应home按键");
	int nIndex = m_CComboBoxKey.GetCurSel();
	if (nIndex < 0)
		return 0;

	SLog("设置按键： %d - %d", nIndex, Keys[nIndex]);
	::PostMessage(g_hWnd, WM_KEYDOWN, Keys[nIndex], VKBParam(Keys[nIndex], 1));
	SLog("发送按键弹起消息");
	Sleep(500);
	::PostMessage(g_hWnd, WM_KEYUP, Keys[nIndex], VKBParam(Keys[nIndex], 0));
	SLog("发送按键消息结束");
	return 0;
}

void CMainDlg::OnCbnSelchangeComboKey()
{
	// TODO:  在此添加控件通知处理程序代码
	bIsHelperRunning = FALSE;
}
