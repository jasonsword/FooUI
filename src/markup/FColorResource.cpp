/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FColorResource.h
* 描述：		FColorResource
* 创建日期：	2013-06-21
*
*/

#include "StdAfx.h"
#include "core/FObject.h"
#include "markup/FColorResource.h"
#include "markup/FMarkup.h"

#define FCOLORRESOURCE_INLINE FOOINLINE

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FColorResourcePrivate
	class FColorResource::FColorResourcePrivate : public Core::FReferenceObject, public Core::FNoncopyableObject
	{
	public:
		bool setCOLORREF(COLORREF clr);
		bool loadFromSysColor(LPCTSTR pszName);

	public:
		Painting::FColor m_color;
	};

	// 
	FCOLORRESOURCE_INLINE bool FColorResource::FColorResourcePrivate::setCOLORREF(COLORREF clr)
	{
		m_color.setColor(GetRValue(clr), GetGValue(clr), GetBValue(clr));
		return true;
	}

	FCOLORRESOURCE_INLINE bool FColorResource::FColorResourcePrivate::loadFromSysColor(LPCTSTR pszName)
	{
		#define COLOR_HANDLER(_pszName, _nColor) \
		if (_tcscmp(pszName, _pszName) == 0) \
		{ \
			return setCOLORREF(::GetSysColor(_nColor)); \
		}
		COLOR_HANDLER(_T("COLOR_SCROLLBAR"), COLOR_SCROLLBAR);
		COLOR_HANDLER(_T("COLOR_BACKGROUND"), COLOR_BACKGROUND);
		COLOR_HANDLER(_T("COLOR_ACTIVECAPTION"), COLOR_ACTIVECAPTION);
		COLOR_HANDLER(_T("COLOR_INACTIVECAPTION"), COLOR_INACTIVECAPTION);
		COLOR_HANDLER(_T("COLOR_MENU"), COLOR_MENU);
		COLOR_HANDLER(_T("COLOR_WINDOW"), COLOR_WINDOW);
		COLOR_HANDLER(_T("COLOR_WINDOWFRAME"), COLOR_WINDOWFRAME);
		COLOR_HANDLER(_T("COLOR_MENUTEXT"), COLOR_MENUTEXT);
		COLOR_HANDLER(_T("COLOR_WINDOWTEXT"), COLOR_WINDOWTEXT);
		COLOR_HANDLER(_T("COLOR_CAPTIONTEXT"), COLOR_CAPTIONTEXT);
		COLOR_HANDLER(_T("COLOR_ACTIVEBORDER"), COLOR_ACTIVEBORDER);
		COLOR_HANDLER(_T("COLOR_INACTIVEBORDER"), COLOR_INACTIVEBORDER);
		COLOR_HANDLER(_T("COLOR_APPWORKSPACE"), COLOR_APPWORKSPACE);
		COLOR_HANDLER(_T("COLOR_HIGHLIGHT"), COLOR_HIGHLIGHT);
		COLOR_HANDLER(_T("COLOR_HIGHLIGHTTEXT"), COLOR_HIGHLIGHTTEXT);
		COLOR_HANDLER(_T("COLOR_BTNFACE"), COLOR_BTNFACE);
		COLOR_HANDLER(_T("COLOR_BTNSHADOW"), COLOR_BTNSHADOW);
		COLOR_HANDLER(_T("COLOR_GRAYTEXT"), COLOR_GRAYTEXT);
		COLOR_HANDLER(_T("COLOR_BTNTEXT"), COLOR_BTNTEXT);
		COLOR_HANDLER(_T("COLOR_INACTIVECAPTIONTEXT"), COLOR_INACTIVECAPTIONTEXT);
		COLOR_HANDLER(_T("COLOR_BTNHIGHLIGHT"), COLOR_BTNHIGHLIGHT);

		#if(WINVER >= 0x0400)
		COLOR_HANDLER(_T("COLOR_3DDKSHADOW"), COLOR_3DDKSHADOW);
		COLOR_HANDLER(_T("COLOR_3DLIGHT"), COLOR_3DLIGHT);
		COLOR_HANDLER(_T("COLOR_INFOTEXT"), COLOR_INFOTEXT);
		COLOR_HANDLER(_T("COLOR_INFOBK"), COLOR_INFOBK);
		#endif /* WINVER >= 0x0400 */

		#if(WINVER >= 0x0500)
		COLOR_HANDLER(_T("COLOR_HOTLIGHT"), COLOR_HOTLIGHT);
		COLOR_HANDLER(_T("COLOR_GRADIENTACTIVECAPTION"), COLOR_GRADIENTACTIVECAPTION);
		COLOR_HANDLER(_T("COLOR_GRADIENTINACTIVECAPTION"), COLOR_GRADIENTINACTIVECAPTION);
		#if(WINVER >= 0x0501)
		COLOR_HANDLER(_T("COLOR_MENUHILIGHT"), COLOR_MENUHILIGHT);
		COLOR_HANDLER(_T("COLOR_MENUBAR"), COLOR_MENUBAR);
		#endif /* WINVER >= 0x0501 */
		#endif /* WINVER >= 0x0500 */

		#if(WINVER >= 0x0400)
		COLOR_HANDLER(_T("COLOR_DESKTOP"), COLOR_DESKTOP);
		COLOR_HANDLER(_T("COLOR_3DFACE"), COLOR_3DFACE);
		COLOR_HANDLER(_T("COLOR_3DSHADOW"), COLOR_3DSHADOW);
		COLOR_HANDLER(_T("COLOR_3DHIGHLIGHT"), COLOR_3DHIGHLIGHT);
		COLOR_HANDLER(_T("COLOR_3DHILIGHT"), COLOR_3DHILIGHT);
		COLOR_HANDLER(_T("COLOR_BTNHILIGHT"), COLOR_BTNHILIGHT);
		#endif /* WINVER >= 0x0400 */

		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// FColorResource
	FCOLORRESOURCE_INLINE FColorResource::FColorResource(void)
	{
		m_colorResPrivate = new FColorResourcePrivate;
		m_colorResPrivate->addRef();
	}

	FCOLORRESOURCE_INLINE FColorResource::FColorResource(const FColorResource& res)
	{
		FASSERT(this != &res);
		m_colorResPrivate = res.m_colorResPrivate;
		m_colorResPrivate->addRef();
	}

	FCOLORRESOURCE_INLINE FColorResource& FColorResource::operator=(const FColorResource& res)
	{
		if (m_colorResPrivate != res.m_colorResPrivate)
		{
			m_colorResPrivate->release();
			m_colorResPrivate = res.m_colorResPrivate;
			m_colorResPrivate->addRef();
		}

		return (*this);
	}

	FCOLORRESOURCE_INLINE FColorResource::~FColorResource(void)
	{
		FASSERT(NULL != m_colorResPrivate);
		m_colorResPrivate->release();
		m_colorResPrivate = NULL;
	}

	FCOLORRESOURCE_INLINE Painting::FColor& FColorResource::getColor(void)
	{
		return m_colorResPrivate->m_color;
	}

	FCOLORRESOURCE_INLINE const Painting::FColor& FColorResource::getColor(void) const
	{
		return m_colorResPrivate->m_color;
	}

	FCOLORRESOURCE_INLINE bool FColorResource::parseRGB(LPCTSTR pszValue)
	{
		FASSERT(NULL != m_colorResPrivate);
		FASSERT(NULL != pszValue);

		LPTSTR pstrRGB = NULL;
		Painting::FOOBYTE r = (Painting::FOOBYTE)_tcstoul(pszValue, &pstrRGB, 10);	FASSERT(NULL != pstrRGB);
		Painting::FOOBYTE g = (Painting::FOOBYTE)_tcstoul(pstrRGB + 1, &pstrRGB, 10); FASSERT(NULL != pstrRGB);
		Painting::FOOBYTE b = (Painting::FOOBYTE)_tcstoul(pstrRGB + 1, &pstrRGB, 10);

		m_colorResPrivate->m_color.setColor(r, g, b);

		return true;
	}

	FCOLORRESOURCE_INLINE bool FColorResource::parseARGB(LPCTSTR pszValue)
	{
		FASSERT(NULL != m_colorResPrivate);
		FASSERT(NULL != pszValue);

		LPTSTR pstrRGB = NULL;
		Painting::FOOBYTE a = (Painting::FOOBYTE)_tcstoul(pszValue, &pstrRGB, 10);	FASSERT(NULL != pstrRGB);
		Painting::FOOBYTE r = (Painting::FOOBYTE)_tcstoul(pstrRGB + 1, &pstrRGB, 10); FASSERT(NULL != pstrRGB);
		Painting::FOOBYTE g = (Painting::FOOBYTE)_tcstoul(pstrRGB + 1, &pstrRGB, 10); FASSERT(NULL != pstrRGB);
		Painting::FOOBYTE b = (Painting::FOOBYTE)_tcstoul(pstrRGB + 1, &pstrRGB, 10);

		m_colorResPrivate->m_color.setColor(a, r, g, b);

		return true;
	}

	FCOLORRESOURCE_INLINE bool FColorResource::parseColor(LPCTSTR pszValue)
	{
		FASSERT(NULL != m_colorResPrivate);
		FASSERT(NULL != pszValue);

		if (pszValue[0] == _T('#') 
			|| pszValue[0] == _T('$'))
		{
			if (!m_colorResPrivate->loadFromSysColor(pszValue + 1))
			{
				m_colorResPrivate->m_color.setColor((Painting::FOOARGB)_tcstoul(pszValue + 1, NULL, 16));
			}
			return true;
		}

		/** rgb(r, g, b)形式 */
		LPCTSTR pstrRGBResult = _tcsstr(pszValue, _T("rgb("));
		if (pstrRGBResult == pszValue)
		{
			return parseRGB(pszValue + _tcslen(_T("rgb(")));
		}

		/** argb(a, r, g, b)形式 */
		LPCTSTR pstrARGBResult = _tcsstr(pszValue, _T("argb("));
		if (pstrARGBResult == pszValue)
		{
			return parseARGB(pszValue + _tcslen(_T("argb(")));
		}

		/** color(#FFRRGGBB)形式 */
		LPCTSTR pstrCOLORResult = _tcsstr(pszValue, _T("color("));
		if (pstrCOLORResult == pszValue)
		{
			LPTSTR pstrCOLOR = (LPTSTR)(pszValue + _tcslen(_T("color(")));
			if (pstrCOLOR[0] == _T('#') || pstrCOLOR[0] == _T('$'))
				m_colorResPrivate->m_color.setColor((Painting::FOOARGB)_tcstoul(pstrCOLOR + 1, &pstrCOLOR, 16));
			else
				m_colorResPrivate->m_color.setColor((Painting::FOOARGB)_tcstoul(pstrCOLOR, &pstrCOLOR, 16));

			return true;
		}

		return false;
	}

	FCOLORRESOURCE_INLINE bool FColorResource::parseAttribute(FMarkup* pMarkup, LPCTSTR pszValue)
	{
		FASSERT(NULL != m_colorResPrivate);
		FASSERT(NULL != pMarkup);
		FASSERT(NULL != pszValue);

		if (pszValue[0] == _T('#'))
		{
			FColorResource *pColorRes = (FColorResource*)pMarkup->getResource(pszValue + 1, FResourceObject::FRESTYPE_COLOR);
			if (NULL != pColorRes)
			{
				*this = *pColorRes;
				return true;
			}
		}

		if (__super::parseAttribute(pMarkup, pszValue))
		{
			return true;
		}

		if (parseColor(pszValue))
		{
			return true;
		}

		return true;
	}

	FCOLORRESOURCE_INLINE bool FColorResource::setAttribute(FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		FASSERT(NULL != m_colorResPrivate);
		FASSERT(NULL != pMarkup);
		FASSERT(NULL != pszName);
		FASSERT(NULL != pszValue);

		if (_tcscmp(pszName, _T("color")) == 0)
		{
			parseColor(pszValue);
		}
		else if (_tcscmp(pszName, _T("rgb")) == 0)
		{
			parseRGB(pszValue);
		}
		else if (_tcscmp(pszName, _T("argb")) == 0)
		{
			parseARGB(pszValue);
		}

		return __super::setAttribute(pMarkup, pszName, pszValue);
	}

}} // !namespace