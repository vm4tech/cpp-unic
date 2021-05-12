
// SenderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sender.h"
#include "SenderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSenderDlg dialog

#define DEFAULT_PORT				5150
#define DEFAULT_COUNT			5
#define DEFAULT_BUFFER_LENGTH	64

BOOL	  bConnect = FALSE;		  // Вызывать connect?
int	  iPort = DEFAULT_PORT;	  // Номер порта
DWORD dwCount = DEFAULT_COUNT;// Количество дейтаграмм
char	  szRecipient[128];		  // Имя или адрес получателя

CListBox* pListBox;	  // Для функции DatagramSend

int 	DatagramSend();


CSenderDlg::CSenderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SENDER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTBOX, m_ListBox);
}

BEGIN_MESSAGE_MAP(CSenderDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECTION, &CSenderDlg::OnBnClickedConnection)
	ON_BN_CLICKED(IDC_SEND, &CSenderDlg::OnBnClickedSend)
END_MESSAGE_MAP()


// CSenderDlg message handlers

BOOL CSenderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	char Str[128];

	GetDlgItem(IDC_RECEIVER)->SetWindowText("localhost");
	sprintf_s(Str, sizeof(Str), "%d", iPort);
	GetDlgItem(IDC_PORT)->SetWindowText(Str);
	sprintf_s(Str, sizeof(Str), "%d", dwCount);
	GetDlgItem(IDC_COUNT)->SetWindowText(Str);
	if (!bConnect)
		((CButton*)GetDlgItem(IDC_CONNECTION))->SetCheck(0);
	else
		((CButton*)GetDlgItem(IDC_CONNECTION))->SetCheck(1);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSenderDlg::OnPaint()
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
HCURSOR CSenderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSenderDlg::OnBnClickedConnection()
{
	// TODO: Add your control notification handler code here
	if (((CButton*)GetDlgItem(IDC_CONNECTION))->
		GetCheck() == 1)
		bConnect = TRUE;
	else
		bConnect = FALSE;

}


void CSenderDlg::OnBnClickedSend()
{
	// TODO: Add your control notification handler code here
	char Str[81];

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
	GetDlgItem(IDC_RECEIVER)->
		GetWindowText(szRecipient, sizeof(szRecipient));

	pListBox = &m_ListBox;
	DatagramSend();

}
int DatagramSend()
{
	WSADATA	wsd;
	SOCKET	s;
	char* sendbuf = NULL;
	int		ret,
		i;
	SOCKADDR_IN	recipient;

	char		Str[200];

	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		sprintf_s(Str, sizeof(Str), "WSAStartup failed!");
		pListBox->AddString((LPTSTR)Str);
		return 1;
	}
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == INVALID_SOCKET)
	{
		sprintf_s(Str, sizeof(Str), "socket() failed; %d",
			WSAGetLastError());
		pListBox->AddString((LPTSTR)Str);
		return 1;
	}
	// Разрешить имя или IP-адрес приемника
	//
	recipient.sin_family = AF_INET;
	recipient.sin_port = htons((short)iPort);
	if ((recipient.sin_addr.s_addr = inet_addr(szRecipient))
		== INADDR_NONE)
	{
		struct hostent* host = NULL;
		host = gethostbyname(szRecipient);
		if (host)
			CopyMemory(&recipient.sin_addr,
				host->h_addr_list[0], host->h_length);
		else
		{
			sprintf_s(Str, sizeof(Str),
				"gethostbyname() failed: %d",
				WSAGetLastError());
			pListBox->AddString((LPTSTR)Str);
			WSACleanup();
			return 1;
		}
	}

	sendbuf = (char*)GlobalAlloc(GMEM_FIXED,
		DEFAULT_BUFFER_LENGTH);
	if (!sendbuf)
	{
		sprintf_s(Str, sizeof(Str),
			"GlobalAlloc() failed: %d",
			GetLastError());
		pListBox->AddString((LPTSTR)Str);
		return 1;
	}
	// Если задана опция connect, выполняется "подключение" 
	// и отправка с использованием функции send()
	if (bConnect)
	{
		if (connect(s, (SOCKADDR*)&recipient,
			sizeof(recipient)) == SOCKET_ERROR)
		{
			sprintf_s(Str, sizeof(Str),
				"connect() failed: %d",
				WSAGetLastError());
			pListBox->AddString((LPTSTR)Str);
			GlobalFree(sendbuf);
			WSACleanup();
			return 1;
		}
		for (i = 0; i < dwCount; i++)
		{
			sprintf_s(sendbuf, DEFAULT_BUFFER_LENGTH,
				"Test datagram (with connection) #%d", i);
			ret = send(s, sendbuf, strlen(sendbuf), 0);
			if (ret == SOCKET_ERROR)
			{
				sprintf_s(Str, sizeof(Str),
					"send() failed: %d",
					WSAGetLastError());
				pListBox->AddString((LPTSTR)Str);
				break;
			}
			else if (ret == 0)
				break;
			// Отправка успешна!
		}
	}
	else
	{
		// Иначе используем функцию sendto()
		//
		for (i = 0; i < dwCount; i++)
		{
			sprintf_s(sendbuf, DEFAULT_BUFFER_LENGTH,
				"Test datagram #%d", i);
			ret = sendto(s, sendbuf, strlen(sendbuf), 0,
				(SOCKADDR*)&recipient, sizeof(recipient));
			if (ret == SOCKET_ERROR)
			{
				sprintf_s(Str, sizeof(Str),
					"sendto() failed; %d",
					WSAGetLastError());
				pListBox->AddString((LPTSTR)Str);
				break;
			}
			else if (ret == 0)
				break;
			// Отправка успешна!
		}
	}
	closesocket(s);

	GlobalFree(sendbuf);
	WSACleanup();
	return 0;
}
