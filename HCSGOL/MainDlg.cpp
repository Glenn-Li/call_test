// MainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HCSGOL.h"
#include "MainDlg.h"
#include "afxdialogex.h"

// CMainDlg �Ի���

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


// CMainDlg ��Ϣ�������

BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SLog("ע��ɹ�������");
	CRect rtGameWnd, rtMyWnd;
	CWnd *pWnd = CWnd::FromHandle(g_hWnd);
	pWnd->GetWindowRect(&rtGameWnd);  // ��ȡ��Ϸ���ڳߴ�
	GetWindowRect(&rtMyWnd);		// ��ȡ��dll�Ĵ��ڳߴ�
	SLog("��Ϸ���ڳߴ�: left=%ld, top=%ld, right=%ld, bottom=%ld, height=%d, width=%d",
		rtGameWnd.left, rtGameWnd.top, rtGameWnd.right, rtGameWnd.bottom, rtGameWnd.Height(), rtGameWnd.Width());
	SLog("DLL���ڳߴ�: left=%ld, top=%ld, right=%ld, bottom=%ld, height=%d, width=%d",
		rtMyWnd.left, rtMyWnd.top, rtMyWnd.right, rtMyWnd.bottom, rtMyWnd.Height(), rtMyWnd.Width());

	//SetWindowPos(g_hWnd, );
	MoveWindow((int)(rtGameWnd.left + (rtGameWnd.Width() - rtMyWnd.Width()) / 2), (int)rtGameWnd.top + 60, rtMyWnd.Width(), rtMyWnd.Height());
	//MoveWindow(20, 20, 0, 0);
	//SetWindowPos();

	m_CComboBoxKey.SetCurSel(0);

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 1);//����������
	MainDlgSetHook(g_hWnd);
	bIsMainWinShow = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

LRESULT CALLBACK MainDlgKeyBoardHookWndProc(
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

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//����˵ ����VK_HOME�� ÿ�ζ��������������ε�����
	SLog("code = %d, wParam = %ld, lParam =%ld", code, wParam, lParam);
	if ((code > 0) && (wParam == VK_INSERT) && (WM_KEYDOWN == wParam || WM_SYSKEYDOWN) && ((lParam&(1 << 31)) == 0))
	{
		SLog("����Insert��");
		if (PMainDialog == NULL)
		{
			SLog("����δ�������������ǳ��˲���Ԥ֪������");
		}
		else
		{
			if (bIsMainWinShow == FALSE)
			{
				SLog("PMainDialog->ShowWindow(SW_SHOW); ��ʾ");
				bIsMainWinShow = TRUE;
				PMainDialog->ShowWindow(SW_SHOW);
			}
		}
	}
	if ((code > 0) && (wParam == VK_DELETE) && (WM_KEYDOWN == wParam || WM_SYSKEYDOWN) && ((lParam&(1 << 31)) == 0))
	{
		SLog("����DELETE��");
		if (PMainDialog == NULL)
		{
			SLog("����δ�������������ǳ��˲���Ԥ֪������");
		}
		else
		{
			if (bIsMainWinShow == TRUE)
			{
				SLog("PMainDialog->ShowWindow(SW_HIDE); ����");
				bIsMainWinShow = FALSE;
				PMainDialog->ShowWindow(SW_HIDE);
			}
		}
	}
	if ((code > 0) && (wParam == VK_HOME) && (WM_KEYDOWN == wParam || WM_SYSKEYDOWN) && ((lParam&(1 << 31)) == 0))
	{
		SLog("����Home��");
		if (PMainDialog == NULL)
		{
			SLog("����δ�������������ǳ��˲���Ԥ֪������");
		}
		else
		{
			if (bIsHelperRunning == TRUE)
			{
// 				SLog("��������ֹͣ");
// 				bIsHelperRunning = FALSE;
// 				PMainDialog->SendMessage(WM_HELPER_CTRL, 0, (LPARAM)(&bIsHelperRunning));
			}
			else
			{
				SLog("����������ʼ");
				bIsHelperRunning = TRUE;
				PMainDialog->SendMessage(WM_HELPER_CTRL, 0, (LPARAM)(&bIsHelperRunning));
				
			}
		}
	}
	return CallNextHookEx(0, code, wParam, lParam);
}

//��װ���ӵĺ���
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
		SLog("��־�����ʼ����ɣ�");
	}

	g_hWnd = hSelWnd;
	SLog("hook��Ϸ�����̣����� g_hWnd = %d", g_hWnd);
	DWORD ndThreadId = GetWindowThreadProcessId(g_hWnd, NULL);
	if (ndThreadId == 0)
		return;
	SLog("My hWnd:%d, Game g_hWnd: %d, ndThreadId: %lld", g_hWnd, ndThreadId);
	// ����ҹ�ֻ���߳�����������SGOLGamePlug�����˳�Ҳ��û����
	// ���еĹ��Ӵ�������Ҫ�ж��¼���Դ�Ƿ�������Ϸ����
	HHOOK hhk = ::SetWindowsHookEx(WH_KEYBOARD, &MainDlgKeyBoardHookWndProc, g_hMyInstance, ndThreadId);  // ��������
	//::SetWindowsHookEx(WH_CBT, &CbtHookWndProc, g_hMyInstance, ndThreadId);  // �����¼�����
	//::SetWindowsHookEx(WH_MOUSE, &MouseHookWndProc, g_hMyInstance, ndThreadId);  // ��깳��
	//::SetWindowsHookEx(WH_CALLWNDPROC, &CallWndProcHookWndProc, g_hMyInstance, ndThreadId);  // ʲô���ܹ�
}



BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	//�ж��Ƿ�Ϊ������Ϣ
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		//�ж��Ƿ��¼���Enter��
		if (pMsg->wParam == VK_INSERT)
		{
			//Do anything what you want to
			if (bIsMainWinShow == FALSE)
			{
				SLog("PMainDialog->ShowWindow(SW_SHOW); ��ʾ");
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
				SLog("PMainDialog->ShowWindow(SW_HIDE); ����");
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

	// TODO:  �ڴ˴������Ϣ����������
	SLog("����ʧȥ����");
	if (bIsMainWinShow == TRUE)
	{
		SLog("PMainDialog->ShowWindow(SW_HIDE); ����");
		bIsMainWinShow = FALSE;
		PMainDialog->ShowWindow(SW_HIDE);
	}
}


//��ʧȥ����ʱ��������
void CMainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO:  �ڴ˴������Ϣ����������
	SLog("���ڼ���״̬�ı�");
	if (WA_INACTIVE == nState)
	{
		if (bIsMainWinShow == TRUE)
		{
			SLog("PMainDialog->ShowWindow(SW_HIDE); ����");
			bIsMainWinShow = FALSE;
			PMainDialog->ShowWindow(SW_HIDE);
		}
	}
}

LPARAM VKBParam(int VirtKey, int flag)
{
	if (flag == 1) // ����
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
	SLog("��Ӧhome����");
	int nIndex = m_CComboBoxKey.GetCurSel();
	if (nIndex < 0)
		return 0;

	SLog("���ð����� %d - %d", nIndex, Keys[nIndex]);
	::PostMessage(g_hWnd, WM_KEYDOWN, Keys[nIndex], VKBParam(Keys[nIndex], 1));
	SLog("���Ͱ���������Ϣ");
	Sleep(500);
	::PostMessage(g_hWnd, WM_KEYUP, Keys[nIndex], VKBParam(Keys[nIndex], 0));
	SLog("���Ͱ�����Ϣ����");
	return 0;
}

void CMainDlg::OnCbnSelchangeComboKey()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	bIsHelperRunning = FALSE;
}
