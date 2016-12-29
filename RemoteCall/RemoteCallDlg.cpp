
// RemoteCallDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteCall.h"
#include "RemoteCallDlg.h"
#include "afxdialogex.h"
#include <windows.h> 
#include <tlhelp32.h> 
#include <tchar.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//  Forward declarations: 
BOOL ListProcessModules(DWORD dwPID);
void printError(TCHAR* msg);
MODULEENTRY32 me32;


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


// CRemoteCallDlg �Ի���



CRemoteCallDlg::CRemoteCallDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRemoteCallDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRemoteCallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_EAX, m_CEditRegs[0]);
	DDX_Control(pDX, IDC_EDIT_EBX, m_CEditRegs[1]);
	DDX_Control(pDX, IDC_EDIT_ECX, m_CEditRegs[2]);
	DDX_Control(pDX, IDC_EDIT_EDX, m_CEditRegs[3]);
}

BEGIN_MESSAGE_MAP(CRemoteCallDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_NO_ARG, &CRemoteCallDlg::OnBnClickedButtonNoArg)
END_MESSAGE_MAP()


// CRemoteCallDlg ��Ϣ�������

BOOL CRemoteCallDlg::OnInitDialog()
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CRemoteCallDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRemoteCallDlg::OnPaint()
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
HCURSOR CRemoteCallDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


typedef struct ParamData    //�����ṹ 
{
	long Param1;
	long Param2;
	DWORD Param3;
	DWORD Param4;
}ParamData, *Paramp;

//************************************************************************************** 
//��������InfusionFunc 
//����  ����װԶ��ע��ĺ��� 
//���� 1������ID  
//���� 2����ע�뺯��ָ��<������>  
//���� 3������  
//���� 4����������  
//************************************************************************************** 
void InfusionFunc(DWORD dwProcId, LPVOID mFunc, LPVOID Param, DWORD ParamSize)
{
	HANDLE hProcess;//Զ�̾�� 
	LPVOID mFuncAddr;//���뺯���ڴ��ַ         
	LPVOID ParamAddr;//��������ڴ��ַ 
	HANDLE hThread;    //�߳̾�� 
	DWORD NumberOfByte; //��������ֵ 
	CString str;
	//�򿪱�ע��Ľ��̾��     
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcId);
	//�����ڴ� 
	mFuncAddr = VirtualAllocEx(hProcess, NULL, 256, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	ParamAddr = VirtualAllocEx(hProcess, NULL, ParamSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	//д�ڴ�  
	WriteProcessMemory(hProcess, mFuncAddr, mFunc, 256, &NumberOfByte);
	WriteProcessMemory(hProcess, ParamAddr, Param, ParamSize, &NumberOfByte);
	//����Զ���߳� 
	hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)mFuncAddr,
		ParamAddr, 0, &NumberOfByte);
	WaitForSingleObject(hThread, INFINITE); //�ȴ��߳̽��� 
	//�ͷ��������ڴ� 
	VirtualFreeEx(hProcess, mFuncAddr, 256, MEM_RELEASE);
	VirtualFreeEx(hProcess, ParamAddr, ParamSize, MEM_RELEASE);
	//�ͷ�Զ�̾�� 
	CloseHandle(hThread);
	CloseHandle(hProcess);
}


void CallNoArg()
{
// 	ParamData * lp;
// 	lp = (ParamData *)lParam;
// 	long lp1 = (long)lp->Param1;
// 	long lp2 = (long)lp->Param2;

	DWORD dwAddr = (DWORD)me32.modBaseAddr + 0x1A50;
	_asm
	{
		pushad
			mov eax, 0x0
			mov ebx, 0x1
			mov ecx, 0x12FE0C
			mov edx, 0x3E8
			call dwAddr
		popad
	}
}


void CRemoteCallDlg::OnBnClickedButtonNoArg()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	DWORD ProcessId = NULL;
	HWND hWnd = ::FindWindow(NULL, "call_test"); //���ڱ���ȡ���
	GetWindowThreadProcessId(hWnd, &ProcessId);
	ListProcessModules(ProcessId);
	::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	if (ProcessId == NULL)
		::AfxMessageBox("δ�ҵ�����");
	else
	{
		InfusionFunc(ProcessId, CallNoArg, NULL, NULL);
	}
}

// int main(void)
// {
// 	ListProcessModules(GetCurrentProcessId());
// 	return 0;
// }

BOOL ListProcessModules(DWORD dwPID)
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;

	//  Take a snapshot of all modules in the specified process. 
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		printError(TEXT("CreateToolhelp32Snapshot (of modules)"));
		return(FALSE);
	}

	//  Set the size of the structure before using it. 
	me32.dwSize = sizeof(MODULEENTRY32);

	//  Retrieve information about the first module, 
	//  and exit if unsuccessful 
	if (!Module32First(hModuleSnap, &me32))
	{
		printError(TEXT("Module32First"));  // Show cause of failure 
		CloseHandle(hModuleSnap);     // Must clean up the snapshot object! 
		return(FALSE);
	}

	//  Now walk the module list of the process, 
	//  and display information about each module 
	do
	{
		TRACE(TEXT("\n\n     MODULE NAME:     %s\n"), me32.szModule);
		TRACE(TEXT("\n     executable     = %s\n"), me32.szExePath);
		TRACE(TEXT("\n     process ID     = 0x%08X\n"), me32.th32ProcessID);
		TRACE(TEXT("\n     ref count (g)  =     0x%04X\n"), me32.GlblcntUsage);
		TRACE(TEXT("\n     ref count (p)  =     0x%04X\n"), me32.ProccntUsage);
		TRACE(TEXT("\n     base address   = 0x%08X\n"), (DWORD)me32.modBaseAddr);
		TRACE(TEXT("\n     base size      = %d\n"), me32.modBaseSize);

	} while (Module32Next(hModuleSnap, &me32));

	TRACE(TEXT("\n"));

	//  Do not forget to clean up the snapshot object. 
	CloseHandle(hModuleSnap);
	return(TRUE);
}


void printError(TCHAR* msg)
{
	DWORD eNum;
	TCHAR sysMsg[256];
	TCHAR* p;

	eNum = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, eNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		sysMsg, 256, NULL);

	// Trim the end of the line and terminate it with a null
	p = sysMsg;
	while ((*p > 31) || (*p == 9))
		++p;
	do { *p-- = 0; } while ((p >= sysMsg) &&
		((*p == '.') || (*p < 33)));

	// Display the message
	TRACE(TEXT("\n  WARNING: %s failed with error %d (%s)"), msg, eNum, sysMsg);
}
