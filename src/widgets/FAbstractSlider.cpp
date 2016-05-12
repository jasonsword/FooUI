/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FAbstractSlider.cpp
* 描述：		FAbstractSlider
* 创建日期：	2013-06-25
*
*/

#include "StdAfx.h"
#include "widgets/FAbstractSlider.h"

#define FABSTRACTSLIDER_INLINE FOOINLINE

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FAbstractSlider
	FABSTRACTSLIDER_INLINE FAbstractSlider::FAbstractSlider(FWidget* parent) : 
		FWidget(parent) 
		, m_minimum(0) 
		, m_maximum(0) 
		, m_page(0) 
		, m_position(0) 
	{

	}

	FABSTRACTSLIDER_INLINE void FAbstractSlider::setMinimum(int min)
	{
		setRange(min, max(min, m_maximum));
	}

	FABSTRACTSLIDER_INLINE int FAbstractSlider::minimum(void) const
	{
		return m_minimum;
	}

	FABSTRACTSLIDER_INLINE void FAbstractSlider::setMaximum(int max)
	{
		setRange(min(max, m_minimum), max);
	}

	FABSTRACTSLIDER_INLINE int FAbstractSlider::maximum(void) const
	{
		return m_maximum;
	}

	FABSTRACTSLIDER_INLINE void FAbstractSlider::setRange(int min, int max)
	{
		m_minimum = min;
		m_maximum = max(min, max);
		m_position = max(m_position, m_minimum);
		m_position = min(m_position, m_maximum);

		sliderPropertiesChanged();
		makeWidgetMessage(FWM_CHANGED, CT_VALUE, 0L);
	}

	FABSTRACTSLIDER_INLINE void FAbstractSlider::setPage(int page)
	{
		m_page = page;

		sliderPropertiesChanged();
	}

	FABSTRACTSLIDER_INLINE int FAbstractSlider::page(void) const
	{
		return m_page;
	}

	FABSTRACTSLIDER_INLINE void FAbstractSlider::setSliderPosition(int position)
	{
		m_position = position;
		m_position = max(m_position, m_minimum);
		m_position = min(m_position, m_maximum);
		makeWidgetMessage(FWM_CHANGED, CT_VALUE, 0L);
	}

	FABSTRACTSLIDER_INLINE int FAbstractSlider::sliderPosition(void) const
	{
		return m_position;
	}

	FABSTRACTSLIDER_INLINE double FAbstractSlider::getSliderPercent(void) const
	{
		if (m_maximum <= m_minimum) return 0.0f;
		return (double)((double)m_position / (double)(m_maximum - m_minimum));
	}

	FABSTRACTSLIDER_INLINE bool FAbstractSlider::setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		if (_tcscmp(pszName, _T("max")) == 0 
			|| _tcscmp(pszName, _T("maximum")) == 0)
		{
			setMaximum(_tcstol(pszValue, NULL, 10));
		}
		else if (_tcscmp(pszName, _T("min")) == 0 
			|| _tcscmp(pszName, _T("minimum")) == 0)
		{
			setMinimum(_tcstol(pszValue, NULL, 10));
		}
		else if (_tcscmp(pszName, _T("value")) == 0 
			|| _tcscmp(pszName, _T("sliderposition")) == 0)
		{
			setSliderPosition(_tcstol(pszValue, NULL, 10));
		}

		return __super::setAttribute(pMarkup, pszName, pszValue);
	}
}} // !namespace