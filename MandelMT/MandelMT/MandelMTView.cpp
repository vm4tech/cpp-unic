
// MandelMTView.cpp : implementation of the CMandelMTView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MandelMT.h"
#endif

#include "MandelMTDoc.h"
#include "MandelMTView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMandelMTView

IMPLEMENT_DYNCREATE(CMandelMTView, CView)

BEGIN_MESSAGE_MAP(CMandelMTView, CView)
END_MESSAGE_MAP()

// CMandelMTView construction/destruction

CMandelMTView::CMandelMTView() noexcept
{
	// TODO: add construction code here

}

CMandelMTView::~CMandelMTView()
{
}

BOOL CMandelMTView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMandelMTView drawing

void CMandelMTView::OnDraw(CDC* /*pDC*/)
{
	CMandelMTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CMandelMTView diagnostics

#ifdef _DEBUG
void CMandelMTView::AssertValid() const
{
	CView::AssertValid();
}

void CMandelMTView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMandelMTDoc* CMandelMTView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMandelMTDoc)));
	return (CMandelMTDoc*)m_pDocument;
}
#endif //_DEBUG


// CMandelMTView message handlers
