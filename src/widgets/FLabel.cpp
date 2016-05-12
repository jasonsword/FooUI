/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FLabel.cpp
* 描述：		FLabel, 静态文本控件
* 创建日期：	2013-07-25
*
*/

#include "StdAfx.h"
#include "widgets/FLabel.h"

#define FLABEL_INLINE FOOINLINE

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FLabel
	FLABEL_INLINE FLabel::FLabel(FWidget* parent/* = NULL*/) : 
		FWidget(parent)
	{
		modifyFlag(0, FOOFLAG_SKIPTAB | FOOFLAG_SKIPMOUSE | FOOFLAG_SKIPKEYBOARD | FOOFLAG_SKIPFOCUS);
	}

	FLABEL_INLINE FLabel::~FLabel(void)
	{
	}

}} // namespace