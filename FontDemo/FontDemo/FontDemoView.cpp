
// FontDemoView.cpp : implementation of the CFontDemoView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "FontDemo.h"
#endif

#include "FontDemoDoc.h"
#include "FontDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFontDemoView

IMPLEMENT_DYNCREATE(CFontDemoView, CView)

BEGIN_MESSAGE_MAP(CFontDemoView, CView)
END_MESSAGE_MAP()

// CFontDemoView construction/destruction

CFontDemoView::CFontDemoView() noexcept
{
	// TODO: add construction code here

}

CFontDemoView::~CFontDemoView()
{
}

BOOL CFontDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CFontDemoView drawing

void CFontDemoView::OnDraw(CDC* /*pDC*/)
{
	CFontDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CFontDemoView diagnostics

#ifdef _DEBUG
void CFontDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CFontDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFontDemoDoc* CFontDemoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFontDemoDoc)));
	return (CFontDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CFontDemoView message handlers
