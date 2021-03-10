
// UsingDLLDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "UsingDLL.h"
#include "UsingDLLDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUsingDLLDlg dialog



CUsingDLLDlg::CUsingDLLDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_USINGDLL_DIALOG, pParent)
	, m_dInput(0)
	, m_dOutput(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUsingDLLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INPUT, m_dInput);
	DDX_Text(pDX, IDC_OUTPUT, m_dOutput);
}

BEGIN_MESSAGE_MAP(CUsingDLLDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_COMPUTE, &CUsingDLLDlg::OnClickedCompute)
END_MESSAGE_MAP()


// CUsingDLLDlg message handlers

BOOL CUsingDLLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUsingDLLDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUsingDLLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUsingDLLDlg::OnClickedCompute()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE); 	// Получить данные из диалогового окна
	m_dOutput = ExampleSquareRoot(m_dInput);
	UpdateData(FALSE);	// Обновить данные в диалоговом окне 

}
