/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FWindowPrivate_p.h
* ������		FWindowPrivate_p
* �������ڣ�	2013-06-06
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
		/** Ĭ�ϵ���Ϣ������� */
		LRESULT defWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		/** �ʼ����Ϣ�ص����� */
		static LRESULT CALLBACK __startWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		/** ��Ϣ���̻ص����� */
		static LRESULT CALLBACK __windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		/** ���ع��̻������ */
		virtual WNDPROC getWindowProc(void) { return FWindowPrivate::__windowProc; }

	private:
		/** ���ھ�� */
		HWND m_hWnd;
		ATL::CStdCallThunk m_thunk;
		WNDPROC m_pfnSuperWindowProc;
		/** ������FWidgetPrivate���� */
		FWidgetPrivate* const m_pWidgetPrivate;
		/** trackMouseEvent��ǣ����Ϊtrue���� */
		bool m_bTrackedMouse;

	protected:
		Painting::FGraphics m_graphics;

	private:
		//////////////////////////////////////////////////////////////////////////
		// FWindowPrivateCreateCriticalSectionLock
		typedef Core::FCriticalSectionLock FWindowPrivateCreateCriticalSectionLock;
	};

	//////////////////////////////////////////////////////////////////////////
	// FWindowCreateHelper : ��Thunk��Ϣ������ʹ�ã��ڹ���������ʹ��
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
			
		/** ��ȡ���һ�������FWindowPrivateָ�� */
		FWindowPrivate* getLast(void) const;

	public:
		/** 
			ע�ᴰ���ࣺ
			ʧ�ܷ���false���ɹ�����true
			* �����ϵͳ�����࣬����false
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