
// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <winsock2.h>

#define PORT 5150			// Порт по умолчанию
#define DATA_BUFSIZE 8192 	// Размер буфера по умолчанию

int  iPort = PORT; 	 // Порт для прослушивания подключений
bool bPrint = false; // Выводить ли сообщения клиентов
typedef struct _SOCKET_INFORMATION {
	OVERLAPPED Overlapped;
	SOCKET Socket;
	CHAR Buffer[DATA_BUFSIZE];
	WSABUF DataBuf;
	DWORD BytesSEND;
	DWORD BytesRECV;
} SOCKET_INFORMATION, * LPSOCKET_INFORMATION;

void CALLBACK WorkerRoutine(DWORD Error,
	DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped,
	DWORD InFlags);

UINT   WorkerThread(LPVOID lpParameter);

SOCKET AcceptSocket;
HWND   hWnd_LB;  // Для вывода в других потоках

UINT   ListenThread(PVOID lpParam);

bool   fAcceptedSocketProcessed;

// CServerDlg dialog

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
	ON_BN_CLICKED(IDC_PRINT, &CServerDlg::OnBnClickedPrint)
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
	char Str[128];

	sprintf_s(Str, sizeof(Str), "%d", iPort);
	GetDlgItem(IDC_PORT)->SetWindowText(Str);

	if (bPrint)
		((CButton *)GetDlgItem(IDC_PRINT))->SetCheck(1);
	else
		((CButton *)GetDlgItem(IDC_PRINT))->SetCheck(0);

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



void CServerDlg::OnBnClickedPrint()
{
	// TODO: Add your control notification handler code here
	if (((CButton *)GetDlgItem(IDC_PRINT))->GetCheck() == 1)
		bPrint = true;
	else
		bPrint = false;
}


void CServerDlg::OnBnClickedStart()
{
	// TODO: Add your control notification handler code here
	char Str[81];

	hWnd_LB = m_ListBox.m_hWnd; // Для вывода в других потоках
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
UINT ListenThread(PVOID lpParam)
{
	WSADATA wsaData;
	SOCKET ListenSocket;
	SOCKADDR_IN InternetAddr;
	INT Ret;
	WSAEVENT AcceptEvent;

	char  Str[200];
	CListBox* pLB =
		(CListBox*)(CListBox::FromHandle(hWnd_LB));

	if ((Ret = WSAStartup(0x0202, &wsaData)) != 0)
	{
		sprintf_s(Str, sizeof(Str),
			"WSAStartup failed with error %d", Ret);
		pLB->AddString(Str);
		WSACleanup();
		return 1;
	}
	if ((ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0,
		NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		sprintf_s(Str, sizeof(Str),
			"Failed to get a socket %d",
			WSAGetLastError());
		pLB->AddString(Str);
		return 1;
	}

	InternetAddr.sin_family = AF_INET;
	InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	InternetAddr.sin_port = htons(iPort);

	if (bind(ListenSocket, (PSOCKADDR)&InternetAddr,
		sizeof(InternetAddr)) == SOCKET_ERROR)
	{
		sprintf_s(Str, sizeof(Str),
			"bind() failed with error %d",
			WSAGetLastError());
		pLB->AddString(Str);
		return 1;
	}

	if (listen(ListenSocket, 5))
	{
		sprintf_s(Str, sizeof(Str),
			"listen() failed with error %d",
			WSAGetLastError());
		pLB->AddString(Str);
		return 1;
	}
	if ((AcceptEvent = WSACreateEvent()) ==
		WSA_INVALID_EVENT)
	{
		sprintf_s(Str, sizeof(Str),
			"WSACreateEvent() failed with error %d",
			WSAGetLastError());
		pLB->AddString(Str);
		return 1;
	}

	// Создание рабочего потока для обслуживания 
	// завершенных запросов ввода/вывода

	if (AfxBeginThread(WorkerThread, (LPVOID)AcceptEvent) ==
		NULL)
	{
		sprintf_s(Str, sizeof(Str),
			"AfxBeginThread failed with error %d",
			GetLastError());
		pLB->AddString(Str);
		return 1;
	}
	fAcceptedSocketProcessed = true;

	while (TRUE)
	{
		// Обработано ли предыдущее соединение
		if (!fAcceptedSocketProcessed)
		{
			Sleep(0); // Переключение на другой поток
			continue;
		}

		AcceptSocket = accept(ListenSocket, NULL, NULL);

		if (WSASetEvent(AcceptEvent) == FALSE)
		{
			sprintf_s(Str, sizeof(Str),
				"WSASetEvent failed with error %d",
				WSAGetLastError());
			pLB->AddString(Str);
			return 1;
		}
	}
	return 0;
}
UINT WorkerThread(LPVOID lpParameter)
{
	DWORD Flags;
	LPSOCKET_INFORMATION SocketInfo;
	WSAEVENT EventArray[1];
	DWORD Index;
	DWORD RecvBytes;

	char  Str[200];
	CListBox* pLB =
		(CListBox*)(CListBox::FromHandle(hWnd_LB));

	// Сохранение события accept в массиве
	EventArray[0] = (WSAEVENT)lpParameter;
	while (TRUE)
	{
		// Ждем пока сработает событие для accept()
		while (TRUE)
		{
			Index = WSAWaitForMultipleEvents(1, EventArray,
				FALSE, WSA_INFINITE, TRUE);

			if (Index == WSA_WAIT_FAILED)
			{
				sprintf_s(Str, sizeof(Str),
					"WSAWaitForMultipleEvents failed "
					"with error %d", WSAGetLastError());
				pLB->AddString(Str);
				return FALSE;
			}

			if (Index != WAIT_IO_COMPLETION)
			{
				// Событие для accept() произошло – 
				// прерываем цикл ожидания
				break;
			}
		}
		WSAResetEvent(EventArray[Index - WSA_WAIT_EVENT_0]);

		// Создаем структуру с информацией о сокете
		// и связываем ее с сокетом, созданным через 
		// вызов accept()

		if ((SocketInfo = (LPSOCKET_INFORMATION)
			GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION)))
			== NULL)
		{
			sprintf_s(Str, sizeof(Str),
				"GlobalAlloc() failed with error %d",
				GetLastError());
			pLB->AddString(Str);
			return FALSE;
		}

		// Заполняем структуру информацией о сокете

		SocketInfo->Socket = AcceptSocket;
		ZeroMemory(&(SocketInfo->Overlapped),
			sizeof(WSAOVERLAPPED));
		SocketInfo->BytesSEND = 0;
		SocketInfo->BytesRECV = 0;
		SocketInfo->DataBuf.len = DATA_BUFSIZE;
		SocketInfo->DataBuf.buf = SocketInfo->Buffer;

		Flags = 0;
		if (WSARecv(SocketInfo->Socket,
			&(SocketInfo->DataBuf), 1, &RecvBytes, &Flags,
			&(SocketInfo->Overlapped), WorkerRoutine) ==
			SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				sprintf_s(Str, sizeof(Str),
					"WSARecv() failed with error %d",
					WSAGetLastError());
				pLB->AddString(Str);
				return FALSE;
			}
		}

		sprintf_s(Str, sizeof(Str),
			"Socket %d connected", AcceptSocket);
		pLB->AddString(Str);

		// Информация из переменной AcceptSocket запомнена
		fAcceptedSocketProcessed = true;
	} // while
	return TRUE;
}
void CALLBACK WorkerRoutine(DWORD Error,
	DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped,
	DWORD InFlags)
{
	DWORD SendBytes, RecvBytes;
	DWORD Flags;

	char  Str[200];
	static  CListBox* pLB =
		(CListBox*)(CListBox::FromHandle(hWnd_LB));

	// Ссылка на структуру WSAOVERLAPPED как 
	// на структуру SOCKET_INFORMATION
	LPSOCKET_INFORMATION SI =
		(LPSOCKET_INFORMATION)Overlapped;
	if (Error != 0)
	{
		sprintf_s(Str, sizeof(Str),
			"I/O operation failed with error %d", Error);
		pLB->AddString(Str);
	}

	if (BytesTransferred == 0)
	{
		sprintf_s(Str, sizeof(Str),
			"Closing socket %d", SI->Socket);
		pLB->AddString(Str);
	}

	if (Error != 0 || BytesTransferred == 0)
	{
		closesocket(SI->Socket);
		GlobalFree(SI);
		return;
	}
	// Проверим, что поле BytesRECV равно нулю. 
   // Это означает, вызов WSARecv только что завершен.
   // В этом случае заполняем BytesRECV значением
   // BytesTransferred, полученным через вызов WSARecv.

	if (SI->BytesRECV == 0)
	{
		SI->BytesRECV = BytesTransferred;
		SI->BytesSEND = 0;

		// Печать сообщения, если требуется
		if (bPrint)
		{
			unsigned l = sizeof(Str) - 1;
			if (l > BytesTransferred) l = BytesTransferred;
			strncpy_s(Str, SI->Buffer, l);
			Str[l] = 0;
			pLB->AddString(Str);
		}
	}
	else
	{
		SI->BytesSEND += BytesTransferred;
	}
	if (SI->BytesRECV > SI->BytesSEND)
	{
		// Посылка еще одного запроса WSASend().
		// Поскольку WSASend() не гарантирует отправку
		// полностью, продолжим посылать запросы WSASend(),
		// пока вся информация не будет отправлена.

		ZeroMemory(&(SI->Overlapped), sizeof(WSAOVERLAPPED));

		SI->DataBuf.buf = SI->Buffer + SI->BytesSEND;
		SI->DataBuf.len = SI->BytesRECV - SI->BytesSEND;

		if (WSASend(SI->Socket, &(SI->DataBuf), 1, &SendBytes,
			0, &(SI->Overlapped), WorkerRoutine) ==
			SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				sprintf_s(Str, sizeof(Str),
					"WSASend() failed with error %d",
					WSAGetLastError());
				pLB->AddString(Str);
				return;
			}
		}
	}
	else
	{
		SI->BytesRECV = 0;

		// Больше посылать нечего.
		// Поэтому выдаем запрос WSARecv().

		Flags = 0;
		ZeroMemory(&(SI->Overlapped), sizeof(WSAOVERLAPPED));

		SI->DataBuf.len = DATA_BUFSIZE;
		SI->DataBuf.buf = SI->Buffer;

		if (WSARecv(SI->Socket, &(SI->DataBuf), 1, &RecvBytes,
			&Flags, &(SI->Overlapped), WorkerRoutine)
			== SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				sprintf_s(Str, sizeof(Str),
					"WSARecv() failed with error %d",
					WSAGetLastError());
				pLB->AddString(Str);
				return;
			}
		}
	}
}
