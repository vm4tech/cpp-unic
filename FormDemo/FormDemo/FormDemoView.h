
// FormDemoView.h : interface of the CFormDemoView class
//

#pragma once


class CFormDemoView : public CFormView
{

public:
	CBrush m_DialogBrush;
	int m_Intensity;
	CRect m_RectSample;

	enum { INT_DARK, INT_LIGHT };

protected: // create from serialization only
	CFormDemoView() noexcept;
	DECLARE_DYNCREATE(CFormDemoView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_FORMDEMO_FORM };
#endif

// Attributes
public:
	CFormDemoDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CFormDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	BOOL m_Blue;
	BOOL m_Green;
	BOOL m_Red;

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnClickedRed();
	afx_msg void OnOptionsLight();
	afx_msg void OnUpdateOptionsLight(CCmdUI* pCmdUI);
	afx_msg void OnOptionsDark();
	afx_msg void OnUpdateOptionsDark(CCmdUI* pCmdUI);
	afx_msg void OnClickedGreen();
	afx_msg void OnClickedBlue();
	virtual void OnDraw(CDC* /*pDC*/);
};

#ifndef _DEBUG  // debug version in FormDemoView.cpp
inline CFormDemoDoc* CFormDemoView::GetDocument() const
   { return reinterpret_cast<CFormDemoDoc*>(m_pDocument); }
#endif

