
// call_testDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// Ccall_testDlg 对话框
class Ccall_testDlg : public CDialogEx
{
// 构造
public:
	Ccall_testDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CALL_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
