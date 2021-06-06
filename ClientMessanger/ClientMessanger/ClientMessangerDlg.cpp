
// ClientMessangerDlg.cpp : implementation file
//
/*
* TODO: 
* 1. Сделать парсинг req/res
* 2. Сделать вывод пользовтелей
* 3. Сделать отправку сообщений
* 4. Отправку сообщений всем
* 5. Сохранение диалогов
* 6. fix костылей
*/

#include "pch.h"
#include "framework.h"
#include "ClientMessanger.h"
#include "ClientMessangerDlg.h"
#include "afxdialogex.h"
#include <winsock2.h>
#define DEFAULT_COUNT	1
#define DEFAULT_PORT	5150
#define DEFAULT_BUFFER	2048
#define DEFAULT_MESSAGE	"Hey bro!"

UINT Recv(LPVOID pParam);
SOCKET m_sClient;
CListBox m_ListBox;
CListBox m_Users;

// CClientMessangerDlg dialog
struct Users {
	char friendSocket[6];
	// Надо вместо ЛИСТБОКСА хранить просто массив строк
	/*CListBox *currentChat;*/
	/*char *currentChat[256];*/
	//  Можно еще попробовать указатель на массив CString (CString* currentChat), но лучше примитивами, как по мне
	char currentChat[256][256];
};
CWnd* parent;
// номер друга
char friendSocket[6];
// Текущий юзер
Users currentUser;
// Список юзеров
Users users[256];

CClientMessangerDlg::CClientMessangerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENTMESSANGER_DIALOG, pParent)
	, m_Number(DEFAULT_COUNT)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	parent = pParent;
}

void CClientMessangerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTBOX, m_ListBox);
	DDX_Control(pDX, IDC_USERS, m_Users);
}

BEGIN_MESSAGE_MAP(CClientMessangerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, &CClientMessangerDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_SEND, &CClientMessangerDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_USER, &CClientMessangerDlg::OnBnClickedUser)
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
	GetDlgItem(IDC_SEND)->EnableWindow(false);
	GetDlgItem(IDC_SERVER)->SetWindowText("localhost");
	sprintf_s(Str, sizeof(Str), "%d", DEFAULT_PORT);
	GetDlgItem(IDC_PORT)->SetWindowText(Str);
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
	GetDlgItem(IDC_USER)->EnableWindow(IsConnected);
	GetDlgItem(IDC_MESSAGE)->EnableWindow(IsConnected);
	GetDlgItem(IDC_CONNECT)->EnableWindow(!IsConnected);
	GetDlgItem(IDC_SERVER)->EnableWindow(!IsConnected);
	GetDlgItem(IDC_PORT)->EnableWindow(!IsConnected);
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
	GetDlgItem(IDC_PORT)->GetWindowText(Str, sizeof(Str));
	iPort = atoi(Str);*/
	//https://github.com/MaseDar/cpp-unic/commit/5158d89cfb96541d3939562612b3e59ebf24f944

	//TODO: Потом сделать, чтобы не только на локалке было (ну это не трудно)
	GetDlgItem(IDC_SERVER)->GetWindowText(szServer, sizeof(szServer));
	/*strcpy(szServer, "localhost");*/
	/*iPort = DEFAULT_PORT;*/
	GetDlgItem(IDC_PORT)->GetWindowText(Str, sizeof(Str));
	iPort = atoi(Str);
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
	// 
	sprintf_s(Str, sizeof(Str), "connect=%d", m_sClient);
	// Отправляем приветственное сообщение, чтобы у всех пользоваетелей обновился список "активных пользователей"
	send(m_sClient, Str, strlen(Str), 0);
	// Запускает поток на прослушивание сокета на сервере
	AfxBeginThread(Recv, NULL);
}

char** getParserMessage(char* buf) {
	char* params[2];
	char* parsed[10], * temp;

	temp = strtok(buf, "=");
	int i = 0;
	while (temp != NULL)
	{
		parsed[i] = temp;
		parsed[i+1] = 0;
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

char* parserUserListBox(char* help) {
	char* params[2];
	char* parsed[10], * temp;

	temp = strtok(help, " ");
	int i = 0;

	/*temp = strtok(params[1], "&");
	i--;
	while (temp != NULL)
	{
		parsed[i++] = temp;
		temp = strtok(NULL, "&");
	}
	parsed[i] = '\0';*/

	return temp;
}
// Удаляем юзера из нашего списка
void deleteUser(int k) {
	for (int i = k;; i++) {
		if (users[i].friendSocket[0] == '\0')
			break;
		strcpy(users[i].friendSocket, users[i+1].friendSocket);
		for (int j = 0; j < 256; j++) {
			if (users[i+1].currentChat[j][0] == '\0') {
				users[i].currentChat[j][0] = '\0';
				break;
			}
			strcpy(users[i].currentChat[j], users[i+1].currentChat[j]);
		}
	}
}

void updateMessage(char* from) {
	char Str[256];
	m_Users.ResetContent();
	for (int i = 0; i < 256; i++) {
		if (users[i].friendSocket[0] != '\0') {
			if (i == 0) {
				sprintf_s(Str, sizeof(Str), "%s (я)", users[i].friendSocket);
				m_Users.AddString(Str);
			}
			else if (strcmp(users[i].friendSocket, from) == 0) {
				sprintf_s(Str, sizeof(Str), "%s *", users[i].friendSocket);
				m_Users.AddString(Str);
			}
			else
				m_Users.AddString(users[i].friendSocket);
		}
			else
				break;
	}
}

UINT Recv(LPVOID pParam) {
	char	szBuffer[DEFAULT_BUFFER];
	int ret;
	char** parsedMess;
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
		parsedMess = getParserMessage(szBuffer);
		char user[256][6];
		// выводим в m_Users список пользователей (только мы делаем ручное копирование, т.к. по-другому почему-то все портится... Скорее всего, из-за того, что указатели перезаписываются, и что-то не отрабатывает)
		if (strcmp(parsedMess[0], "users") == 0) {
			
			for (int i = 1;; i++) {
				if (parsedMess[i] != NULL) {
					for (int j = 0; j < 6; j++) {
						if (parsedMess[i][j] != '\0')
							user[i-1][j] = parsedMess[i][j];
						else {
							user[i-1][j] = '\0';
							break;
						}
					}

				}
				else {
					user[i-1][0] = '\0';
					break;
				}
			}
			// Добавляем пользовтелей именно здесь
			for (int i = 0;; i++) {
				int flag = 0;
				if (user[i][0] == '\0') {
					break;
				}
				for (int j = 0;; j++) {
					if (users[j].friendSocket[0] == '\0') {
						strcpy(users[j].friendSocket, user[i]);
						strcpy(users[j].currentChat[0], users[j].friendSocket);
						break;
					}
					else if (strcmp(users[j].friendSocket, user[i]) == 0) {
						break;
						///*currentUser.friendSocket = users[j].friendSocket;
						//currentUser.currentChat = users[j].currentChat;*/
						//flag = 1;
						//break;
					}
				}
			}
			// Удаляем пользователей, которые вышли из чата
			// user - полученные данные, users - наши локальные данные
			for (int i = 0;; i++) {
				if (users[i].friendSocket[0] == '\0')
					break;
				int flag = 0;
				for (int j = 0;; j++) {
					if (user[j][0] == '\0')
						break;
					if (strcmp(users[i].friendSocket, user[j]) == 0) {
						flag++;
					}
				}
				if (flag == 0) {
					deleteUser(i);
				}
			}

			// обновляем CListBox
			m_Users.ResetContent();
			for (int i = 0; i < 256; i++) {
				if (users[i].friendSocket[0] != '\0')
					if (i == 0) {
						sprintf_s(Str, sizeof(Str), "%s (я)", users[i].friendSocket);
						m_Users.AddString(Str);
					}
					else
						m_Users.AddString(users[i].friendSocket);
				else
					break;


			}

			/*for (int i = 1; i < sizeof(parsedMess); i++) {
				strcpy(Str, parsedMess[i]);
				m_Users.AddString(Str);
			}*/
		}
		else if (strcmp(parsedMess[0], "message") == 0){
			// т.е. если у нас открыт диалог не с тем человеком, кто отправил сообщение
			if (strcmp(currentUser.friendSocket, parsedMess[2]) != 0) {
				// находим ту самую историю и записываем туда сообщение
				for (int i = 0;; i++) {
					if (users[i].friendSocket[0] != '\0') {
						// Можно оптимизировать, путем запоминания индекса пользователя в списке, но нужно больше времени для разработки...
						if (strcmp(users[i].friendSocket, parsedMess[2]) == 0){
							// высчитываем количество сообщений до этого
							int size = 0;
							for (int k = 0; k < 256; k++){
								if (users[i].currentChat[k][0] == '\0') {
									size = k;
									break;
								}
							}
							char *help = parsedMess[2];
							sprintf_s(Str, sizeof(Str), "friend:%s", CString(parsedMess[1]));
							strcpy(users[i].currentChat[size], Str);
							
							strcpy(Str, CString(help));
							/*sprintf_s(help, sizeof(help), "friend:%s", parsedMess[2]);*/
							updateMessage(Str);
							/*sprintf_s(Str, sizeof(Str), "Вам сообщение от %s:%s", CString(parsedMess[2]), CString(parsedMess[1]));*/
							/*CString str;
							str.Format(_T("%s"), Str);
							MessageBox(str);*/
							break;
						}
					}
					else
						break;
				}
			}
			else {
				sprintf_s(Str, sizeof(Str), "friend: %s", CString(parsedMess[1]));
				m_ListBox.AddString(Str);
			}
			
		}
		/*sprintf_s(Str, sizeof(Str), "response from server: %s", szBuffer);*/
	/*	m_ListBox.AddString((LPTSTR)Str);*/
	}
	return 0;
}

void CClientMessangerDlg::OnBnClickedSend()
{
	// TODO: Add your control notification handler code here
	char	szMessage[1024];		// Сообщение для отправки
	char	szBuffer[DEFAULT_BUFFER];
	int		ret, i;

	char	Str[256];

	UpdateData();
	if (m_Number < 1 || m_Number>20)
	{
		m_ListBox.AddString("Number of messages must be between 1 and 20");
		return;
	}

	GetDlgItem(IDC_MESSAGE)->GetWindowText(szMessage, sizeof(szMessage));

	// Отправка данных
	if (szMessage[0] == '\0')
		return;
	int size = sprintf_s(Str, sizeof(Str), "message=%s&%d&%s", CString(szMessage), m_sClient, currentUser.friendSocket);
	Str[size] = '\0';
	ret = send(m_sClient, Str, size, 0);
	sprintf_s(Str, sizeof(Str), "You:%s", CString(szMessage));
	m_ListBox.AddString(Str);
	if (ret == SOCKET_ERROR)
	{
		sprintf_s(Str, sizeof(Str), "send() failed: %d", WSAGetLastError());
		m_ListBox.AddString((LPTSTR)Str);
	}
	GetDlgItem(IDC_MESSAGE)->SetWindowTextA("");
	/*sprintf_s(Str, sizeof(Str), "Send %d bytes\n", ret);
	m_ListBox.AddString((LPTSTR)Str);*/
}

// Устанавливает/переносит листбокс в память для конкретного пользователя (при смене пользователей)
void setInMemoryListBox() {
	m_ListBox.AddString(friendSocket);
	for (int i = 0;; i++) {
		if (users[i].friendSocket[0] != '\0') {
			// Можно оптимизировать, путем запоминания индекса пользователя в списке, но нужно больше времени для разработки...
			if (strcmp(users[i].friendSocket, currentUser.friendSocket) == 0) {
				/*users[i].currentChat->ResetContent();*/
				// очищаем весь список
				for (int k = 0; i < 256; k++) {
					if (users[i].currentChat[k][0] == '\0')
						break;
					users[i].currentChat[k][0] = '\0';
				}
				// копируем список
				for (int j = 0; j < m_ListBox.GetCount(); j++) {
					char copy[256];
					char help[256];
					/*currentUser.currentChat->GetText(i, copy);
					users[i].currentChat->AddString(copy);*/
					m_ListBox.GetText(j, copy);
			/*			strcpy(help, copy);*/
					strcpy(users[i].currentChat[j], copy);
					
					// мб будут проблемы с ууказателем
					/*users[i].currentChat[j] = setMessages(users[i].currentChat[j], copy);*/
					
				}
				break;
			}
		}
		else
			break;
	}
	
}

// Копируем ИЗ памяти в сам листбокс для отображения сообщений
void setFromMemoryListBox(char* friendSocket2) {
	for (int i = 0;; i++) {
		if (users[i].friendSocket[0] != '\0') {
			// Можно оптимизировать, путем запоминания индекса пользователя в списке, но нужно больше времени для разработки...
			if (strcmp(users[i].friendSocket, friendSocket2) == 0) {
				m_ListBox.ResetContent();
				// копируем список
				for (int j = 0; j < 256; j++) {
					CString copy = "";
					if (users[i].currentChat[j][0] == '\0')
						break;
					m_ListBox.AddString(users[i].currentChat[j]);
				}
				break;
			}
		}else
			break;
	}
}

void CClientMessangerDlg::OnBnClickedUser()
{
	// TODO: Add your control notification handler code here
	

	int index;
	CString strText;
	char help[25];
	char *parsered;
	index = m_Users.GetCurSel();
	// Надо сделать предупреждение
	if (index == -1)
		return;
	GetDlgItem(IDC_SEND)->EnableWindow(true);
	m_Users.GetText(index, help);
	/*m_ListBox.ResetContent();*/
	//sprintf_s(Str, sizeof(Str), "Чат с пользователем: %s", friendSocket);
	// Копируем значение, т.к. GetText копирует значение в буфер
	/*strcpy(friendSocket, help);*/
	parsered = parserUserListBox(help);

	for (int i = 0; i < sizeof(parsered); i++) {
		friendSocket[i] = parsered[i];
	}
	if (index != 0) {
		m_Users.DeleteString(index);
		m_Users.InsertString(index, friendSocket);
	}
	
	// Если не только зашел в приложение
	if (currentUser.friendSocket[0] != '\0') {
		// Если пользователь не выбрал снова того же друга
		if (strcmp(currentUser.friendSocket, friendSocket) != 0) {
			setInMemoryListBox();
			setFromMemoryListBox(friendSocket);
		/*	m_ListBox.AddString(friendSocket);*/
			// Если не новая переписка
			/*if (m_ListBox.GetCount() != 0) {

				for (int i = 0; i < currentUser.currentChat.GetCount(); i++) {

				}
			}
			else {

			}*/
			
			strcpy(currentUser.friendSocket, friendSocket);
		}
	}
	else {
		strcpy(currentUser.friendSocket, friendSocket);
		
		/*currentUser.currentChat->AddString((LPCTSTR)"firstMessage");*/
	}
	CWnd* label = GetDlgItem(IDC_TEXT_USER);
	label->SetWindowTextA(friendSocket);
}