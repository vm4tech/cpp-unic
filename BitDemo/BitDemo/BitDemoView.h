
// BitDemoView.h : interface of the CBitDemoView class
//

#pragma once


class CBitDemoView : public CView
{
protected: // create from serialization only
	CBitDemoView() noexcept;
	DECLARE_DYNCREATE(CBitDemoView)

// Attributes
public:
	CBitDemoDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CBitDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BitDemoView.cpp
inline CBitDemoDoc* CBitDemoView::GetDocument() const
   { return reinterpret_cast<CBitDemoDoc*>(m_pDocument); }
#endif

