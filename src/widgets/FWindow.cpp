/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FWindow.cpp
* 描述：		FWindow
* 创建日期：	2013-07-04
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