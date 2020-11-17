
// FontDemoView.h : interface of the CFontDemoView class
//

#pragma once


class CFontDemoView : public CView
{
protected: // create from serialization only
	CFontDemoView() noexcept;
	DECLARE_DYNCREATE(CFontDemoView)

// Attributes
public:
	CFontDemoDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CFontDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FontDemoView.cpp
inline CFontDemoDoc* CFontDemoView::GetDocument() const
   { return reinterpret_cast<CFontDemoDoc*>(m_pDocument); }
#endif

