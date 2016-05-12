/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FHeader.h
* 描述：		FHeader, 列表头
* 创建日期：	2013-07-25
*
*/

#ifndef FHEADER_H
#define FHEADER_H

#include "core/FBase.h"
#include "widgets/FWidget.h"
#include "widgets/FLayout.h"

namespace FooUI { namespace Widgets {

	class FHeader;
	class FHeaderElement;
	//////////////////////////////////////////////////////////////////////////
	// FHeader
	#define FOOWC_HEADER _T("header")
	class FOOUI_LIB FHeader : public FLayout
	{
		friend FHeaderElement;
	public:
		explicit FHeader(FWidget* parent = NULL);
		~FHeader(void);
		FOODECLARE_CLASS(FOOWC_HEADER)
		FOODECLARE_CONTAINER(true)

	protected:
		FWidget* widgetFromPoint(int xPos, int yPos) const;

	protected:
		void elementWidthChanged(FHeaderElement* element, int width);
	};

	//////////////////////////////////////////////////////////////////////////
	// FHeaderElement
	#define FOOWC_HEADERELEMENT _T("headerelement")
	class FOOUI_LIB FHeaderElement : public FWidget
	{
		friend FHeader;
	public:
		explicit FHeaderElement(FWidget* parent = NULL);
		~FHeaderElement(void);
		FOODECLARE_CLASS(FOOWC_HEADERELEMENT)
		FOODECLARE_CONTAINER(true)

	public:
		void sizable(bool bSizable = true);
		bool isSizable(void) const;

	protected:
		void cursorEvent(Event::FCursorEvent* e);
		void mousePressEvent(Event::FMouseEvent* e);
		void mouseMoveEvent(Event::FMouseEvent* e);
		void mouseReleaseEvent(Event::FMouseEvent* e);
		void mouseDoubleClickEvent(Event::FMouseEvent* e);

	protected:
		bool setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue);

	protected:
		bool m_bSizable;
	};

}} // !namespace

#endif // !FHEADER_H