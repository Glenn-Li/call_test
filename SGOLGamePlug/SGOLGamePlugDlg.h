
// SGOLGamePlugDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CSGOLGamePlugDlg �Ի���
class CSGOLGamePlugDlg : public CDialogEx
{
// ����
public:
	CSGOLGamePlugDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SGOLGAMEPLUG_DIALOG };

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
	CComboBox m_CComboBoxProcess;
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonInjectdll();
	afx_msg void OnBnClickedButtonSethook();
	afx_msg void OnBnClickedButtonUpdate();
};
