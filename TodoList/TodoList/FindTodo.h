#pragma once


// FindTodo dialog

class FindTodo : public CDialogEx
{
	DECLARE_DYNAMIC(FindTodo)

public:
	FindTodo(CWnd* pParent = nullptr);   // standard constructor
	virtual ~FindTodo();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FIND_TODO_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_FindTodoText;
};
