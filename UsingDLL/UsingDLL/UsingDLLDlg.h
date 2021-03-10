
// UsingDLLDlg.h : header file
//

#pragma once

extern "C" __declspec(dllimport)
double ExampleSquareRoot(double d);

// CUsingDLLDlg dialog
class CUsingDLLDlg : public CDialogEx
{
// Construction
public:
	CUsingDLLDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USINGDLL_DIALOG };
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
	double m_dInput;
	double m_dOutput;
	afx_msg void OnClickedCompute();
};
