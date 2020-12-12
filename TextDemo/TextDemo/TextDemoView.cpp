
// TextDemoView.cpp : implementation of the CTextDemoView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TextDemo.h"
#endif

#include "TextDemoDoc.h"
#include "TextDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTextDemoView

IMPLEMENT_DYNCREATE(CTextDemoView, CScrollView)

BEGIN_MESSAGE_MAP(CTextDemoView, CScrollView)
END_MESSAGE_MAP()

// CTextDemoView construction/destruction

CTextDemoView::CTextDemoView() noexcept
{
	// TODO: add construction code here

}

CTextDemoView::~CTextDemoView()
{
}

BOOL CTextDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CTextDemoView drawing

void CTextDemoView::OnDraw(CDC* /*pDC*/)
{
	CTextDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CTextDemoView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CTextDemoView diagnostics

#ifdef _DEBUG
void CTextDemoView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTextDemoView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTextDemoDoc* CTextDemoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTextDemoDoc)));
	return (CTextDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CTextDemoView message handlers
