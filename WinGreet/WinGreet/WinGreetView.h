
// WinGreetView.h: интерфейс класса CWinGreetView
//

#pragma once


class CWinGreetView : public CView
{
protected: // создать только из сериализации
	CWinGreetView() noexcept;
	DECLARE_DYNCREATE(CWinGreetView)

// Атрибуты
public:
	CWinGreetDoc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Реализация
public:
	virtual ~CWinGreetView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // версия отладки в WinGreetView.cpp
inline CWinGreetDoc* CWinGreetView::GetDocument() const
   { return reinterpret_cast<CWinGreetDoc*>(m_pDocument); }
#endif

