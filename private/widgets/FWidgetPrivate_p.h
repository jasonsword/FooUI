/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FWidgetPrivate_p.h
* 描述：		FWidgetPrivate_p
* 创建日期：	2013-06-06
*
*/

#ifndef FWidgetPrivate_P_H
#define FWidgetPrivate_P_H

#include "core/FTypes.h"
#include "core/FPtrList.h"
#include "core/FLock.h"

#include "widgets/FWidget.h"
#include "widgets/events/FEvent.h"

#include "../../private/widgets/FWindowPrivate_p.h"

namespace FooUI { namespace Widgets {
	
	//////////////////////////////////////////////////////////////////////////
	// FWidgetPrivate， 将一些不想让外部看到的方法封装到该类
	class FWidgetPrivate
	{
		friend FWindowPrivate;
		friend FWidget;
	public:
		explicit FWidgetPrivate(FWidget* pWidget);
		~FWidgetPrivate(void);

	public:
		/** 
			子控件管理
		*/
		/** 将pWidget插入到最后，成功返回true，失败返回false */
		bool addChild(FWidget* pWidget);
		/** 将pWidget插入到nIndex位置，成功返回true，失败返回false */
		bool insertChild(int nIndex, FWidget* pWidget);
		/** 移除一个Widget，成功返回true，失败返回false */
		bool removeChild(int nIndex);
		bool removeChild(FWidget* pWidget);
		bool removeChildAndNoDel(int nIndex);
		bool removeChildAndNoDel(FWidget* pWidget);
		/** 清空所有Widget，成功返回true，失败返回false */
		bool clearChildren(void);
		/** 获取Widget对象 */
		FWidget* getChild(int nIndex) const;
		/** 获取Widget索引 */
		int indexOfChild(FWidget* widget) const;
		/** 获取Widget个数 */
		int getChildrenCount(void) const;

		/** 锁定子控件的插入、删除操作 */
		void lockChildren(void);
		void unlockChildren(void);

		/** 根据控件名查找控件 */
		FWidget* findWidget(LPCTSTR pszName) const;
		FWidget* findNextWidget(FWidget* pCurWidget, LPCTSTR pszName) const;

		/** 根据控件ID查找控件 */
		FWidget* findWidget(int nID) const;
		FWidget* findNextWidget(FWidget* pCurWidget, int nID) const;

		/** 初始化父控件，如果toplevel为NULL，parent将被设置为参数中的parent */
		bool initParent(FWidget* parent, FWidget* toplevel);

	public:
		bool createWindow(HWND hParentWnd = NULL);
		bool createWindow(
					DWORD dwExStyle,
					LPCTSTR lpClassName,
					LPCTSTR lpWindowName,
					DWORD dwStyle,
					int x,
					int y,
					int nWidth,
					int nHeight,
					HWND hWndParent,
					HMENU hMenu,
					LPVOID lpCreateParams
					);
		bool destroyWindow(void);

		UINT doModal(HWND hWndParent = NULL, bool bModaless = false);
		void endModal(UINT uRetCode = IDCANCEL);

		HWND getHwnd(void) const;
		bool isWindow(void) const;

		/** 如果是分层窗口返回true */
		bool isLayeredWindow(void) const;

	protected:
		/**
			消息处理
		*/
		/** WM_NCCREATE */
		void createMessage(HWND hWnd);

		/** WM_NCDESTROY */
		void finalMessage(HWND hWnd);

	protected:
		void _modalMessageLoop(void);

	public:
		void needLayout(bool need) { m_bNeedLayout = need; }
		void endLayout(void) { m_bNeedLayout = false; }
		bool isNeedLayout(void) const { return m_bNeedLayout; }

		/** 设置控件大小*/
		void setLayoutRect(const RECT* rect)
		{
			Event::FLayoutChangedEvent e(m_rcLayout, Core::FRect(rect));
			m_pWidget->event(&e);
			if (e.isAccept())
			{
				m_rcLayout = e.newLayoutRect;
			}
			else
			{
				m_rcLayout = rect;
			}
		}
		/** 设置布局位置 可见的区域用于窗口的更新操作 -- 2015/4/21 by hyh*/
		void setLayoutRectReal(const RECT* rect){m_rcLayoutReal = rect;}
		void setLayoutRectEmpty(void) { m_rcLayout.setEmpty(); m_rcLayoutReal.setEmpty(); }
		void setLayoutRect(int x0, int y0, int x1, int y1) { m_rcLayout.setLTRB(x0, y0, x1, y1); }
		void setLayoutRectReal(int x0, int y0, int x1, int y1) { m_rcLayoutReal.setLTRB(x0, y0, x1, y1); }
		const Core::FRect& getLayoutRect(void) const { return m_rcLayout; }
		const Core::FRect& getLayoutRectReal(void) const { 
			if (m_rcLayoutReal.isEmpty())
				return m_rcLayout;
			else
				return m_rcLayoutReal;
		}
	public:
		void needUpdate(bool update) { m_bNeedUpdate = update; }
		void endUpdate(void) { m_bNeedUpdate = false; }
		bool isNeedUpdate(void) const { return m_bNeedUpdate; }

		/** 广播Paint事件给子控件（如果该子控件需要重绘） */
		void broadcastPaintEvent(Event::FPaintEvent* e);

		/** 广播事件给所有子控件 
			e: 广播的事件
			broadSelf： 广播前是否传递给自己一次
		*/
		void broadcastEvent(Event::FEvent* e, bool broadSelf = false);
		
		void reflectMouseEvent(Event::FEvent* e);
		void reflectKeyboardEvent(Event::FEvent* e);

	public:
		/** 关联的Widget对象 */
		FWidget* const m_pWidget;

		/** 子控件列表 */
		Core::FPtrList m_children;
		Core::FCriticalSectionLock m_lock;

		/** 顶层控件与父控件 */
		FWidget* m_pTopLevelWidget;
		FWidget* m_pParentWidget;

		/** 标记是否需要重新布局 */
		bool m_bNeedLayout;
		/** 布局位置 */
		Core::FRect m_rcLayout;

		/** 布局位置 可见的区域 用于窗口的更新操作 -- 2015/4/21 by hyh*/
		Core::FRect m_rcLayoutReal;

		/** 标记是否需要更新 */
		bool m_bNeedUpdate;

		/** doModal的返回值 */
		UINT m_uRetCode;
		bool m_bExitModalLoop;

		/** 一些窗口的属性写在这里 */
		struct FWindowPrivateStyle
		{
			FWindowPrivateStyle(void) : 
				dwStyle(WS_OVERLAPPEDWINDOW) 
				, dwExStyle(0) 
				, border(true) 
				, center(false)
				, windowncspace(0, 0, 0, 0) 
			{ }

			DWORD dwStyle;
			DWORD dwExStyle;
			bool border;	/** 窗口是否具有边框 */
			bool center;	/** 窗口创建时是否居中显示 */
			Core::FEdge windowncspace; /** 窗口的可拉伸边距 */
		};
		FWindowPrivateStyle m_wndstyle;
		void parseWindowStyle(LPCTSTR pszValue);

		/** 本函数只解析windowncspace：
			当返回false表示没有处理windowncspace
		*/
		bool hittest(const Core::FRect& rcWindow, const Core::FPoint& ptMouse, LRESULT& lResult);

	public:
		/** 保存了当前焦点、高亮、按压控件 */
		static FWidget* fFocusedWidget;
		static FWidget* fHoverWidget;
		static FWidget* fPushedWidget;
		static FWidget* fActiveWidget;

	protected:
		/** 维护了整个Tab顺序链 
		*/
		/** 当控件析构应当调用该函数从Tab顺序链中删除 */
		static bool removeTabWidget(FWidget* widget);
		/** 改变widget的tab索引，如果在链中不存在则插入 */
		static bool setWidgetTabIndex(int nTabIndex, FWidget* widget);
		/** 获取下一个Tab顺序链的控件，如果widget为NULL将返回第一个
			如果列表为空则返回NULL
			会忽略!isVisible()、isSkipFocus()、isSkipTab()的控件
		*/
		static FWidget* getNextTabWidget(FWidget* widget = NULL);
		/** 如果返回true，widget是一个有效的tab控件 */
		static bool isValidTabWidget(FWidget* widget);

	protected:
		/** 窗口对象 */
		FWindowPrivate* m_pWindow;
	};

}} // !namespace

#endif // !FWidgetPrivate_P_H