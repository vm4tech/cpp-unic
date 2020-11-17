// CFormat.cpp : implementation file
//

#include "pch.h"
#include "FontDemo.h"
#include "CFormat.h"
#include "afxdialogex.h"


// CFormat dialog

IMPLEMENT_DYNAMIC(CFormat, CDialog)

CFormat::CFormat(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
	, m_Bold(FALSE)
	, m_Italic(FALSE)
	, m_Underline(FALSE)
	, m_Justify(0)
	, m_Pitch(0)
	, m_Spacing(0)
{

}

CFormat::~CFormat()
{
}

void CFormat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_BOLD, m_Bold);
	DDX_Check(pDX, IDC_ITALIC, m_Italic);
	DDX_Check(pDX, IDC_UNDERLINE, m_Underline);
	DDX_Radio(pDX, IDC_LEFT, m_Justify);
	DDX_Radio(pDX, IDC_VARIABLE, m_Pitch);
	DDX_Text(pDX, IDC_SPACING, m_Spacing);
	DDV_MinMaxInt(pDX, m_Spacing, 1, 3);
	DDX_Control(pDX, IDC_SPACING, m_SpacingEdit);
}


BEGIN_MESSAGE_MAP(CFormat, CDialog)
	
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BOLD, &CFormat::OnBnClickedBold)
	ON_BN_CLICKED(IDC_ITALIC, &CFormat::OnBnClickedItalic)
	ON_BN_CLICKED(IDC_UNDERLINE, &CFormat::OnBnClickedUnderline)
	ON_EN_CHANGE(IDC_SPACING, &CFormat::OnEnChangeSpacing)
END_MESSAGE_MAP()


// CFormat message handlers




BOOL CFormat::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	GetDlgItem(IDC_SAMPLE)->GetWindowRect(&m_RectSample);
	ScreenToClient(&m_RectSample);
	m_SpacingEdit.LimitText(1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CFormat::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialog::OnPaint() for painting messages
}


void CFormat::OnBnClickedBold()
{
	// TODO: Add your control notification handler code here
	m_Bold = !m_Bold;
	InvalidateRect(&m_RectSample);
	UpdateWindow();

}


void CFormat::OnBnClickedItalic()
{
	// TODO: Add your control notification handler code here
	m_Italic = !m_Italic;
	InvalidateRect(&m_RectSample);
	UpdateWindow();
}


void CFormat::OnBnClickedUnderline()
{
	// TODO: Add your control notification handler code here
	m_Underline = !m_Underline;
	InvalidateRect(&m_RectSample);
	UpdateWindow();
}


void CFormat::OnEnChangeSpacing()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
