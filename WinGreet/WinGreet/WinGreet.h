
// WinGreet.h: основной файл заголовка для приложения WinGreet
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CWinGreetApp:
// Сведения о реализации этого класса: WinGreet.cpp
//

class CWinGreetApp : public CWinApp
{
public:
	CWinGreetApp() noexcept;


// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CWinGreetApp theApp;
