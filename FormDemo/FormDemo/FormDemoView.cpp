
// FormDemoView.cpp : implementation of the CFormDemoView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "FormDemo.h"
#endif

#include "FormDemoDoc.h"
#include "FormDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFormDemoView

IMPLEMENT_DYNCREATE(CFormDemoView, CFormView)

BEGIN_MESSAGE_MAP(CFormDemoView, CFormView)
END_MESSAGE_MAP()

// CFormDemoView construction/destruction

CFormDemoView::CFormDemoView() noexcept
	: CFormView(IDD_FORMDEMO_FORM)
{
	// TODO: add construction code here

}

CFormDemoView::~CFormDemoView()
{
}

void CFormDemoView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CFormDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CFormDemoView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CFormDemoView diagnostics

#ifdef _DEBUG
void CFormDemoView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormDemoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CFormDemoDoc* CFormDemoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFormDemoDoc)));
	return (CFormDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CFormDemoView message handlers
