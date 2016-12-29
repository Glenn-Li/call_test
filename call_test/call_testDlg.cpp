
// call_testDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "call_test.h"
#include "call_testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// Ccall_testDlg 对话框



Ccall_testDlg::Ccall_testDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Ccall_testDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Ccall_testDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_DISPLAY, m_CStaticDisplay);
}

BEGIN_MESSAGE_MAP(Ccall_testDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_NO_ARG, &Ccall_testDlg::OnBnClickedButtonNoArg)
	ON_BN_CLICKED(IDC_BUTTON_HAS_ARG, &Ccall_testDlg::OnBnClickedButtonHasArg)
	ON_BN_CLICKED(IDC_BUTTON_HAS_ARG2, &Ccall_testDlg::OnBnClickedButtonHasArg2)
END_MESSAGE_MAP()


// Ccall_testDlg 消息处理程序

BOOL Ccall_testDlg::OnInitDialog()
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
	nIndex = 0;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Ccall_testDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Ccall_testDlg::OnPaint()
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
HCURSOR Ccall_testDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Ccall_testDlg::DoChangeDisplay()
{
	strDisPlay = "test1";
	m_CStaticDisplay.SetWindowText(strDisPlay);
}


void Ccall_testDlg::DoChangeDisplay(int a)
{
	if (a == 1)
		strDisPlay = "test21";

	else if (a == 2)
		strDisPlay = "test22";

	else
		strDisPlay = "test2";

	m_CStaticDisplay.SetWindowText(strDisPlay);
}


void Ccall_testDlg::DoChangeDisplay(int a, int b)
{
	if (a == 1 && b == 1)
		strDisPlay = "test311";

	else if (a == 2 && b == 2)
		strDisPlay = "test322";

	else
		strDisPlay = "test3";

	m_CStaticDisplay.SetWindowText(strDisPlay);
}


void Ccall_testDlg::OnBnClickedButtonNoArg()
{
	// TODO:  在此添加控件通知处理程序代码
	DoChangeDisplay();
}


void Ccall_testDlg::OnBnClickedButtonHasArg()
{
	// TODO:  在此添加控件通知处理程序代码
	DoChangeDisplay(nIndex++);
	if (nIndex > 2) nIndex = 0;
}


void Ccall_testDlg::OnBnClickedButtonHasArg2()
{
	// TODO:  在此添加控件通知处理程序代码
	DoChangeDisplay(nIndex, nIndex);
	nIndex++;
	if (nIndex > 2) nIndex = 0;
}
