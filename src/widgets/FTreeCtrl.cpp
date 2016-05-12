/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FTreeCtrl.cpp
* 描述：		FTreeCtrl
* 创建日期：	2013-08-24
*
*/

#include "StdAfx.h"
#include "widgets/FTreeCtrl.h"
#include "widgets/FScrollBar.h"

#define FTREECTRL_INLINE FOOINLINE

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FTreeCtrl
	/*explicit*/ FTreeCtrl::FTreeCtrl(FWidget* parent/* = NULL*/) : 
		FAbstractList(parent)
	{
	}

	FTreeCtrl::~FTreeCtrl(void)
	{

	}

	//////////////////////////////////////////////////////////////////////////
	// FTreeCtrlElement
	/*explicit*/ FTreeCtrlElement::FTreeCtrlElement(FWidget* parent/* = NULL*/) : 
	FAbstractListElement(parent)
	{
	}

	FTreeCtrlElement::~FTreeCtrlElement(void)
	{

	}

}} // !namespace