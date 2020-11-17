
// FontDemo.h : main header file for the FontDemo application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CFontDemoApp:
// See FontDemo.cpp for the implementation of this class
//

class CFontDemoApp : public CWinApp
{
public:
	CFontDemoApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFontDemoApp theApp;
