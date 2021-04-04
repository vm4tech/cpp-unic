
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
	, m_Delay(0)
	, m_LanaNumber(0)
	, m_Mes_Number(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNbDgramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_SENDER, m_Sender);
	DDX_Control(pDX, IDC_LISTBOX, m_ListBox);
	DDX_Text(pDX, IDC_DELAY, m_Delay);
	DDX_Text(pDX, IDC_LANA_NUMBER, m_LanaNumber);
	DDX_Text(pDX, IDC_NUMBER_MES, m_Mes_Number);
}

BEGIN_MESSAGE_MAP(CNbDgramDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SENDER, &CNbDgramDlg::OnClickedSender)
	ON_COMMAND(IDC_RECEIVER, &CNbDgramDlg::OnReceiver)
	ON_BN_CLICKED(IDC_START, &CNbDgramDlg::OnClickedStart)
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
void CNbDgramDlg::RefreshValues()
{
	UpdateData();
	GetDlgItem(IDC_NAME)->GetWindowText(szLocalName,
		NCBNAMSZ);
	GetDlgItem(IDC_RECEIVER_NAME)->
		GetWindowText(szRecipientName, NCBNAMSZ);
	dwNumDatagrams = m_Mes_Number;
	dwOneLana = m_LanaNumber;
	dwDelay = m_Delay;
	bSender = (((CButton*)GetDlgItem(IDC_SENDER))->
		GetCheck() == 1) ? TRUE : FALSE;
	bUniqueName = (((CButton*)GetDlgItem(IDC_UNIQUE_NAME))->
		GetCheck() == 1) ? TRUE : FALSE;
	bBroadcast = (((CButton*)GetDlgItem(IDC_BROADCAST))->
		GetCheck() == 1) ? TRUE : FALSE;
	bOneLana = (((CButton*)GetDlgItem(IDC_ALL_LANA))->
		GetCheck() == 1) ? FALSE : TRUE;
	bRecvAny = (((CButton*)GetDlgItem(IDC_RECV_ANY))->
		GetCheck() == 1) ? TRUE : FALSE;

}


void CNbDgramDlg::OnClickedStart()
{
	// TODO: Add your control notification handler code here
	hWnd_LB = m_ListBox.m_hWnd;
	RefreshValues();
	GetDlgItem(IDC_START)->EnableWindow(false);
	GetDlgItem(IDC_SENDER)->EnableWindow(false);
	GetDlgItem(IDC_RECEIVER)->EnableWindow(false);
	AfxBeginThread(MainThread, NULL);

}

int DatagramSend(int lana, int num, char* recipient,
	char* buffer, int buflen)
{
	NCB		 ncb;

	char		Str[200];
	CListBox* pLB =
		(CListBox*)(CListBox::FromHandle(hWnd_LB));

	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBDGSEND;
	ncb.ncb_lana_num = lana;
	ncb.ncb_num = num;
	ncb.ncb_buffer = (PUCHAR)buffer;
	ncb.ncb_length = buflen;

	memset(ncb.ncb_callname, ' ', NCBNAMSZ);
	strncpy_s((char*)ncb.ncb_callname,
		sizeof(ncb.ncb_callname),
		recipient, strlen(recipient));

	if (Netbios(&ncb) != NRC_GOODRET)
	{
		sprintf_s(Str, sizeof(Str),
			"Netbios: NCBDGSEND failed: %d",
			ncb.ncb_retcode);
		pLB->AddString((LPTSTR)Str);
		return ncb.ncb_retcode;
	}
	return NRC_GOODRET;
}
int DatagramSendBC(int lana, int num, char* buffer,
	int buflen)
{
	NCB		 ncb;

	char		Str[200];
	CListBox* pLB =
		(CListBox*)(CListBox::FromHandle(hWnd_LB));

	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBDGSENDBC;
	ncb.ncb_lana_num = lana;
	ncb.ncb_num = num;
	ncb.ncb_buffer = (PUCHAR)buffer;
	ncb.ncb_length = buflen;

	if (Netbios(&ncb) != NRC_GOODRET)
	{
		sprintf_s(Str, sizeof(Str),
			"Netbios: NCBDGSENDBC failed: %d",
			ncb.ncb_retcode);
		pLB->AddString((LPTSTR)Str);
		return ncb.ncb_retcode;
	}
	return NRC_GOODRET;
}
int DatagramRecv(PNCB pncb, int lana, int num, char* buffer,
	int buflen, HANDLE hEvent)
{
	char		Str[200];
	CListBox* pLB =
		(CListBox*)(CListBox::FromHandle(hWnd_LB));

	ZeroMemory(pncb, sizeof(NCB));
	if (hEvent)
	{
		pncb->ncb_command = NCBDGRECV | ASYNCH;
		pncb->ncb_event = hEvent;
	}
	else
		pncb->ncb_command = NCBDGRECV;
	pncb->ncb_lana_num = lana;
	pncb->ncb_num = num;
	pncb->ncb_buffer = (PUCHAR)buffer;
	pncb->ncb_length = buflen;

	if (Netbios(pncb) != NRC_GOODRET)
	{
		sprintf_s(Str, sizeof(Str),
			"Netbos: NCBDGRECV failed: %d",
			pncb->ncb_retcode);
		pLB->AddString((LPTSTR)Str);
		return pncb->ncb_retcode;
	}
	return NRC_GOODRET;
}
int DatagramRecvBC(PNCB pncb, int lana, int num,
	char* buffer, int buflen, HANDLE hEvent)
{
	char		Str[200];
	CListBox* pLB =
		(CListBox*)(CListBox::FromHandle(hWnd_LB));

	ZeroMemory(pncb, sizeof(NCB));
	if (hEvent)
	{
		pncb->ncb_command = NCBDGRECVBC | ASYNCH;
		pncb->ncb_event = hEvent;
	}
	else
		pncb->ncb_command = NCBDGRECVBC;
	pncb->ncb_lana_num = lana;
	pncb->ncb_num = num;
	pncb->ncb_buffer = (PUCHAR)buffer;
	pncb->ncb_length = buflen;

	if (Netbios(pncb) != NRC_GOODRET)
	{
		sprintf_s(Str, sizeof(Str),
			"Netbios: NCBDGRECVBC failed: %d",
			pncb->ncb_retcode);
		pLB->AddString((LPTSTR)Str);
		return pncb->ncb_retcode;
	}
	return NRC_GOODRET;
}
UINT MainThread(PVOID lpParam)
{
	LANA_ENUM	lenum;
	int			i, j;
	char		szMessage[MAX_DATAGRAM_SIZE],
		szSender[NCBNAMSZ + 1];
	DWORD* dwNum = NULL,
		dwBytesRead,
		dwErr;
	char		Str[200];
	CListBox* pLB = (CListBox*)(CListBox::FromHandle(hWnd_LB));

	if ((dwErr = LanaEnum(&lenum)) != NRC_GOODRET)
	{
		if (*NbCommonErrorMsg)
			pLB->AddString((LPTSTR)NbCommonErrorMsg);
		sprintf_s(Str, sizeof(Str),
			"LanaEnum failed: %d", dwErr);
		pLB->AddString((LPTSTR)Str);
		return 1;
	}

	if (lenum.length == 0)
	{
		pLB->AddString((LPTSTR)"Sorry, no existing LANA...");
		return 1;
	}

	if ((dwErr = ResetAll(&lenum, (UCHAR)MAX_SESSIONS,
		(UCHAR)MAX_NAMES, FALSE)) != NRC_GOODRET)
	{
		if (*NbCommonErrorMsg)
			pLB->AddString((LPTSTR)NbCommonErrorMsg);
		sprintf_s(Str, sizeof(Str),
			"ResetAll failed: %d", dwErr);
		pLB->AddString((LPTSTR)Str);
		return 1;
	}

	dwNum = (DWORD*)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,
		sizeof(DWORD) * lenum.length);
	if (dwNum == NULL)
	{
		sprintf_s(Str, sizeof(Str), "Out of memory");
		pLB->AddString((LPTSTR)Str);
		return 1;
	}

	if (bOneLana)
	{
		if (bUniqueName)
		{
			AddName(dwOneLana, szLocalName,
				(int*)&dwNum[0]);
			if (*NbCommonErrorMsg)
				pLB->AddString((LPTSTR)NbCommonErrorMsg);
		}
		else
		{
			AddGroupName(dwOneLana, szLocalName,
				(int*)&dwNum[0]);
			if (*NbCommonErrorMsg)
				pLB->AddString((LPTSTR)NbCommonErrorMsg);
		}
	}
	else
	{
		for (i = 0; i < lenum.length; i++)
		{
			if (bUniqueName)
			{
				AddName(lenum.lana[i], szLocalName,
					(int*)&dwNum[i]);
				if (*NbCommonErrorMsg)
					pLB->AddString((LPTSTR)NbCommonErrorMsg);
			}
			else
			{
				AddGroupName(lenum.lana[i], szLocalName,
					(int*)&dwNum[i]);
				if (*NbCommonErrorMsg)
					pLB->AddString((LPTSTR)NbCommonErrorMsg);
			}
		}
	}

	if (bSender) // Посылаем дейтаграммы
	{
		if (bBroadcast) // Широковещательная рассылка
		{
			if (bOneLana)
			{ // Широковещательная рассылка только на одном LANA
				for (j = 0; j < dwNumDatagrams; j++)
				{
					wsprintf(szMessage,
						"[%03d] Test broadcast datagram", j);
					if (DatagramSendBC(dwOneLana, dwNum[0],
						szMessage, strlen(szMessage)) != NRC_GOODRET)
						return 1;
					Sleep(dwDelay);
				}
			}
			else
			{ // Широковещательная отправка на каждом номере LANA
			  // локальной машины
				for (j = 0; j < dwNumDatagrams; j++)
				{
					for (i = 0; i < lenum.length; i++)
					{
						wsprintf(szMessage,
							"[%03d] Test broadcast datagram", j);
						if (DatagramSendBC(lenum.lana[i], dwNum[i],
							szMessage, strlen(szMessage)) != NRC_GOODRET)
							return 1;
					}
					Sleep(dwDelay);
				}
			}
		}
		else // Отправка конкретному получателю
		{
			if (bOneLana) // Отправка на конкретный номер LANА
			{
				for (j = 0; j < dwNumDatagrams; j++)
				{
					wsprintf(szMessage,
						"[%03d] Test directed datagram", j);
					if (DatagramSend(dwOneLana, dwNum[0],
						szRecipientName, szMessage,
						strlen(szMessage)) != NRC_GOODRET)
						return 1;
					Sleep(dwDelay);
				}
			}
			else
			{ // Отправка направленного сообщения на каждом
			  // номере LANA локальной машины
				for (j = 0; j < dwNumDatagrams; j++)
				{
					for (i = 0; i < lenum.length; i++)
					{
						wsprintf(szMessage,
							"[%03d] Test directed datagram", j);
						sprintf_s(Str, sizeof(Str),
							"count: %d.%d", j, i);
						pLB->AddString((LPTSTR)Str);
						if (DatagramSend(lenum.lana[i], dwNum[i],
							szRecipientName, szMessage,
							strlen(szMessage)) != NRC_GOODRET)
							return 1;
					}
					Sleep(dwDelay);
				}
			}
		}
	}
	else // Принимаем дейтаграммы
	{
		NCB* ncb = NULL;
		char** szMessageArray = NULL;
		HANDLE* hEvent = NULL;
		DWORD   dwRet;

		// Выделение массива структур NCB для передачи каждому
		// приемнику на каждом номере LANA
		ncb = (NCB*)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,
			sizeof(NCB) * lenum.length);

		// Выделение массива буферов входящих данных
		szMessageArray = (char**)GlobalAlloc(GMEM_FIXED,
			sizeof(char*) * lenum.length);
		for (i = 0; i < lenum.length; i++)
			szMessageArray[i] = (char*)GlobalAlloc(GMEM_FIXED,
				MAX_DATAGRAM_SIZE);

		// Выделение массива описателей событий для
		// асинхронного приема
		hEvent =
			(HANDLE*)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,
				sizeof(HANDLE) * lenum.length);
		for (i = 0; i < lenum.length; i++)
			hEvent[i] = CreateEvent(0, TRUE, FALSE, 0);

		if (bBroadcast) // Широковещание
		{
			if (bOneLana)
			{ // Синхронный широковещательный прием 
			  // на указанном номере LANA
				for (j = 0; j < dwNumDatagrams; j++)
				{
					if (DatagramRecvBC(&ncb[0], dwOneLana, dwNum[0],
						szMessageArray[0], MAX_DATAGRAM_SIZE,
						NULL) != NRC_GOODRET)
						return 1;
					FormatNetbiosName((char*)ncb[0].ncb_callname,
						szSender);
					sprintf_s(Str, sizeof(Str),
						"%03d [LANA %d] Message: '%s' "
						"received from: %s", j, ncb[0].ncb_lana_num,
						szMessageArray[0], szSender);
					pLB->AddString((LPTSTR)Str);
				}
			}
			else
			{ // Асинхронный ширроковещательный прием на каждом
			  // доступном номере LANA. В случае успеха выдается
			  // сообщение, иначе команда отменяется
				for (j = 0; j < dwNumDatagrams; j++)
				{
					for (i = 0; i < lenum.length; i++)
					{
						dwBytesRead = MAX_DATAGRAM_SIZE;
						if (DatagramRecvBC(&ncb[i], lenum.lana[i],
							dwNum[i], szMessageArray[i],
							MAX_DATAGRAM_SIZE, hEvent[i])
							!= NRC_GOODRET)
							return 1;
					}
					dwRet = WaitForMultipleObjects(lenum.length,
						hEvent, FALSE, INFINITE);
					if (dwRet == WAIT_FAILED)
					{
						sprintf_s(Str, sizeof(Str),
							"WaitForMultipleObjects failed: %d",
							GetLastError());
						pLB->AddString((LPTSTR)Str);
						return 1;
					}
					for (i = 0; i < lenum.length; i++)
					{
						if (ncb[i].ncb_cmd_cplt == NRC_PENDING)
						{
							Cancel(&ncb[i]);
							if (*NbCommonErrorMsg)
								pLB->AddString((LPTSTR)NbCommonErrorMsg);
						}
						else
						{
							ncb[i].ncb_buffer[ncb[i].ncb_length] = 0;
							FormatNetbiosName((char*)ncb[i].ncb_callname,
								szSender);
							sprintf_s(Str, sizeof(Str),
								"%03d [LANA %d] Message: '%s' "
								"received from: %s", j, ncb[i].ncb_lana_num,
								szMessageArray[i], szSender);
							pLB->AddString((LPTSTR)Str);
						}
						ResetEvent(hEvent[i]);
					}
				}
			}
		}
		else // Широковещание отключено
		{
			if (bOneLana)
			{ // Блокирующий прием дейтаграмм на указанном
			  // номере LANA.
				for (j = 0; j < dwNumDatagrams; j++)
				{
					if (bRecvAny)
					{
						// Прием данных, предназначенных для любого
						// имени NetBIOS в таблице имен этого процесса.
						if (DatagramRecv(&ncb[0], dwOneLana, 0xFF,
							szMessageArray[0], MAX_DATAGRAM_SIZE,
							NULL) != NRC_GOODRET)
							return 1;
					}
					else
					{ // Прием данных, направленных на наше
					  // имя NetBIOS.
						if (DatagramRecv(&ncb[0], dwOneLana,
							dwNum[0], szMessageArray[0],
							MAX_DATAGRAM_SIZE, NULL)
							!= NRC_GOODRET)
							return 1;
					}
					FormatNetbiosName((char*)ncb[0].ncb_callname,
						szSender);
					sprintf_s(Str, sizeof(Str),
						"%03d [LANA %d] Message: '%s' "
						"received from: %s", j, ncb[0].ncb_lana_num,
						szMessageArray[0], szSender);
					pLB->AddString((LPTSTR)Str);
				}
			}
			else
			{
				// Асинхронный прием дейтаграмм на каждом доступном 
				// номере LANA. В случае успеха выдается
				// сообщение, иначе команда отменяется
				for (j = 0; j < dwNumDatagrams; j++)
				{
					for (i = 0; i < lenum.length; i++)
					{
						if (bRecvAny)
						{
							// Прием данных, предназначенных для любого
							// имени NetBIOS в таблице имен этого процесса
							if (DatagramRecv(&ncb[i], lenum.lana[i],
								0xFF, szMessageArray[i],
								MAX_DATAGRAM_SIZE, hEvent[i])
								!= NRC_GOODRET)
								return 1;
						}
						else
						{ // Прием данных, направленных на наше
						  // имя NetBIOS.
							if (DatagramRecv(&ncb[i], lenum.lana[i],
								dwNum[i], szMessageArray[i],
								MAX_DATAGRAM_SIZE, hEvent[i])
								!= NRC_GOODRET)
								return 1;
						}
					}
					dwRet = WaitForMultipleObjects(lenum.length,
						hEvent, FALSE, INFINITE);
					if (dwRet == WAIT_FAILED)
					{
						sprintf_s(Str, sizeof(Str),
							"WaitForMultipleObjects failed: %d",
							GetLastError());
						pLB->AddString((LPTSTR)Str);
						return 1;
					}
					for (i = 0; i < lenum.length; i++)
					{
						if (ncb[i].ncb_cmd_cplt == NRC_PENDING)
							Cancel(&ncb[i]);
						else
						{
							ncb[i].ncb_buffer[ncb[i].ncb_length] = 0;
							FormatNetbiosName((char*)ncb[i].ncb_callname,
								szSender);
							sprintf_s(Str, sizeof(Str),
								"%03d [LANA %d] Message: '%s' "
								"from: %s", j, ncb[i].ncb_lana_num,
								szMessageArray[i], szSender);
							pLB->AddString((LPTSTR)Str);
						}
						ResetEvent(hEvent[i]);
					}
				}
			}
		}
		// Очистка
		for (i = 0; i < lenum.length; i++)
		{
			CloseHandle(hEvent[i]);
			GlobalFree(szMessageArray[i]);
		}
		GlobalFree(hEvent);
		GlobalFree(szMessageArray);
	}
	// Очистка
	if (bOneLana)
	{
		DelName(dwOneLana, szLocalName);
		if (*NbCommonErrorMsg)
			pLB->AddString((LPTSTR)NbCommonErrorMsg);
	}
	else
	{
		for (i = 0; i < lenum.length; i++)
		{
			DelName(lenum.lana[i], szLocalName);
			if (*NbCommonErrorMsg)
				pLB->AddString((LPTSTR)NbCommonErrorMsg);
		}
	}
	GlobalFree(dwNum);
	return 0;
}
