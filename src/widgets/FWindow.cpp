/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FWindow.cpp
* ������		FWindow
* �������ڣ�	2013-07-04
*
*/

#include "StdAfx.h"
#include "widgets/FWindow.h"

#define FWINDOW_INLINE FOOINLINE
namespace FooUI { namespace Widgets {
	//////////////////////////////////////////////////////////////////////////
	// FWindow
	FWINDOW_INLINE FWindow::FWindow(FWidget* parent/* = NULL*/) : 
		FWidget(parent)
	{
		modifyFlag(0, FOOFLAG_SKIPTAB);
	}

}} // !namespace