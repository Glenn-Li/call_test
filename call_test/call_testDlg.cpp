
// call_testDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "call_test.h"
#include "call_testDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// Ccall_testDlg �Ի���



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


// Ccall_testDlg ��Ϣ�������

BOOL Ccall_testDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	nIndex = 0;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Ccall_testDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	DoChangeDisplay();
}


void Ccall_testDlg::OnBnClickedButtonHasArg()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	DoChangeDisplay(nIndex++);
	if (nIndex > 2) nIndex = 0;
}


void Ccall_testDlg::OnBnClickedButtonHasArg2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	DoChangeDisplay(nIndex, nIndex);
	nIndex++;
	if (nIndex > 2) nIndex = 0;
}
