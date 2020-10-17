
// WinGreetView.cpp: реализация класса CWinGreetView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "WinGreet.h"
#endif

#include "WinGreetDoc.h"
#include "WinGreetView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWinGreetView

IMPLEMENT_DYNCREATE(CWinGreetView, CView)

BEGIN_MESSAGE_MAP(CWinGreetView, CView)
	// Стандартные команды печати
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// Создание или уничтожение CWinGreetView

CWinGreetView::CWinGreetView() noexcept
{
	// TODO: добавьте код создания

}

CWinGreetView::~CWinGreetView()
{
}

BOOL CWinGreetView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Рисование CWinGreetView

void CWinGreetView::OnDraw(CDC* pDC)
{
	CWinGreetDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	RECT ClientRect;
	GetClientRect(&ClientRect);
	pDC->DrawText(
		CString(pDoc->GetMessage()),  // получить строку
		&ClientRect,
		DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}


// Печать CWinGreetView

BOOL CWinGreetView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// подготовка по умолчанию
	return DoPreparePrinting(pInfo);
}

void CWinGreetView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте дополнительную инициализацию перед печатью
}

void CWinGreetView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте очистку после печати
}


// Диагностика CWinGreetView

#ifdef _DEBUG
void CWinGreetView::AssertValid() const
{
	CView::AssertValid();
}

void CWinGreetView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWinGreetDoc* CWinGreetView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinGreetDoc)));
	return (CWinGreetDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CWinGreetView
