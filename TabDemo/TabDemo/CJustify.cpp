// CJustify.cpp : implementation file
//

#include "pch.h"
#include "TabDemo.h"
#include "CJustify.h"
#include "afxdialogex.h"


// CJustify dialog

IMPLEMENT_DYNAMIC(CJustify, CPropertyPage)

CJustify::CJustify(CWnd* pParent /*=nullptr*/)
	: CPropertyPage(IDD_DIALOG2, 0)
	, m_Justify(0)
{

}

CJustify::~CJustify()
{
}

void CJustify::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_LEFT, m_Justify);
}


BEGIN_MESSAGE_MAP(CJustify, CPropertyPage)
END_MESSAGE_MAP()


// CJustify message handlers
