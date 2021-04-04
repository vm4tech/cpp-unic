
// NbDgramDlg.h : header file
//

#pragma once


// CNbDgramDlg dialog
class CNbDgramDlg : public CDialogEx
{
// Construction
public:
	CNbDgramDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NBDGRAM_DIALOG };
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
	void SetUI(bool IsSender);

	BOOL m_Sender;
	afx_msg void OnClickedSender();
	afx_msg void OnReceiver();
	CListBox m_ListBox;
	int m_Delay;
	int m_LanaNumber;
	int m_Mes_Number;
	void RefreshValues();
	afx_msg void OnClickedStart();
};
