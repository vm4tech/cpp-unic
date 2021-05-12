
// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerDlg dialog
#define DEFAULT_PORT		5150
#define DEFAULT_BUFFER	4096

int		iPort = DEFAULT_PORT; // Порт для прослушивания
BOOL		bInterface = FALSE,	  // Конкретный клиент?
bRecvOnly = FALSE;	  // Только прием данных
char		szAddress[128];		  // Интерфейс для прослушивания
HWND		hWnd_LB;				  // Для вывода в других потоках

UINT ClientThread(PVOID lpParam);
UINT ListenThread(PVOID lpParam);



CServerDlg::CServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTBOX, m_ListBox);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ECHO, &CServerDlg::OnBnClickedEcho)
	ON_BN_CLICKED(IDC_START, &CServerDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_ALL_ADDR, &CServerDlg::OnBnClickedAllAddr)
END_MESSAGE_MAP()


// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
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
	if (bRecvOnly)
		((CButton*)GetDlgItem(IDC_ECHO))->SetCheck(0);
	else
		((CButton*)GetDlgItem(IDC_ECHO))->SetCheck(1);

	if (bInterface)
		((CButton*)GetDlgItem(IDC_ALL_ADDR))->SetCheck(0);
	else
		((CButton*)GetDlgItem(IDC_ALL_ADDR))->SetCheck(1);

	SetStarted(false);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerDlg::OnPaint()
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
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CServerDlg::SetStarted(bool IsStarted)
{
	m_IsStarted = IsStarted;

	GetDlgItem(IDC_ALL_ADDR)->EnableWindow(!IsStarted);
	GetDlgItem(IDC_CLIENT)->EnableWindow(bInterface);
	GetDlgItem(IDC_PORT)->EnableWindow(!IsStarted);
	GetDlgItem(IDC_START)->EnableWindow(!IsStarted);
}


void CServerDlg::OnBnClickedEcho()
{
	// TODO: Add your control notification handler code here
	if (((CButton*)GetDlgItem(IDC_ECHO))->GetCheck() == 1)
		bRecvOnly = FALSE;
	else
		bRecvOnly = TRUE;

}


void CServerDlg::OnBnClickedStart()
{
	// TODO: Add your control notification handler code here
	char Str[81];

	hWnd_LB = m_ListBox.m_hWnd;	// Для потоков
	GetDlgItem(IDC_PORT)->GetWindowText(Str, sizeof(Str));
	iPort = atoi(Str);
	if (iPort <= 0 || iPort >= 0x10000)
	{
		AfxMessageBox("Incorrect Port number");
		return;
	}
	GetDlgItem(IDC_CLIENT)->
		GetWindowText(szAddress, sizeof(szAddress));

	AfxBeginThread(ListenThread, NULL);

	SetStarted(true);

}


void CServerDlg::OnBnClickedAllAddr()
{
	// TODO: Add your control notification handler code here
	if (((CButton*)GetDlgItem(IDC_ALL_ADDR))->
		GetCheck() == 1)
		bInterface = FALSE;
	else
		bInterface = TRUE;

	GetDlgItem(IDC_CLIENT)->EnableWindow(bInterface);
}

UINT ListenThread(PVOID lpParam)
{
	WSADATA		wsd;
	SOCKET		sListen,
		sClient;
	int			iAddrSize;

	struct sockaddr_in	local,
		client;
	char		Str[200];
	CListBox* pLB =
		(CListBox*)(CListBox::FromHandle(hWnd_LB));

	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		sprintf_s(Str, sizeof(Str),
			"Failed to load Winsock!");
		pLB->AddString((LPTSTR)Str);
		return 1;
	}
	sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (sListen == SOCKET_ERROR)
	{
		sprintf_s(Str, sizeof(Str),
			"socket() failed: %d", WSAGetLastError());
		pLB->AddString((LPTSTR)Str);
		WSACleanup();
		return 1;
	}
	// Выбрать локальный интерфейс и привязаться к нему
	if (bInterface)
	{
		local.sin_addr.s_addr = inet_addr(szAddress);
		if (local.sin_addr.s_addr == INADDR_NONE)
		{
			AfxMessageBox("Incorrect local interface");
			closesocket(sListen);
			WSACleanup();
			return 1;
		}
	}
	else
		local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_family = AF_INET;
	local.sin_port = htons(iPort);

	if (bind(sListen, (struct sockaddr*)&local,
		sizeof(local)) == SOCKET_ERROR)
	{
		sprintf_s(Str, sizeof(Str),
			"bind() failed: %d", WSAGetLastError());
		pLB->AddString((LPTSTR)Str);
		closesocket(sListen);
		WSACleanup();
		return 1;
	}

	listen(sListen, 8);
	// Ожидание клиентов, создание потока для каждого соединения
	while (1)
	{
		iAddrSize = sizeof(client);
		sClient = accept(sListen,
			(struct sockaddr*)&client, &iAddrSize);
		if (sClient == INVALID_SOCKET)
		{
			sprintf_s(Str, sizeof(Str),
				"accept() failed: %d", WSAGetLastError());
			pLB->AddString((LPTSTR)Str);
			break;
		}
		sprintf_s(Str, sizeof(Str),
			"Accepted client: %s:%d",
			inet_ntoa(client.sin_addr),
			ntohs(client.sin_port));
		pLB->AddString((LPTSTR)Str);

		AfxBeginThread(ClientThread, (LPVOID)sClient);
	}
	closesocket(sListen);

	WSACleanup();
	return 0;
}
UINT ClientThread(PVOID lpParam)
{
	SOCKET	sock = (SOCKET)lpParam;
	char		szBuff[DEFAULT_BUFFER];
	int		ret,
		nLeft,
		idx;
	char		Str[200];
	CListBox* pLB =
		(CListBox*)(CListBox::FromHandle(hWnd_LB));

	while (1)
	{
		// Блокируюший вызов recv()
		//
		ret = recv(sock, szBuff, DEFAULT_BUFFER, 0);
		if (ret == 0)	// Корректное завершение
			break;
		else if (ret == SOCKET_ERROR)
		{
			sprintf_s(Str, sizeof(Str),
				"recv() failed: %d", WSAGetLastError());
			pLB->AddString((LPTSTR)Str);
			break;
		}
		szBuff[ret] = '\0';
		sprintf_s(Str, sizeof(Str), "RECV: '%s'", szBuff);
		pLB->AddString((LPTSTR)Str);

		// Ответная отправка данных, если требуется
		//
		if (!bRecvOnly)
		{
			nLeft = ret;
			idx = 0;

			// Проверка, что все данные записаны
			//
			while (nLeft > 0)
			{
				ret = send(sock, &szBuff[idx], nLeft, 0);
				if (ret == 0)
					break;
				else if (ret == SOCKET_ERROR)
				{
					sprintf_s(Str, sizeof(Str),
						"send() failed: %d",
						WSAGetLastError());
					pLB->AddString((LPTSTR)Str);
					break;
				}
				nLeft -= ret;
				idx += ret;
			}
		}
	}
	return 0;
}
