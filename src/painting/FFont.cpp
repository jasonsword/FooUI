/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FFont.cpp
* 描述：		FFont
* 创建日期：	2013-06-06
*
*/

#include "StdAfx.h"
#include "painting/FFont.h"
#include "F3rd_p.h"

#define FFONT_INLINE FOOINLINE

namespace FooUI { namespace Painting {

	//////////////////////////////////////////////////////////////////////////
	// FFont
	FFONT_INLINE FFont::FFont(void)
	{
		m_fontPrivate = new FFontPrivate;
		m_fontPrivate->addRef();
	}

	FFONT_INLINE FFont::FFont(const FFont& font)
	{
		m_fontPrivate = font.m_fontPrivate;
		m_fontPrivate->addRef();
	}

	FFONT_INLINE FFont& FFont::operator=(const FFont& font)
	{
		if (m_fontPrivate != font.m_fontPrivate)
		{
			m_fontPrivate->release();
			m_fontPrivate = font.m_fontPrivate;
			m_fontPrivate->addRef();
		}

		return (*this);
	}

	FFONT_INLINE FFont::~FFont(void)
	{
		m_fontPrivate->release();
		m_fontPrivate = NULL;
	}

	FFONT_INLINE bool FFont::create(void)
	{
		FASSERT(NULL != m_fontPrivate);
		return m_fontPrivate->create();
	}

	FFONT_INLINE bool FFont::create(const LOGFONT* lplf)
	{
		FASSERT(NULL != m_fontPrivate);
		return m_fontPrivate->create(lplf);
	}

	FFONT_INLINE bool FFont::create(LPCTSTR pszFaceName, int nSize, 
									bool bBold, 
									bool bItalic, 
									bool bUnderLine, 
									bool bStrickeOut)
	{
		FASSERT(NULL != m_fontPrivate);
		return m_fontPrivate->create(pszFaceName, nSize, bBold, bItalic, bUnderLine, bStrickeOut);
	}

	FFONT_INLINE bool FFont::copy(HFONT hFont)
	{
		FASSERT(NULL != m_fontPrivate);
		return m_fontPrivate->copy(hFont);
	}

	FFONT_INLINE bool FFont::destroy(void)
	{
		FASSERT(NULL != m_fontPrivate);
		return m_fontPrivate->destroy();
	}

	FFONT_INLINE bool FFont::isNull(void) const
	{
		FASSERT(NULL != m_fontPrivate);
		return m_fontPrivate->isNull();
	}

	FFONT_INLINE LPVOID FFont::font(void) const
	{
		FASSERT(NULL != m_fontPrivate);
		return m_fontPrivate->font();
	}

	FFONT_INLINE HFONT FFont::handle(void) const
	{
		FASSERT(NULL != m_fontPrivate);
		return m_fontPrivate->handle();
	}

}} // !namespace