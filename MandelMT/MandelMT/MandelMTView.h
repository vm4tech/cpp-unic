
// MandelMTView.h : interface of the CMandelMTView class
//

#pragma once


class CMandelMTView : public CView
{
protected: // create from serialization only
	CMandelMTView() noexcept;
	DECLARE_DYNCREATE(CMandelMTView)

// Attributes
public:
	CMandelMTDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CMandelMTView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MandelMTView.cpp
inline CMandelMTDoc* CMandelMTView::GetDocument() const
   { return reinterpret_cast<CMandelMTDoc*>(m_pDocument); }
#endif

