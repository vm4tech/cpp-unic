
// FormDemo.h : main header file for the FormDemo application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CFormDemoApp:
// See FormDemo.cpp for the implementation of this class
//

class CFormDemoApp : public CWinApp
{
public:
	CFormDemoApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFormDemoApp theApp;
