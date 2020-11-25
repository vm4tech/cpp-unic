#pragma once


// CPitch dialog

class CPitch : public CPropertyPage
{
	DECLARE_DYNAMIC(CPitch)

public:
	CPitch(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPitch();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
