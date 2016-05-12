/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FApplication.cpp
* 描述：		FApplication
* 创建日期：	2013-06-06
*
*/

#include "StdAfx.h"
#include "core/FApplication.h"

#include "F3rd_p.h"

#define FAPPLICATION_GM_ERROR (-1)

#define FAPPLICATION_INLINE FOOINLINE

FooUI::FModule _fmodule;

namespace FooUI { namespace Core {

	//////////////////////////////////////////////////////////////////////////
	// FApplication
	static FApplication* s_application = NULL;
	static HINSTANCE s_instance_handle = NULL;
	static Core::FString s_application_directory = NULL;

	FAPPLICATION_INLINE FApplication::FApplication(void)
	{
		FASSERT(NULL == s_application);

		s_application = this;
	}

	FAPPLICATION_INLINE FApplication::~FApplication(void)
	{
		FASSERT(this == s_application);

		s_application = NULL;
	}

	FAPPLICATION_INLINE int FApplication::exce(void)
	{
		FASSERT(this == s_application);

		bool bDoIdle = true;
		int nIdleCount = 0;

		MSG msg;
		BOOL bRet;

		for (;;)
		{
			while (bDoIdle && !::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				if (!s_application->idle(nIdleCount++))
					bDoIdle = false;
			}

			bRet = ::GetMessage(&msg, NULL, 0, 0);
			if (FAPPLICATION_GM_ERROR == bRet)
			{
				continue;
			}
			else if (!bRet)
			{
				break;
			}

			if (!translateMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}

			if (isIdleMessage(&msg))
			{
				bDoIdle = true;
				nIdleCount = 0;
			}
		}

		return (int)msg.wParam;
	}

	FAPPLICATION_INLINE /*static*/ void FApplication::exit(int nExitCode)
	{
		::PostQuitMessage(nExitCode);
	}

	FAPPLICATION_INLINE /*static*/ void FApplication::doEvents(void)
	{
		MSG msg;
		while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if (NULL != s_application)
			{
				if (s_application->translateMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam))
					continue;
			}

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		} 
	}

	FAPPLICATION_INLINE /*static*/ bool FApplication::isIdleMessage(MSG* pMsg)
	{
		switch(pMsg->message)
		{
		case WM_MOUSEMOVE:
#ifndef _WIN32_WCE
		case WM_NCMOUSEMOVE:
#endif // !_WIN32_WCE
		case WM_PAINT:
		case 0x0118:	// WM_SYSTIMER (caret blink)
			return false;
			break;

		default:
			break;
		}

		return true;
	}

	FAPPLICATION_INLINE /*static*/ int FApplication::getWindowCount(void)
	{
		return FModule::window_create_helper()->getCount();
	}

	FAPPLICATION_INLINE bool FApplication::translateMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		FASSERT(NULL != FModule::msgfilterlist());
		for (int i = FModule::msgfilterlist()->size() - 1; i >= 0; --i)
		{
			FMessageFilter *pMsgFilter = reinterpret_cast<FMessageFilter*>(FModule::msgfilterlist()->get(i));
			FASSERT(NULL != pMsgFilter);
			if (pMsgFilter->translateMessage(hWnd, uMsg, wParam, lParam))
			{
				return true;
			}
		}

		return false;
	}

	FAPPLICATION_INLINE /*static*/ HINSTANCE FApplication::getInstance(void)
	{
		if (NULL == s_instance_handle)
		{
			s_instance_handle = ::GetModuleHandle(NULL);
		}
		return s_instance_handle;
	}

	FAPPLICATION_INLINE FApplication::operator HINSTANCE()
	{
		return FApplication::getInstance();
	}

	FAPPLICATION_INLINE LPCTSTR FApplication::getApplicationDirectory(void)
	{
		/*** 获取当前根目录 */
		if (s_application_directory.isEmpty()) 
		{
			::GetModuleFileName(FApplication::getInstance(), s_application_directory.getBuffer(MAX_PATH + 1), MAX_PATH);
			s_application_directory.releaseBuffer();

			int nPos = s_application_directory.reverseFind(_T('\\'));
			s_application_directory = (LPCTSTR)s_application_directory.left(nPos + 1);
		}

		return s_application_directory;
	}

	FAPPLICATION_INLINE /*static*/ FApplication* FApplication::getApplication(void)
	{
		return s_application;
	}

	FAPPLICATION_INLINE /*static*/ FApplication::eFRenderEngineType FApplication::getRenderEngineType(void)
	{
#ifdef FOOUI_SUPPORT_GDI
		return FApplication::gdi;
#endif

#ifdef FOOUI_SUPPORT_GDIP
		return FApplication::gdip;
#endif

#ifdef FOOUI_SUPPORT_SKIA
		return FApplication::skia;
#endif

#ifdef FOOUI_SUPPORT_AGG
		return FApplication::agg;
#endif

#ifdef FOOUI_SUPPORT_CAIRO
		return FApplication::cairo;
#endif


		return FApplication::none;
	}

	FAPPLICATION_INLINE /*static*/ bool FApplication::addMessageFilter(FMessageFilter* msgFilter)
	{
		FASSERT(NULL != FModule::msgfilterlist());

		return FModule::msgfilterlist()->add(msgFilter);
	}

	FAPPLICATION_INLINE /*static*/ bool FApplication::removeMessageFilter(FMessageFilter* msgFilter)
	{
		FASSERT(NULL != FModule::msgfilterlist());

		Core::FPtrList::POSITION position = FModule::msgfilterlist()->find(msgFilter);
		while (NULL != position)
		{
			FModule::msgfilterlist()->removeAtPosition(position);
			position = FModule::msgfilterlist()->find(msgFilter);
		}

		return true;
	}

}} // !namespace