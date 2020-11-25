// CStyle.cpp : implementation file
//

#include "pch.h"
#include "TabDemo.h"
#include "CStyle.h"
#include "afxdialogex.h"


// CStyle dialog

IMPLEMENT_DYNAMIC(CStyle, CPropertyPage)

CStyle::CStyle(CWnd* pParent /*=nullptr*/)
	: CPropertyPage(IDD_DIALOG1, 0)
	, m_Bold(FALSE)
	, m_Italic(FALSE)
	, m_Underline(FALSE)
{

}

CStyle::~CStyle()
{
}

void CStyle::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_BOLD, m_Bold);
	DDX_Check(pDX, IDC_ITALIC, m_Italic);
	DDX_Check(pDX, IDC_UNDERLINE, m_Underline);
}


BEGIN_MESSAGE_MAP(CStyle, CPropertyPage)
END_MESSAGE_MAP()


// CStyle message handlers
