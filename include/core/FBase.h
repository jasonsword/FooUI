/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FBase.h
* ������		FBase
* �������ڣ�	2013-06-06
*
*/

#ifndef FBASE_H
#define FBASE_H

#include <windows.h>
#include <tchar.h>

#include "config/FConfig.h"
#include "core/FGloabl.h"
#include "core/FDebug.h"

namespace FooUI
{
	/** ��갴�� */
	enum eMouseButton
	{
		NoButton         = 0x00000000, 
		LeftButton       = MK_LBUTTON, 
		RightButton      = MK_RBUTTON, 
		MidButton        = MK_MBUTTON, 
		MiddleButton     = MidButton, 
		XButton1         = MK_XBUTTON1, 
		XButton2         = MK_XBUTTON2, 
	};
} // !namespace

#endif // !FBASE_H