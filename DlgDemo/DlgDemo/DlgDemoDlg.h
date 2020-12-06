
// DlgDemoDlg.h : header file
//

#pragma once


// CDlgDemoDlg dialog
class CDlgDemoDlg : public CDialogEx
{
// Construction
public:
	CDlgDemoDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
public:
	CRect m_RectSample;
	enum { INT_DARK, INT_LIGHT };

protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_Red;
	BOOL m_Green;
	BOOL m_Blue;
	int m_Intensity;
	afx_msg void OnBnClickedRed();
	afx_msg void OnBnClickedGreen();
	afx_msg void OnBnClickedBlue();
	afx_msg void OnBnClickedDark();
	afx_msg void OnBnClickedLight();
};
