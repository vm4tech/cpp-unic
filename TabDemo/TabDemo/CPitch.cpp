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
	, m_Spacing(0)
	, m_Pitch(0)
{

}

CPitch::~CPitch()
{
}

void CPitch::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_VARIABLE, m_Pitch);
	DDX_Control(pDX, IDC_SPACING, m_SpacingEdit);
	DDX_Text(pDX, IDC_SPACING, m_Spacing);
	DDV_MinMaxInt(pDX, m_Spacing, 1, 3);

}


BEGIN_MESSAGE_MAP(CPitch, CPropertyPage)
END_MESSAGE_MAP()


// CPitch message handlers


BOOL CPitch::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	m_SpacingEdit.LimitText(1);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
