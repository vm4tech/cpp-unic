
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
#include "CFormat.h"

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

void CFontDemoView::OnDraw(CDC* pDC)
{
	CFontDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	RECT ClientRect;
	CFont Font;
	LOGFONT LF;
	int LineHeight;
	CFont* PtrOldFont;
	int X, Y;
	// заполнение структуры LF свойствами
	// стандартного системного шрифта:
	CFont TempFont;
	if (pDoc->m_Pitch == PITCH_VARIABLE)
		TempFont.CreateStockObject(SYSTEM_FONT);
	else
		TempFont.CreateStockObject(SYSTEM_FIXED_FONT);
	TempFont.GetObject(sizeof(LOGFONT), &LF);
	// инициализируем поля lfWeight, lfItalic и lfUnderline: 
	if (pDoc->m_Bold)
		LF.lfWeight = FW_BOLD;
	if (pDoc->m_Italic)
		LF.lfItalic = 1;
	if (pDoc->m_Underline)
		LF.lfUnderline = 1;

	// создание и выбор шрифта:
	Font.CreateFontIndirect(&LF);
	PtrOldFont = pDC->SelectObject(&Font);
	// задаем выравнивание:
	GetClientRect(&ClientRect);
	switch (pDoc->m_Justify)
	{
	case JUSTIFY_LEFT:
		pDC->SetTextAlign(TA_LEFT);
		X = ClientRect.left + 5;
		break;
	case JUSTIFY_CENTER:
		pDC->SetTextAlign(TA_CENTER);
		X = (ClientRect.left + ClientRect.right) / 2;
		break;
	case JUSTIFY_RIGHT:
		pDC->SetTextAlign(TA_RIGHT);
		X = ClientRect.right - 5;
		break;
	}
	// установка цвета текста и режима фона:
	pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	pDC->SetBkMode(TRANSPARENT);

	// вывод строк текста:
	LineHeight = LF.lfHeight * pDoc->m_Spacing;
	Y = 5;
	pDC->TextOut(X, Y,
		_T("This is the first line of sample text."));
	Y += LineHeight;
	pDC->TextOut(X, Y,
		_T("This is the second line of sample text."));
	Y += LineHeight;
	pDC->TextOut(X, Y,
		_T("This is the third line of sample text."));

	// отмена выбора шрифта:
	pDC->SelectObject(PtrOldFont);

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
