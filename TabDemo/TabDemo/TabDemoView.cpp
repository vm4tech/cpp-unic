
// TabDemoView.cpp : implementation of the CTabDemoView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TabDemo.h"
#endif

#include "TabDemoDoc.h"
#include "TabDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTabDemoView

IMPLEMENT_DYNCREATE(CTabDemoView, CView)

BEGIN_MESSAGE_MAP(CTabDemoView, CView)
END_MESSAGE_MAP()

// CTabDemoView construction/destruction

CTabDemoView::CTabDemoView() noexcept
{
	// TODO: add construction code here

}

CTabDemoView::~CTabDemoView()
{
}

BOOL CTabDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTabDemoView drawing

void CTabDemoView::OnDraw(CDC* /*pDC*/)
{
	CTabDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CTabDemoView diagnostics

#ifdef _DEBUG
void CTabDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CTabDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTabDemoDoc* CTabDemoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTabDemoDoc)));
	return (CTabDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CTabDemoView message handlers
