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
{

}

CFormat::~CFormat()
{
}

void CFormat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFormat, CDialog)
END_MESSAGE_MAP()


// CFormat message handlers
