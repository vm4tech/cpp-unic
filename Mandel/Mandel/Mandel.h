
// Mandel.h : main header file for the Mandel application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMandelApp:
// See Mandel.cpp for the implementation of this class
//

class CMandelApp : public CWinApp
{
public:
	CMandelApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CMandelApp theApp;
