
// ReceiverDlg.h : header file
//

#pragma once
#include <winsock2.h>

// CReceiverDlg dialog
class CReceiverDlg : public CDialogEx
{
// Construction
public:
	CReceiverDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RECEIVER_DIALOG };
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
	bool m_IsStarted;
	void SetStarted(bool IsStarted);
	CListBox m_ListBox;
	afx_msg void OnBnClickedAllIp();
	afx_msg void OnBnClickedReceive();
};
