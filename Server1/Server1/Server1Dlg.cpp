
// Server1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server1.h"
#include "Server1Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServer1Dlg dialog
struct ThreadArgs TA;


CServer1Dlg::CServer1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServer1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTBOX, m_ListBox);
}

BEGIN_MESSAGE_MAP(CServer1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CServer1Dlg::OnBnClickedStart)
	ON_BN_CLICKED(IDCANCEL, &CServer1Dlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CServer1Dlg message handlers

BOOL CServer1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	GetDlgItem(IDC_MAILSLOT)->SetWindowText(
		(LPTSTR)"Myslot");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServer1Dlg::OnPaint()
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
HCURSOR CServer1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServer1Dlg::OnBnClickedStart()
{
	// TODO: Add your control notification handler code here
	char ServerName[120], Mailslot[81];
	char Str[200];

	// Создание почтового ящика
	GetDlgItem(IDC_MAILSLOT)->GetWindowText(Mailslot, 80);
	sprintf_s(ServerName, sizeof(ServerName),
		"\\\\.\\Mailslot\\%s", Mailslot);
	if ((m_Mailslot = CreateMailslot(ServerName, 0,
		MAILSLOT_WAIT_FOREVER, NULL)) ==
		INVALID_HANDLE_VALUE)
	{
		sprintf_s(Str, sizeof(Str),
			"Failed to create a mailslot %d", GetLastError());
		((CListBox*)GetDlgItem(IDC_LISTBOX))->
			AddString((LPTSTR)Str);
		return;
	}
	else
	{
		sprintf_s(Str, sizeof(Str),
			"Serving mailslot: %s", ServerName);
		GetDlgItem(IDC_CURRENT_CONN)->SetWindowText(
			(LPTSTR)Str);

		sprintf_s(Str, sizeof(Str),
			"Mailslot %s was successfully created",
			ServerName);
		((CListBox*)GetDlgItem(IDC_LISTBOX))->
			AddString((LPTSTR)Str);
	}

	TA.ms = m_Mailslot;
	TA.hWnd = m_ListBox.m_hWnd;

	m_pMailslotThread = AfxBeginThread(ServeMailslot, &TA);

	GetDlgItem(IDC_START)->EnableWindow(false);
	GetDlgItem(IDC_MAILSLOT)->EnableWindow(false);

}


void CServer1Dlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	::TerminateThread(m_pMailslotThread, 0);
	CloseHandle(m_Mailslot);
	CDialogEx::OnCancel();
}
UINT ServeMailslot(LPVOID lpParameter)
{
	struct ThreadArgs* pTA =
		(struct ThreadArgs*)lpParameter;
	char buffer[256];
	DWORD NumberOfBytesRead;
	CListBox* pLB =
		(CListBox*)(CListBox::FromHandle(pTA->hWnd));

	pLB->AddString("Reading thread started");

	// Бесконечное чтение данных
	while (ReadFile(pTA->ms, buffer, 256, &NumberOfBytesRead,
		NULL) != 0)
	{
		buffer[NumberOfBytesRead] = 0;
		pLB->AddString(buffer);
	}

	pLB->AddString("Reading thread finished");

	return 0;
}

