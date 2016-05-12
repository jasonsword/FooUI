/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FTreeCtrl.cpp
* ������		FTreeCtrl
* �������ڣ�	2013-08-24
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