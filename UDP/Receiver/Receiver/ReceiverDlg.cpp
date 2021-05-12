
// ReceiverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Receiver.h"
#include "ReceiverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CReceiverDlg dialog
#define DEFAULT_PORT				5150
#define DEFAULT_COUNT			  25
#define DEFAULT_BUFFER_LENGTH	4096

int		iPort = DEFAULT_PORT; 	// Номер порта
DWORD	dwCount = DEFAULT_COUNT,	// Количество дейтаграмм
dwLength = DEFAULT_BUFFER_LENGTH;  // Размер буфера
BOOL 	bInterface = FALSE;	// Конкретный отправитель?
char		szInterface[32];		// Адрес отправителя 
HWND		hWnd_LB;				// Для вывода в другом потоке

UINT ReceiveThread(PVOID lpParam);



CReceiverDlg::CReceiverDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RECEIVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CReceiverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTBOX, m_ListBox);
}

BEGIN_MESSAGE_MAP(CReceiverDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ALL_IP, &CReceiverDlg::OnBnClickedAllIp)
	ON_BN_CLICKED(IDC_RECEIVE, &CReceiverDlg::OnBnClickedReceive)
END_MESSAGE_MAP()


// CReceiverDlg message handlers

BOOL CReceiverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	char Str[128];

	sprintf_s(Str, sizeof(Str), "%d", iPort);
	GetDlgItem(IDC_PORT)->SetWindowText(Str);

	sprintf_s(Str, sizeof(Str), "%d", dwCount);
	GetDlgItem(IDC_COUNT)->SetWindowText(Str);

	if (bInterface)
		((CButton*)GetDlgItem(IDC_ALL_IP))->SetCheck(0);
	else
		((CButton*)GetDlgItem(IDC_ALL_IP))->SetCheck(1);

	SetStarted(false);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CReceiverDlg::OnPaint()
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
HCURSOR CReceiverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CReceiverDlg::SetStarted(bool IsStarted)
{
	m_IsStarted = IsStarted;

	GetDlgItem(IDC_ALL_IP)->EnableWindow(!IsStarted);
	GetDlgItem(IDC_SENDER)->EnableWindow(bInterface);
	GetDlgItem(IDC_PORT)->EnableWindow(!IsStarted);
	GetDlgItem(IDC_COUNT)->EnableWindow(!IsStarted);
	GetDlgItem(IDC_RECEIVE)->EnableWindow(!IsStarted);
}


void CReceiverDlg::OnBnClickedAllIp()
{
	// TODO: Add your control notification handler code here
	if (((CButton*)GetDlgItem(IDC_ALL_IP))->GetCheck() == 1)
		bInterface = FALSE;
	else
		bInterface = TRUE;

	GetDlgItem(IDC_SENDER)->EnableWindow(bInterface);

}


void CReceiverDlg::OnBnClickedReceive()
{
	// TODO: Add your control notification handler code here
	char Str[81];

	hWnd_LB = m_ListBox.m_hWnd;	// Для потока
	GetDlgItem(IDC_PORT)->GetWindowText(Str, sizeof(Str));
	iPort = atoi(Str);
	if (iPort <= 0 || iPort >= 0x10000)
	{
		AfxMessageBox("Incorrect Port number");
		return;
	}

	GetDlgItem(IDC_COUNT)->GetWindowText(Str, sizeof(Str));
	dwCount = atoi(Str);
	if (dwCount <= 0)
	{
		AfxMessageBox("Incorrect number of datagrams");
		return;
	}

	GetDlgItem(IDC_SENDER)->
		GetWindowText(szInterface, sizeof(szInterface));

	AfxBeginThread(ReceiveThread, NULL);

	SetStarted(true);

}
UINT ReceiveThread(PVOID lpParam)
{

	WSADATA		wsd;
	SOCKET		s;
	char* recvbuf = NULL;
	int			ret,
		i;
	DWORD		dwSenderSize;
	SOCKADDR_IN	sender,
		local;

	char			Str[200];
	CListBox* pLB =
		(CListBox*)(CListBox::FromHandle(hWnd_LB));

	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		sprintf_s(Str, sizeof(Str), "WSAStartup failed!");
		pLB->AddString((LPTSTR)Str);
		return 1;
	}
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == INVALID_SOCKET)
	{
		sprintf_s(Str, sizeof(Str), "socket() failed; %d",
			WSAGetLastError());
		pLB->AddString((LPTSTR)Str);
		return 1;
	}
	local.sin_family = AF_INET;
	local.sin_port = htons((short)iPort);
	if (bInterface)
		local.sin_addr.s_addr = inet_addr(szInterface);
	else
		local.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(s, (SOCKADDR*)&local, sizeof(local)) ==
		SOCKET_ERROR)
	{
		sprintf_s(Str, sizeof(Str),
			"bind() failed: %d", WSAGetLastError());
		pLB->AddString((LPTSTR)Str);
		return 1;
	}
	recvbuf = (char*)GlobalAlloc(GMEM_FIXED, dwLength);
	if (!recvbuf)
	{
		sprintf_s(Str, sizeof(Str),
			"GlobalAlloc() failed: %d", GetLastError());
		pLB->AddString((LPTSTR)Str);
		return 1;
	}
	for (i = 0; i < dwCount; i++)
	{
		dwSenderSize = sizeof(sender);
		ret = recvfrom(s, recvbuf, dwLength, 0,
			(SOCKADDR*)&sender, (int*)&dwSenderSize);
		if (ret == SOCKET_ERROR)
		{
			sprintf_s(Str, sizeof(Str),
				"recvfrom() failed; %d", WSAGetLastError());
			pLB->AddString((LPTSTR)Str);
			break;
		}
		else if (ret == 0)
			break;
		else
		{
			recvbuf[ret] = '\0';
			sprintf_s(Str, sizeof(Str),
				"[%s] sent me: '%s'",
				inet_ntoa(sender.sin_addr), recvbuf);
			pLB->AddString((LPTSTR)Str);
		}
	}

	sprintf_s(Str, sizeof(Str),
		"%d datagrams was received. Socket closed",
		dwCount);
	pLB->AddString((LPTSTR)Str);

	closesocket(s);

	GlobalFree(recvbuf);
	WSACleanup();

	return 0;
}
