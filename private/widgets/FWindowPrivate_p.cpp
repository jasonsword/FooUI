/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FWindowPrivate_p.h
* 描述：		FWindowPrivate_p
* 创建日期：	2013-06-06
*
*/

#include "StdAfx.h"

#include "core/FApplication.h"

#include "widgets/FWidgetPrivate_p.h"
#include "widgets/FWindowPrivate_p.h"

#ifdef _X86_
#include "widgets/thunk/x86/atlthunk.cpp"
#endif

#ifdef _AMD64_
#include "widgets/thunk/amd64/atlthunk.cpp"
#endif

#ifndef FENSURE_RETURN_VAL
#define FENSURE_RETURN_VAL(expr, val)        \
	do {                                           \
	int __atl_condVal=!!(expr);                \
	FASSERT(__atl_condVal);                  \
	if(!(__atl_condVal)) return val;           \
	} while (0) 
#endif // FENSURE_RETURN_VAL

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FWindowPrivate
	FWindowPrivate::FWindowPrivate(FWidgetPrivate* widgetdata) : 
		m_pWidgetPrivate(widgetdata) 
		, m_bTrackedMouse(false) 
		, m_hWnd(NULL)
		, m_pfnSuperWindowProc(::DefWindowProc) 
	{
		FASSERT(NULL != m_pWidgetPrivate);
	}

	FWindowPrivate::~FWindowPrivate(void)
	{
		FASSERT(NULL == m_hWnd && "析构前需先销毁窗口句柄！");
	}

	bool FWindowPrivate::create(const FWNDCREATEINFO& wci)
	{
		FASSERT(NULL == m_hWnd);

		static Core::FCriticalSectionLock window_create_lock;
		Core::FAutoLock autoLock(&window_create_lock);

		FModule::window_create_helper()->push(this);

		WNDCLASSEX wc;
		memset(&wc, 0, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);

		if (::GetClassInfoEx(NULL, wci.lpszClassName, &wc))
		{
			HWND hWnd = ::CreateWindowEx(wci.wndinfo.dwExStyle, 
										wc.lpszClassName, 
										wci.wndinfo.pszWindowText, 
										wci.wndinfo.dwStyle, 
										wci.wndinfo.x, 
										wci.wndinfo.y, 
										wci.wndinfo.cx, 
										wci.wndinfo.cy, 
										wci.wndinfo.hwndParent, 
										wci.wndinfo.hMenu, 
										Core::FApplication::getInstance(), 
										wci.wndinfo.lpCreateParams);

			FASSERT(NULL != hWnd);
			m_hWnd = hWnd;

			if (NULL != m_hWnd)
			{
				m_thunk.Init((DWORD_PTR)getWindowProc(), this);
				WNDPROC pWndProc = (WNDPROC)m_thunk.GetCodeAddress();
				WNDPROC pOldProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)pWndProc);
				m_pfnSuperWindowProc = pOldProc;
			}

			return (NULL != m_hWnd);
		}
		else
		{
			wc.style = wci.classinfo.style;
			wc.lpfnWndProc = FWindowPrivate::__startWindowProc;
			wc.hInstance = Core::FApplication::getInstance();
			wc.hIcon = wci.classinfo.hIcon;
			wc.hCursor = wci.classinfo.hCursor;
			wc.hbrBackground = wci.classinfo.hbrBackground;
			if (NULL != wci.lpszClassName && _tcslen(wci.lpszClassName) != 0) 
			{
				wc.lpszClassName = wci.lpszClassName;
			}
			else 
			{
				wc.lpszClassName = FWindowPrivate_CLASSNAME;
			}

			if (!FModule::window_create_helper()->registerWndClass(wc))
			{
				FASSERT(false);
				return false;
			}

			HWND hWnd = ::CreateWindowEx(wci.wndinfo.dwExStyle, 
										wc.lpszClassName, 
										wci.wndinfo.pszWindowText, 
										wci.wndinfo.dwStyle, 
										wci.wndinfo.x, 
										wci.wndinfo.y, 
										wci.wndinfo.cx, 
										wci.wndinfo.cy, 
										wci.wndinfo.hwndParent, 
										wci.wndinfo.hMenu, 
										Core::FApplication::getInstance(), 
										wci.wndinfo.lpCreateParams);
			 
			FASSERT(hWnd == m_hWnd && NULL != m_hWnd);

			return (NULL != m_hWnd);
		}
		
		return false;
	}

	bool FWindowPrivate::centerWindow(HWND hWndCenter)
	{
		if (!::IsWindow(m_hWnd))
		{
			FASSERT(false);
			return false;
		}

		// determine owner window to center against
		DWORD dwStyle = ::GetWindowLong(getHwnd(), GWL_STYLE);
		if (NULL == hWndCenter)
		{
			if (dwStyle & WS_CHILD)
			{
				hWndCenter = ::GetParent(getHwnd());
			}
			else
			{
				hWndCenter = ::GetWindow(getHwnd(), GW_OWNER);
			}
		}

		// get coordinates of the window relative to its parent
		RECT rcWindow;
		::GetWindowRect(getHwnd(), &rcWindow);

		RECT rcArea;
		RECT rcCenter;

		HWND hWndParent;
		if (!(dwStyle & WS_CHILD))
		{
			// don't center against invisible or minimized windows
			if(hWndCenter != NULL)
			{
				DWORD dwStyleCenter = ::GetWindowLong(hWndCenter, GWL_STYLE);
				if(!(dwStyleCenter & WS_VISIBLE) || (dwStyleCenter & WS_MINIMIZE))
					hWndCenter = NULL;
			}

			// center within screen coordinates
#if WINVER < 0x0500
			::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);
#else
			HMONITOR hMonitor = NULL;
			if(hWndCenter != NULL)
			{
				hMonitor = ::MonitorFromWindow(hWndCenter, MONITOR_DEFAULTTONEAREST);
			}
			else
			{
				hMonitor = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
			}
			FENSURE_RETURN_VAL(hMonitor != NULL, FALSE);

			MONITORINFO minfo;
			minfo.cbSize = sizeof(MONITORINFO);
			BOOL bResult = ::GetMonitorInfo(hMonitor, &minfo);
			FENSURE_RETURN_VAL(bResult, FALSE);

			rcArea = minfo.rcWork;
#endif
			if(hWndCenter == NULL)
				rcCenter = rcArea;
			else
				::GetWindowRect(hWndCenter, &rcCenter);
		}
		else
		{
			// center within parent client coordinates
			hWndParent = ::GetParent(m_hWnd);
			FASSERT(::IsWindow(hWndParent));

			::GetClientRect(hWndParent, &rcArea);
			FASSERT(::IsWindow(hWndCenter));
			::GetClientRect(hWndCenter, &rcCenter);
			::MapWindowPoints(hWndCenter, hWndParent, (POINT*)&rcCenter, 2);
		}

		int DlgWidth = rcWindow.right - rcWindow.left;
		int DlgHeight = rcWindow.bottom - rcWindow.top;

		// find dialog's upper left based on rcCenter
		int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
		int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

		// if the dialog is outside the screen, move it inside
		if(xLeft + DlgWidth > rcArea.right)
			xLeft = rcArea.right - DlgWidth;
		if(xLeft < rcArea.left)
			xLeft = rcArea.left;

		if(yTop + DlgHeight > rcArea.bottom)
			yTop = rcArea.bottom - DlgHeight;
		if(yTop < rcArea.top)
			yTop = rcArea.top;

		// map screen coordinates to child coordinates
		if (::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1,
			SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE))
		{
			return true;
		}

		return false;
	}

	DWORD FWindowPrivate::getStyle(void) const
	{
		FASSERT(NULL != m_hWnd);
		return ::GetWindowLong(m_hWnd, GWL_STYLE);
	}

	DWORD FWindowPrivate::getStyleEx(void) const
	{
		FASSERT(NULL != m_hWnd);
		return ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	}

	bool FWindowPrivate::trackMouseEvent(DWORD dwFlags, DWORD dwHoverTime)
	{
		if (m_bTrackedMouse) return false;
		m_bTrackedMouse = true;

		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = dwFlags;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = dwHoverTime;

		return (::TrackMouseEvent(&tme) == TRUE);
	}

	void FWindowPrivate::endTrackMouseEvent(void)
	{
		m_bTrackedMouse = false;
	}

	LRESULT FWindowPrivate::defWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		/*FASSERT(NULL != m_hWnd);
		FASSERT(NULL != m_pfnSuperWindowProc);*/

		if (NULL == m_hWnd 
			|| NULL == m_pfnSuperWindowProc)
		{
			return 0;
		}
		else 
		{
			return ::CallWindowProc(m_pfnSuperWindowProc, m_hWnd, uMsg, wParam, lParam);
		}
	}

	/*static*/ LRESULT CALLBACK FWindowPrivate::__startWindowProc(
					HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		FWindowPrivate *pThis = FModule::window_create_helper()->getLast();
		if (NULL == pThis)
		{
			FASSERT(false);
			return -1;
		}

		FASSERT(NULL == pThis->m_hWnd);
		pThis->m_hWnd = hWnd;

		pThis->m_thunk.Init((DWORD_PTR)pThis->getWindowProc(), pThis);
		WNDPROC pWndProc = (WNDPROC)pThis->m_thunk.GetCodeAddress();
		WNDPROC	pOldProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)pWndProc);
		FASSERT(pOldProc == FWindowPrivate::__startWindowProc);

		return pWndProc(hWnd, uMsg, wParam, lParam);
	}

	/*static*/ LRESULT CALLBACK FWindowPrivate::__windowProc(
					HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{

		FWindowPrivate *pThis = (FWindowPrivate*)hWnd;
		FASSERT(NULL != pThis);

		FASSERT(NULL != pThis->m_pWidgetPrivate);
		FASSERT(NULL != pThis->m_pWidgetPrivate->m_pWidget);

		LRESULT lResult = pThis->m_pWidgetPrivate->m_pWidget->processWindowMessage(uMsg, wParam, lParam);

		if (WM_NCCREATE == uMsg)
		{
			HWND hWndThis = pThis->m_hWnd;
			pThis->m_pWidgetPrivate->createMessage(hWndThis);
		}
		else if (WM_NCDESTROY == uMsg)
		{
			HWND hWndThis = pThis->m_hWnd;
			pThis->m_hWnd = NULL;

			if (NULL != pThis->m_pfnSuperWindowProc 
				&& pThis->m_pfnSuperWindowProc != ::DefWindowProc)
			{
				::SetWindowLongPtr(hWndThis, GWLP_WNDPROC, (LONG_PTR)pThis->m_pfnSuperWindowProc);
				pThis->m_pfnSuperWindowProc = NULL;
			}

			if (NULL != FModule::window_create_helper())
			{
				FModule::window_create_helper()->remove(pThis);
			}

			pThis->m_pWidgetPrivate->finalMessage(hWndThis);
		}

		return lResult;
	}

	//////////////////////////////////////////////////////////////////////////
	// FWindowCreateHelper
	FWindowCreateHelper::~FWindowCreateHelper(void)
	{
		clearWndClass();
	}

	bool FWindowCreateHelper::push(FWindowPrivate* pWindow)
	{
		Core::FAutoLock lock(&m_lock);

		m_listWindow.push_back(pWindow);

		return true;
	}

	bool FWindowCreateHelper::remove(FWindowPrivate* pWindow)
	{
		Core::FAutoLock lock(&m_lock);

		m_listWindow.remove(pWindow);

		return true;
	}

	bool FWindowCreateHelper::clear(void)
	{
		Core::FAutoLock lock(&m_lock);

		m_listWindow.clear();

		return true;
	}

	bool FWindowCreateHelper::isEmpty(void) const
	{
		Core::FAutoLock lock(const_cast<Core::FCriticalSectionLock*>(&m_lock));

		return m_listWindow.empty();
	}

	int FWindowCreateHelper::getCount(void) const
	{
		return (int)m_listWindow.size();
	}

	FWindowPrivate* FWindowCreateHelper::getLast(void) const
	{
		Core::FAutoLock lock(const_cast<Core::FCriticalSectionLock*>(&m_lock));
 
		if (isEmpty())
		{
			return NULL;
		}

		return *m_listWindow.rbegin();
	}

	bool FWindowCreateHelper::registerWndClass(const WNDCLASSEX& wc)
	{
		Core::FAutoLock lock(&m_lock);

		WNDCLASSEX wcCheck;
		memset(&wcCheck, 0, sizeof(WNDCLASSEX));
		wcCheck.cbSize = sizeof(WNDCLASSEX);

		if (::GetClassInfoEx(Core::FApplication::getInstance(), wc.lpszClassName, &wcCheck))
		{
			return true;
		}
		else
		{
			ATOM atom = ::RegisterClassEx(&wc);
			if (0 != atom)
			{
				m_listAtom.push_back(atom);
				return true;
			}
		}

		return false;
	}

	bool FWindowCreateHelper::clearWndClass(void)
	{
		Core::FAutoLock lock(&m_lock);

		for (std::list<ATOM>::iterator it = m_listAtom.begin(); it != m_listAtom.end(); it++)
		{
			::UnregisterClass(MAKEINTATOM(*it), Core::FApplication::getInstance());
		}

		m_listAtom.clear();

		return true;
	}

}} // ! namespace