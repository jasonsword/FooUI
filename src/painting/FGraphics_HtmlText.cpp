/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FGraphics_HtmlText.cpp
* 描述：		FGraphics
* 创建日期：	2013-06-06
*
*/

#include "StdAfx.h"

#include "painting/FGraphics.h"

#include "widgets/FWidget.h"

#include "F3rd_p.h"
#include <assert.h>

#define FGRAPHICS_HTMLTEXT_INLINE FOOINLINE

namespace FooUI { namespace Painting {

	//////////////////////////////////////////////////////////////////////////
	// FGraphics
	FGRAPHICS_HTMLTEXT_INLINE bool FGraphics::drawHtmlText(LPCTSTR pString, int nCount, Core::FRect& rect, UINT uFormat, const FPaint* paint)
	{
		if (NULL == m_graphicsPrivate 
			|| NULL == pString)
		{
			FASSERT(FALSE);
			return FALSE;
		}

		if (nCount == -1) nCount = _tcslen(pString);
		if (nCount <= 0) return false;

		HDC hDC = getSafeHdc();
		int nDCState = ::SaveDC(hDC);

		assert(::GetObjectType(hDC)==OBJ_DC || ::GetObjectType(hDC)==OBJ_MEMDC);
	  

		bool bDraw = (uFormat & DT_CALCRECT) == 0;
		 
		return false;
	}

}} // !namespace