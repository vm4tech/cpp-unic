
// MandelMT.h : main header file for the MandelMT application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMandelMTApp:
// See MandelMT.cpp for the implementation of this class
//

class CMandelMTApp : public CWinApp
{
public:
	CMandelMTApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMandelMTApp theApp;
