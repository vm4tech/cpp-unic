// SRdll.h : main header file for the SRdll DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSRdllApp
// See SRdll.cpp for the implementation of this class
//

class CSRdllApp : public CWinApp
{
public:
	CSRdllApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
