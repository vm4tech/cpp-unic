
// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	MAX_SESSIONS	254
#define	MAX_NAMES		254
#define	MAX_BUFFER		1024
#define	MaxMess			200

HANDLE* hArray;
NCB* pncb;
DWORD	dwIndex;
char	szServerName[NCBNAMSZ];
char	szClientName[NCBNAMSZ];
char	Str[MaxMess];

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


// CClientDlg dialog



CClientDlg::CClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
	, m_Number(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NUMBER, m_Number);
	DDV_MinMaxInt(pDX, m_Number, 1, 20);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
	GetDlgItem(IDC_SERVER)->SetWindowText(_T("TEST-SERVER"));
	GetDlgItem(IDC_CLIENT)->SetWindowText(_T("TEST-CLIENT"));
	SetConnected(false);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
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
	GetDlgItem(IDC_NUMBER)->EnableWindow(IsConnected);
	GetDlgItem(IDC_SERVER)->EnableWindow(!IsConnected);
	GetDlgItem(IDC_CLIENT)->EnableWindow(!IsConnected);
	GetDlgItem(IDC_CONNECT)->EnableWindow(!IsConnected);

	if (IsConnected)
	{
		char Str[180], Server[81], Client[81];
		GetDlgItem(IDC_SERVER)->GetWindowText(
			(LPTSTR)Server, 80);
		GetDlgItem(IDC_CLIENT)->GetWindowText(
			(LPTSTR)Client, 80);
		sprintf_s(Str, sizeof(Str),
			"Connected: Server: %s Client:%s",
			Server, Client);
		GetDlgItem(IDC_CURRENT_CONN)->SetWindowText(
			(LPTSTR)Str);
	}
	else
	{
		GetDlgItem(IDC_CURRENT_CONN)
			->SetWindowText(_T("No connection"));
	}
}
//
// Функция: Connect
// Устанавливает  соединение  с  сервером  на  заданном  номере  LANA.
// В переданной структуре NCB поле ncb_event уже заполнено
int CClientDlg::Connect
(PNCB pncb, int lana, char* server, char* client)
{
	pncb->ncb_command = NCBCALL | ASYNCH;
	pncb->ncb_lana_num = lana;

	memset(pncb->ncb_name, ' ', NCBNAMSZ);
	strncpy_s((char*)pncb->ncb_name,
		sizeof(pncb->ncb_name), client, strlen(client));

	memset(pncb->ncb_callname, ' ', NCBNAMSZ);
	strncpy_s((char*)pncb->ncb_callname,
		sizeof(pncb->ncb_callname), server,
		strlen(server));

	if (Netbios(pncb) != NRC_GOODRET)
	{
		sprintf_s(Str, sizeof(Str),
			"ERROR: Netbios: NCBCONNECT: %d",
			pncb->ncb_retcode);
		((CListBox*)GetDlgItem(IDC_LISTBOX))
			->AddString((LPTSTR)Str);
		return pncb->ncb_retcode;
	}
	return NRC_GOODRET;
}

