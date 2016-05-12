/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FDebug.h
* 描述：		FDebug
* 创建日期：	2013-06-07
*
*/

#ifndef FDEBUG_H
#define FDEBUG_H

#include <crtdbg.h>

#ifdef _DEBUG
# define FASSERT(_expr) _ASSERT(_expr)
#else
# define FASSERT(_expr)
#endif // !_DEBUG

#include "core/FBase.h"

namespace FooUI { namespace Core { 

	FOOUI_LIB void __cdecl __fooDebug(LPCTSTR pszDebugString);
	FOOUI_LIB void __cdecl __fooDebugf(LPCTSTR pszFormat, ...);

#ifdef _DEBUG
#	define fooDebug(_pszDebugString) FooUI::Core::__fooDebug(_pszDebugString)
#	define fooDebugf(pszFormat, ...) FooUI::Core::__fooDebugf(pszFormat, ##__VA_ARGS__)
#else
#	define fooDebug(_pszDebugString) (_pszDebugString)
#	define fooDebugf(pszFormat, ...) (pszFormat, ##__VA_ARGS__)
#endif // _DEBUG

}} // namespace

#endif // !FDEBUG_H
