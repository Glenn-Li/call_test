
// RemoteCallDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CRemoteCallDlg �Ի���
class CRemoteCallDlg : public CDialogEx
{
// ����
public:
	CRemoteCallDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_REMOTECALL_DIALOG };

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
	CEdit m_CEditRegs[4];   // EAX EBX ECX EDX
	afx_msg void OnBnClickedButtonNoArg();
};
