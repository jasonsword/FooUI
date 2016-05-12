/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FEvent.h
* 描述：		FEvent
* 创建日期：	2013-06-08
*
*/

#ifndef FEVENT_H
#define FEVENT_H

#include "core/FBase.h"
#include "core/FObject.h"
#include "core/FString.h"
#include "core/FTypes.h"

#include "widgets/FWidgetDef.h"

namespace FooUI { namespace Painting { class FGraphics; }} // !namespace
namespace FooUI { namespace Widgets { class FWidget; }} // !namespace

namespace FooUI { namespace Widgets { namespace Event {

	//////////////////////////////////////////////////////////////////////////
	// FEvent
	class FOOUI_LIB FEvent : public Core::FNoncopyableObject
	{
	public:
		enum Type
		{
			None = 0,                           // invalid event
			PreCreate,							// before widget creation
			Create,								// after widget creation
			Destroy,							// during widget destruction
			Paint,								// paint widget
			Show,								// widget is shown
			Hide,								// widget is hidden
			Cursor,								// show cursor
			Changed,							// changed
			LayoutChanged,						// layout changed
			FocusIn,                            // keyboard focus received
			FocusOut,                           // keyboard focus lost
			MouseButtonPress,                   // mouse button pressed
			MouseButtonRelease,                 // mouse button released
			MouseButtonDblClick,                // mouse button double click
			MouseButtonClick,                   // mouse click widget
			MouseMove,                          // mouse move
			MouseWheel,                         // wheel event
			MouseEnter,                         // mouse enters widget
			MouseLeave,                         // mouse leaves widget
			KeyPress,                           // key pressed
			KeyRelease,                         // key released
			KeyChar,							// key char
			reflectMouse,						// child reflect mouse event
			reflectKey,							// child reflect keyboard event
			ParentChange,						// widget has been reparented
			ParentWindowCreated,				// parent window was created
			ParentWindowSized,					// parent window was sized
			WindowPreCreate,					// window before create
			WindowCreated,						// window was created
			WindowActivate,						// window was activated
			WindowDeactivate,					// window was deactivated
			WindowTitleChange,					// window title changed
			Custom,	

			User = 1024, // first user event id
			MaxUser = 65535, // last user event id
		};
		explicit FEvent(Type type);
		virtual ~FEvent(void) { }

	private:
		FEvent(void);
		FEvent(const FEvent&);
		void operator=(const FEvent&);

	public:
		FOOINLINE void accpet(bool bAccept = true) { m_accept = bAccept; }
		FOOINLINE void ignore(void) { m_accept = false; }
		FOOINLINE bool isAccept(void) const { return m_accept; }
		FOOINLINE Type type(void) const { return static_cast<Type>(m_type); }

	public:
		operator FEvent*();
		operator const FEvent*() const;

	private:
		bool m_accept;
		int m_type;
	};

	//////////////////////////////////////////////////////////////////////////
	// FPaintEvent
	class FOOUI_LIB FPaintEvent : public FEvent
	{
	public:
		FPaintEvent(Painting::FGraphics* pGraphics, const Core::FRect& rcUpdate);

	public:
		Painting::FGraphics* graphics(void) const;
		const Core::FRect& updateRect(void) const;

	private:
		Painting::FGraphics* m_pGraphics; /** 画布 */
		Core::FRect m_rcUpdate;	/** 脏区域 */
	};

	//////////////////////////////////////////////////////////////////////////
	// FFocusedEvent
	class FOOUI_LIB FFocusedEvent : public FEvent
	{
	public:
		FFocusedEvent(bool bFocused);

	public:
		bool focused;
	};

	//////////////////////////////////////////////////////////////////////////
	// FChangedEvent
	class FOOUI_LIB FChangedEvent : public FEvent
	{
	public:
		FChangedEvent(UINT changedType);

	public:
		UINT changedType; // 参考 eChangedType
	};

	//////////////////////////////////////////////////////////////////////////
	// FLayoutChangedEvent
	class FOOUI_LIB FLayoutChangedEvent : public FEvent
	{
	public:
		FLayoutChangedEvent(const RECT& rcOld, const RECT& rcNew);

	public:
		Core::FRect oldLayoutRect;
		Core::FRect newLayoutRect;
	};

	//////////////////////////////////////////////////////////////////////////
	// FMouseEvent
	class FOOUI_LIB FMouseEvent : public FEvent
	{
	public:
		FMouseEvent(FEvent::Type type, UINT button, 
					POINT mouse, POINT cursor);

	public:
		UINT modifiers;
		UINT button;
		Core::FPoint mouse;
		Core::FPoint cursor;
	};

	//////////////////////////////////////////////////////////////////////////
	// FWheelEvent
	class FOOUI_LIB FWheelEvent : public FEvent
	{
	public:
		FWheelEvent(POINT point, UINT button, int delta);

	public:
		UINT modifiers;
		UINT button;
		int delta;
		Core::FPoint point;
	};

	//////////////////////////////////////////////////////////////////////////
	// FKeyEvent
	class FOOUI_LIB FKeyEvent : public FEvent
	{
	public:
		FKeyEvent(FEvent::Type type, UINT key, UINT repeat, UINT flags);
		FKeyEvent(FEvent::Type type, WPARAM wParam, LPARAM lParam);

	public:
		UINT modifiers;
		UINT key;
		UINT repeat;
		UINT flags;
	};

	//////////////////////////////////////////////////////////////////////////
	// FCharEvent
	class FOOUI_LIB FCharEvent : public FEvent
	{
	public:
		FCharEvent(FEvent::Type type, UINT character);

	public:
		UINT character;
	};

	//////////////////////////////////////////////////////////////////////////
	// FReflectEvent
	class FOOUI_LIB FReflectEvent : public FEvent
	{
	public:
		FReflectEvent(FEvent::Type type, Widgets::FWidget* widget, FEvent* event);
	
	public:
		Widgets::FWidget* widget;
		FEvent* event;
	};

	//////////////////////////////////////////////////////////////////////////
	// FCursorEvent
	class FOOUI_LIB FCursorEvent : public FEvent
	{
	public:
		FCursorEvent(POINT mouse, POINT cursor);

	public:
		Core::FPoint mouse;
		Core::FPoint cursor;
	};

	//////////////////////////////////////////////////////////////////////////
	// FParentWindowEvent
	class FOOUI_LIB FParentWindowEvent : public FEvent
	{
	public:
		FParentWindowEvent(FEvent::Type type, HWND hParentWnd);

		operator HWND() const;

	public:
		HWND hWnd;
	};

	//////////////////////////////////////////////////////////////////////////
	// FPreCreateEvent
	class FOOUI_LIB FWindowPreCreateEvent : public FEvent
	{
	public:
		FWindowPreCreateEvent(void);

	public:
		FWNDCREATEINFO wci;
	};

	//////////////////////////////////////////////////////////////////////////
	// FWindowCreatedEvent
	class FOOUI_LIB FWindowCreatedEvent : public FEvent
	{
	public:
		FWindowCreatedEvent(LPCREATESTRUCT cs);

	public:
		LPCREATESTRUCT cs;
	};

	//////////////////////////////////////////////////////////////////////////
	// FCustomEvent
	class FOOUI_LIB FCustomEvent : public FEvent
	{
	public:
		FCustomEvent(LPVOID data = NULL);

	public:
		void setData(LPVOID data);
		LPVOID data(void) const;

	private:
		LPVOID m_data;
	};

}}} // !namespace

#endif // !FEVENT_H