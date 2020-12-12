
// TextDemoView.h : interface of the CTextDemoView class
//

#pragma once


class CTextDemoView : public CScrollView
{
protected: // create from serialization only
	CTextDemoView() noexcept;
	DECLARE_DYNCREATE(CTextDemoView)

// Attributes
public:
	CTextDemoDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CTextDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TextDemoView.cpp
inline CTextDemoDoc* CTextDemoView::GetDocument() const
   { return reinterpret_cast<CTextDemoDoc*>(m_pDocument); }
#endif

