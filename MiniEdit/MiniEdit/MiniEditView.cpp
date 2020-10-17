
// MiniEditView.cpp : implementation of the CMiniEditView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MiniEdit.h"
#endif

#include "MiniEditDoc.h"
#include "MiniEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMiniEditView

IMPLEMENT_DYNCREATE(CMiniEditView, CEditView)

BEGIN_MESSAGE_MAP(CMiniEditView, CEditView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMiniEditView construction/destruction

CMiniEditView::CMiniEditView() noexcept
{
	// TODO: add construction code here

}

CMiniEditView::~CMiniEditView()
{
}

BOOL CMiniEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}


// CMiniEditView printing

BOOL CMiniEditView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CMiniEditView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CMiniEditView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}


// CMiniEditView diagnostics

#ifdef _DEBUG
void CMiniEditView::AssertValid() const
{
	CEditView::AssertValid();
}

void CMiniEditView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CMiniEditDoc* CMiniEditView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMiniEditDoc)));
	return (CMiniEditDoc*)m_pDocument;
}
#endif //_DEBUG


// CMiniEditView message handlers
