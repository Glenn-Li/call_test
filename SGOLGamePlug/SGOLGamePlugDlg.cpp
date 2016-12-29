
// SGOLGamePlugDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SGOLGamePlug.h"
#include "SGOLGamePlugDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define dllName "HCSGOL.dll"

HANDLE hSelProc;
HWND hSelWnd;
int m_WinNum;
struct sWindowInfo
{
	HWND hHwnd;
	HANDLE hProc;
	DWORD dwProcId;
	CString csWindowName;
	CString csClassName;
};
struct sWindowInfo m_WindowInfo[32];

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSGOLGamePlugDlg 对话框



CSGOLGamePlugDlg::CSGOLGamePlugDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSGOLGamePlugDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSGOLGamePlugDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PROCESS, m_CComboBoxProcess);
}

BEGIN_MESSAGE_MAP(CSGOLGamePlugDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CSGOLGamePlugDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_INJECTDLL, &CSGOLGamePlugDlg::OnBnClickedButtonInjectdll)
	ON_BN_CLICKED(IDC_BUTTON_SETHOOK, &CSGOLGamePlugDlg::OnBnClickedButtonSethook)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CSGOLGamePlugDlg::OnBnClickedButtonUpdate)
END_MESSAGE_MAP()

BOOL CALLBACK MyEnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	// 	if (::GetWindowLong(hwnd, GWL_STYLE)& WS_VISIBLE)
	// 	{
	// 		m_WindowInfo[m_num].hHwnd = hwnd;//record the HWND handle into array
	// 		::GetWindowThreadProcessId(hwnd, &m_WindowInfo[m_num].dwProcId); //获取窗口进程ID
	// 		CWnd* pWnd = CWnd::FromHandle(hwnd);
	// 		pWnd->GetWindowText(m_WindowInfo[m_num].csWindowName);// 获取窗口名称
	// 		//m_CComboBoxProcess.AddString(m_WindowInfo[m_num].csWindowName);
	// 		m_num++;//count start
	// 	}

	CSGOLGamePlugDlg* pView = (CSGOLGamePlugDlg*)lParam;
	/*	CListCtrl& lc = pView->GetListCtrl();*/

	if (hwnd != pView->GetParentFrame()->GetSafeHwnd()) //不是本程序
	{
		DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
		if ((dwStyle & WS_OVERLAPPEDWINDOW) && (dwStyle & WS_VISIBLE))
		{
			CString csWinName;
			CWnd* pWnd = CWnd::FromHandle(hwnd);
			// 窗口标题
			pWnd->GetWindowText(csWinName);
			if (csWinName.Find("三国群英") >= 0 || csWinName.Find("online") >= 0 
				|| csWinName.Find("loader") >= 0 || csWinName.Find("CallTest") >= 0 || csWinName.Find("计算器") >= 0 || csWinName.Find("记事本") >= 0)
			{
				m_WindowInfo[m_WinNum].hHwnd = hwnd;//record the HWND handle into array

				//保存窗口名
				m_WindowInfo[m_WinNum].csWindowName = csWinName;
				//获取窗口进程ID
				::GetWindowThreadProcessId(hwnd, &m_WindowInfo[m_WinNum].dwProcId);
				// 窗口类名
				::GetClassName(hwnd, m_WindowInfo[m_WinNum].csClassName.GetBuffer(256), 256);
				m_WinNum++;
			}

		}
	}

	return TRUE;
}

// CSGOLGamePlugDlg 消息处理程序

_declspec(dllimport) void SetHook(HWND gameh);

BOOL CSGOLGamePlugDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	m_WinNum = 0;
	EnumWindows(MyEnumWindowsProc, (LPARAM)this);//遍历窗口程序

	CString strTemp;
	for (int i = 0; i < m_WinNum; i++)
	{
		strTemp.Format("%08d-%s", m_WindowInfo[i].dwProcId, m_WindowInfo[i].csWindowName);
		m_CComboBoxProcess.AddString(strTemp);
	}

	m_CComboBoxProcess.SetCurSel(0);

	OnBnClickedButtonOpen();
	OnBnClickedButtonInjectdll();
	OnBnClickedButtonSethook();


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSGOLGamePlugDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSGOLGamePlugDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSGOLGamePlugDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSGOLGamePlugDlg::OnBnClickedButtonOpen()
{
	// TODO:  在此添加控件通知处理程序代码
	int nCurSel = m_CComboBoxProcess.GetCurSel();
	::ShowWindow(m_WindowInfo[nCurSel].hHwnd, SW_SHOWNORMAL);
	::SetForegroundWindow(m_WindowInfo[nCurSel].hHwnd);
	hSelWnd = m_WindowInfo[nCurSel].hHwnd;
	hSelProc = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_WindowInfo[nCurSel].dwProcId); //打开进程句柄
}


void InjectDll(HWND Gameh, const char* DllNameFullPath)
{
	DWORD pid = 0;
	DWORD byWriteSize = NULL;
	HANDLE hProcess = NULL;
	LPDWORD AddressDW = NULL;
	HANDLE threadHandle = NULL;
	//获取游戏窗口句柄

	if (Gameh != 0)
	{
		//根据游戏窗口句柄获取进程PID
		GetWindowThreadProcessId(Gameh, &pid);
		if (pid != 0)
		{
			//通过进程PID 获取进程句柄
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
			if (hProcess != NULL)
			{
				//在游戏进程分配一片内存空间
				AddressDW = (LPDWORD)VirtualAllocEx(hProcess, NULL, 256, MEM_COMMIT, PAGE_READWRITE);
				if (AddressDW != NULL)
				{
					//写入 DLL 全路径名
					WriteProcessMemory(hProcess, AddressDW, DllNameFullPath, strlen(DllNameFullPath) + 1, &byWriteSize);
					if (byWriteSize >= strlen(DllNameFullPath))
					{
						threadHandle = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, AddressDW, NULL, NULL);
						WaitForSingleObject(threadHandle, INFINITE);//等待注入DLL的线程执行完再进行下一步
						CloseHandle(threadHandle);
						VirtualFreeEx(hProcess, AddressDW, 256, MEM_DECOMMIT);
						CloseHandle(hProcess);
					}
					else
					{
						AfxMessageBox("启动外挂失败\r\n");  // 注入dll失败
					}
				}
			}
		}
	}
	return;
}

DWORD WINAPI DoInjectDll(LPARAM lpData)
{
	//添加显示窗口的代码
	char DirName[256] = "";//存放目录名
	char dllNamePath[256] = "";//DLL全路径名
	GetCurrentDirectoryA(sizeof(DirName), DirName);
	strcpy_s(dllNamePath, DirName);
	strcat_s(dllNamePath, "\\");
	strcat_s(dllNamePath, dllName);//追加DLL名字
	printf("%s \r\n", dllNamePath);
	// 此处需要改成用线程注入
	InjectDll(hSelWnd, dllNamePath); //注入DLL到游戏窗口

	return TRUE;
}


void CSGOLGamePlugDlg::OnBnClickedButtonInjectdll()
{
	// TODO:  在此添加控件通知处理程序代码
	// 启动一个线程来注入dll
	::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DoInjectDll, NULL, NULL, NULL);
}


void CSGOLGamePlugDlg::OnBnClickedButtonSethook()
{
	// TODO:  在此添加控件通知处理程序代码
	SetHook(hSelWnd);
}


void CSGOLGamePlugDlg::OnBnClickedButtonUpdate()
{
	// TODO:  在此添加控件通知处理程序代码
	m_WinNum = 0;
	EnumWindows(MyEnumWindowsProc, (LPARAM)this);//遍历窗口程序

	m_CComboBoxProcess.Clear();

	CString strTemp;
	for (int i = 0; i < m_WinNum; i++)
	{
		strTemp.Format("%08d-%s", m_WindowInfo[i].dwProcId, m_WindowInfo[i].csWindowName);
		m_CComboBoxProcess.AddString(strTemp);
	}

	m_CComboBoxProcess.SetCurSel(0);
}
