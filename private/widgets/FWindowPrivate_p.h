/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FWindowPrivate_p.h
* 描述：		FWindowPrivate_p
* 创建日期：	2013-06-06
*
*/

#ifndef FWindowPrivate_P_H
#define FWindowPrivate_P_H

#include "core/FBase.h"
#include "core/FLock.h"

#include "painting/FGraphics.h"
#include "../../private/widgets/thunk/atlstdthunk.h"
#include "widgets/FWidgetDef.h"
#include "../../private/widgets/FWidgetPrivate_p.h"
#include <list>

namespace FooUI { namespace Widgets {
	
	//////////////////////////////////////////////////////////////////////////
	// FWindowPrivate
	#define FWindowPrivate_CLASSNAME _T("FGuiFoundation")
	class FWindowPrivate
	{
		friend FWidgetPrivate;
		friend FWidget;
	public:
		explicit FWindowPrivate(FWidgetPrivate* widgetdata);
		~FWindowPrivate(void);

	public:
		bool create(const FWNDCREATEINFO& ci);
		bool centerWindow(HWND hWndCenter = NULL);

		HWND getHwnd(void) const { return m_hWnd; }

		DWORD getStyle(void) const;
		DWORD getStyleEx(void) const;

	public:
		bool trackMouseEvent(DWORD dwFlags, DWORD dwHoverTime);
		void endTrackMouseEvent(void);

	public:
		/** 默认的消息处理过程 */
		LRESULT defWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		/** 最开始的消息回调函数 */
		static LRESULT CALLBACK __startWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		/** 消息过程回调函数 */
		static LRESULT CALLBACK __windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		/** 返回过程会调函数 */
		virtual WNDPROC getWindowProc(void) { return FWindowPrivate::__windowProc; }

	private:
		/** 窗口句柄 */
		HWND m_hWnd;
		ATL::CStdCallThunk m_thunk;
		WNDPROC m_pfnSuperWindowProc;
		/** 关联的FWidgetPrivate对象 */
		FWidgetPrivate* const m_pWidgetPrivate;
		/** trackMouseEvent标记，如果为true忽略 */
		bool m_bTrackedMouse;

	protected:
		Painting::FGraphics m_graphics;

	private:
		//////////////////////////////////////////////////////////////////////////
		// FWindowPrivateCreateCriticalSectionLock
		typedef Core::FCriticalSectionLock FWindowPrivateCreateCriticalSectionLock;
	};

	//////////////////////////////////////////////////////////////////////////
	// FWindowCreateHelper : 在Thunk消息过程中使用，在管理窗口类中使用
	class FWindowCreateHelper
	{
	public:
		~FWindowCreateHelper(void);

	public:
		bool push(FWindowPrivate* pWindow);
		bool remove(FWindowPrivate* pWindow);
		bool clear(void);
		bool isEmpty(void) const;
		int getCount(void) const;
			
		/** 获取最后一个插入的FWindowPrivate指针 */
		FWindowPrivate* getLast(void) const;

	public:
		/** 
			注册窗口类：
			失败返回false，成功返回true
			* 如果是系统窗口类，返回false
		*/
		bool registerWndClass(const WNDCLASSEX& wc);
		bool clearWndClass(void);

	public:

	private:
		std::list<FWindowPrivate*> m_listWindow;
		std::list<ATOM> m_listAtom;
		Core::FCriticalSectionLock m_lock;
	};

}} // !namespace

#endif // !FWindowPrivate_P_H