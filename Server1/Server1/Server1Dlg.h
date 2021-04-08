
// Server1Dlg.h : header file
//

#pragma once

struct ThreadArgs {
	HANDLE ms;
	HWND hWnd;
};
UINT ServeMailslot(LPVOID lpParameter);
// CServer1Dlg dialog
class CServer1Dlg : public CDialogEx
{
// Construction
public:
	CServer1Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER1_DIALOG };
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
	HANDLE m_Mailslot;
	CWinThread* m_pMailslotThread;

	CListBox m_ListBox;
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedCancel();
};
