/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FDebug.cpp
* 描述：		FDebug
* 创建日期：	2013-06-07
*
*/

#include "StdAfx.h"
#include "core/FDebug.h"

#include <windows.h>
#include <tchar.h>

#define FDEBUG_BUFFER_LENGTH 2048

namespace FooUI { namespace Core { 

	FOOUI_LIB void __cdecl __fooDebug(LPCTSTR pszDebugString)
	{
		::OutputDebugString(pszDebugString);
	}

	FOOUI_LIB void __cdecl __fooDebugf(LPCTSTR pszFormat, ...)
	{
		TCHAR tchBuffer[FDEBUG_BUFFER_LENGTH + 1];
		va_list args;
		va_start(args, pszFormat);
		_vsntprintf_s(tchBuffer, FDEBUG_BUFFER_LENGTH, pszFormat, args);
		va_end(args);

		::OutputDebugString(tchBuffer);
	}

}} // namespace