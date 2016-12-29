
// call_testDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// Ccall_testDlg �Ի���
class Ccall_testDlg : public CDialogEx
{
// ����
public:
	Ccall_testDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CALL_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int nIndex;
	CString strDisPlay;
	CStatic m_CStaticDisplay;
	void Ccall_testDlg::DoChangeDisplay();
	void Ccall_testDlg::DoChangeDisplay(int a);
	void Ccall_testDlg::DoChangeDisplay(int a, int b);
	afx_msg void OnBnClickedButtonNoArg();
	afx_msg void OnBnClickedButtonHasArg();
	afx_msg void OnBnClickedButtonHasArg2();
};
