
// MandelView.cpp : implementation of the CMandelView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Mandel.h"
#endif

#include "MandelDoc.h"
#include "MandelView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMandelView

IMPLEMENT_DYNCREATE(CMandelView, CView)

BEGIN_MESSAGE_MAP(CMandelView, CView)
END_MESSAGE_MAP()

// CMandelView construction/destruction

CMandelView::CMandelView() noexcept
{
	// TODO: add construction code here

}

CMandelView::~CMandelView()
{
}

BOOL CMandelView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMandelView drawing

void CMandelView::OnDraw(CDC* /*pDC*/)
{
	CMandelDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CMandelView diagnostics

#ifdef _DEBUG
void CMandelView::AssertValid() const
{
	CView::AssertValid();
}

void CMandelView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMandelDoc* CMandelView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMandelDoc)));
	return (CMandelDoc*)m_pDocument;
}
#endif //_DEBUG


// CMandelView message handlers
