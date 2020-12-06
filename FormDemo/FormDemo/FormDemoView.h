
// FormDemoView.h : interface of the CFormDemoView class
//

#pragma once


class CFormDemoView : public CFormView
{
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

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FormDemoView.cpp
inline CFormDemoDoc* CFormDemoView::GetDocument() const
   { return reinterpret_cast<CFormDemoDoc*>(m_pDocument); }
#endif

