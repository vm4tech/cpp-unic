// CPitch.cpp : implementation file
//

#include "pch.h"
#include "TabDemo.h"
#include "CPitch.h"
#include "afxdialogex.h"


// CPitch dialog

IMPLEMENT_DYNAMIC(CPitch, CPropertyPage)

CPitch::CPitch(CWnd* pParent /*=nullptr*/)
	: CPropertyPage(IDD_DIALOG3, 0)
{

}

CPitch::~CPitch()
{
}

void CPitch::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPitch, CPropertyPage)
END_MESSAGE_MAP()


// CPitch message handlers
