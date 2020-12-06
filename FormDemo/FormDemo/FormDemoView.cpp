
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
	ON_BN_CLICKED(IDC_RED, &CFormDemoView::OnClickedRed)
	ON_COMMAND(ID_OPTIONS_LIGHT, &CFormDemoView::OnOptionsLight)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_LIGHT, &CFormDemoView::OnUpdateOptionsLight)
	ON_COMMAND(ID_OPTIONS_DARK, &CFormDemoView::OnOptionsDark)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_DARK, &CFormDemoView::OnUpdateOptionsDark)
	ON_BN_CLICKED(IDC_GREEN, &CFormDemoView::OnClickedGreen)
	ON_BN_CLICKED(IDC_BLUE, &CFormDemoView::OnClickedBlue)
END_MESSAGE_MAP()

// CFormDemoView construction/destruction

CFormDemoView::CFormDemoView() noexcept
	: CFormView(IDD_FORMDEMO_FORM)
	, m_Blue(FALSE)
	, m_Green(FALSE)
	, m_Red(FALSE)

{
	// TODO: add construction code here
	m_Intensity = INT_LIGHT;
}

CFormDemoView::~CFormDemoView()
{
}

void CFormDemoView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_BLUE, m_Blue);
	DDX_Check(pDX, IDC_GREEN, m_Green);
	DDX_Check(pDX, IDC_RED, m_Red);

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
	GetDlgItem(IDC_SAMPLE)->GetWindowRect(&m_RectSample);
	ScreenToClient(&m_RectSample);
	int Border = (m_RectSample.right - m_RectSample.left) / 8;
	m_RectSample.InflateRect(-Border, -Border);

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



void CFormDemoView::OnClickedRed()
{
	// TODO: Add your control notification handler code here
	m_Red = IsDlgButtonChecked(IDC_RED);
	CClientDC ClientDC(this);
	OnPrepareDC(&ClientDC);
	CRect Rect = m_RectSample;
	ClientDC.LPtoDP(&Rect);
	InvalidateRect(&Rect);
	UpdateWindow();
}


void CFormDemoView::OnOptionsLight()
{
	// TODO: Add your command handler code here
	m_Intensity = INT_LIGHT;
	CClientDC ClientDC(this);
	OnPrepareDC(&ClientDC);
	CRect Rect = m_RectSample;
	ClientDC.LPtoDP(&Rect);
	InvalidateRect(&Rect);
	UpdateWindow();

}


void CFormDemoView::OnUpdateOptionsLight(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(m_Intensity == INT_LIGHT);
}


void CFormDemoView::OnOptionsDark()
{
	// TODO: Add your command handler code here
	m_Intensity = INT_DARK;
	CClientDC ClientDC(this);
	OnPrepareDC(&ClientDC);
	CRect Rect = m_RectSample;
	ClientDC.LPtoDP(&Rect);
	InvalidateRect(&Rect);
	UpdateWindow();
}


void CFormDemoView::OnUpdateOptionsDark(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(m_Intensity == INT_DARK);
}


void CFormDemoView::OnClickedGreen()
{
	// TODO: Add your control notification handler code here
	m_Green = IsDlgButtonChecked(IDC_GREEN);
	CClientDC ClientDC(this);
	OnPrepareDC(&ClientDC);
	CRect Rect = m_RectSample;
	ClientDC.LPtoDP(&Rect);
	InvalidateRect(&Rect);
	UpdateWindow();
}


void CFormDemoView::OnClickedBlue()
{
	// TODO: Add your control notification handler code here
	m_Blue = IsDlgButtonChecked(IDC_BLUE);
	CClientDC ClientDC(this);
	OnPrepareDC(&ClientDC);
	CRect Rect = m_RectSample;
	ClientDC.LPtoDP(&Rect);
	InvalidateRect(&Rect);
	UpdateWindow();
}


void CFormDemoView::OnDraw(CDC* pDC)
{
	// TODO: Add your specialized code here and/or call the base class
	COLORREF Color = RGB
	(m_Red ? (m_Intensity == INT_DARK ? 128 : 255) : 0,
		m_Green ? (m_Intensity == INT_DARK ? 128 : 255) : 0,
		m_Blue ? (m_Intensity == INT_DARK ? 128 : 255) : 0);
	CBrush Brush(Color);
	pDC->FillRect(&m_RectSample, &Brush);

}
