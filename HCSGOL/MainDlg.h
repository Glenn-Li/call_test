#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMainDlg �Ի���

class CMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMainDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_CTabCtrlMain;
	virtual BOOL OnInitDialog();

	void CMainDlg::MainDlgSetHook(HWND hSelWnd);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	CComboBox m_CComboBoxKey;
	LRESULT CMainDlg::OnHelperCtrl(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCbnSelchangeComboKey();
};
