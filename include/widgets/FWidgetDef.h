/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FWidgetDef.h
* 描述：		FWidgetDef
* 创建日期：	2013-06-08
*
*/

#ifndef FWIDGETDEF_H
#define FWIDGETDEF_H

#include "core/FBase.h"

namespace FooUI { namespace Widgets {

	class FWidget;

	//////////////////////////////////////////////////////////////////////////
	// 
	class FOOUI_LIB IWidget
	{
	public:
		virtual LPCTSTR getClass(void) const = 0;
		virtual FWidget* getInterface(LPCTSTR pszClass) { return NULL; }
		virtual bool isContainer(void) const = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	// IWidgetContainer
	class FOOUI_LIB IWidgetContainer
	{
	public:
		virtual bool addChild(FWidget* pWidget) = 0;
		virtual bool insertChild(int nIndex, FWidget* pWidget) = 0;
		virtual bool removeChild(int nIndex) = 0;
		virtual bool removeChild(FWidget* pWidget) = 0;
		virtual bool clearChildren(void) = 0;
		virtual FWidget* getChild(int nIndex) const = 0;
		virtual int childIndex(void) const = 0;
		virtual int indexOfChild(FWidget* widget) const = 0;
		virtual int getChildrenCount(void) const = 0;
		virtual bool hasChildren(void) const = 0;
		virtual FWidget* findWidget(LPCTSTR pszName) const = 0;
		virtual FWidget* findNextWidget(FWidget* pCurWidget, LPCTSTR pszName) const = 0;
		virtual FWidget* findWidget(int nID) const = 0;
		virtual FWidget* findNextWidget(FWidget* pCurWidget, int nID) const = 0;
		
	};

	//////////////////////////////////////////////////////////////////////////
	// Window CreateInfo
	struct FWNDCREATEINFO 
	{
		struct CLASSINFO 
		{
			UINT style;
			HICON hIcon;
			HCURSOR hCursor;
			HBRUSH hbrBackground;
		};
		struct WNDINFO
		{
			DWORD dwExStyle;
			LPCTSTR pszWindowText;
			DWORD dwStyle;
			int x;
			int y;
			int cx;
			int cy;
			HWND hwndParent;
			HMENU hMenu;
			LPVOID lpCreateParams;
		};

		LPCTSTR lpszClassName;
		CLASSINFO classinfo;
		WNDINFO wndinfo;
	};

	/** 定义类名 */
#define FOODECLARE_CLASS(_className) \
public: \
	virtual LPCTSTR getClass(void) const \
	{ \
		return _className; \
	} \
	virtual FWidget* getInterface(LPCTSTR pszClass) \
	{ \
		if (_tcscmp(_className, pszClass) == 0) \
			return this; \
		else \
			return __super::getInterface(pszClass); \
	}

#define FOODECLARE_CONTAINER(_bContainer) \
public: \
	virtual bool isContainer(void) const \
	{ \
		return _bContainer; \
	} \

//////////////////////////////////////////////////////////////////////////
/** 消息映射 */
#define foo_msg

#define FOOMSG_DECLARE_MAP() \
protected: \
	virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

#define FOOMSG_BEGIN_MAP(theClass) \
	/*virtual*/ LRESULT theClass::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) \
	{ \
	LRESULT lResult = 0;

/** 无条件转发消息 */
/** LRESULT onMessageTransmit(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) */
#define FOOMSG_TRANSMIT(fun) \
	do { \
		lResult = fun(uMsg, wParam, lParam, bHandled); \
		if (bHandled) return lResult; \
	} while(0)

/** 对特定消息处理 */
/** LRESULT onMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) */
#define FOOMSG_ON_MESSAGE(message, fun) \
	if ((message) == uMsg) \
	{ \
		lResult = fun(uMsg, wParam, lParam, bHandled); \
		if (bHandled) return lResult; \
	}

/** 对WM_CREATE消息处理 */
/** LRESULT onCreateMsg(LPCREATESTRUCT lpCreateStruct, bool& bHandled) */
#define FOOMSG_ON_CREATE(fun) \
	if (WM_CREATE == uMsg) \
	{ \
		lResult = fun((LPCREATESTRUCT)lParam, bHandled); \
		if (bHandled) return lResult; \
	}

/** 对WM_PAINT消息处理 */
/** LRESULT onPaintMsg(bool& bHandled) */
#define FOOMSG_ON_PAINT(fun) \
	if (WM_PAINT == uMsg) \
	{ \
		lResult = fun(bHandled); \
		if (bHandled) return lResult; \
	}

/** 对WM_MOVE消息处理 */
/** LRESULT onMoveMsg(int x, int y, bool& bHandled) */
#define FOOMSG_ON_MOVE(fun) \
	if (WM_MOVE == uMsg) \
	{ \
		lResult = fun(LOWORD(lParam), HIWORD(lParam), bHandled); \
		if (bHandled) return lResult; \
	}

/** 对WM_SIZE消息处理 */
/** LRESULT onSizeMsg(UINT nType, int cx, int cy, bool& bHandled) */
#define FOOMSG_ON_SIZE(fun) \
	if (WM_SIZE == uMsg) \
	{ \
		lResult = fun(wParam, (int)LOWORD(lParam), (int)HIWORD(lParam), bHandled); \
		if (bHandled) return lResult; \
	}

/** 对WM_TIMER消息处理 */
/** LRESULT onTimerMsg(UINT_PTR nIDEvent, bool& bHandled) */
#define FOOMSG_ON_TIMER(fun) \
	if (WM_TIMER == uMsg) \
	{ \
		lResult = fun((UINT_PTR)wParam, bHandled); \
		if (bHandled) return lResult; \
	}

#define FOOMSG_END_MAP() \
	return __super::handleMessage(uMsg, wParam, lParam, bHandled); \
	} 
	/** !消息映射 */

	//////////////////////////////////////////////////////////////////////////
	// Widget消息
#define FWM_NULL			0x0000
#define FWM_CREATE			0x0001
#define FWM_DESTROY			0x0002
#define FWM_LAYOUTCHANGED	0x0003
#define FWM_FOCUSIN			0x0004
#define FWM_FOCUSOUT		0x0005
#define FWM_ENABLED			0x0006
#define FWM_SHOW			0x0007

/** 鼠标系列消息：
	wParam：	eMouseButton
	lParam：	low-order word为鼠标x坐标，high-order word为鼠标y坐标
*/
#define FWM_MOUSEMOVE		0x0008
#define FWM_MOUSEPRESS		0x0009
#define FWM_MOUSERELEASE	0x000A
#define FWM_MOUSEDBLCLK		0x000B
#define FWM_MOUSECLICK		0x000C
/** 鼠标移入和离开：
	wParam：	未使用
	lParam：	未使用
*/
#define FWM_MOUSEENTER		0x000D
#define FWM_MOUSELEAVE		0x000E

/** 鼠标滚轮：
	wParam：	未使用
	lParam：	未使用
*/
#define FWM_MOUSEWHEEL		0x000F

#define FWM_KEYPRESS		0x0010
#define FWM_KEYRELEASE		0x0011
#define FWM_CHAR			0x0012

/** FWM_HSCROLL、FWM_VSCROLL：
	wParam:	FAbstractSlider::eFSliderAction
	lParam:	nTrackPos
	滚动
*/
#define FWM_HSCROLL			0x0013
#define FWM_VSCROLL			0x0014

/** FWM_SELECTED：
	wParam:	true勾选,false未勾选
	lParam:	未使用
	勾选状态改变
*/
#define FWM_CHECKED			0x0015

/**	列表项被勾选
	wParam: 列表项索引
	lParam: 列表项对象(FWidget*)
*/
#define FWM_ITEMCHECKED		0x0016

/** FWM_SELECTED：
	wParam:	true选中,false未选中
	lParam:	未使用
	选中状态改变
*/
#define FWM_SELECTED		0x0017



/**	列表项被选择
	wParam: 列表项索引
	lParam: 列表项对象(FWidget*)
*/
#define FWM_ITEMSELECTED	0x0018

/** FWM_CHANGED：
	wParam:	eChangedType
	lParam:	未使用
	目前暂时用于TEXT改变的时候传递 
*/
enum eChangedType
{
	CT_NULL		= 0, 
	CT_TEXT		= 1, 
	CT_VALUE	= 2, 
};
#define FWM_CHANGED			0x0019

/**	通知消息
	wParam: code
	lParam: FWNMHDR*
*/
enum eNotifyCode
{
	NC_CHANGED = 0, 
	NC_RETURN = 1, 
};
typedef struct tagFWNMHDR
{
	bool bHandled;
} FWNMHDR, *PFWNMHDR, *LPFWNMHDR;
#define FWM_NOTIFY			0x001F

#define FWM_USER			(FWM_NULL + 1024)

//////////////////////////////////////////////////////////////////////////
// Widget消息映射
#define FOOWMSG_DECLARE_MAP() \
protected: \
	virtual LRESULT handleWidgetMessage(FWidget* pSender, UINT nID, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

#define FOOWMSG_BEGIN_MAP(theClass) \
	/*virtual*/ LRESULT theClass::handleWidgetMessage(FWidget* pSender, UINT nID, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) \
	{ \
	LRESULT lResult = 0;

	/** 无条件处理消息 */
	/** LRESULT onWidgetMsgTransmit(FWidget* pSender, UINT nID, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) */
	#define FOOWMSG_TRANSMIT(fun) \
	do { \
		lResult = fun(pSender, nID, uMsg, wParam, lParam, bHandled); \
		if (bHandled) return lResult; \
	} while(0)

	/** 处理指定消息 */
	/** LRESULT onWidgetMsg(FWidget* pSender, UINT nID, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) */
	#define FOOWMSG_ON_MESSAGE(message, fun) \
		if ((message) == uMsg) \
	{ \
		lResult = fun(pSender, nID, uMsg, wParam, lParam, bHandled); \
		if (bHandled) return lResult; \
	}
	
	#define SENDER(sender) \
		((sender) == pSender)

	#define SENDERID(id) \
		((id) == nID)

	#define SENDERNAME(sendername) \
		(_tcscmp(pSender->getName(), sendername) == 0)
	
	#define GROUPNAME(groupname) \
		(_tcscmp(pSender->getGroup(), groupname) == 0)

	/** 如果expr成立， 调用fun，例如：
		FOOWMSG_ON_HANDLE(SENDERNAME(_T("testbutton")), fun)
	*/
	/** LRESULT onWidgetMsg(FWidget* pSender, UINT nID, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) */
	#define FOOWMSG_HANDLER(expr, fun) \
	if ((expr)) \
	{ \
		lResult = fun(pSender, nID, uMsg, wParam, lParam, bHandled); \
		if (bHandled) return lResult; \
	}
	
	/** 处理指定消息 */
	/** LRESULT onWidgetButtonClick(FWidget* pSender, UINT nID, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) */
	#define FOOWMSG_MESSAGE_HANDLER(expr, message, fun) \
	if ((expr) && (message) == uMsg) \
	{ \
		lResult = fun(pSender, nID, uMsg, wParam, lParam, bHandled); \
		if (bHandled) return lResult; \
	}

	/** 处理FWM_MOUSECLICK消息 */
	/** LRESULT onWidgetButtonClick(FWidget* pSender, UINT nID, UINT uMsg, UINT uButton, int x, int y, bool& bHandled) */
	#define FOOWMSG_BUTTONCLICK_HANDLER(expr, fun) \
		if ((expr) && FWM_MOUSECLICK == uMsg) \
	{ \
		lResult = fun(pSender, nID, uMsg, (UINT)wParam, LOWORD(lParam), HIWORD(lParam), bHandled); \
		if (bHandled) return lResult; \
	}

	/** 处理FWM_MOUSECLICK左键消息 */
	/** LRESULT onWidgetButtonClick(FWidget* pSender, UINT nID, UINT uMsg, UINT uButton, int x, int y, bool& bHandled) */
	#define FOOWMSG_LBUTTONCLICK_HANDLER(expr, fun) \
		if ((expr) && FWM_MOUSECLICK == uMsg \
			&& FooUI::LeftButton == wParam) \
	{ \
		lResult = fun(pSender, nID, uMsg, (UINT)wParam, LOWORD(lParam), HIWORD(lParam), bHandled); \
		if (bHandled) return lResult; \
	}

	/** 处理FWM_CHECKED消息 */
	/** LRESULT onWidgetChecked(FWidget* pSender, UINT nID, bool bChecked, LPARAM lParam, bool& bHandled) */
	#define FOOWMSG_CHECKEDMSG_HANDLER(expr, fun) \
		if ((expr) && FWM_CHECKED == uMsg) \
	{ \
		lResult = fun(pSender, nID, wParam ? true : false, lParam, bHandled); \
		if (bHandled) return lResult; \
	}

	/** 处理FWM_CHECKED消息 */
	/** LRESULT onWidgetChecked(FWidget* pSender, UINT nID, bool bChecked, LPARAM lParam, bool& bHandled) */
	#define FOOWMSG_CHECKED_HANDLER(expr, fun) \
		if ((expr) && FWM_CHECKED == uMsg && TRUE == wParam) \
	{ \
		lResult = fun(pSender, nID, wParam ? true : false, lParam, bHandled); \
		if (bHandled) return lResult; \
	}

	/** 处理FWM_CHECKED消息 */
	/** LRESULT onWidgetChecked(FWidget* pSender, UINT nID, bool bChecked, LPARAM lParam, bool& bHandled) */
	#define FOOWMSG_UNCHECKED_HANDLER(expr, fun) \
	if ((expr) && FWM_CHECKED == uMsg && FALSE == wParam) \
		{ \
		lResult = fun(pSender, nID, wParam ? true : false, lParam, bHandled); \
		if (bHandled) return lResult; \
	}

	/** 处理FWM_CHANGED消息 */
	/** LRESULT onWidgetChecked(FWidget* pSender, UINT nID, bool bChecked, LPARAM lParam, bool& bHandled) */
	#define FOOWMSG_CHANGED_HANDLER(expr, fun) \
	if ((expr) && FWM_CHANGED == uMsg) \
	{ \
		lResult = fun(pSender, nID, wParam ? true : false, lParam, bHandled); \
		if (bHandled) return lResult; \
	}

	/** 处理FWM_CHANGED消息 */
	/** LRESULT onWidgetChanged(FWidget* pSender, UINT nID, WPARAM wParam, LPARAM lParam, bool& bHandled) */
	#define FOOWMSG_CHANGEDTYPE_HANDLER(expr, ct, fun) \
	if ((expr) && FWM_CHANGED == uMsg && (ct) == wParam) \
	{ \
		lResult = fun(pSender, nID, wParam, lParam, bHandled); \
		if (bHandled) return lResult; \
	}

#define FOOWMSG_END_MAP() \
	return __super::handleWidgetMessage(pSender, nID, uMsg, wParam, lParam, bHandled); \
	}

/** !Widget消息映射 */

}} // !namespace

#endif // !FWIDGETDEF_H