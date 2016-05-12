/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		StdAfx.h
* 描述：		StdAfx，预编译头文件
* 创建日期：	2013-06-06
*
*/

#ifndef STDAFX_H
#define STDAFX_H

#include "config/FConfig.h"
#include "core/FGloabl.h"

#include <windows.h>
#include <tchar.h>

#include <CommCtrl.h>

#undef FOOUI_LIB
#define FOOUI_LIB FOOUI_DECL_EXPORT

#undef FOOUI_API
#define FOOUI_API FOOUI_DECL_EXPORT

#pragma warning(push)
#pragma warning(disable:4251)
#include <atlstr.h>
#undef FSTRINGA_DATA
#undef FSTRINGW_DATA
#define FSTRINGA_DATA ATL::CStringA
#define FSTRINGW_DATA ATL::CStringW
#include "core/FString.h"
#pragma warning(pop)

#include "FModule_p.h"


#define lengthof(x) (sizeof(x)/sizeof(*x))


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#endif // !STDAFX_H
