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
{
}

CStyle::~CStyle()
{
}

void CStyle::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStyle, CPropertyPage)
END_MESSAGE_MAP()


// CStyle message handlers
