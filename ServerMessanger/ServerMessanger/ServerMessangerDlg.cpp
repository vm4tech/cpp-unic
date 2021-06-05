
// ServerMessangerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ServerMessanger.h"
#include "ServerMessangerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <winsock2.h>
#define PORT 5150			// Порт по умолчанию
#define DATA_BUFSIZE 8192 	// Размер буфера по умолчанию

int  iPort = PORT; 	 // Порт для прослушивания подключений
bool bPrint = false; // Выводить ли сообщения клиентов
CListBox m_ListBox;
typedef struct _SOCKET_INFORMATION {
	CHAR Buffer[DATA_BUFSIZE];
	WSABUF DataBuf;
	SOCKET Socket;
	DWORD BytesSEND;
	DWORD BytesRECV;
} SOCKET_INFORMATION, * LPSOCKET_INFORMATION;

BOOL CreateSocketInformation(SOCKET s, char* Str,
	CListBox* pLB);
void FreeSocketInformation(DWORD Event, char* Str,
	CListBox* pLB);

DWORD EventTotal = 0;
WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];
LPSOCKET_INFORMATION SocketArray[WSA_MAXIMUM_WAIT_EVENTS];
DWORD RecvBytes;
DWORD SendBytes;
HWND   hWnd_LB;  // Для вывода в других потоках

UINT ListenThread(PVOID lpParam);
char** parsedMess;

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


// CServerMessangerDlg dialog



CServerMessangerDlg::CServerMessangerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVERMESSANGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerMessangerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTBOX, m_ListBox);
}

BEGIN_MESSAGE_MAP(CServerMessangerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_PRINT, &CServerMessangerDlg::OnBnClickedPrint)
	ON_BN_CLICKED(IDC_START, &CServerMessangerDlg::OnBnClickedStart)
END_MESSAGE_MAP()


// CServerMessangerDlg message handlers

BOOL CServerMessangerDlg::OnInitDialog()
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
	char Str[128];

	sprintf_s(Str, sizeof(Str), "%d", iPort);
	GetDlgItem(IDC_PORT)->SetWindowText(Str);

	if (bPrint)
		((CButton*)GetDlgItem(IDC_PRINT))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_PRINT))->SetCheck(0);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerMessangerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CServerMessangerDlg::OnPaint()
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
HCURSOR CServerMessangerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServerMessangerDlg::OnBnClickedPrint()
{
	// TODO: Add your control notification handler code here
	if (((CButton*)GetDlgItem(IDC_PRINT))->GetCheck() == 1)
		bPrint = true;
	else
		bPrint = false;
}


void CServerMessangerDlg::OnBnClickedStart()
{
	// TODO: Add your control notification handler code here
	char Str[81];

	hWnd_LB = m_ListBox.m_hWnd;   // Для ListenThread
	GetDlgItem(IDC_PORT)->GetWindowText(Str, sizeof(Str));
	iPort = atoi(Str);
	if (iPort <= 0 || iPort >= 0x10000)
	{
		AfxMessageBox("Incorrect Port number");
		return;
	}

	AfxBeginThread(ListenThread, NULL);

	GetDlgItem(IDC_START)->EnableWindow(false);

}

char** getParserMessage(char* buf) {
	char* params[2];
	char* parsed[10], * temp;

	temp = strtok(buf, "=");
	int i = 0;
	while (temp != NULL)
	{
		parsed[i] = temp;
		parsed[i + 1] = 0;
		params[i++] = temp;
		temp = strtok(NULL, "=");
	}

	temp = strtok(params[1], "&");
	i--;
	while (temp != NULL)
	{
		parsed[i++] = temp;
		temp = strtok(NULL, "&");
	}
	parsed[i] = '\0';
	return parsed;
}
void sendMessage(char* message, SOCKET from, char* to) {
	char postMessage[1024];
	char Str[1024];
	//char to2[1024];
	LPSOCKET_INFORMATION SocketInfo = 0;
	for (int i = 0; i < sizeof(SocketArray); i++) {
		if (SocketArray[i] != NULL) {

			sprintf_s(Str, sizeof(Str), "%d", SocketArray[i]->Socket);
			//// TODO: Только дял теста отправляю сеье
			//sprintf_s(to2, sizeof(to2), "%d", from);
			if (strcmp(Str, to) == 0) {
				SocketInfo = SocketArray[i];
				sprintf_s(postMessage, sizeof(postMessage), "message=%s&%d", message, from);
			}

		}
		else
			break;
		
	}
	SocketInfo->DataBuf.buf = postMessage;
	SocketInfo->DataBuf.len = sizeof(postMessage);

	if (WSASend(SocketInfo->Socket,
		&(SocketInfo->DataBuf), 1,
		&SendBytes, 0, NULL, NULL) ==
		SOCKET_ERROR)
	{
		SocketInfo->BytesSEND += SendBytes;
	}
	SocketInfo->BytesSEND = 0;
	SocketInfo->BytesRECV = 0;

}

void sendUsers() {
	char users[1024];
	char Str[1024];
	char to2[1024];
	int size = 0;
	LPSOCKET_INFORMATION SocketInfo = SocketArray[0];
	if (SocketArray[1] == NULL)
		return;
	sprintf_s(users, sizeof(users), "users=%d", SocketArray[1]->Socket);
	for (int i = 2; i < sizeof(SocketArray); i++) {
		if (SocketArray[i] == NULL)
			break;
		sprintf_s(users, sizeof(users), "%s&%d", users, SocketArray[i]->Socket);
	}
	for (int i = 0; i < sizeof(users); i++) {
		if (users[i] != '\0')
			size++;
		else
			break;
	}
	SocketInfo->DataBuf.buf = users;
	SocketInfo->DataBuf.len = size+1;
	for (int i = 1; i < sizeof(SocketArray); i++) {
		if (SocketArray[i] == NULL)
			break;
		if (WSASend(SocketArray[i]->Socket,
			&(SocketInfo->DataBuf), 1,
			&SendBytes, 0, NULL, NULL) ==
			SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				SocketInfo->BytesSEND = 0;
				SocketInfo->BytesRECV = 0;
			}

			// Произошла ошибка WSAEWOULDBLOCK. 
			// Событие FD_WRITE будет отправлено, когда
			// в буфере будет больше свободного места
		}
	}
	SocketInfo->BytesSEND = 0;
	SocketInfo->BytesRECV = 0;
}
UINT ListenThread(PVOID lpParam)
{
	SOCKET Listen;
	SOCKET Accept;
	SOCKADDR_IN InternetAddr;
	DWORD Event;
	WSANETWORKEVENTS NetworkEvents;
	WSADATA wsaData;
	DWORD Ret;
	DWORD Flags;
	
	char  Str[200];
	CListBox* pLB =
		(CListBox*)(CListBox::FromHandle(hWnd_LB));

	if ((Ret = WSAStartup(0x0202, &wsaData)) != 0)
	{
		sprintf_s(Str, sizeof(Str),
			"WSAStartup() failed with error %d", Ret);
		pLB->AddString(Str);
		return 1;
	}
	if ((Listen = socket(AF_INET, SOCK_STREAM, 0)) ==
		INVALID_SOCKET)
	{
		sprintf_s(Str, sizeof(Str),
			"socket() failed with error %d",
			WSAGetLastError());
		pLB->AddString(Str);
		return 1;
	}
	CreateSocketInformation(Listen, Str, pLB);

	if (WSAEventSelect(Listen, EventArray[EventTotal - 1],
		FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR)
	{
		sprintf_s(Str, sizeof(Str),
			"WSAEventSelect() failed with error %d",
			WSAGetLastError());
		pLB->AddString(Str);
		return 1;
	}
	InternetAddr.sin_family = AF_INET;
	InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	InternetAddr.sin_port = htons(iPort);

	if (bind(Listen, (PSOCKADDR)&InternetAddr,
		sizeof(InternetAddr)) == SOCKET_ERROR)
	{
		sprintf_s(Str, sizeof(Str),
			"bind() failed with error %d",
			WSAGetLastError());
		pLB->AddString(Str);
		return 1;
	}

	if (listen(Listen, 5))
	{
		sprintf_s(Str, sizeof(Str),
			"listen() failed with error %d",
			WSAGetLastError());
		pLB->AddString(Str);
		return 1;
	}
	while (TRUE)
	{
		// Дожидаемся уведомления о событии на любом сокете
		if ((Event = WSAWaitForMultipleEvents(EventTotal,
			EventArray, FALSE, WSA_INFINITE, FALSE)) ==
			WSA_WAIT_FAILED)
		{
			sprintf_s(Str, sizeof(Str),
				"WSAWaitForMultipleEvents failed"
				" with error %d", WSAGetLastError());
			pLB->AddString(Str);
			return 1;
		}

		if (WSAEnumNetworkEvents(
			SocketArray[Event - WSA_WAIT_EVENT_0]->Socket,
			EventArray[Event - WSA_WAIT_EVENT_0],
			&NetworkEvents) == SOCKET_ERROR)
		{
			sprintf_s(Str, sizeof(Str),
				"WSAEnumNetworkEvents failed"
				" with error %d", WSAGetLastError());
			pLB->AddString(Str);
			return 1;
		}
		if (NetworkEvents.lNetworkEvents & FD_ACCEPT)
		{
			if (NetworkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
			{
				sprintf_s(Str, sizeof(Str),
					"FD_ACCEPT failed with error %d",
					NetworkEvents.iErrorCode[FD_ACCEPT_BIT]);
				pLB->AddString(Str);
				break;
			}

			// Прием нового соединения и добавление его
			// в списки сокетов и событий
			if ((Accept = accept(
				SocketArray[Event - WSA_WAIT_EVENT_0]->Socket,
				NULL, NULL)) == INVALID_SOCKET)
			{
				sprintf_s(Str, sizeof(Str),
					"accept() failed with error %d",
					WSAGetLastError());
				pLB->AddString(Str);
				break;
			}

			// Слишком много сокетов. Закрываем соединение.
			if (EventTotal > WSA_MAXIMUM_WAIT_EVENTS)
			{
				sprintf_s(Str, sizeof(Str),
					"Too many connections - closing socket.");
				pLB->AddString(Str);
				closesocket(Accept);
				break;
			}

			CreateSocketInformation(Accept, Str, pLB);

			if (WSAEventSelect(Accept,
				EventArray[EventTotal - 1],
				FD_READ | FD_WRITE | FD_CLOSE) ==
				SOCKET_ERROR)
			{
				sprintf_s(Str, sizeof(Str),
					"WSAEventSelect()failed"
					" with error %d", WSAGetLastError());
				pLB->AddString(Str);
				return 1;
			}

			sprintf_s(Str, sizeof(Str),
				"Socket %d connected", Accept);
			pLB->AddString(Str);
		}
		// Пытаемся читать или писать данные, 
	  // если произошло соответствующее событие

		if (NetworkEvents.lNetworkEvents & FD_READ ||
			NetworkEvents.lNetworkEvents & FD_WRITE)
		{
			if (NetworkEvents.lNetworkEvents & FD_READ &&
				NetworkEvents.iErrorCode[FD_READ_BIT] != 0)
			{
				sprintf_s(Str, sizeof(Str),
					"FD_READ failed with error %d",
					NetworkEvents.iErrorCode[FD_READ_BIT]);
				pLB->AddString(Str);
				break;
			}

			if (NetworkEvents.lNetworkEvents & FD_WRITE &&
				NetworkEvents.iErrorCode[FD_WRITE_BIT] != 0)
			{
				sprintf_s(Str, sizeof(Str),
					"FD_WRITE failed with error %d",
					NetworkEvents.iErrorCode[FD_WRITE_BIT]);
				pLB->AddString(Str);
				break;
			}

			LPSOCKET_INFORMATION SocketInfo =
				SocketArray[Event - WSA_WAIT_EVENT_0];

			// Читаем данные только если приемный буфер пуст

			if (SocketInfo->BytesRECV == 0)
			{
				SocketInfo->DataBuf.buf = SocketInfo->Buffer;
				SocketInfo->DataBuf.len = DATA_BUFSIZE;

				Flags = 0;
				if (WSARecv(SocketInfo->Socket,
					&(SocketInfo->DataBuf), 1, &RecvBytes,
					&Flags, NULL, NULL) == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						sprintf_s(Str, sizeof(Str),
							"WSARecv()failed with "
							" error %d", WSAGetLastError());
						pLB->AddString(Str);
						FreeSocketInformation(
							Event - WSA_WAIT_EVENT_0, Str, pLB);
						return 1;
					}
				}
				else
				{
					/* get: Парсер сообщений, шаблон:
					* param=...
					* message=asdlasdkals;ld&3213&13821
					* Параметры (param):
					* connect - first connect user (con=socket_user)
					* message - сообщение (mes=message&from&to)
					* all - масовая рассылка (all=message&from)
					* 
					*/

					parsedMess = getParserMessage(SocketInfo->Buffer);
					// Можно потом все сообщения перевести в константы
					if (strcmp(parsedMess[0], "message") == 0) {
						sendMessage(parsedMess[1], SocketInfo->Socket, parsedMess[3]);
					}
					else if (strcmp(parsedMess[0], "connect") == 0) {
						sendUsers();
					}
					else if (strcmp(parsedMess[0], "all") == 0) {
						SocketInfo->BytesRECV = RecvBytes;
					}

					SocketInfo->BytesRECV = RecvBytes;
					// Вывод сообщения, если требуется
					if (bPrint)
					{
						unsigned l = sizeof(Str) - 1;
						if (l > RecvBytes) l = RecvBytes;
						strncpy_s(Str, SocketInfo->Buffer, l);
						Str[l] = 0;
						pLB->AddString(Str);
					}
					SocketInfo->BytesSEND = 0;
					SocketInfo->BytesRECV = 0;
				}
			}
			else {
			}

			// Отправка данных, если это возможно

			//if (SocketInfo->BytesRECV > SocketInfo->BytesSEND)
			//{
			//	char sockets[1024];
			//	sprintf_s(sockets, sizeof(sockets), "users=%d", SocketArray[1]->Socket);
			//	for (int i = 2; i < sizeof(SocketArray); i++) {
			//		if (SocketArray[i] == NULL)
			//			break;
			//		sprintf_s(sockets, sizeof(sockets), "%s&%d", sockets, SocketArray[i]->Socket);
			//	}
			//	/*SocketInfo->DataBuf.buf =
			//		SocketInfo->Buffer + SocketInfo->BytesSEND;*/
			//	SocketInfo->DataBuf.buf = sockets;
			//	SocketInfo->DataBuf.len = sizeof(sockets);
			//	for (int i = 1; i < sizeof(SocketArray); i++) {
			//		if (SocketArray[i] == NULL)
			//			break;
			//		if (WSASend(SocketArray[i]->Socket,
			//			&(SocketInfo->DataBuf), 1,
			//			&SendBytes, 0, NULL, NULL) ==
			//			SOCKET_ERROR)
			//		{
			//			if (WSAGetLastError() != WSAEWOULDBLOCK)
			//			{
			//				sprintf_s(Str, sizeof(Str),
			//					"WSASend() failed with "
			//					"error %d", WSAGetLastError());
			//				pLB->AddString(Str);
			//				FreeSocketInformation(
			//					Event - WSA_WAIT_EVENT_0, Str, pLB);
			//				return 1;
			//			}

			//			// Произошла ошибка WSAEWOULDBLOCK. 
			//			// Событие FD_WRITE будет отправлено, когда
			//			// в буфере будет больше свободного места
			//		}
			//	}
			//	SocketInfo->BytesSEND += SendBytes;

			//	/*if (SocketInfo->BytesSEND ==
			//		SocketInfo->BytesRECV)
			//	{*/
			//		SocketInfo->BytesSEND = 0;
			//		SocketInfo->BytesRECV = 0;
			//	//}
			//}
		}
		if (NetworkEvents.lNetworkEvents & FD_CLOSE)
		{
			if (NetworkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
			{
				sprintf_s(Str, sizeof(Str),
					"FD_CLOSE failed with error %d",
					NetworkEvents.iErrorCode[FD_CLOSE_BIT]);
				pLB->AddString(Str);
			}

			sprintf_s(Str, sizeof(Str),
				"Closing socket information %d",
				SocketArray[Event - WSA_WAIT_EVENT_0]->Socket);
			pLB->AddString(Str);
			FreeSocketInformation(Event - WSA_WAIT_EVENT_0,
				Str, pLB);
			sendUsers();
		}
   } // while
   return 0;
}
BOOL CreateSocketInformation(SOCKET s, char* Str,
	CListBox* pLB)
{
	LPSOCKET_INFORMATION SI;

	if ((EventArray[EventTotal] = WSACreateEvent()) ==
		WSA_INVALID_EVENT)
	{
		sprintf_s(Str, sizeof(Str),
			"WSACreateEvent() failed with error %d",
			WSAGetLastError());
		pLB->AddString(Str);
		return FALSE;
	}

	if ((SI = (LPSOCKET_INFORMATION)GlobalAlloc(GPTR,
		sizeof(SOCKET_INFORMATION))) == NULL)
	{
		sprintf_s(Str, sizeof(Str),
			"GlobalAlloc() failed with error %d",
			GetLastError());
		pLB->AddString(Str);
		return FALSE;
	}

	// Подготовка структуры SocketInfo для использования.
	SI->Socket = s;
	SI->BytesSEND = 0;
	SI->BytesRECV = 0;

	SocketArray[EventTotal] = SI;
	EventTotal++;
	return(TRUE);
}
void FreeSocketInformation(DWORD Event, char* Str,
	CListBox* pLB)
{
	LPSOCKET_INFORMATION SI = SocketArray[Event];
	DWORD i;

	closesocket(SI->Socket);
	GlobalFree(SI);
	WSACloseEvent(EventArray[Event]);

	// Сжатие массивов сокетов и событий

	for (i = Event; i < EventTotal; i++)
	{
		EventArray[i] = EventArray[i + 1];
		SocketArray[i] = SocketArray[i + 1];
	}

	EventTotal--;
}
