
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
// Ќабор констант дл€ построени€ изображени€:
#define CIMAX 1.2
#define CIMIN -1.2
#define CRMAX 1.0
#define CRMIN -2.0
#define NMAX 128

// √лобальные переменные дл€ св€зи между потоками:
int ColMax;
int RowMax;
BOOL StopDraw;


// CMandelMTView

IMPLEMENT_DYNCREATE(CMandelMTView, CView)

BEGIN_MESSAGE_MAP(CMandelMTView, CView)
	ON_WM_SIZE()
END_MESSAGE_MAP()

UINT DrawFractal(LPVOID PHWndView)
{
	CClientDC ClientDC(CWnd::FromHandle(*(HWND*)PHWndView));
	int Col, Row;
	static DWORD ColorTable[6] =
	{ 0x0000ff,  // красный
	 0x00ff00,  // зеленый
	 0xff0000,  // синий
	 0x00ffff,  // желтый
	 0xffff00,  // бирюзовый
	 0xff00ff }; // сиреневый
	int ColorVal;
	float CI, CR = (float)CRMIN;
	float DCI = (float)((CIMAX - CIMIN) / (RowMax - 1));
	float DCR = (float)((CRMAX - CRMIN) / (ColMax - 1));
	float I, ISqr;
	float R, RSqr;
	for (Col = 0; Col < ColMax; ++Col)
	{
		if (StopDraw)
			break;
		CI = (float)CIMAX;
		for (Row = 0; Row < RowMax; ++Row)
		{
			R = I = RSqr = ISqr = (float)0.0;
			ColorVal = 0;
			while (ColorVal < NMAX && RSqr + ISqr < 4)
			{
				++ColorVal;
				RSqr = R * R;
				ISqr = I * I;
				I *= R;
				I += I + CI;
				R = RSqr - ISqr + CR;
			}
			ClientDC.SetPixelV(Col, Row, ColorTable[ColorVal % 6]);
			CI -= DCI;
		}
		CR += DCR;
	}
	return (0);
}


// CMandelMTView construction/destruction


CMandelMTView::CMandelMTView() noexcept
{
	// TODO: add construction code here
	m_PDrawThread = 0;
}

CMandelMTView::~CMandelMTView()
{
	delete m_PDrawThread;
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
	if (m_PDrawThread)
	{
		StopDraw = TRUE;
		m_PDrawThread->ResumeThread();
		::WaitForSingleObject
		(m_PDrawThread->m_hThread,  // ждать завершени€
			INFINITE);                 // столько, сколько надо
		delete m_PDrawThread;
	}
	m_PDrawThread = AfxBeginThread
	(DrawFractal,
		&m_hWnd,
		THREAD_PRIORITY_BELOW_NORMAL,
		0,
		CREATE_SUSPENDED);
	m_PDrawThread->m_bAutoDelete = FALSE;
	StopDraw = FALSE;
	m_PDrawThread->ResumeThread();

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


void CMandelMTView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (cx <= 1 || cy <= 1)
		return;
	ColMax = cx;
	RowMax = cy;

}
