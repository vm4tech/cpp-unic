
// TabDemo.h : main header file for the TabDemo application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTabDemoApp:
// See TabDemo.cpp for the implementation of this class
//

class CTabDemoApp : public CWinApp
{
public:
	CTabDemoApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTabDemoApp theApp;
