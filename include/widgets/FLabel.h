/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FLabel.h
* ������		FLabel, ��̬�ı��ؼ�
* �������ڣ�	2013-07-25
*
*/

#ifndef FLABEL_H
#define FLABEL_H

#include "core/FBase.h"
#include "widgets/FWidget.h"

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FLabel
	#define FOOWC_LABEL _T("label")
	class FOOUI_LIB FLabel : public FWidget
	{
	public:
		explicit FLabel(FWidget* parent = NULL);
		~FLabel(void);
		FOODECLARE_CLASS(FOOWC_LABEL)
		FOODECLARE_CONTAINER(false)
	};

}} // !namespace

#endif // !FLABEL_H