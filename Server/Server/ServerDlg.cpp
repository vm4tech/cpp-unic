
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
#define MAX_SESSIONS		254
#define MAX_NAMES		254
#define MAX_BUFFER		2048
char		szServerName[NCBNAMSZ];
char		Str[200];
HWND		hWnd_LB;	 // Для использования в потоках

NCB* g_Clients = NULL; // Глобальная структура NCB для клиентов

int 		Listen(PNCB pncb, int lana, char* name);
UINT 	ClientThread(PVOID lpParam);
UINT 	ListenThread(PVOID lpParam);



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
	ON_BN_CLICKED(IDC_START, &CServerDlg::OnBnClickedStart)
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
	GetDlgItem(IDC_SERVER)->SetWindowText(_T("TEST-SERVER"));
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

	GetDlgItem(IDC_SERVER)->EnableWindow(!IsStarted);
	GetDlgItem(IDC_START)->EnableWindow(!IsStarted);
}



void CServerDlg::OnBnClickedStart()
{
	// TODO: Add your control notification handler code here
	hWnd_LB = m_ListBox.m_hWnd;
	GetDlgItem(IDC_SERVER)->GetWindowText(
		(LPTSTR)szServerName, NCBNAMSZ);
	AfxBeginThread(ListenThread, NULL);
	SetStarted(true);

}

UINT ListenThread(PVOID lpParam)
{
	PNCB		pncb = NULL;
	HANDLE		hArray[64];
	DWORD		dwHandleCount = 0,
		dwRet;
	int			i,
		num;
	LANA_ENUM	lenum;

	char		Str[200];
	CListBox* pLB = (CListBox*)(CListBox::FromHandle(hWnd_LB));

	if (LanaEnum(&lenum) != NRC_GOODRET)
	{
		if (*NbCommonErrorMsg)
			pLB->AddString((LPTSTR)NbCommonErrorMsg);
		sprintf_s(Str, sizeof(Str),
			"LanaEnum failed with error %d:",
			GetLastError());
		pLB->AddString((LPTSTR)Str);
		return 1;
	}

	if (lenum.length == 0)
	{
		pLB->AddString((LPTSTR)"Sorry, no existing LANA...");
		return 1;
	}

	if (ResetAll(&lenum, (UCHAR)MAX_SESSIONS,
		(UCHAR)MAX_NAMES, FALSE) != NRC_GOODRET)
	{
		if (*NbCommonErrorMsg)
			pLB->AddString((LPTSTR)NbCommonErrorMsg);
		sprintf_s(Str, sizeof(Str),
			"ResetAll failed with error %d:",
			GetLastError());
		pLB->AddString((LPTSTR)Str);
		return 1;
	}

	g_Clients = (PNCB)GlobalAlloc(
		GMEM_FIXED | GMEM_ZEROINIT,
		sizeof(NCB) * lenum.length);
	for (i = 0; i < lenum.length; i++)
	{
		hArray[i] = g_Clients[i].ncb_event =
			CreateEvent(NULL, TRUE, FALSE, NULL);
		AddName(lenum.lana[i], szServerName, &num);
		if (*NbCommonErrorMsg)
			pLB->AddString((LPTSTR)NbCommonErrorMsg);
		Listen(&g_Clients[i], lenum.lana[i], szServerName);
	}
	pLB->AddString((LPTSTR)"Wait for connections");

	while (1)
	{
		// Ожидание подключения клиента
		dwRet = WaitForMultipleObjects(lenum.length, hArray,
			FALSE, INFINITE);
		if (dwRet == WAIT_FAILED)
		{
			sprintf_s(Str, sizeof(Str),
				"ERROR: WaitForMultipleObjects: %d",
				GetLastError());
			pLB->AddString((LPTSTR)Str);
			break;
		}
		pLB->AddString((LPTSTR)"Client connected");

		// Проверка всех структур NCB для определения, 
		// достигла ли успеха более, чем одна структура. 
		// Если поле ncb_cmd_plt не содержит значение 
		// NRC_PENDING, значит существует клиент, необходимо 
		// создать поток и выделить ему новую структуру NCB.
		// Старая структура используется повторно.
		for (i = 0; i < lenum.length; i++)
		{
			if (g_Clients[i].ncb_cmd_cplt != NRC_PENDING)
			{
				pncb = (PNCB)GlobalAlloc(GMEM_FIXED,
					sizeof(NCB));
				memcpy(pncb, &g_Clients[i], sizeof(NCB));
				pncb->ncb_event = 0;

				AfxBeginThread(ClientThread, (PVOID)pncb);
				// Событие сбрасывается, начинаем еще 
				// одно прослушивание
				ResetEvent(hArray[i]);
				Listen(&g_Clients[i], lenum.lana[i],
					szServerName);
			}
		}
	}

	for (i = 0; i < lenum.length; i++)
	{
		DelName(lenum.lana[i], szServerName);
		if (*NbCommonErrorMsg)
			pLB->AddString((LPTSTR)NbCommonErrorMsg);
		CloseHandle(hArray[i]);
	}

	GlobalFree(g_Clients);
	return 0;
}

int Listen(PNCB pncb, int lana, char* name)
{
	char		Str[200];
	CListBox* pLB =
		(CListBox*)(CListBox::FromHandle(hWnd_LB));

	pncb->ncb_command = NCBLISTEN | ASYNCH;
	pncb->ncb_lana_num = lana;

	// Имя, с которым клиенты будут соединяться
	memset(pncb->ncb_name, ' ', NCBNAMSZ);
	strncpy_s((char*)pncb->ncb_name,
		sizeof(pncb->ncb_name), name, strlen(name));

	// Имя клиента,
	// '*' означает, что примем соединение от любого клиента
	memset(pncb->ncb_callname, ' ', NCBNAMSZ);
	pncb->ncb_callname[0] = '*';
	if (Netbios(pncb) != NRC_GOODRET)
	{
		sprintf_s(Str, sizeof(Str),
			"ERROR: Netbios: NCBLISTEN: %d",
			pncb->ncb_retcode);
		pLB->AddString((LPTSTR)Str);
		return pncb->ncb_retcode;
	}

	return NRC_GOODRET;
}

UINT ClientThread(PVOID lpParam)
{
	PNCB		pncb = (PNCB)lpParam;
	NCB		ncb;
	char		szRecvBuff[MAX_BUFFER],
		szClientName[NCBNAMSZ + 1];
	DWORD	dwBufferLen = MAX_BUFFER,
		dwRetVal = NRC_GOODRET;
	char		Str[200];
	CListBox* pLB =
		(CListBox*)(CListBox::FromHandle(hWnd_LB));

	//
	// Отправка и прием сообщений, пока сеанс не закрыт
	// 
	FormatNetbiosName((char*)pncb->ncb_callname,
		szClientName);
	if (*NbCommonErrorMsg)
		pLB->AddString((LPTSTR)NbCommonErrorMsg);
	while (1)
	{
		dwBufferLen = MAX_BUFFER;
		dwRetVal = Recv(pncb->ncb_lana_num, pncb->ncb_lsn,
			szRecvBuff, &dwBufferLen);
		if (dwRetVal != NRC_GOODRET)
		{
			if (*NbCommonErrorMsg)
				pLB->AddString((LPTSTR)NbCommonErrorMsg);
			sprintf_s(Str, sizeof(Str),
				"Recv failed with error %d",
				GetLastError());
			pLB->AddString((LPTSTR)Str);
			break;
		}

		szRecvBuff[dwBufferLen] = 0;
		sprintf_s(Str, sizeof(Str),
			"READ [LANA=%d]: '%s'",
			pncb->ncb_lana_num, szRecvBuff);
		pLB->AddString((LPTSTR)Str);

		dwRetVal = Send(pncb->ncb_lana_num, pncb->ncb_lsn,
			szRecvBuff, dwBufferLen);
		if (dwRetVal != NRC_GOODRET)
		{
			if (*NbCommonErrorMsg)
				pLB->AddString((LPTSTR)NbCommonErrorMsg);
			sprintf_s(Str, sizeof(Str),
				"Send failed with error %d",
				GetLastError());
			pLB->AddString((LPTSTR)Str);
			break;
		}
	}
	sprintf_s(Str, sizeof(Str),
		"Client '%s' on LANA %d disconnected ",
		szClientName, pncb->ncb_lana_num);
	pLB->AddString((LPTSTR)Str);

	//
	// Обработка ошибок в ходе чтения или записи
	//
	if (dwRetVal != NRC_SCLOSED)
	{
		ZeroMemory(&ncb, sizeof(NCB));
		ncb.ncb_command = NCBHANGUP;
		ncb.ncb_lsn = pncb->ncb_lsn;
		ncb.ncb_lana_num = pncb->ncb_lana_num;

		if (Netbios(&ncb) != NRC_GOODRET)
		{
			sprintf_s(Str, sizeof(Str),
				"ERROR: Netbios: NCBHANGUP: %d ",
				ncb.ncb_retcode);
			pLB->AddString((LPTSTR)Str);
			GlobalFree(pncb);
			dwRetVal = ncb.ncb_retcode;
		}
	}

	//
	// Удаление динамически выделенной структуры NCB
	//
	GlobalFree(pncb);
	return NRC_GOODRET;
}
