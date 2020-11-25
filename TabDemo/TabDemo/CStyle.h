#pragma once


// CStyle dialog

class CStyle : public CPropertyPage
{
	DECLARE_DYNAMIC(CStyle)

public:
	CStyle(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CStyle();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
