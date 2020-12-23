
// TodoListView.cpp : implementation of the CTodoListView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TodoList.h"
#endif

#include "TodoListDoc.h"
#include "TodoListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "CListBoxCustom.h"


// CTodoListView

IMPLEMENT_DYNCREATE(CTodoListView, CFormView)

BEGIN_MESSAGE_MAP(CTodoListView, CFormView)
	ON_BN_CLICKED(IDC_ADD, &CTodoListView::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REMOVE, &CTodoListView::OnBnClickedRemove)
	ON_BN_CLICKED(IDC_GET, &CTodoListView::OnBnClickedGet)
	ON_BN_CLICKED(IDC_ROWS_SQUARED, &CTodoListView::OnBnClickedRowsSquared)
	ON_BN_CLICKED(IDC_ALL_ACTIONS, &CTodoListView::OnBnClickedAllActions)
END_MESSAGE_MAP()

// CTodoListView construction/destruction

CTodoListView::CTodoListView() noexcept
	: CFormView(IDD_TODOLIST_FORM)
	, m_ctlListBox()
	, m_txtListbox(_T(""))
	, m_ListBoxCustom()


{	
	
	// TODO: add construction code here

}

CTodoListView::~CTodoListView()
{
}

void CTodoListView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//DDX_Check(pDX, IDC_MYLISTBOX, m_ctlListBox);
	DDX_Control(pDX, IDC_MYLISTBOX, m_ctlListBox);
	DDX_Text(pDX, IDC_LISTBOXTEXT, m_txtListbox);
}

BOOL CTodoListView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CTodoListView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CTodoListView diagnostics

#ifdef _DEBUG
void CTodoListView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTodoListView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CTodoListDoc* CTodoListView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTodoListDoc)));
	return (CTodoListDoc*)m_pDocument;
}
#endif //_DEBUG


// CTodoListView message handlers


void CTodoListView::OnBnClickedAdd()
{
	// TODO: Add your control notification handler code here
	CString strText;
	CListBoxCustom count;
	UpdateData();
	strText = m_txtListbox;
	UpdateData(FALSE);
	
	m_ctlListBox.AddString(strText); //«десь добавл€ютс€ актуальные данные
	
}


void CTodoListView::OnBnClickedRemove()
{
	// TODO: Add your control notification handler code here
	int index;
	CListBoxCustom count;
	CString strText;
	index = m_ctlListBox.GetCurSel();
	
	m_ctlListBox.DeleteString(index);
}


void CTodoListView::OnBnClickedGet()
{
	// TODO: Add your control notification handler code here
	int index;
	CListBoxCustom count;
	CString strText;
	index = m_ctlListBox.GetCurSel();
	m_ctlListBox.GetText(index, strText);

	MessageBox(strText);
}


void CTodoListView::OnBnClickedRowsSquared()
{
	// TODO: Add your control notification handler code here
	int rows;
	CString strText;
	CListBoxCustom sqRows;
	
	rows = m_ctlListBox.GetCount();

	int squareRows = sqRows.RowsSquared(rows);
	strText.Format(_T("%d"), squareRows);
	MessageBox(strText);
}


void CTodoListView::OnBnClickedAllActions()
{
	// TODO: Add your control notification handler code here
	CListBoxCustom allActions;
	CString str;
	str.Format(_T("%d"));
	MessageBox(str);
}
