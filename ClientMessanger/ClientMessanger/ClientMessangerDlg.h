
// CClientMessangerDlg.h : header file
//

#pragma once


// CClientMessangerDlg dialog
class CClientMessangerDlg : public CDialogEx
{
	// Construction
public:
	CClientMessangerDlg(CWnd* pParent = nullptr);	// standard constructor

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
	bool	m_IsConnected;
	/*static SOCKET	m_sClient;*/
	void	SetConnected(bool IsConnected);
	/*CListBox m_ListBox;*/
	CButton m_NoEcho;
	int m_Number;
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedSend();
};
