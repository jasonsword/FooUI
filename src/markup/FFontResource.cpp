/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FFontResource.h
* 描述：		FFontResource
* 创建日期：	2013-06-21
*
*/

#include "StdAfx.h"
#include "core/FObject.h"
#include "markup/FFontResource.h"
#include "markup/FMarkup.h"

#define FFONTRESOURCE_INLINE FOOINLINE
namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FFontResourcePrivate
	class FFontResource::FFontResourcePrivate : public Core::FReferenceObject, public Core::FNoncopyableObject
	{
	public:
		FFontResourcePrivate(void)
		{
			memset(&m_initFont, 0, sizeof(LOGFONT));
			::GetObject((HFONT)::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &m_initFont);
		}

	public:
		Painting::FFont m_font;
		LOGFONT m_initFont;
	};

	//////////////////////////////////////////////////////////////////////////
	// FFontResource
	FFONTRESOURCE_INLINE FFontResource::FFontResource(void)
	{
		m_fontResPrivate = new FFontResourcePrivate;
		m_fontResPrivate->addRef();
	}

	FFONTRESOURCE_INLINE FFontResource::FFontResource(const FFontResource& res)
	{
		FASSERT(this != &res);
		m_fontResPrivate = res.m_fontResPrivate;
		m_fontResPrivate->addRef();
	}

	FFONTRESOURCE_INLINE FFontResource& FFontResource::operator=(const FFontResource& res)
	{
		if (m_fontResPrivate != res.m_fontResPrivate)
		{
			m_fontResPrivate->release();
			m_fontResPrivate = res.m_fontResPrivate;
			m_fontResPrivate->addRef();
		}

		return (*this);
	}

	FFONTRESOURCE_INLINE FFontResource::~FFontResource(void)
	{
		FASSERT(NULL != m_fontResPrivate);
		m_fontResPrivate->release();
		m_fontResPrivate = NULL;
	}

	FFONTRESOURCE_INLINE const Painting::FFont& FFontResource::font(void) const
	{
		FASSERT(NULL != m_fontResPrivate);
		return m_fontResPrivate->m_font;
	}

	FFONTRESOURCE_INLINE bool FFontResource::isNull(void) const
	{
		FASSERT(NULL != m_fontResPrivate);
		return m_fontResPrivate->m_font.isNull();
	}

	FFONTRESOURCE_INLINE bool FFontResource::parseAttribute(FMarkup* pMarkup, LPCTSTR pszValue)
	{
		FASSERT(NULL != m_fontResPrivate);
		FASSERT(NULL != pMarkup);
		FASSERT(NULL != pszValue);

		if (pszValue[0] == _T('#'))
		{
			FFontResource *pFontRes = (FFontResource*)pMarkup->getResource(pszValue + 1, FResourceObject::FRESTYPE_FONT);
			if (NULL != pFontRes)
			{
				*this = *pFontRes;
				return true;
			}
		}

		return __super::parseAttribute(pMarkup, pszValue);
	}

	FFONTRESOURCE_INLINE bool FFontResource::setAttribute(FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		if (_tcscmp(pszName, _T("facename")) == 0 
			|| _tcscmp(pszName, _T("fontname")) == 0)
		{
			_tcscpy_s(m_fontResPrivate->m_initFont.lfFaceName, LF_FACESIZE, pszValue);
		}
		else if (_tcscmp(pszName, _T("size")) == 0 
			|| _tcscmp(pszName, _T("fontsize")) == 0)
		{
			m_fontResPrivate->m_initFont.lfWidth	= 0;
			m_fontResPrivate->m_initFont.lfHeight = -(_tcstol(pszValue, NULL, 10));
		}
		else if (_tcscmp(pszName, _T("italic")) == 0)
		{
			m_fontResPrivate->m_initFont.lfItalic = fooMarkupParseBoolean(pszValue) ? TRUE : FALSE;
		}
		else if (_tcscmp(pszName, _T("underline")) == 0)
		{
			m_fontResPrivate->m_initFont.lfUnderline = fooMarkupParseBoolean(pszValue) ? TRUE : FALSE;
		}
		else if (_tcscmp(pszName, _T("strikeout")) == 0)
		{
			m_fontResPrivate->m_initFont.lfStrikeOut = fooMarkupParseBoolean(pszValue) ? TRUE : FALSE;
		}
		else if (_tcscmp(pszName, _T("thin")) == 0)
		{
			if (fooMarkupParseBoolean(pszValue))
				m_fontResPrivate->m_initFont.lfWeight |= FW_THIN;
			else
				m_fontResPrivate->m_initFont.lfWeight &= ~FW_THIN;
		}
		else if (_tcscmp(pszName, _T("extralight")) == 0)
		{
			if (fooMarkupParseBoolean(pszValue))
				m_fontResPrivate->m_initFont.lfWeight |= FW_EXTRALIGHT;
			else
				m_fontResPrivate->m_initFont.lfWeight &= ~FW_EXTRALIGHT;
		}
		else if (_tcscmp(pszName, _T("ultralight")) == 0)
		{
			if (fooMarkupParseBoolean(pszValue))
				m_fontResPrivate->m_initFont.lfWeight |= FW_ULTRALIGHT;
			else
				m_fontResPrivate->m_initFont.lfWeight &= ~FW_ULTRALIGHT;
		}
		else if (_tcscmp(pszName, _T("normal")) == 0)
		{
			if (fooMarkupParseBoolean(pszValue))
				m_fontResPrivate->m_initFont.lfWeight |= FW_NORMAL;
			else
				m_fontResPrivate->m_initFont.lfWeight &= ~FW_NORMAL;
		}
		else if (_tcscmp(pszName, _T("regular")) == 0)
		{
			if (fooMarkupParseBoolean(pszValue))
				m_fontResPrivate->m_initFont.lfWeight |= FW_REGULAR;
			else
				m_fontResPrivate->m_initFont.lfWeight &= ~FW_REGULAR;
		}
		else if (_tcscmp(pszName, _T("medium")) == 0)
		{
			if (fooMarkupParseBoolean(pszValue))
				m_fontResPrivate->m_initFont.lfWeight |= FW_MEDIUM;
			else
				m_fontResPrivate->m_initFont.lfWeight &= ~FW_MEDIUM;
		}
		else if (_tcscmp(pszName, _T("semibold")) == 0)
		{
			if (fooMarkupParseBoolean(pszValue))
				m_fontResPrivate->m_initFont.lfWeight |= FW_SEMIBOLD;
			else
				m_fontResPrivate->m_initFont.lfWeight &= ~FW_SEMIBOLD;
		}
		else if (_tcscmp(pszName, _T("demibold")) == 0)
		{
			if (fooMarkupParseBoolean(pszValue))
				m_fontResPrivate->m_initFont.lfWeight |= FW_DEMIBOLD;
			else
				m_fontResPrivate->m_initFont.lfWeight &= ~FW_DEMIBOLD;
		}
		else if (_tcscmp(pszName, _T("bold")) == 0)
		{
			if (fooMarkupParseBoolean(pszValue))
				m_fontResPrivate->m_initFont.lfWeight |= FW_BOLD;
			else
				m_fontResPrivate->m_initFont.lfWeight &= ~FW_BOLD;
		}
		else if (_tcscmp(pszName, _T("extrabold")) == 0)
		{
			if (fooMarkupParseBoolean(pszValue))
				m_fontResPrivate->m_initFont.lfWeight |= FW_EXTRABOLD;
			else
				m_fontResPrivate->m_initFont.lfWeight &= ~FW_EXTRABOLD;
		}
		else if (_tcscmp(pszName, _T("ultrabold")) == 0)
		{
			if (fooMarkupParseBoolean(pszValue))
				m_fontResPrivate->m_initFont.lfWeight |= FW_ULTRABOLD;
			else
				m_fontResPrivate->m_initFont.lfWeight &= ~FW_ULTRABOLD;
		}
		else if (_tcscmp(pszName, _T("heavy")) == 0)
		{
			if (fooMarkupParseBoolean(pszValue))
				m_fontResPrivate->m_initFont.lfWeight |= FW_HEAVY;
			else
				m_fontResPrivate->m_initFont.lfWeight &= ~FW_HEAVY;
		}
		else if (_tcscmp(pszName, _T("black")) == 0)
		{
			if (fooMarkupParseBoolean(pszValue))
				m_fontResPrivate->m_initFont.lfWeight |= FW_BLACK;
			else
				m_fontResPrivate->m_initFont.lfWeight &= ~FW_BLACK;
		}

		return __super::setAttribute(pMarkup, pszName, pszValue);
	}

	FFONTRESOURCE_INLINE bool FFontResource::endMarkup(FMarkup* pMarkup)
	{
		FASSERT(NULL != m_fontResPrivate);
		return m_fontResPrivate->m_font.create(&m_fontResPrivate->m_initFont);
	}

}} // !namespace