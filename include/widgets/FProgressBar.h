/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FProgressBar.h
* 描述：		FProgressBar
* 创建日期：	2013-07-25
*
*/

#ifndef FPROGRESSBAR_H
#define FPROGRESSBAR_H

#include "core/FBase.h"
#include "widgets/FAbstractSlider.h"

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FProgressBar
	#define FOOWC_PROGRESSBAR _T("progressbar")
	#define FOOWC_HORZPROGRESSBAR _T("horzprogressbar")
	#define FOOWC_VERTPROGRESSBAR _T("vertprogressbar")
	class FOOUI_LIB FProgressBar : public FAbstractSlider
	{
	public:
		enum eFProgressBarOrientation
		{
			HorzProgressBar = SBS_HORZ, 
			VertProgressBar = SBS_VERT, 
		};
		explicit FProgressBar(DWORD dwOrientation = HorzProgressBar, FWidget* parent = NULL);
		~FProgressBar(void);
		FOODECLARE_CLASS(FOOWC_PROGRESSBAR)
		FOODECLARE_CONTAINER(true)

	public:
		void setProgressBarOrientation(DWORD dwOrientation);
		DWORD getProgressBarOrientation(void) const;

	protected:
		void sliderPropertiesChanged(void);

	protected:
		void paintEvent(Event::FPaintEvent* e);

	protected:
		bool setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue);

	private:
		DWORD m_dwOrientation;
	};

}} // !namespace
#endif // !FPROGRESSBAR_H