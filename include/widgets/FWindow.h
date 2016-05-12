/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FWindow.h
* ������		FWindow
* �������ڣ�	2013-07-04
*
*/

#ifndef FWINDOW_H
#define FWINDOW_H

#include "core/FBase.h"

#include "widgets/FWidget.h"

namespace FooUI { namespace Widgets {
	//////////////////////////////////////////////////////////////////////////
	// FWindow
	#define FOOWC_WINDOW _T("window")
	class FOOUI_LIB FWindow : public FWidget
	{
	public:
		explicit FWindow(FWidget* parent = NULL);
		FOODECLARE_CLASS(FOOWC_WINDOW)
		FOODECLARE_CONTAINER(true)
	};

}} // !namespace

#endif //!FWINDOW_H