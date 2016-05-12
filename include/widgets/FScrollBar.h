/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FScrollBar.h
* 描述：		FScrollBar
* 创建日期：	2013-06-25
*
*/

#ifndef FSCROLLBAR_H
#define FSCROLLBAR_H

#include "core/FBase.h"
#include "widgets/FAbstractSlider.h"

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FScrollBar
	#define FOOWC_SCROLLBAR _T("scrollbar")
	#define FOOWC_HORZSCROLLBAR _T("horzscrollbar")
	#define FOOWC_VERTSCROLLBAR _T("vertscrollbar")
	class FOOUI_LIB FScrollBar : public FAbstractSlider
	{
	public:
		enum eFScrollBarOrientation
		{
			HorzScrollBar = SBS_HORZ, 
			VertScrollBar = SBS_VERT, 
		};
		explicit FScrollBar(DWORD dwOrientation = HorzScrollBar, FWidget* parent = NULL);
		~FScrollBar(void);
		FOODECLARE_CLASS(FOOWC_SCROLLBAR)
		FOODECLARE_CONTAINER(false)

	protected:
		void paintEvent(Event::FPaintEvent* e);
		void mousePressEvent(Event::FMouseEvent* e);
		void mouseReleaseEvent(Event::FMouseEvent* e);
		void mouseDoubleClickEvent(Event::FMouseEvent* e);
		void mouseMoveEvent(Event::FMouseEvent* e);
		void mouseEnterEvent(Event::FEvent* e);
		void mouseLeaveEvent(Event::FEvent* e);
		void layoutChangedEvent(Event::FLayoutChangedEvent* e);

	protected:
		void sliderPropertiesChanged(void);

	protected:
		bool setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue);

	private:
		class FScrollBarPrivate;
		friend FScrollBarPrivate;
		FScrollBarPrivate* const m_scrollbarPrivate;
	};
}} // !namespace

#endif // !FSCROLLBAR_H