/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FAbstractSlider.h
* 描述：		FAbstractSlider
* 创建日期：	2013-06-25
*
*/

#ifndef FABSTRACTSLIDER_H
#define FABSTRACTSLIDER_H

#include "core/FBase.h"

#include "widgets/FWidget.h"

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FAbstractSlider
	#define FOOWC_ABSTRACTSLIDER _T("abstractslider")
	class FOOUI_LIB FAbstractSlider : public FWidget
	{
	public:
		enum eFSliderOrientation
		{
			NoOrientation = 0, 
			Horz, 
			Vert, 
		};

		enum eFSliderAction
		{
			NoAction = 0, 
			Add, 
			Sub, 
			PageAdd, 
			PageSub, 
			Track, 
			Minimum, 
			Maximum, 
		};

		explicit FAbstractSlider(FWidget* parent = NULL);
		FOODECLARE_CLASS(FOOWC_ABSTRACTSLIDER)
		FOODECLARE_CONTAINER(false)

	public:
		/** 最小值 */
		void setMinimum(int min);
		int minimum(void) const;

		/** 最大之 */
		void setMaximum(int max);
		int maximum(void) const;

		/** 设置范围 */
		void setRange(int min, int max);

		/** 设置页大小 */
		void setPage(int page);
		int page(void) const;

		/** 滑块值 */
		void setSliderPosition(int position);
		int sliderPosition(void) const;

		/** 当前值的百分比 */
		double getSliderPercent(void) const;

	protected:
		virtual void sliderPropertiesChanged(void) = 0;

	protected:
		bool setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue);

	private:
		int m_minimum;
		int m_maximum;
		int m_page;
		int m_position;
	};

}} // !namespace

#endif // !FABSTRACTSLIDER_H