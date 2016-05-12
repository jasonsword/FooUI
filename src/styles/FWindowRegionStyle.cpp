/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FWindowRegionStyle.cpp
* 描述：		FWindowRegionStyle
* 创建日期：	2013-07-03
*
*/

#include "StdAfx.h"
#include "core/FObject.h"
#include "styles/FWindowRegionStyle.h"

#define FWINDOWREGIONSTYLE_INLINE FOOINLINE

namespace FooUI { namespace Styles {

	//////////////////////////////////////////////////////////////////////////
	// FWindowRegionStyle
	FWINDOWREGIONSTYLE_INLINE FWindowRegionStyle::FWindowRegionStyle(void) : 
		m_shape(NoneShape) 
		, m_combine(RGN_AND) 
	{
	}

	FWINDOWREGIONSTYLE_INLINE FWindowRegionStyle::FWindowRegionStyle(const FWindowRegionStyle& style)
	{
	}

	FWINDOWREGIONSTYLE_INLINE FWindowRegionStyle& FWindowRegionStyle::operator=(const FWindowRegionStyle& style)
	{
		return (*this);
	}

	FWINDOWREGIONSTYLE_INLINE FWindowRegionStyle::~FWindowRegionStyle(void)
	{

	}

	FWINDOWREGIONSTYLE_INLINE bool FWindowRegionStyle::setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		if (_tcscmp(pszName, _T("shape")) == 0)
		{
			if (_tcscmp(pszValue, _T("roundrect")) == 0)
				m_shape = RoundRectShape;
			else if (_tcscmp(pszValue, _T("rectangle")) == 0)
				m_shape = RectAngleShape;
		}
		else if (_tcscmp(pszName, _T("combine")) == 0)
		{
			if (_tcscmp(pszValue, _T("and")) == 0)
				m_combine = RGN_AND;
			else if (_tcscmp(pszValue, _T("copy")) == 0)
				m_combine = RGN_COPY;
			else if (_tcscmp(pszValue, _T("diff")) == 0)
				m_combine = RGN_DIFF;
			else if (_tcscmp(pszValue, _T("or")) == 0)
				m_combine = RGN_OR;
			else if (_tcscmp(pszValue, _T("xor")) == 0)
				m_combine = RGN_XOR;
		}
		else if (_tcscmp(pszName, _T("roundsize")) == 0)
		{
			Markup::fooMarkupParseSize(&m_roundsize, pszValue);
		}
		else if (_tcscmp(pszName, _T("deflaterect")) == 0)
		{
			Markup::fooMarkupParseEdge(&m_deflaterect, pszValue);
		}

		return __super::setAttribute(pMarkup, pszName, pszValue);
	}

}} // !namespace