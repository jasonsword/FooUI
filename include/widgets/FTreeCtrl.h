/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FTreeCtrl.h
* 描述：		FTreeCtrl
* 创建日期：	2013-08-24
*
*/

#ifndef FTREECTRL_H
#define FTREECTRL_H

#include "core/FBase.h"
#include "widgets/FAbstractList.h"

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FTreeCtrl
	#define FOOWC_TREECTRL _T("treectrl")
	class FOOUI_LIB FTreeCtrl : public FAbstractList
	{
	public:
		explicit FTreeCtrl(FWidget* parent = NULL);
		~FTreeCtrl(void);
		FOODECLARE_CLASS(FOOWC_TREECTRL)
		FOODECLARE_CONTAINER(true)
	};

	//////////////////////////////////////////////////////////////////////////
	// FTreeCtrlElement
	#define FOOWC_TREECTRLELEMENT _T("treectrlelement")
	class FOOUI_LIB FTreeCtrlElement : public FAbstractListElement
	{
	public:
		explicit FTreeCtrlElement(FWidget* parent = NULL);
		~FTreeCtrlElement(void);
		FOODECLARE_CLASS(FOOWC_TREECTRL)
		FOODECLARE_CONTAINER(true)
	};

}} // !namespace

#endif