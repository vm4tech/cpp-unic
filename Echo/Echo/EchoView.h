
// EchoView.h : interface of the CEchoView class
//

#pragma once


class CEchoView : public CView
{
private:
	POINT m_CaretPos;
	int m_XCaret, m_YCaret;

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
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditClear();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};

#ifndef _DEBUG  // debug version in EchoView.cpp
inline CEchoDoc* CEchoView::GetDocument() const
   { return reinterpret_cast<CEchoDoc*>(m_pDocument); }
#endif

