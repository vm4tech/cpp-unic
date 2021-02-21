
// MandelView.h : interface of the CMandelView class
//

#pragma once


class CMandelView : public CView
{

private:
	int m_Col;	// Номер следующего столбца пикселей
	int m_ColMax;	// Последний столбец
	float m_CR;	// Используются
	float m_DCI;	// при масштабировании
	float m_DCR;	// изображения
	int m_RowMax;	// Последняя строка

public:
	void DrawCol(); // Рисование очередного столбца

protected: // create from serialization only
	CMandelView() noexcept;
	DECLARE_DYNCREATE(CMandelView)

// Attributes
public:
	CMandelDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CMandelView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // debug version in MandelView.cpp
inline CMandelDoc* CMandelView::GetDocument() const
   { return reinterpret_cast<CMandelDoc*>(m_pDocument); }
#endif

