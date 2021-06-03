
// ClientMessangerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ClientMessanger.h"
#include "ClientMessangerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define DEFAULT_COUNT	10
#define DEFAULT_PORT		5150
#define DEFAULT_BUFFER	2048
#define DEFAULT_MESSAGE	"This is a test message"


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClientMessangerDlg dialog



CClientMessangerDlg::CClientMessangerDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CLIENTMESSANGER_DIALOG, pParent)
	, m_Number(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientMessangerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTBOX, m_ListBox);
	DDX_Control(pDX, IDC_NO_ECHO, m_NoEcho);
	DDX_Text(pDX, IDC_NUMBER, m_Number);
}

BEGIN_MESSAGE_MAP(CClientMessangerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, &CClientMessangerDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_SEND, &CClientMessangerDlg::OnBnClickedSend)
END_MESSAGE_MAP()


// CClientMessangerDlg message handlers

BOOL CClientMessangerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	char Str[128];

	GetDlgItem(IDC_SERVER)->SetWindowText("localhost");
	sprintf_s(Str, sizeof(Str), "%d", DEFAULT_COUNT);
	GetDlgItem(IDC_NUMBER)->SetWindowText(Str);
	sprintf_s(Str, sizeof(Str), "%d", DEFAULT_PORT);
	GetDlgItem(IDC_PORT)->SetWindowText(Str);
	GetDlgItem(IDC_MESSAGE)->SetWindowText(DEFAULT_MESSAGE);
	m_NoEcho.SetCheck(0);
	SetConnected(false);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientMessangerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientMessangerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CClientMessangerDlg::SetConnected (bool IsConnected)
{
	m_IsConnected = IsConnected;

	GetDlgItem(IDC_SEND)->EnableWindow(IsConnected);
	GetDlgItem(IDC_MESSAGE)->EnableWindow(IsConnected);
	GetDlgItem(IDC_NUMBER)->EnableWindow(IsConnected);
	GetDlgItem(IDC_NO_ECHO)->EnableWindow(IsConnected);
	GetDlgItem(IDC_SERVER)->EnableWindow(!IsConnected);
	GetDlgItem(IDC_PORT)->EnableWindow(!IsConnected);
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

	GetDlgItem(IDC_SERVER)->
		GetWindowText(szServer, sizeof(szServer));
	GetDlgItem(IDC_PORT)->GetWindowText(Str, sizeof(Str));
	iPort = atoi(Str);
	if (iPort <= 0 || iPort >= 0x10000)
	{
		m_ListBox.AddString((LPTSTR)"Port number incorrect");
		return;
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		m_ListBox.AddString
		((LPTSTR)"Failed to load Winsock library!");
		return;
	}
	m_sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sClient == INVALID_SOCKET)
	{
		sprintf_s(Str, sizeof(Str), "socket() failed: %d\n",
			WSAGetLastError());
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
			sprintf_s(Str, sizeof(Str),
				"Unable to resolve server: %s", szServer);
			m_ListBox.AddString((LPTSTR)Str);
			return;
		}
		CopyMemory(&server.sin_addr, host->h_addr_list[0],
			host->h_length);
	}
	if (connect(m_sClient, (struct sockaddr*)&server,
		sizeof(server)) == SOCKET_ERROR)
	{
		sprintf_s(Str, sizeof(Str), "connect() failed: %d",
			WSAGetLastError());
		m_ListBox.AddString((LPTSTR)Str);
		return;
	}
	SetConnected(true);

}


void CClientMessangerDlg::OnBnClickedSend()
{
	// TODO: Add your control notification handler code here
	char	szMessage[1024];		// Сообщение для отправки
	BOOL	bSendOnly = FALSE;	// Только отправка данных

	char		szBuffer[DEFAULT_BUFFER];
	int		ret,
		i;

	char		Str[256];

	UpdateData();
	if (m_Number < 1 || m_Number>20)
	{
		m_ListBox.AddString
		("Number of messages must be between 1 and 20");
		return;
	}

	GetDlgItem(IDC_MESSAGE)->
		GetWindowText(szMessage, sizeof(szMessage));
	if (m_NoEcho.GetCheck() == 1)
		bSendOnly = TRUE;

	// Отправка и прием данных 
	//
	for (i = 0; i < m_Number; i++)
	{
		ret = send(m_sClient, szMessage, strlen(szMessage), 0);

		if (ret == 0)
			break;
		else if (ret == SOCKET_ERROR)
		{
			sprintf_s(Str, sizeof(Str), "send() failed: %d",
				WSAGetLastError());
			m_ListBox.AddString((LPTSTR)Str);
			break;
		}

		sprintf_s(Str, sizeof(Str), "Send %d bytes\n", ret);
		m_ListBox.AddString((LPTSTR)Str);

		if (!bSendOnly)
		{
			ret = recv(m_sClient, szBuffer, DEFAULT_BUFFER, 0);
			if (ret == 0)	// Корректное завершение
				break;
			else if (ret == SOCKET_ERROR)
			{
				sprintf_s(Str, sizeof(Str),
					"recv() failed: %d", WSAGetLastError());
				m_ListBox.AddString((LPTSTR)Str);
				break;
			}
			szBuffer[ret] = '\0';
			sprintf_s(Str, sizeof(Str),
				"RECV [%d bytes]: '%s'", ret, szBuffer);
			m_ListBox.AddString((LPTSTR)Str);
		}
	}
	closesocket(m_sClient);

	WSACleanup();

	SetConnected(false);

}
