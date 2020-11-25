#pragma once


// CJustify dialog

class CJustify : public CPropertyPage
{
	DECLARE_DYNAMIC(CJustify)

public:
	CJustify(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CJustify();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_Justify;
};
