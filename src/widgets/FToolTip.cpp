/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FToolTip.h
* 描述：		FToolTip
* 创建日期：	2013-08-04
*
*/

#include "StdAfx.h"
#include "widgets/FToolTip.h"
#include "core/FApplication.h"

#include <CommCtrl.h>

#define FTOOLTIP_INLINE FOOINLINE

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FToolTip

	//////////////////////////////////////////////////////////////////////////
	// FCommToolTip
	FTOOLTIP_INLINE bool FCommToolTip::showToolTip(LPCTSTR pszToolTip, FWidget* widget/* = NULL*/)
	{
		if (!isWindow())
		{
			FWidget::createWindow(0, TOOLTIPS_CLASS, _T(""), WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, 
				CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
				NULL, NULL, NULL);
		}

		/** 创建窗口失败？ */
		if (!isWindow())
		{
			FASSERT(FALSE);
			return false;
		}

		HWND hOwnerWnd = NULL;
		if (NULL != widget) hOwnerWnd = widget->getSafeHwnd();
		if (NULL == hOwnerWnd) hOwnerWnd = ::GetActiveWindow();

		TOOLINFO toolInfo = {0};
		toolInfo.cbSize = sizeof(TOOLINFO);
		toolInfo.hwnd = hOwnerWnd;
		toolInfo.uId = (UINT_PTR)hOwnerWnd;
		toolInfo.uFlags = TTF_IDISHWND;

		if (NULL == widget)
		{
			::GetClientRect(hOwnerWnd, &toolInfo.rect);
		}
		else
		{
			toolInfo.rect = widget->getLayoutRect();
		}

		toolInfo.hinst = Core::FApplication::getInstance();
		toolInfo.lpszText = (LPTSTR)(NULL == pszToolTip ? _T("") : pszToolTip);

		::SendMessage(getHwnd(), TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
		::SendMessage(getHwnd(), TTM_SETTOOLINFO, 0, (LPARAM)&toolInfo);
		::SendMessage(getHwnd(), TTM_TRACKACTIVATE, TRUE, (LPARAM)&toolInfo);
		return true;
	}

	FTOOLTIP_INLINE bool FCommToolTip::hideToolTip(void)
	{
		if (isWindow())
		{
			::SendMessage(getHwnd(), TTM_TRACKACTIVATE, FALSE, 0L);
			return true;
		}

		return false;
	}

	FTOOLTIP_INLINE LRESULT FCommToolTip::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
	{
		switch (uMsg)
		{
		case WM_ERASEBKGND:
		case WM_PAINT:
			if (!bHandled)
			{
				bHandled = true;
				return defWindowProc(uMsg, wParam, lParam);
			}
			break;

		default:
			break;
		}

		return 0;
	}

}} // !namespace