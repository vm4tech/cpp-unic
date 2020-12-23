// FindTodo.cpp : implementation file
//

#include "pch.h"
#include "TodoList.h"
#include "FindTodo.h"
#include "afxdialogex.h"


// FindTodo dialog

IMPLEMENT_DYNAMIC(FindTodo, CDialogEx)

FindTodo::FindTodo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FIND_TODO_DIALOG, pParent)
	, m_FindTodoText(_T(""))
{

}

FindTodo::~FindTodo()
{
}

void FindTodo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FIND_TODO, m_FindTodoText);
}


BEGIN_MESSAGE_MAP(FindTodo, CDialogEx)
END_MESSAGE_MAP()


// FindTodo message handlers
