/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FFocusRectStyle.cpp
* 描述：		FFocusRectStyle
* 创建日期：	2013-06-27
*
*/

#include "StdAfx.h"
#include "core/FObject.h"
#include "styles/FFocusRectStyle.h"

#define FFOCUSRECTSTYLE_INLINE FOOINLINE

namespace FooUI { namespace Styles {

	//////////////////////////////////////////////////////////////////////////
	// FFocusRectStyle
	FFOCUSRECTSTYLE_INLINE bool FFocusRectStyle::draw(Widgets::FWidget* pWidget, /** 控件对象 */
													Painting::FGraphics* pGraphics, /** 画布 */
													const Core::FRect* prcUpdate, /** 更新区域 */
													DWORD dwState, /** 状态 */
													const Core::FRect& rcDest, /** 绘图区域 */
													LPCTSTR pszType/* = NULL*/, 
													LPCTSTR pszPart/* = NULL*/, 
													FSTYLESTRUCT* fss/* = NULL*/)
	{
		FASSERT(NULL != pGraphics);

		if (!fooTestAttribute(dwState, getState())) return false;
		if (NULL != pszType && _tcscmp(pszType, getType()) != 0) return false;
		if (NULL != pszPart && _tcscmp(pszPart, getPart()) != 0) return false;

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

		Painting::FAutoClipRect autoclip(pGraphics, rcClip);
		return pGraphics->drawFocusRect(rcDraw);
	}

}} // !namespace