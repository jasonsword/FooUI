/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FFocusRectStyle.cpp
* ������		FFocusRectStyle
* �������ڣ�	2013-06-27
*
*/

#include "StdAfx.h"
#include "core/FObject.h"
#include "styles/FFocusRectStyle.h"

#define FFOCUSRECTSTYLE_INLINE FOOINLINE

namespace FooUI { namespace Styles {

	//////////////////////////////////////////////////////////////////////////
	// FFocusRectStyle
	FFOCUSRECTSTYLE_INLINE bool FFocusRectStyle::draw(Widgets::FWidget* pWidget, /** �ؼ����� */
													Painting::FGraphics* pGraphics, /** ���� */
													const Core::FRect* prcUpdate, /** �������� */
													DWORD dwState, /** ״̬ */
													const Core::FRect& rcDest, /** ��ͼ���� */
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

		/** ������������ */
		if (NULL != prcUpdate 
			&& !prcUpdate->isIntersects(rcDraw))
		{
			return false;
		}

		Painting::FAutoClipRect autoclip(pGraphics, rcClip);
		return pGraphics->drawFocusRect(rcDraw);
	}

}} // !namespace