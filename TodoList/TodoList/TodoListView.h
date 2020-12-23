
// TodoListView.h : interface of the CTodoListView class
//

#pragma once
#include "CListBoxCustom.h"
#include "FindTodo.h"

class CTodoListView : public CFormView
{

	

protected: // create from serialization only
	CTodoListView() noexcept;
	DECLARE_DYNCREATE(CTodoListView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_TODOLIST_FORM };
#endif

// Attributes
public:
	CTodoListDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CTodoListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedRemove();
	afx_msg void OnBnClickedGet();
	afx_msg void OnBnClickedRowsSquared();
	CListBox m_ctlListBox;
	CString m_txtListbox;
	CListBoxCustom m_ListBoxCustom;
	FindTodo m_findTODO;
	
	afx_msg void OnBnClickedAllActions();
	afx_msg void OnClickedAllDelete();
	afx_msg void OnClickedFindString();
};

#ifndef _DEBUG  // debug version in TodoListView.cpp
inline CTodoListDoc* CTodoListView::GetDocument() const
   { return reinterpret_cast<CTodoListDoc*>(m_pDocument); }
#endif

