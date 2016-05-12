/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FGloabl.h
* 描述：		FGloabl
* 创建日期：	2013-06-06
*
*/

#ifndef FGLOBAL_H
#define FGLOBAL_H

#include "config/FConfig.h"

#include <windows.h>

#ifdef __cplusplus

#ifndef FOOUI_BUILD_LIB
	#define FOOUI_DECL_IMPORT __declspec(dllimport)
	#define FOOUI_DECL_EXPORT __declspec(dllexport)
	#define FOOINLINE inline
#else
	#define FOOUI_DECL_IMPORT
	#define FOOUI_DECL_EXPORT
	#define FOOINLINE
#endif

#define FOOUI_LIB FOOUI_DECL_IMPORT
#define FOOUI_API FOOUI_DECL_IMPORT


#endif // !__cplusplus

namespace FooUI
{
	/** 属性 */
	enum eFooFlag
	{
		FOOFLAG_NONE			= 0x00000000, 
		FOOFLAG_TRANSPARENT		= 0x00000001,	/** 透明 */
		FOOFLAG_SKIPTAB			= 0x00000002,	/** 忽略TAB键 */
		FOOFLAG_SKIPMOUSE		= 0x00000004,	/** 忽略鼠标 */
		FOOFLAG_SKIPKEYBOARD	= 0x00000008,	/** 忽略键盘 */
		FOOFLAG_SKIPFOCUS		= 0x00000010,	/** 忽略焦点 */
		FOOFLAG_REFLECTMOUSE	= 0x00000020,	/** 反射鼠标消息 */
		FOOFLAG_REFLECTKEYBOARD	= 0x00000040,	/** 反射键盘消息 */
	};

	/** 状态 */
	enum eFooState
	{
		FOOSTATE_NONE			= 0x00000000, 
		FOOSTATE_ALL			= FOOSTATE_NONE, 
		FOOSTATE_NORMAL			= 0x00000001, 
		FOOSTATE_HOVER			= 0x00000002, 
		FOOSTATE_PUSHED			= 0x00000004, 
		FOOSTATE_DISABLED		= 0x00000008, 
		FOOSTATE_ENABLED		= 0x00000010, 
		FOOSTATE_INVISIBLE		= 0x00000020, 
		FOOSTATE_VISIBLE		= 0x00000040, 
		FOOSTATE_UNFOCUSED		= 0x00000080, 
		FOOSTATE_FOCUSED		= 0x00000100, 
		FOOSTATE_UNSELECTED		= 0x00000200, 
		FOOSTATE_SELECTED		= 0x00000400, 
		FOOSTATE_UNCHECKED		= 0x00000800, 
		FOOSTATE_CHECKED		= 0x00001000, 
		FOOSTATE_DEACTIVATED	= 0x00002000, 
		FOOSTATE_ACTIVATED		= 0x00004000, 
		FOOSTATE_NONEMPTY		= 0x00008000, 
		FOOSTATE_EMPTY			= 0x00010000, 
	};

	/** 停靠方式 */
	enum eFooDock
	{
		FOODOCK_NONE = 0, 
		FOODOCK_LEFT, 
		FOODOCK_TOP, 
		FOODOCK_RIGHT, 
		FOODOCK_BOTTOM, 
		FOODOCK_FILL, 
	};

	/** 对齐方式 */
	enum eFooAlign
	{
		FOOALIGN_NONE		= 0x00000000, 
		FOOALIGN_LEFT		= FOOALIGN_NONE, 
		FOOALIGN_TOP		= FOOALIGN_NONE, 
		FOOALIGN_RIGHT		= 0x00000001, 
		FOOALIGN_XCENTER	= 0x00000002, 
		FOOALIGN_YCENTER	= 0x00000004, 
		FOOALIGN_BOTTOM		= 0x00000008, 
	};

	/** 角色 */
	enum eFooRole
	{
		FOOROLE_NONE		= 0,
		FOOROLE_CAPTION, 
		FOOROLE_SYSMIN, 
		FOOROLE_SYSMAX, 
		FOOROLE_SYSRESTORE, 
		FOOROLE_SYSCLOSE, 
		FOOROLE_SIZEGRIP, 
	};

#ifndef GET_X_LPARAM
#	define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#	define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
#endif

#define fooTestAttribute(_attr, _check) \
	( ((_attr) & (_check)) == (_check) )

	template <typename T>
	bool fooModifyAttribute(T& _attr, T _remove, T _add)
	{
		T _new = _attr;
		_new &= ~_remove;
		_new |= _add;

		if (_new == _attr)
		{
			return false;
		}

		_attr = _new;

		return true;
	}

#define fooSafeDelete(_ptr)					if (NULL != _ptr)		{ delete _ptr; _ptr = NULL; }
#define fooSafeDeleteT(_type, _ptr)			if (NULL != _ptr)		{ delete (_type)_ptr; _ptr = NULL; }
#define fooSafeDeleteArray(_ptr)			if (NULL != _ptr)		{ delete []_ptr; _ptr = NULL; }
#define fooSafeDeleteArrayT(_type, _ptr)	if (NULL != _ptr)		{ delete [](_type)_ptr; _ptr = NULL; }
#define fooSafeCloseHandle(_handle)			if (NULL != _handle)	{ ::CloseHandle(_handle); _handle = NULL; }
#define fooSafeDeleteObject(_hobject)		if (NULL != _hobject)	{ ::DeleteObject(_hobject); _hobject = NULL; }
#define fooSafeDeleteDC(_hdc)				if (NULL != _hdc)		{ ::DeleteDC(_hdc); _hdc = NULL; }

} // !namespace

#endif // FGLOBAL_H
