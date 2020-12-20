
// Echo.h : main header file for the Echo application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CEchoApp:
// See Echo.cpp for the implementation of this class
//

class CEchoApp : public CWinApp
{
public:
	CEchoApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CEchoApp theApp;
