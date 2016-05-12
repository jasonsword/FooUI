/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FColorStyle.cpp
* 描述：		FColorStyle
* 创建日期：	2013-06-27
*
*/

#include "StdAfx.h"
#include "core/FObject.h"
#include "markup/FColorResource.h"
#include "styles/FColorStyle.h"

#define FCOLORSTYLE_INLINE FOOINLINE

namespace FooUI { namespace Styles {

	//////////////////////////////////////////////////////////////////////////
	// FColorStylePrivate
	class FColorStyle::FColorStylePrivate : public Core::FReferenceObject, public Core::FNoncopyableObject
	{
	public:
		Markup::FColorResource m_resColor;
		Painting::FPaint m_paint;
	};

	//////////////////////////////////////////////////////////////////////////
	// FColorStyle
	FCOLORSTYLE_INLINE FColorStyle::FColorStyle(void)
	{
		m_colorResPrivate = new FColorStylePrivate;
		m_colorResPrivate->addRef();
	}

	FCOLORSTYLE_INLINE FColorStyle::FColorStyle(const FColorStyle& style)
	{
		FASSERT(this != &style);
		m_colorResPrivate = style.m_colorResPrivate;
		m_colorResPrivate->addRef();
	}

	FCOLORSTYLE_INLINE FColorStyle& FColorStyle::operator=(const FColorStyle& style)
	{
		if (m_colorResPrivate != style.m_colorResPrivate)
		{
			m_colorResPrivate->release();
			m_colorResPrivate = style.m_colorResPrivate;
			m_colorResPrivate->addRef();
		}

		return (*this);
	}

	FCOLORSTYLE_INLINE FColorStyle::~FColorStyle(void)
	{
		FASSERT(NULL != m_colorResPrivate);
		m_colorResPrivate->release();
		m_colorResPrivate = NULL;
	}

	FCOLORSTYLE_INLINE bool FColorStyle::draw(Widgets::FWidget* pWidget, /** 控件对象 */
											Painting::FGraphics* pGraphics, /** 画布 */
											const Core::FRect* prcUpdate, /** 更新区域 */
											DWORD dwState, /** 状态 */
											const Core::FRect& rcDest, /** 绘图区域 */
											LPCTSTR pszType/* = NULL*/, 
											LPCTSTR pszPart/* = NULL*/, 
											FSTYLESTRUCT* fss/* = NULL*/)
	{
		FASSERT(NULL != m_colorResPrivate);

		if (!fooTestAttribute(dwState, getState())) return false;
		if (NULL != pszType && _tcscmp(pszType, getType()) != 0) return false;
		if (NULL != pszPart && _tcscmp(pszPart, getPart()) != 0) return false;

		Markup::FColorResource& resColor = m_colorResPrivate->m_resColor;
		/*if (resColor.getColor().alpha() == 0)
		{
		return false;
		}
		*/

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

		pGraphics->setBackColor(resColor.getColor());

		Painting::FAutoClipRect autoclip(pGraphics, rcClip);
		return pGraphics->drawRect(rcDraw, &m_colorResPrivate->m_paint);
	}

	FCOLORSTYLE_INLINE bool FColorStyle::setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		FASSERT(NULL != m_colorResPrivate);
		FASSERT(NULL != pMarkup);
		FASSERT(NULL != pszName);
		FASSERT(NULL != pszValue);

		if (_tcscmp(pszName, _T("color")) == 0)
		{
			m_colorResPrivate->m_resColor.parseAttribute(pMarkup, pszValue);
		}
		else if (_tcscmp(pszName, _T("rgb")) == 0)
		{
			m_colorResPrivate->m_resColor.parseRGB(pszValue);
		}
		else if (_tcscmp(pszName, _T("argb")) == 0)
		{
			m_colorResPrivate->m_resColor.parseARGB(pszValue);
		}
		else if (_tcscmp(pszName, _T("alpha")) == 0)
		{
			m_colorResPrivate->m_paint.setAlpha((unsigned char)min(255, _tcstoul(pszValue, NULL, 10)));
		}

		return __super::setAttribute(pMarkup, pszName, pszValue);
	}

}} // !namespace