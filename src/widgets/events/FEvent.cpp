/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FEvent.cpp
* 描述：		FEvent
* 创建日期：	2013-06-08
*
*/

#include "StdAfx.h"

#include "widgets/events/FEvent.h"
#include "painting/FGraphics.h"

#define FEVENT_INLINE FOOINLINE

namespace FooUI { namespace Widgets { namespace Event {

	//////////////////////////////////////////////////////////////////////////
	// FEvent
	FEVENT_INLINE FEvent::FEvent(Type type) : 
		m_accept(false) 
		, m_type(type) 
	{
	}

	FEVENT_INLINE FEvent::operator FEvent*()
	{
		return this;
	}

	FEVENT_INLINE FEvent::operator const FEvent*() const
	{
		return this;
	}

	//////////////////////////////////////////////////////////////////////////
	// FPaintEvent
	FEVENT_INLINE FPaintEvent::FPaintEvent(Painting::FGraphics* pGraphics, const Core::FRect& rcUpdate) : 
		FEvent(FEvent::Paint) 
		, m_pGraphics(pGraphics) 
		, m_rcUpdate(rcUpdate) 
	{
	}
	FEVENT_INLINE Painting::FGraphics* FPaintEvent::graphics(void) const { return m_pGraphics; }
	FEVENT_INLINE const Core::FRect& FPaintEvent::updateRect(void) const { return m_rcUpdate; }

	//////////////////////////////////////////////////////////////////////////
	// FFocusedEvent(bool bFocused);
	FEVENT_INLINE FFocusedEvent::FFocusedEvent(bool bFocused) : 
		FEvent(bFocused ? FEvent::FocusIn : FEvent::FocusOut)
		, focused(bFocused)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	// FChangedEvent
	FChangedEvent::FChangedEvent(UINT changedType) : 
		FEvent(FEvent::Changed)
	{
		this->changedType = changedType;
	}

	//////////////////////////////////////////////////////////////////////////
	// FLayoutChangedEvent
	FEVENT_INLINE FLayoutChangedEvent::FLayoutChangedEvent(const RECT& rcOld, const RECT& rcNew) : 
		FEvent(FEvent::LayoutChanged)
		, oldLayoutRect(rcOld) 
		, newLayoutRect(rcNew) 
	{
	}

	//////////////////////////////////////////////////////////////////////////
	// FMouseEvent
	FEVENT_INLINE FMouseEvent::FMouseEvent(FEvent::Type type, UINT button, 
									POINT mouse, POINT cursor) : 
		FEvent(type) 
	{
		modifiers = 0;
		if (::GetKeyState(VK_LWIN) < 0 || ::GetKeyState(VK_RWIN) < 0) modifiers |= MOD_WIN;
		if (::GetKeyState(VK_CONTROL) < 0) modifiers |= MOD_CONTROL;
		if (::GetKeyState(VK_SHIFT) < 0) modifiers |= MOD_SHIFT;
		if (::GetKeyState(VK_MENU) < 0) modifiers |= MOD_ALT;

		this->button = button;
		this->mouse = mouse;
		this->cursor = cursor;
	}

	//////////////////////////////////////////////////////////////////////////
	// FWheelEvent
	FEVENT_INLINE FWheelEvent::FWheelEvent(POINT point, UINT button, int delta) : 
		FEvent(FEvent::MouseWheel) 
	{
		modifiers = 0;
		if (::GetKeyState(VK_LWIN) < 0 || ::GetKeyState(VK_RWIN) < 0) modifiers |= MOD_WIN;
		if (::GetKeyState(VK_CONTROL) < 0) modifiers |= MOD_CONTROL;
		if (::GetKeyState(VK_SHIFT) < 0) modifiers |= MOD_SHIFT;
		if (::GetKeyState(VK_MENU) < 0) modifiers |= MOD_ALT;

		this->point = point;
		this->button = button;
		this->delta = delta;
	}

	//////////////////////////////////////////////////////////////////////////
	// FKeyEvent
	FEVENT_INLINE FKeyEvent::FKeyEvent(FEvent::Type type, UINT key, UINT repeat, UINT flags) 
		: FEvent(type)
	{
		modifiers = 0;
		if (::GetKeyState(VK_LWIN) < 0 || ::GetKeyState(VK_RWIN) < 0) modifiers |= MOD_WIN;
		if (::GetKeyState(VK_CONTROL) < 0) modifiers |= MOD_CONTROL;
		if (::GetKeyState(VK_SHIFT) < 0) modifiers |= MOD_SHIFT;
		if (::GetKeyState(VK_MENU) < 0) modifiers |= MOD_ALT;

		this->key = key;
		this->repeat = repeat;
		this->flags = flags;
	}

	FEVENT_INLINE FKeyEvent::FKeyEvent(FEvent::Type type, WPARAM wParam, LPARAM lParam) 
		: FEvent(type)
	{
		modifiers = 0;
		if (::GetKeyState(VK_LWIN) < 0 || ::GetKeyState(VK_RWIN) < 0) modifiers |= MOD_WIN;
		if (::GetKeyState(VK_CONTROL) < 0) modifiers |= MOD_CONTROL;
		if (::GetKeyState(VK_SHIFT) < 0) modifiers |= MOD_SHIFT;
		if (::GetKeyState(VK_MENU) < 0) modifiers |= MOD_ALT;

		this->key = (UINT)wParam;
		this->repeat = (UINT)lParam & 0xFFFF;
		this->flags = (UINT)((lParam & 0xFFFF0000) >> 16);
	}

	//////////////////////////////////////////////////////////////////////////
	// FParentWindowEvent
	FEVENT_INLINE FParentWindowEvent::FParentWindowEvent(FEvent::Type type, HWND hParentWnd) : 
		FEvent(type)
		, hWnd(hParentWnd)
	{
	}

	FEVENT_INLINE FParentWindowEvent::operator HWND() const
	{
		return hWnd;
	}

	//////////////////////////////////////////////////////////////////////////
	// FPreCreateEvent
	FEVENT_INLINE FWindowPreCreateEvent::FWindowPreCreateEvent(void) : FEvent(FEvent::WindowPreCreate)
	{
		memset(&wci, 0, sizeof(FWNDCREATEINFO));

		wci.classinfo.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wci.classinfo.hCursor = ::LoadCursor(NULL, IDC_ARROW);

		wci.wndinfo.x = CW_USEDEFAULT;
		wci.wndinfo.y = CW_USEDEFAULT;
		wci.wndinfo.cx = CW_USEDEFAULT;
		wci.wndinfo.cy = CW_USEDEFAULT;
		wci.wndinfo.dwStyle = WS_OVERLAPPEDWINDOW /* | WS_CLIPCHILDREN | WS_CLIPSIBLINGS*/;
	}

	//////////////////////////////////////////////////////////////////////////
	// FCharEvent
	FEVENT_INLINE FCharEvent::FCharEvent(FEvent::Type type, UINT character) : 
		FEvent(type) 
	{
		this->character = character;
	}

	//////////////////////////////////////////////////////////////////////////
	// FReflectEvent
	FEVENT_INLINE FReflectEvent::FReflectEvent(FEvent::Type type, Widgets::FWidget* widget, FEvent* event) : 
		FEvent(type)
	{
		this->widget = widget;
		this->event = event;
	}

	//////////////////////////////////////////////////////////////////////////
	// FCursorEvent
	FEVENT_INLINE FCursorEvent::FCursorEvent(POINT mouse, POINT cursor) : 
		FEvent(FEvent::Cursor)
	{
		this->mouse = mouse;
		this->cursor = cursor;
	}

	//////////////////////////////////////////////////////////////////////////
	// FWindowCreatedEvent
	FEVENT_INLINE FWindowCreatedEvent::FWindowCreatedEvent(LPCREATESTRUCT cs) : FEvent(FEvent::WindowCreated)
	{
		this->cs = cs;
	}

	//////////////////////////////////////////////////////////////////////////
	// FCustomEvent
	FEVENT_INLINE FCustomEvent::FCustomEvent(LPVOID data) : 
		FEvent(FEvent::Custom)
		, m_data(data)
	{
	}

	FEVENT_INLINE void FCustomEvent::setData(LPVOID data)
	{
		m_data = data;
	}

	FEVENT_INLINE LPVOID FCustomEvent::data(void) const
	{
		return m_data;
	}

}}} // !namespace