
// MiniEditView.h : interface of the CMiniEditView class
//

#pragma once


class CMiniEditView : public CEditView
{
protected: // create from serialization only
	CMiniEditView() noexcept;
	DECLARE_DYNCREATE(CMiniEditView)

// Attributes
public:
	CMiniEditDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMiniEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MiniEditView.cpp
inline CMiniEditDoc* CMiniEditView::GetDocument() const
   { return reinterpret_cast<CMiniEditDoc*>(m_pDocument); }
#endif

