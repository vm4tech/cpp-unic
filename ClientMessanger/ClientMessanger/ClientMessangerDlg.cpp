
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
	//  Можно еще попробовать указатель на массив CString (CString* currentChat), но лучше примитивами, как по мне
	char currentChat[256][256];
};
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
	GetDlgItem(IDC_MESSAGE)->EnableWindow(false);
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

	// Получаем значения из полей, для подключения к порту и серверу
	GetDlgItem(IDC_SERVER)->GetWindowText(szServer, sizeof(szServer));
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
	// Формируем сообщение для сервера ( на сервере парсим все сообщения и уже принимаем решения)
	// шаблон: connect=user 
	sprintf_s(Str, sizeof(Str), "connect=%d", m_sClient);
	// Отправляем приветственное сообщение, чтобы у всех пользоваетелей обновился список "активных пользователей" (сервер отправляет ответный user=...)
	send(m_sClient, Str, strlen(Str), 0);
	// Запускает поток на прослушивание сокета, чтобы мы всегда смогли получить новые данные
	AfxBeginThread(Recv, NULL);

}

/* 
* Парсер сообщений от сервера. 
* Т.к. я передаю строчку в формате: команда=данные, то необходим парсер для корректного обмена информацией
*/
char** getParserMessage(char* buf) {
	char* params[2];
	char* parsed[10], * temp;
	// разбиваем на первые 2 части (до = и после)
	temp = strtok(buf, "=");
	int i = 0;
	while (temp != NULL)
	{
		parsed[i] = temp;
		parsed[i+1] = 0;
		params[i++] = temp;
		temp = strtok(NULL, "=");
	}
	// т.к. после = идет наша информация, то мы парсим их отдельно (они идут в формате data&data&...)
	temp = strtok(params[1], "&");
	i--;
	while (temp != NULL)
	{
		parsed[i++] = temp;
		temp = strtok(NULL, "&");
	}
	parsed[i] = '\0';
	// возвращаем итоговую ссылку
	return parsed;
}

// Парсер для списка пользователей (необходим для корректного получения socket из списка, т.к. в m_Users может быть 888 (я) или * (что символизирует непрочитанное сообщение))
char* parserUserListBox(char* help) {
	char* params[2];
	char* parsed[10], * temp;

	temp = strtok(help, " ");
	int i = 0;
	return temp;
}

// Удаляем юзера из нашего списка, т.к. он вышел из чата
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

// Обновляем список пользователей, т.к. пришло сообщение (функция очищает список пользователей и строит новый с пользователями, которые написали)
// Наверно, можно было бы сделать через получение конкретной строчки и замена там значения, но я изначально додумался только до этого варианта...
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

// Бесконечный прослушиватель, который ждет обновленную информацию на сокете
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
		// Получаем распарсенную строчку. В 0 элементе у нас строчка с коммандой
		parsedMess = getParserMessage(szBuffer);
		// Создаем массив пользователей, чтобы записать в неё все пришедшие данные с сервера
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

			// Проверяем пришедших пользователей с локальными (users).
			// В случае, если нет пользователя в локальном хранилище, то дополняем локальные данные новым пользователем.
			for (int i = 0;; i++) {
				int flag = 0;
				if (user[i][0] == '\0') {
					break;
				}
				for (int j = 0;; j++) {
					if (users[j].friendSocket[0] == '\0') {
						strcpy(users[j].friendSocket, user[i]);
						break;
					}
					else if (strcmp(users[j].friendSocket, user[i]) == 0) {
						break;
					}
				}
			}
			// Удаляем пользователей, которые вышли из чата
			// user - полученные данные, users - наши локальные данные
			// В случае, если есть пользователь в локальном хранилище, но пришедших данных нет, то удаляем из локального хранилища пользователя.
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
					// Функция удаления
					deleteUser(i);
				}
			}

			// обновляем CListBox, и отрисовываем всех пользователей заново
			m_Users.ResetContent();
			for (int i = 0; i < 256; i++) {
				if (users[i].friendSocket[0] != '\0')
					if (i == 0) {
						sprintf_s(Str, sizeof(Str), "%s (я)", users[i].friendSocket);
						strcpy(currentUser.friendSocket, users[i].friendSocket);
						m_Users.AddString(Str);
					}
					else
						m_Users.AddString(users[i].friendSocket);
				else
					break;


			}
		}
		// Если пришло сообщение (формат сообщения - message=text&from&to)
		else if (strcmp(parsedMess[0], "message") == 0){
			// Проверяем если у нас открыт диалог не с тем человеком, кто отправил сообщение (необходимо для того, чтобы отрисовывались сообщения от других пользователей)
			if (strcmp(currentUser.friendSocket, parsedMess[2]) != 0) {
				// Находим в локальном хранилище массив с текстом, и записываем в конец пришедшее сообщение
				for (int i = 0;; i++) {
					if (users[i].friendSocket[0] != '\0') {
						// Можно оптимизировать, путем запоминания индекса пользователя в списке, но нужно больше времени для разработки...
						if (strcmp(users[i].friendSocket, parsedMess[2]) == 0){
							// высчитываем количество сообщений до этого... Скорее всего, это кривой способ((
							int size = 0;
							for (int k = 0; k < 256; k++){
								if (users[i].currentChat[k][0] == '\0') {
									size = k;
									break;
								}
							}
							// запоминаю ссылку на нашего автора сообщения, т.к. после следующей строчки с ссылками что-то происходит, видимо перезаписываются или что-то...
							char *help = parsedMess[2];
							sprintf_s(Str, sizeof(Str), "friend:%s", CString(parsedMess[1]));
							// Добавляем в последнюю строчку сообщение
							strcpy(users[i].currentChat[size], Str);
							// Формируем строчку для функции обновления списка пользователей (чтобы поставилось *)
							strcpy(Str, CString(help));
							updateMessage(Str);
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
					
					m_ListBox.GetText(j, copy);
					strcpy(users[i].currentChat[j], copy);
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
	// Если не выбрал пользователя
	if (index == -1)
		return;
	GetDlgItem(IDC_SEND)->EnableWindow(true);
	GetDlgItem(IDC_MESSAGE)->EnableWindow(true);
	m_Users.GetText(index, help);
	// Копируем значение, т.к. GetText копирует значение в буфер
	parsered = parserUserListBox(help);

	for (int i = 0; i < sizeof(parsered); i++) {
		friendSocket[i] = parsered[i];
	}
	if (index != 0) {
		m_Users.DeleteString(index);
		m_Users.InsertString(index, friendSocket);
	}
	
	// Если пользователь не только зашел в приложение (почти всегда true, т.к. позже изменил подход определения текущего пользователя, но страшно убирать)
	if (currentUser.friendSocket[0] != '\0') {
		// Если пользователь не выбрал снова того же друга (иначе зачем тратить время на отравку, перенос листбокса)
		if (strcmp(currentUser.friendSocket, friendSocket) != 0) {
			setInMemoryListBox();
			setFromMemoryListBox(friendSocket);			
			strcpy(currentUser.friendSocket, friendSocket);
		}
	}
	else {
		// Ставим пользователя
		strcpy(currentUser.friendSocket, friendSocket);
	}
	// Изменяем текст над листбоксом сообщений (диалог с: *)
	CWnd* label = GetDlgItem(IDC_TEXT_USER);
	label->SetWindowTextA(friendSocket);
}