#pragma once


// CFormat dialog

class CFormat : public CDialog
{
	DECLARE_DYNAMIC(CFormat)

public:
	CFormat(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CFormat();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck1();
	BOOL m_Bold;
	BOOL m_Italic;
	BOOL m_Underline;
};
