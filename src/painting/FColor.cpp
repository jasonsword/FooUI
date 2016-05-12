/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FColor.cpp
* 描述：		FColor
* 创建日期：	2013-06-14
*
*/

#include "StdAfx.h"
#include "painting/FColor.h"

#define FCOLOR_INLINE FOOINLINE

namespace FooUI { namespace Painting {
	//////////////////////////////////////////////////////////////////////////
	// FColor
	FCOLOR_INLINE FColor::FColor(void) : m_argb(0x00000000)
	{
	}

	FCOLOR_INLINE FColor::FColor(FOOBYTE r, FOOBYTE g, FOOBYTE b)
	{
		m_argb = fooMakeRGB(r, g, b);
	}

	FCOLOR_INLINE FColor::FColor(FOOBYTE a, FOOBYTE r, FOOBYTE g, FOOBYTE b)
	{
		m_argb = fooMakeARGB(a, r, g, b);
	}

	FCOLOR_INLINE FColor::FColor(FOOARGB argb)
	{
		m_argb = argb;
	}

	FCOLOR_INLINE FColor::FColor(const FColor& clr)
	{
		m_argb = clr.color();
	}

	FCOLOR_INLINE void FColor::setColor(FOOARGB argb)
	{
		m_argb = argb;
	}

	FCOLOR_INLINE void FColor::setColor(FOOBYTE r, FOOBYTE g, FOOBYTE b)
	{
		m_argb = fooMakeRGB(r, g, b);
	}

	FCOLOR_INLINE void FColor::setColor(FOOBYTE a, FOOBYTE r, FOOBYTE g, FOOBYTE b)
	{
		m_argb = fooMakeARGB(a, r, g, b);
	}

	FCOLOR_INLINE FOOBYTE FColor::alpha(void) const
	{
		return FCOLOR_GET_ALPHA(m_argb);
	}

	FCOLOR_INLINE FOOBYTE FColor::red(void) const
	{
		return FCOLOR_GET_RED(m_argb);
	}

	FCOLOR_INLINE FOOBYTE FColor::green(void) const
	{
		return FCOLOR_GET_GREEN(m_argb);
	}

	FCOLOR_INLINE FOOBYTE FColor::blue(void) const
	{
		return FCOLOR_GET_BLUE(m_argb);
	}

	FCOLOR_INLINE FOOARGB FColor::color(void) const
	{
		return m_argb;
	}

	FCOLOR_INLINE COLORREF FColor::toCOLORREF(void) const
	{
		return RGB(red(), green(), blue());
	}

	FCOLOR_INLINE /*static*/ FColor FColor::make(FOOBYTE r, FOOBYTE g, FOOBYTE b)
	{
		return FColor(r, g, b);
	}

	FCOLOR_INLINE /*static*/ FColor FColor::make(FOOBYTE a, FOOBYTE r, FOOBYTE g, FOOBYTE b)
	{
		return FColor(a, r, g, b);
	}

	FCOLOR_INLINE bool FColor::operator==(FOOARGB argb) const
	{
		return (m_argb == argb);
	}

	FCOLOR_INLINE bool FColor::operator!=(FOOARGB argb) const
	{
		return (m_argb != argb);
	}

	FCOLOR_INLINE FOOARGB FColor::operator=(FOOARGB argb)
	{
		return (m_argb = argb);
	}

}} // !namespace