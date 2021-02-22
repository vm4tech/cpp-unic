
// BitDemo.h : main header file for the BitDemo application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CBitDemoApp:
// See BitDemo.cpp for the implementation of this class
//

class CBitDemoApp : public CWinApp
{
public:
	CBitDemoApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBitDemoApp theApp;
