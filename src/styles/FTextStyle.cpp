/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FTextStyle.cpp
* 描述：		FTextStyle
* 创建日期：	2013-06-27
*
*/

#include "StdAfx.h"
#include "core/FObject.h"
#include "widgets/FWidget.h"
#include "markup/FFontResource.h"
#include "markup/FColorResource.h"
#include "markup/FStringResource.h"
#include "styles/FTextStyle.h"

#define FTEXTSTYLE_INLINE FOOINLINE

namespace FooUI { namespace Styles {

	//////////////////////////////////////////////////////////////////////////
	// FTextStylePrivate
	class FTextStyle::FTextStylePrivate : public Core::FReferenceObject, public Core::FNoncopyableObject
	{
	public:
		FTextStylePrivate(void) : 
			m_uTextAlign(DT_LEFT | DT_WORDBREAK)
		{
		}

		Markup::FFontResource m_resFont;
		Markup::FColorResource m_resColor;
		UINT m_uTextAlign;
		Painting::FPaint m_paint;
	};

	//////////////////////////////////////////////////////////////////////////
	// FStringStylePrivate
	class FStringStyle::FStringStylePrivate : public Core::FReferenceObject, public Core::FNoncopyableObject
	{
	public:
		Markup::FStringResource m_resString;
	};

	//////////////////////////////////////////////////////////////////////////
	// FTextStyle
	FTEXTSTYLE_INLINE FTextStyle::FTextStyle(void)
	{
		m_textStylePrivate = new FTextStylePrivate;
		m_textStylePrivate->addRef();
	}

	FTEXTSTYLE_INLINE FTextStyle::FTextStyle(const FTextStyle& style)
	{
		FASSERT(this != &style);
		m_textStylePrivate = style.m_textStylePrivate;
		m_textStylePrivate->addRef();
	}

	FTEXTSTYLE_INLINE FTextStyle& FTextStyle::operator=(const FTextStyle& style)
	{
		if (m_textStylePrivate != style.m_textStylePrivate)
		{
			m_textStylePrivate->release();
			m_textStylePrivate = style.m_textStylePrivate;
			m_textStylePrivate->addRef();
		}

		return (*this);
	}

	FTEXTSTYLE_INLINE FTextStyle::~FTextStyle(void)
	{
		FASSERT(NULL != m_textStylePrivate);
		m_textStylePrivate->release();
		m_textStylePrivate = NULL;
	}

	FTEXTSTYLE_INLINE bool FTextStyle::draw(Widgets::FWidget* pWidget, /** 控件对象 */
											Painting::FGraphics* pGraphics, /** 画布 */
											const Core::FRect* prcUpdate, /** 更新区域 */
											DWORD dwState, /** 状态 */
											const Core::FRect& rcDest, /** 绘图区域 */
											LPCTSTR pszType/* = NULL*/, 
											LPCTSTR pszPart/* = NULL*/, 
											FSTYLESTRUCT* fss/* = NULL*/)
	{
		return drawText(pWidget->getText(), pGraphics, prcUpdate, dwState, rcDest, pszType, pszPart, fss);
	}

	FTEXTSTYLE_INLINE bool FTextStyle::drawText(LPCTSTR pszText, /** 控件对象 */
											Painting::FGraphics* pGraphics, /** 画布 */
											const Core::FRect* prcUpdate, /** 更新区域 */
											DWORD dwState, /** 状态 */
											const Core::FRect& rcDest, /** 绘图区域 */
											LPCTSTR pszType/* = NULL*/, 
											LPCTSTR pszPart/* = NULL*/, 
											FSTYLESTRUCT* fss/* = NULL*/)
	{
		FASSERT(NULL != m_textStylePrivate);
		FASSERT(NULL != pszText);

		if (!fooTestAttribute(dwState, getState())) return false;
		if (NULL != pszType && _tcscmp(pszType, getType()) != 0) return false;
		if (NULL != pszPart && _tcscmp(pszPart, getPart()) != 0) return false;

		if (_tcslen(pszText) == 0)
		{
			return false;
		}

		Core::FRect rcDraw;
		Core::FRect rcClip;
		calcRect(rcDraw, rcClip, rcDest);
		if (rcDraw.isEmpty() || rcClip.isEmpty())
		{
			return false;
		}

		if (!rcClip.isIntersects(rcDraw))
		{
			return false;
		}

		/** 不在脏区域内 */
		if (NULL != prcUpdate 
			&& !prcUpdate->isIntersects(rcDraw))
		{
			return false;
		}

		pGraphics->setFont(m_textStylePrivate->m_resFont.font());
		pGraphics->setForeColor(m_textStylePrivate->m_resColor.getColor());
		Painting::FAutoClipRect autoclip(pGraphics, rcClip);
		return pGraphics->drawText(pszText, -1, rcDraw, m_textStylePrivate->m_uTextAlign, &m_textStylePrivate->m_paint);
	}

	FTEXTSTYLE_INLINE bool FTextStyle::setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		FASSERT(NULL != m_textStylePrivate);
		FASSERT(NULL != pMarkup);
		FASSERT(NULL != pszName);
		FASSERT(NULL != pszValue);

		if (_tcscmp(pszName, _T("textcolor")) == 0)
		{
			m_textStylePrivate->m_resColor.parseAttribute(pMarkup, pszValue);
		}
		else if (_tcscmp(pszName, _T("font")) == 0 
			|| _tcscmp(pszName, _T("textfont")) == 0)
		{
			m_textStylePrivate->m_resFont.parseAttribute(pMarkup, pszValue);
		}
		else if (_tcscmp(pszName, _T("textalign")) == 0)
		{
			if ( Markup::fooMarkupCheckNameFlag(pszValue, FOOTEXTALIGN_NAME_NONE) 
				|| Markup::fooMarkupCheckNameFlag(pszValue, FOOTEXTALIGN_NAME_LEFT))
				m_textStylePrivate->m_uTextAlign |= DT_LEFT;
			if ( Markup::fooMarkupCheckNameFlag(pszValue, FOOTEXTALIGN_NAME_TOP) )
				m_textStylePrivate->m_uTextAlign |= DT_TOP;
			if ( Markup::fooMarkupCheckNameFlag(pszValue, FOOTEXTALIGN_NAME_RIGHT) )
				m_textStylePrivate->m_uTextAlign |= DT_RIGHT;
			if ( Markup::fooMarkupCheckNameFlag(pszValue, FOOTEXTALIGN_NAME_XCENTER) )
				m_textStylePrivate->m_uTextAlign |= DT_CENTER | DT_SINGLELINE;
			if ( Markup::fooMarkupCheckNameFlag(pszValue, FOOTEXTALIGN_NAME_YCENTER) )
				m_textStylePrivate->m_uTextAlign |= DT_VCENTER | DT_SINGLELINE;
			if ( Markup::fooMarkupCheckNameFlag(pszValue, FOOTEXTALIGN_NAME_BOTTOM) )
				m_textStylePrivate->m_uTextAlign |= DT_BOTTOM | DT_SINGLELINE;
			if ( Markup::fooMarkupCheckNameFlag(pszValue, FOOTEXTALIGN_NAME_END_ELLIPSIS) )
				m_textStylePrivate->m_uTextAlign |= DT_END_ELLIPSIS;
		}
		else if (_tcscmp(pszName, _T("shadow")) == 0 
			|| _tcscmp(pszName, _T("textshadow")) == 0)
		{
			m_textStylePrivate->m_paint.setTextShadow(Markup::fooMarkupParseBoolean(pszValue));
		}
		else if (_tcscmp(pszName, _T("shadowcolor")) == 0)
		{
			Markup::FColorResource resColor;
			resColor.parseAttribute(pMarkup, pszValue);
			m_textStylePrivate->m_paint.setTextShadowColor(resColor.getColor());
		}
		else if (_tcscmp(pszName, _T("shadowoffset")) == 0)
		{
			Core::FPoint ptOffset;
			Markup::fooMarkupParsePoint(&ptOffset, pszValue);
			m_textStylePrivate->m_paint.setTextShadowOffset(ptOffset);
		}

		return __super::setAttribute(pMarkup, pszName, pszValue);
	}

	//////////////////////////////////////////////////////////////////////////
	// FStringStyle
	FTEXTSTYLE_INLINE FStringStyle::FStringStyle(void)
	{
		m_stringStylePrivate = new FStringStylePrivate;
		m_stringStylePrivate->addRef();
	}

	FTEXTSTYLE_INLINE FStringStyle::FStringStyle(const FStringStyle& style)
	{
		FASSERT(this != &style);
		m_stringStylePrivate = style.m_stringStylePrivate;
		m_stringStylePrivate->addRef();
	}

	FTEXTSTYLE_INLINE FStringStyle& FStringStyle::operator=(const FStringStyle& style)
	{
		if (m_stringStylePrivate != style.m_stringStylePrivate)
		{
			m_stringStylePrivate->release();
			m_stringStylePrivate = style.m_stringStylePrivate;
			m_stringStylePrivate->addRef();
		}

		return (*this);
	}

	FTEXTSTYLE_INLINE FStringStyle::~FStringStyle(void)
	{
		FASSERT(NULL != m_stringStylePrivate);
		m_stringStylePrivate->release();
		m_stringStylePrivate = NULL;
	}

	FTEXTSTYLE_INLINE bool FStringStyle::draw(Widgets::FWidget* pWidget, /** 控件对象 */
										Painting::FGraphics* pGraphics, /** 画布 */
										const Core::FRect* prcUpdate, /** 更新区域 */
										DWORD dwState, /** 状态 */
										const Core::FRect& rcDest, /** 绘图区域 */
										LPCTSTR pszType/* = NULL*/, 
										LPCTSTR pszPart/* = NULL*/, 
										FSTYLESTRUCT* fss/* = NULL*/)
	{
		FASSERT(NULL != m_stringStylePrivate);
		return __super::drawText(m_stringStylePrivate->m_resString.getString(), 
								pGraphics, prcUpdate, dwState, rcDest, pszType, pszPart, fss);
	}

	FTEXTSTYLE_INLINE bool FStringStyle::setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		FASSERT(NULL != m_stringStylePrivate);
		FASSERT(NULL != pMarkup);
		FASSERT(NULL != pszName);
		FASSERT(NULL != pszValue);

		if (_tcscmp(pszName, _T("")) == 0)
		{
			m_stringStylePrivate->m_resString.setString(pszValue);
		}
		else if (_tcscmp(pszName, _T("text")) == 0 
			|| _tcscmp(pszName, _T("string")) == 0)
		{
			m_stringStylePrivate->m_resString.parseAttribute(pMarkup, pszValue);
		}

		return __super::setAttribute(pMarkup, pszName, pszValue);
	}

}} // !namespace