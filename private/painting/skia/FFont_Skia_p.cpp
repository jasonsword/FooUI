/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FFont_Skia_p.cpp
* 描述：		FFont_Skia_p
* 创建日期：	2013-06-14
*
*/

#include "StdAfx.h"
#include "painting/skia/FFont_Skia_p.h"

#ifdef FOOUI_SUPPORT_SKIA
namespace FooUI { namespace Painting {

	//////////////////////////////////////////////////////////////////////////
	// FFontPrivate
	FFont::FFontPrivate::FFontPrivate(void) : m_hFont(NULL)
	{
		//create();
	}

	FFont::FFontPrivate::~FFontPrivate(void)
	{
		destroy();
	}

	bool FFont::FFontPrivate::create(void)
	{
		if (NULL != m_hFont)
		{
			FASSERT(false);
			return false;
		}

		return copy((HFONT)::GetStockObject(DEFAULT_GUI_FONT));
	}

	bool FFont::FFontPrivate::create(const LOGFONT* lplf)
	{
		if (NULL != m_hFont || NULL == lplf)
		{
			FASSERT(false);
			return false;
		}

		m_hFont = ::CreateFontIndirect(lplf);

		return (NULL != m_hFont);
	}

	bool FFont::FFontPrivate::create(LPCTSTR pszFaceName, 
									int nSize, 
									bool bBold, 
									bool bItalic, 
									bool bUnderLine, 
									bool bStrickeOut)
	{
		if (NULL != m_hFont)
		{
			FASSERT(false);
			return false;
		}

		LOGFONT lf = {0};
		::GetObject((HFONT)::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);

		if (NULL != pszFaceName) _tcscpy_s(lf.lfFaceName, LF_FACESIZE, pszFaceName);
		if (nSize != 0)
		{
			lf.lfWidth = 0;
			lf.lfHeight = -nSize;
		}
		lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
		lf.lfItalic = bItalic;
		lf.lfUnderline = bUnderLine;
		lf.lfStrikeOut = bStrickeOut;

		return create(&lf);
	}

	bool FFont::FFontPrivate::copy(HFONT hFont)
	{
		if (NULL != m_hFont || NULL == hFont)
		{
			FASSERT(false);
			return false;
		}

		LOGFONT lf = {0};
		::GetObject(hFont, sizeof(LOGFONT), &lf);

		return create(&lf);
	}

	bool FFont::FFontPrivate::destroy(void)
	{
		if (NULL != m_hFont)
		{
			fooSafeDeleteObject(m_hFont);
		}

		return true;
	}

	bool FFont::FFontPrivate::isNull(void) const
	{
		return (NULL == m_hFont);
	}

	LPVOID FFont::FFontPrivate::font(void) const
	{
		return (LPVOID)handle();
	}

	HFONT FFont::FFontPrivate::handle(void) const
	{
		if (isNull()) const_cast<FFont::FFontPrivate*>(this)->create();
		return m_hFont;
	}

}} // !namespace
#endif // !FOOUI_SUPPORT_SKIA