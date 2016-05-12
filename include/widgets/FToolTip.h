/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FToolTip.h
* 描述：		FToolTip
* 创建日期：	2013-08-04
*
*/

#ifndef FTOOLTIP_H
#define FTOOLTIP_H

#include "core/FBase.h"

#include "widgets/FWidget.h"

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FToolTip
	#define FOOWC_TOOLTIP _T("tooltip")
	class FOOUI_LIB FToolTip : public FWidget
	{
	public:
		virtual ~FToolTip(void) { }
		FOODECLARE_CLASS(FOOWC_TOOLTIP)
		FOODECLARE_CONTAINER(true)

	public:
		virtual bool showToolTip(LPCTSTR pszToolTip, FWidget* widget = NULL) = 0;
		virtual bool hideToolTip(void) = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	// FCommToolTip
	#define FOOWC_COMMTOOLTIP _T("commtooltip")
	class FOOUI_LIB FCommToolTip : public FToolTip
	{
	public:
		FOODECLARE_CLASS(FOOWC_COMMTOOLTIP)
		FOODECLARE_CONTAINER(true)

	public:
		virtual bool showToolTip(LPCTSTR pszToolTip, FWidget* widget = NULL);
		virtual bool hideToolTip(void);

	protected:
		LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	};

}} // !namespace

#endif //!FTOOLTIP_H