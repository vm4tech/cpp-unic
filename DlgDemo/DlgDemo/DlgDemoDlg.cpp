
// DlgDemoDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "DlgDemo.h"
#include "DlgDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDlgDemoDlg dialog



CDlgDemoDlg::CDlgDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLGDEMO_DIALOG, pParent)
	, m_Red(FALSE)
	, m_Green(FALSE)
	, m_Blue(FALSE)
	, m_Intensity(INT_LIGHT)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDlgDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_RED, m_Red);
	DDX_Check(pDX, IDC_GREEN, m_Green);
	DDX_Check(pDX, IDC_BLUE, m_Blue);
	DDX_Radio(pDX, IDC_DARK, m_Intensity);
}

BEGIN_MESSAGE_MAP(CDlgDemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RED, &CDlgDemoDlg::OnBnClickedRed)
	ON_BN_CLICKED(IDC_GREEN, &CDlgDemoDlg::OnBnClickedGreen)
	ON_BN_CLICKED(IDC_BLUE, &CDlgDemoDlg::OnBnClickedBlue)
	ON_BN_CLICKED(IDC_DARK, &CDlgDemoDlg::OnBnClickedDark)
	ON_BN_CLICKED(IDC_LIGHT, &CDlgDemoDlg::OnBnClickedLight)
END_MESSAGE_MAP()


// CDlgDemoDlg message handlers

BOOL CDlgDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	GetDlgItem(IDC_SAMPLE)->GetWindowRect(&m_RectSample);
	ScreenToClient(&m_RectSample);
	int Border = (m_RectSample.right - m_RectSample.left) / 8;
	m_RectSample.InflateRect(-Border, -Border);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDlgDemoDlg::OnPaint()
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
		//CDialogEx::OnPaint();
		// вызов CDialog::OnPaint() был удален
		COLORREF Color = RGB
		(m_Red ? (m_Intensity == INT_DARK ? 128 : 255) : 0,
			m_Green ? (m_Intensity == INT_DARK ? 128 : 255) : 0,
			m_Blue ? (m_Intensity == INT_DARK ? 128 : 255) : 0);
		CBrush Brush(Color);
		CPaintDC dc(this);
		dc.FillRect(&m_RectSample, &Brush);

	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDlgDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDlgDemoDlg::OnBnClickedRed()
{
	// TODO: Add your control notification handler code here
	m_Red = IsDlgButtonChecked(IDC_RED);
	InvalidateRect(&m_RectSample);
	UpdateWindow();
}


void CDlgDemoDlg::OnBnClickedGreen()
{
	// TODO: Add your control notification handler code here
	m_Green = IsDlgButtonChecked(IDC_GREEN);
	InvalidateRect(&m_RectSample);
	UpdateWindow();
}


void CDlgDemoDlg::OnBnClickedBlue()
{
	// TODO: Add your control notification handler code here
	m_Blue = IsDlgButtonChecked(IDC_BLUE);
	InvalidateRect(&m_RectSample);
	UpdateWindow();
}


void CDlgDemoDlg::OnBnClickedDark()
{
	// TODO: Add your control notification handler code here
	if (IsDlgButtonChecked(IDC_DARK))
	{
		m_Intensity = INT_DARK;
		InvalidateRect(&m_RectSample);
		UpdateWindow();
	}
}


void CDlgDemoDlg::OnBnClickedLight()
{
	// TODO: Add your control notification handler code here
	if (IsDlgButtonChecked(IDC_LIGHT))
	{
		m_Intensity = INT_LIGHT;
		InvalidateRect(&m_RectSample);
		UpdateWindow();
	}
}
