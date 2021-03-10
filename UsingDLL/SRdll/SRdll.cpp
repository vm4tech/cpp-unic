// SRdll.cpp : Defines the initialization routines for the DLL.
//
#include <math.h>
#include "pch.h"
#include "framework.h"
#include "SRdll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CSRdllApp

BEGIN_MESSAGE_MAP(CSRdllApp, CWinApp)
END_MESSAGE_MAP()


// CSRdllApp construction

CSRdllApp::CSRdllApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSRdllApp object

CSRdllApp theApp;


// CSRdllApp initialization

BOOL CSRdllApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

extern "C" __declspec(dllexport)
double ExampleSquareRoot(double d)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TRACE("Entering ExampleSquareRoot\n");
	if (d >= 0.0)
		return sqrt(d);
	AfxMessageBox
	(_T("Can't take square root of a negative number."));
	return 0.0;
}
