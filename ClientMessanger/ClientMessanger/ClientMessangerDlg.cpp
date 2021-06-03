
// ClientMessangerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ClientMessanger.h"
#include "ClientMessangerDlg.h"
#include "afxdialogex.h"

#include <winsock2.h>

#define DEFAULT_COUNT	1
#define DEFAULT_PORT	5150
#define DEFAULT_BUFFER	2048
#define DEFAULT_MESSAGE	"This is a test message"

UINT Recv(LPVOID pParam);
SOCKET m_sClient;
CListBox m_ListBox;
// CClientMessangerDlg dialog

CClientMessangerDlg::CClientMessangerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENTMESSANGER_DIALOG, pParent)
	, m_Number(DEFAULT_COUNT)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientMessangerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTBOX, m_ListBox);
}

BEGIN_MESSAGE_MAP(CClientMessangerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, &CClientMessangerDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_SEND, &CClientMessangerDlg::OnBnClickedSend)
END_MESSAGE_MAP()


// CClientMessangerDlg message handlers

BOOL CClientMessangerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	char Str[128];


	GetDlgItem(IDC_MESSAGE)->SetWindowText(DEFAULT_MESSAGE);
	SetConnected(false);

	return TRUE;  // return TRUE  unless you set the focus to a control
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientMessangerDlg::OnPaint()
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
HCURSOR CClientMessangerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CClientMessangerDlg::SetConnected(bool IsConnected)
{
	m_IsConnected = IsConnected;

	GetDlgItem(IDC_SEND)->EnableWindow(IsConnected);
	GetDlgItem(IDC_MESSAGE)->EnableWindow(IsConnected);
	GetDlgItem(IDC_CONNECT)->EnableWindow(!IsConnected);
}


void CClientMessangerDlg::OnBnClickedConnect()
{
	// TODO: Add your control notification handler code here
	char		szServer[128];	// Имя или IP-адрес сервера
	int		iPort;			// Порт

	WSADATA	wsd;

	struct sockaddr_in 	server;
	struct hostent* host = NULL;

	char Str[256];

	/*GetDlgItem(IDC_SERVER)->GetWindowText(szServer, sizeof(szServer));
	GetDlgItem(IDC_PORT)->GetWindowText(Str, sizeof(Str));*/
	strcpy(szServer, "localhost");
	iPort = DEFAULT_PORT;
	if (iPort <= 0 || iPort >= 0x10000)
	{
		m_ListBox.AddString((LPTSTR)"Port number incorrect");
		return;
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		m_ListBox.AddString((LPTSTR)"Failed to load Winsock library!");
		return;
	}

	m_sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sClient == INVALID_SOCKET)
	{
		sprintf_s(Str, sizeof(Str), "socket() failed: %d\n", WSAGetLastError());
		m_ListBox.AddString((LPTSTR)Str);
		return;
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(iPort);
	server.sin_addr.s_addr = inet_addr(szServer);

	if (server.sin_addr.s_addr == INADDR_NONE)
	{
		host = gethostbyname(szServer);
		if (host == NULL)
		{
			sprintf_s(Str, sizeof(Str), "Unable to resolve server: %s", szServer);
			m_ListBox.AddString((LPTSTR)Str);
			return;
		}
		CopyMemory(&server.sin_addr, host->h_addr_list[0], host->h_length);
	}
	if (connect(m_sClient, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		sprintf_s(Str, sizeof(Str), "connect() failed: %d", WSAGetLastError());
		m_ListBox.AddString((LPTSTR)Str);
		return;
	}
	SetConnected(true);
	// Запускает поток на прослушивание сокета на сервере
	AfxBeginThread(Recv, NULL);
}

UINT Recv(LPVOID pParam) {
	char	szBuffer[DEFAULT_BUFFER];
	int ret;
	
	char	Str[256];

	for (;;) {
		ret = recv(m_sClient, szBuffer, DEFAULT_BUFFER, 0);
		if (ret == 0)
			continue;
		else if (ret == SOCKET_ERROR)
		{
			sprintf_s(Str, sizeof(Str), "send() failed: %d", WSAGetLastError());
			m_ListBox.AddString((LPTSTR)Str);
			break;
		}
		szBuffer[ret] = '\0';
		sprintf_s(Str, sizeof(Str), "response from server: %s", szBuffer);
		m_ListBox.AddString((LPTSTR)Str);
	}
	return 0;
}

void CClientMessangerDlg::OnBnClickedSend()
{
	// TODO: Add your control notification handler code here
	char	szMessage[1024];		// Сообщение для отправки
	BOOL	bSendOnly = FALSE;	// Только отправка данных

	char	szBuffer[DEFAULT_BUFFER];
	int		ret,
		i;

	char	Str[256];

	UpdateData();
	if (m_Number < 1 || m_Number>20)
	{
		m_ListBox.AddString("Number of messages must be between 1 and 20");
		return;
	}

	GetDlgItem(IDC_MESSAGE)->GetWindowText(szMessage, sizeof(szMessage));

	// Отправка данных
	 
	sprintf_s(Str, sizeof(Str), "mes=%s&%d&%d", CString(szMessage),m_sClient,1234);
	
	ret = send(m_sClient, Str, strlen(Str), 0);
	m_ListBox.AddString((LPTSTR)Str);
	if (ret == SOCKET_ERROR)
	{
		sprintf_s(Str, sizeof(Str), "send() failed: %d", WSAGetLastError());
		m_ListBox.AddString((LPTSTR)Str);
	}

	sprintf_s(Str, sizeof(Str), "Send %d bytes\n", ret);
	m_ListBox.AddString((LPTSTR)Str);
		
	//

	
	/*closesocket(m_sClient);

	WSACleanup();

	SetConnected(false);*/
}
