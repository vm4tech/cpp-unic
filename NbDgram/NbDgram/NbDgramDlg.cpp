
// NbDgramDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NbDgram.h"
#include "NbDgramDlg.h"
#include "afxdialogex.h"
#include "Nb30.h"

#include"nbcommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_SESSIONS            254
#define MAX_NAMES               254
#define MAX_DATAGRAM_SIZE       512

BOOL   bSender = FALSE,		// Прием или отправка
bRecvAny = FALSE,		// Принимать с любого имени
bUniqueName = TRUE,	// Уникальное или групповое имя
bBroadcast = FALSE,	// Отправлять широковещательно
bOneLana = FALSE;		// Использовать один/все LANA

char   szLocalName[NCBNAMSZ + 1],		// Свое NetBIOS имя
szRecipientName[NCBNAMSZ + 1];	// Имя приемника
DWORD  dwNumDatagrams = 25,	// Число отправляемых дейтаграмм
dwOneLana,			// Номер LANA, если конкретный
dwDelay = 0;			// Задержка между отправками

HWND   hDlg;
HWND   hWnd_LB;		// Дескриптор окна вывода для потоков

int DatagramSend(int lana, int num, char *recipient, char *buffer, int buflen);
int DatagramSendBC(int lana, int num, char *buffer, int buflen);
int DatagramRecv(PNCB pncb, int lana, int num, char *buffer, int buflen, HANDLE hEvent);
int DatagramRecvBC(PNCB pncb, int lana, int num, char *buffer, int buflen, HANDLE hEvent);

UINT MainThread(PVOID lpParam);

// CNbDgramDlg dialog

CNbDgramDlg::CNbDgramDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NBDGRAM_DIALOG, pParent)
	, m_Sender(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNbDgramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_SENDER, m_Sender);
}

BEGIN_MESSAGE_MAP(CNbDgramDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SENDER, &CNbDgramDlg::OnClickedSender)
	ON_COMMAND(IDC_RECEIVER, &CNbDgramDlg::OnReceiver)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDatagramDlg methods

void CNbDgramDlg::SetUI(bool IsSender)
{
	GetDlgItem(IDC_DELAY)->EnableWindow(IsSender);
	GetDlgItem(IDC_RECV_ANY)->EnableWindow(!IsSender);
	GetDlgItem(IDC_RECEIVER_NAME)->EnableWindow(IsSender);
}

// CNbDgramDlg message handlers

BOOL CNbDgramDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	if (bOneLana)
	{
		((CButton *)GetDlgItem(IDC_UNIQUE_LANA))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_ALL_LANA))->SetCheck(0);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_UNIQUE_LANA))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_ALL_LANA))->SetCheck(1);
	}

	if (bUniqueName)
	{
		((CButton *)GetDlgItem(IDC_UNIQUE_NAME))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_GROUP_NAME))->SetCheck(0);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_UNIQUE_NAME))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_GROUP_NAME))->SetCheck(1);
	}

	if (bBroadcast)
	{
		((CButton *)GetDlgItem(IDC_BROADCAST))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_ONE_RECEIVER))->SetCheck(0);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_BROADCAST))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_ONE_RECEIVER))->SetCheck(1);
	}

	if (bSender)
	{
		m_Sender = 0;
		((CButton *)GetDlgItem(IDC_SENDER))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_RECEIVER))->SetCheck(0);
		SetUI(true);
	}
	else
	{
		m_Sender = 1;
		((CButton *)GetDlgItem(IDC_SENDER))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_RECEIVER))->SetCheck(1);
		SetUI(false);
	}

	if (bRecvAny)
		((CButton *)GetDlgItem(IDC_RECV_ANY))->SetCheck(1);
	else
		((CButton *)GetDlgItem(IDC_RECV_ANY))->SetCheck(0);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNbDgramDlg::OnPaint()
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
HCURSOR CNbDgramDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CNbDgramDlg::OnClickedSender()
{
	// TODO: Add your control notification handler code here
	BOOL last = bSender;

	UpdateData();
	bSender = ((m_Sender == 0) ? TRUE : FALSE);
	if (bSender != last)
		SetUI(true); // Sender
}


void CNbDgramDlg::OnReceiver()
{
	// TODO: Add your command handler code here
	BOOL last = bSender;

	UpdateData();
	bSender = ((m_Sender == 1) ? FALSE : TRUE);
	if (bSender != last)
		SetUI(false); // Reciever
}
