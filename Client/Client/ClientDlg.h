
// ClientDlg.h : header file
//

#pragma once
#include "nbcommon.h"

// CClientDlg dialog
class CClientDlg : public CDialogEx
{
// Construction

public:
	CClientDlg(CWnd* pParent = nullptr);	// standard constructor

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
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	bool m_IsConnected;
	int m_Number;
	void SetConnected(bool IsConnected);
	int Connect(PNCB pncb, int lana, char* server, char* client);
};
