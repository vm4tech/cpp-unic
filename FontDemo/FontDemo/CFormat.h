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
};
