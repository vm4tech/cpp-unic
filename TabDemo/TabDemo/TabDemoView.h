
// TabDemoView.h : interface of the CTabDemoView class
//

#pragma once


class CTabDemoView : public CView
{
protected: // create from serialization only
	CTabDemoView() noexcept;
	DECLARE_DYNCREATE(CTabDemoView)

// Attributes
public:
	CTabDemoDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CTabDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TabDemoView.cpp
inline CTabDemoDoc* CTabDemoView::GetDocument() const
   { return reinterpret_cast<CTabDemoDoc*>(m_pDocument); }
#endif

