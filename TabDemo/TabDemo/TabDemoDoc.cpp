
// TabDemoDoc.cpp : implementation of the CTabDemoDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TabDemo.h"
#endif

#include "TabDemoDoc.h"
#include "CStyle.h"
#include "CPitch.h"
#include "CJustify.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTabDemoDoc

IMPLEMENT_DYNCREATE(CTabDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CTabDemoDoc, CDocument)
	ON_COMMAND(ID_TEXT_FORMAT, &CTabDemoDoc::OnTextFormat)
END_MESSAGE_MAP()


// CTabDemoDoc construction/destruction

CTabDemoDoc::CTabDemoDoc() noexcept
{
	// TODO: add one-time construction code here
	m_Bold = FALSE;
	m_Italic = FALSE;
	m_Justify = JUSTIFY_LEFT;
	m_Pitch = PITCH_VARIABLE;
	m_Spacing = 1;
	m_Underline = FALSE;

}

CTabDemoDoc::~CTabDemoDoc()
{
}

BOOL CTabDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CTabDemoDoc serialization

void CTabDemoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CTabDemoDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CTabDemoDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CTabDemoDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CTabDemoDoc diagnostics

#ifdef _DEBUG
void CTabDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTabDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTabDemoDoc commands


void CTabDemoDoc::OnTextFormat()
{
	// TODO: Add your command handler code here
	// создание объекта диалогового окна с вкладками:
	CPropertySheet PropertySheet(_T("Format"));

	// создание объекта для каждой страницы:
	CStyle StylePage;
	CJustify JustifyPage;
	CPitch PitchPage;

	// добавление страниц к объекту диалогового окна:
	PropertySheet.AddPage(&StylePage);
	PropertySheet.AddPage(&JustifyPage);
	PropertySheet.AddPage(&PitchPage);
	// инициализация объектов страниц:
	StylePage.m_Bold = m_Bold;
	StylePage.m_Italic = m_Italic;
	StylePage.m_Underline = m_Underline;
	JustifyPage.m_Justify = m_Justify;
	PitchPage.m_Pitch = m_Pitch;
	PitchPage.m_Spacing = m_Spacing;

	// отображение диалогового окна с вкладками:   
	if (PropertySheet.DoModal() == IDOK)
	{

		// сохранение значений элементов управления страниц:
		m_Bold = StylePage.m_Bold;
		m_Italic = StylePage.m_Italic;
		m_Underline = StylePage.m_Underline;
		m_Justify = JustifyPage.m_Justify;
		m_Pitch = PitchPage.m_Pitch;
		m_Spacing = PitchPage.m_Spacing;

		// перерисовка текста:
		UpdateAllViews(NULL);
	}

}
