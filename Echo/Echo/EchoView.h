
// EchoView.h : interface of the CEchoView class
//

#pragma once


class CEchoView : public CView
{
protected: // create from serialization only
	CEchoView() noexcept;
	DECLARE_DYNCREATE(CEchoView)

// Attributes
public:
	CEchoDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CEchoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in EchoView.cpp
inline CEchoDoc* CEchoView::GetDocument() const
   { return reinterpret_cast<CEchoDoc*>(m_pDocument); }
#endif

