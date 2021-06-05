
// ClientMessangerDlg.cpp : implementation file
//
/*
* TODO: 
* 1. ������� ������� req/res
* 2. ������� ����� ������������
* 3. ������� �������� ���������
* 4. �������� ��������� ����
* 5. ���������� ��������
* 6. fix ��������
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
// ������� �� ���������� ��������
struct ChatUser {
	char friendSocket[6];
	CListBox currentChat;
}ChatUser;
// ����� �����
char friendSocket[6];

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
}


void CClientMessangerDlg::OnBnClickedConnect()
{
	// TODO: Add your control notification handler code here
	char		szServer[128];	// ��� ��� IP-����� �������
	int		iPort;			// ����

	WSADATA	wsd;

	struct sockaddr_in 	server;
	struct hostent* host = NULL;

	char Str[256];

	/*GetDlgItem(IDC_SERVER)->GetWindowText(szServer, sizeof(szServer));
	GetDlgItem(IDC_PORT)->GetWindowText(Str, sizeof(Str));
	iPort = atoi(Str);*/
	//https://github.com/MaseDar/cpp-unic/commit/5158d89cfb96541d3939562612b3e59ebf24f944

	//TODO: ����� �������, ����� �� ������ �� ������� ���� (�� ��� �� ������)
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
	// 
	sprintf_s(Str, sizeof(Str), "connect=%d", m_sClient);
	// ���������� �������������� ���������, ����� � ���� �������������� ��������� ������ "�������� �������������"
	send(m_sClient, Str, strlen(Str), 0);
	// ��������� ����� �� ������������� ������ �� �������
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
		// ������� � m_Users ������ ������������� (������ �� ������ ������ �����������, �.�. ��-������� ������-�� ��� ��������... ������ �����, ��-�� ����, ��� ��������� ����������������, � ���-�� �� ������������)
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
			// ��������� ������������ ������ �����
			m_Users.ResetContent();
			for (int i = 0; i < 256; i++) {
				if (user[i][0] != '\0')
					m_Users.AddString(user[i]);
				else
					break;
			}

			/*for (int i = 1; i < sizeof(parsedMess); i++) {
				strcpy(Str, parsedMess[i]);
				m_Users.AddString(Str);
			}*/
		}
		else if (strcmp(parsedMess[0], "message") == 0){
			sprintf_s(Str, sizeof(Str), "friend: %s", CString(parsedMess[1]));
			m_ListBox.AddString(Str);
		}
		/*sprintf_s(Str, sizeof(Str), "response from server: %s", szBuffer);*/
	/*	m_ListBox.AddString((LPTSTR)Str);*/
	}
	return 0;
}

void CClientMessangerDlg::OnBnClickedSend()
{
	// TODO: Add your control notification handler code here
	char	szMessage[1024];		// ��������� ��� ��������
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

	// �������� ������
	
	sprintf_s(Str, sizeof(Str), "message=%s&%d&%s", CString(szMessage), m_sClient, friendSocket);
	
	ret = send(m_sClient, Str, strlen(Str), 0);
	sprintf_s(Str, sizeof(Str), "You:%s", CString(szMessage));
	m_ListBox.AddString(Str);
	if (ret == SOCKET_ERROR)
	{
		sprintf_s(Str, sizeof(Str), "send() failed: %d", WSAGetLastError());
		m_ListBox.AddString((LPTSTR)Str);
	}

	/*sprintf_s(Str, sizeof(Str), "Send %d bytes\n", ret);
	m_ListBox.AddString((LPTSTR)Str);*/

}

void CClientMessangerDlg::OnBnClickedUser()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_SEND)->EnableWindow(true);
	int index;
	CString strText;
	char help[6];
	index = m_Users.GetCurSel();
	m_Users.GetText(index, help);
	m_ListBox.ResetContent();
	//sprintf_s(Str, sizeof(Str), "��� � �������������: %s", friendSocket);
	// �������� ��������, �.�. GetText �������� �������� � �����
	strcpy(friendSocket, help);
	m_ListBox.AddString(friendSocket);

}