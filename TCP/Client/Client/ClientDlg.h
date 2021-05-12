
// ClientDlg.h : header file
//

#pragma once

#include <winsock2.h>
// CClientDlg dialog
class CClientDlg : public CDialogEx
{
// Construction
public:
	CClientDlg(CWnd* pParent = nullptr);	// standard constructor
	bool		m_IsConnected;
	SOCKET	m_sClient;
	void SetConnected(bool IsConnected);


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ListBox;
	CButton m_NoEcho;
	int m_Number;
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedSend();
};
