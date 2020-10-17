
// MiniDrawView.h: интерфейс класса CMiniDrawView
//

#pragma once


class CMiniDrawView : public CView
{
protected: // создать только из сериализации
	CMiniDrawView() noexcept;
	DECLARE_DYNCREATE(CMiniDrawView)

// Атрибуты
public:
	CMiniDrawDoc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Реализация
public:
	virtual ~CMiniDrawView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // версия отладки в MiniDrawView.cpp
inline CMiniDrawDoc* CMiniDrawView::GetDocument() const
   { return reinterpret_cast<CMiniDrawDoc*>(m_pDocument); }
#endif

