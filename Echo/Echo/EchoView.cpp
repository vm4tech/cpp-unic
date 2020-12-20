
// EchoView.cpp : implementation of the CEchoView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Echo.h"
#endif

#include "EchoDoc.h"
#include "EchoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEchoView

IMPLEMENT_DYNCREATE(CEchoView, CView)

BEGIN_MESSAGE_MAP(CEchoView, CView)
	ON_WM_CHAR()
	ON_COMMAND(ID_EDIT_CLEAR, &CEchoView::OnEditClear)
END_MESSAGE_MAP()

// CEchoView construction/destruction

CEchoView::CEchoView() noexcept
{
	// TODO: add construction code here

}

CEchoView::~CEchoView()
{
}

BOOL CEchoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CEchoView drawing

void CEchoView::OnDraw(CDC* pDC)
{
	CEchoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(0, 0, pDoc->m_TextLine);

}


// CEchoView diagnostics

#ifdef _DEBUG
void CEchoView::AssertValid() const
{
	CView::AssertValid();
}

void CEchoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEchoDoc* CEchoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEchoDoc)));
	return (CEchoDoc*)m_pDocument;
}
#endif //_DEBUG


// CEchoView message handlers


void CEchoView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar < 32)
	{
		::MessageBeep(MB_OK); // генерация стандартного звука
		return;
	}

	CEchoDoc* PDoc = GetDocument();
	PDoc->m_TextLine += (wchar_t)nChar;

	CClientDC ClientDC(this);

	ClientDC.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	ClientDC.SetBkMode(TRANSPARENT);
	ClientDC.TextOut(0, 0, PDoc->m_TextLine);

	CView::OnChar(nChar, nRepCnt, nFlags);
}


void CEchoView::OnEditClear()
{
	// TODO: Add your command handler code here
	CEchoDoc* PDoc = GetDocument();
	PDoc->m_TextLine.Empty();
	PDoc->UpdateAllViews(NULL);

}
