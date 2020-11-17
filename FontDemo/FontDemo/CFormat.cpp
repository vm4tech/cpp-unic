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
	
END_MESSAGE_MAP()


// CFormat message handlers


