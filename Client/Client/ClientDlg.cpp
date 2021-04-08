
// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientDlg dialog



CClientDlg::CClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, &CClientDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_SEND, &CClientDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDCANCEL, &CClientDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	GetDlgItem(IDC_SERVER)->SetWindowText((LPTSTR)".");
	GetDlgItem(IDC_MAILSLOT)->SetWindowText(
		(LPTSTR)"Myslot");
	GetDlgItem(IDC_STRING)->
		SetWindowText((LPTSTR)"This is a test string");
	SetConnected(false);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint()
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CClientDlg::SetConnected(bool IsConnected)
{
	m_IsConnected = IsConnected;

	GetDlgItem(IDC_SEND)->EnableWindow(IsConnected);
	GetDlgItem(IDC_STRING)->EnableWindow(IsConnected);
	GetDlgItem(IDC_SERVER)->EnableWindow(!IsConnected);
	GetDlgItem(IDC_MAILSLOT)->EnableWindow(!IsConnected);

	if (IsConnected)
	{
		char Str[180], Server[81], Mailslot[81];
		GetDlgItem(IDC_SERVER)->GetWindowText(
			(LPTSTR)Server, 80);
		GetDlgItem(IDC_MAILSLOT)->
			GetWindowText(Mailslot, 80);
		sprintf_s(Str, sizeof(Str),
			"\\\\%s\\Mailslot\\%s",
			Server, Mailslot);
		GetDlgItem(IDC_CURRENT_CONN)->SetWindowText(
			(LPTSTR)Str);
		GetDlgItem(IDC_CONNECT)->SetWindowText(
			(LPTSTR)"Reconnect");
	}
	else
	{
		GetDlgItem(IDC_CURRENT_CONN)->
			SetWindowText((LPTSTR)"No connection");
		GetDlgItem(IDC_CONNECT)->SetWindowText(
			(LPTSTR)"Connect");
	}
}


void CClientDlg::OnBnClickedConnect()
{
	// TODO: Add your control notification handler code here
	if (m_IsConnected)
	{
		CloseHandle(m_Mailslot);
		SetConnected(false);
		return;
	}

	char ServerName[180], Server[81], Mailslot[81];
	char Str[200];

	GetDlgItem(IDC_SERVER)->GetWindowText(Server, 80);
	GetDlgItem(IDC_MAILSLOT)->GetWindowText(Mailslot, 80);
	sprintf_s(ServerName, sizeof(ServerName),
		"\\\\%s\\Mailslot\\%s",
		Server, Mailslot);

	if ((m_Mailslot = CreateFile(ServerName, GENERIC_WRITE,
		FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL)) ==
		INVALID_HANDLE_VALUE)
	{
		sprintf_s(Str, sizeof(Str),
			"CreateFile failed with error %d",
			GetLastError());
		((CListBox*)GetDlgItem(IDC_LISTBOX))->
			AddString((LPTSTR)Str);
	}
	else
	{
		sprintf_s(Str, sizeof(Str),
			"Successfully opened %s", ServerName);
		((CListBox*)GetDlgItem(IDC_LISTBOX))->
			AddString((LPTSTR)Str);
		SetConnected(true);
	}

}


void CClientDlg::OnBnClickedSend()
{
	// TODO: Add your control notification handler code here

	char Str[121];
	DWORD BytesWritten;

	GetDlgItem(IDC_STRING)->GetWindowText(Str, 120);

	if (WriteFile(m_Mailslot, Str, strlen(Str),
		&BytesWritten, NULL) == 0)
	{
		sprintf_s(Str, sizeof(Str),
			"WriteFile failed with error %d",
			GetLastError());
		((CListBox*)GetDlgItem(IDC_LISTBOX))->
			AddString((LPTSTR)Str);
	}
	else
	{
		sprintf_s(Str, sizeof(Str),
			"Wrote %d bytes", BytesWritten);
		((CListBox*)GetDlgItem(IDC_LISTBOX))->
			AddString((LPTSTR)Str);
	}

}


void CClientDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
		CloseHandle(m_Mailslot);
	CDialogEx::OnCancel();
}
