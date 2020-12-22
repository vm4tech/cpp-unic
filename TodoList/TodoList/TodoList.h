
// TodoList.h : main header file for the TodoList application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTodoListApp:
// See TodoList.cpp for the implementation of this class
//

class CTodoListApp : public CWinApp
{
public:
	CTodoListApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTodoListApp theApp;
