/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FWidget.h
* 描述：		FWidget
* 创建日期：	2013-06-06
*
*/

#ifndef FWIDGET_H
#define FWIDGET_H

#include "core/FBase.h"
#include "core/FTypes.h"
#include "core/FObject.h"


#include "painting/FGraphics.h"

#include "markup/FMarkup.h"
#include "markup/FResourceManager.h"
#include "markup/FStyleSheetResource.h"
#include "markup/FCursorResource.h"

#include "widgets/FWidgetDef.h"
#include "widgets/events/FEvent.h"

namespace FooUI { namespace Widgets {

	class FWidgetPrivate;
	class FWindowPrivate;
	class FScrollBar;
	class FToolTip;

	//////////////////////////////////////////////////////////////////////////
	// FWidget
	#define FOOWC_WIDGET _T("widget")
	class FOOUI_LIB FWidget : 
		public Core::FObject, 
		public Core::FNoncopyableObject, 
		public IWidget, 
		public IWidgetContainer, 
		public Markup::FMarkupObject
	{
		friend FWidgetPrivate;
		friend FWindowPrivate;
		friend FScrollBar;
	public:
		explicit FWidget(FWidget* parent = NULL);
		virtual ~FWidget(void);
		FOODECLARE_CLASS(FOOWC_WIDGET)
		FOODECLARE_CONTAINER(false)

	public:
		/** 
			控件属性相关
		*/
		/** 设置/返回name，不会检测唯一性 */
		void setName(LPCTSTR pszName);
		LPCTSTR getName(void) const;

		/** 设置/返回id，不会检测唯一性 */
		void setID(int nID);
		int getID(void) const;

		/** 控件组 */
		void setGroup(LPCTSTR pszGroup);
		LPCTSTR getGroup(void) const;

		/** 设置/返回text */
		void setText(LPCTSTR pszText);
		LPCTSTR getText(void) const;

		/** 设置/返回Tab索引 */
		void setTabIndex(int tabIndex);
		int getTabIndex(void) const;

		/** 设置/返回Attributes */
		void setFlags(DWORD dwAttribute);
		void modifyFlag(DWORD dwRemove, DWORD dwAdd);
		bool testFlag(DWORD dwAttribute) const;
		DWORD getFlags(void) const;

		/** 设置/返回状态 */
		void setState(DWORD dwState, bool bRepaint = false);
		bool modifyState(DWORD dwRemove, DWORD dwAdd, bool bRepaint = false);
		bool testState(DWORD dwState) const;
		DWORD getState(void) const;

		/** 设置/返回Dock */
		void setDock(DWORD dwDock);
		DWORD getDock(void) const;

		/** 设置/返回Align */
		void setAlign(DWORD dwAlign);
		bool testAlign(DWORD dwAlign) const;
		DWORD getAlign(void) const;

		/** 返回/设置控件提示文本 */
		void setToolTipText(LPCTSTR pszToolTip);
		LPCTSTR getToolTipText(void) const;

		/** 返回/设置控件Role */
		void setRole(DWORD dwRole);
		DWORD getRole(void) const;

		/** 获取有效的布局区域
			排除内边距及滚动条
		*/
		void getValidLayoutRect(RECT* lpRect) const;

		/** 返回/设置外边距 */
		void setMargin(const Core::FEdge& egMargin);
		void setMargin(int xLeft, int yTop, int xRight, int yBottom);
		const Core::FEdge& getMargin(void) const;

		/** 返回/设置内边距 */
		void setPadding(const Core::FEdge& egPadding);
		void setPadding(int xLeft, int yTop, int xRight, int yBottom);
		const Core::FEdge& getPadding(void) const;

		/** 返回/设置控件位置 */
		void setLeft(int nLeft);
		void setTop(int nTop);
		int getLeft(void) const;
		int getTop(void) const;
		void setPosition(const Core::FPoint& position);
		void setPosition(int xLeft, int yTop);
		/** 清空水平和垂直坐标（将自动调整） */
		void emptyXPosition(void);
		void emptyYPosition(void);
		void emptyPosition(void);
		const Core::FPoint& getPosition(void) const;
		/** 返回/设置绝对位置 */
		void relativePosition(bool bRelative);
		bool isRelativePosition(void) const;
		/** 返回/设置浮动位置 */
		void floatPosition(bool bFloat);
		bool isFloatPosition(void) const;
		/** 返回/设置是否固定水平或垂直坐标（如果不固定则自动调整) */
		bool isFixedXPosition(void) const;
		bool isFixedYPosition(void) const;

		/** 返回设置控件大小 */
		void setWidth(int nWidth);
		void setHeight(int nHeight);
		int getWidth(void) const;
		int getHeight(void) const;
		void setSize(Core::FSize& size);
		void setSize(int cx, int cy);
		/** 清空宽度和高度（将自动调整） */
		void emptyWidth(void);
		void emptyHeight(void);
		void emptySize(void);
		const Core::FSize& getSize(void) const;
		/** 返回/设置是否固定宽度或高度（如果不固定则自动调整） */
		bool isFixedWidth(void) const;
		bool isFixedHeight(void) const;

		/** 返回设置控件最小大小 */
		void setMinWidth(int nMinWidth);
		void setMinHeight(int nMinHeight);
		int getMinWidth(void) const;
		int getMinHeight(void) const;
		void setMinSize(Core::FSize& size);
		void setMinSize(int cx, int cy);
		void clearMinSize(void);
		const Core::FSize& getMinSize(void) const;

		/** 返回设置控件最大大小 */
		void setMaxWidth(int nMaxWidth);
		void setMaxHeight(int nMaxHeight);
		int getMaxWidth(void) const;
		int getMaxHeight(void) const;
		void setMaxSize(Core::FSize& size);
		void setMaxSize(int cx, int cy);
		void clearMaxSize(void);
		const Core::FSize& getMaxSize(void) const;

		/** 设置/返回样式表 */
		void setStyle(const Markup::FStyleSheetResource& stylesheet);
		bool setStyle(LPCTSTR pszName);
		const Markup::FStyleSheetResource& getStyle(void) const;
		/** 检查是否包含dwState1或dwState2状态的风格 
			当状态改变时， 避免不必要的重绘
			当使用了额外的样式表，请重载该函数
			如果返回false，即使状态改变也不进行重绘。
		*/
		/** 检查是否包含dwState的样式（重绘前检查，减少无用重绘） */
		virtual bool testStyle(DWORD dwState, LPCTSTR pszType = NULL, LPCTSTR pszPart = NULL) const;

		/** 设置/返回资源管理器 */
		void setResourceManager(const Markup::FResourceManager& resourceMgr);
		Markup::FResourceManager& resourceManager(void);
		const Markup::FResourceManager& resourceManager(void) const;
		Markup::FResourceObject* getResource(LPCTSTR pszName, DWORD dwType) const;

		void setUserData(LPVOID lpData);
		LPVOID getUserData(void) const;

		void setUserTag(LPCTSTR pszUserTag);
		LPCTSTR getUserTag(void) const;

	public:
		/** 
			子控件管理
		*/
		/** 将pWidget插入到最后，成功返回true，失败返回false */
		bool addChild(FWidget* pWidget);
		/** 将pWidget插入到nIndex位置，成功返回true，失败返回false */
		bool insertChild(int nIndex, FWidget* pWidget);
		/** 移除一个Widget，成功返回true，失败返回false 
			*注意，任何移除操作都可能造成对象被析构
		*/
		bool removeChild(int nIndex);
		bool removeChild(FWidget* pWidget);
		/** 清空所有Widget，成功返回true，失败返回false */
		bool clearChildren(void);
		/** 获取Widget对象 */
		FWidget* getChild(int nIndex) const;
		/** 获取此控件在父控件的顺序索引 */
		int childIndex(void) const;
		/** 获取Widget在parent中的顺序索引 */
		int indexOfChild(FWidget* widget) const;
		/** 获取Widget个数 */
		int getChildrenCount(void) const;
		bool hasChildren(void) const;

		/** 根据控件名查找控件 */
		FWidget* findWidget(LPCTSTR pszName) const;
		FWidget* findNextWidget(FWidget* pCurWidget, LPCTSTR pszName) const;

		/** 根据控件ID查找控件 */
		FWidget* findWidget(int nID) const;
		FWidget* findNextWidget(FWidget* pCurWidget, int nID) const;

		/** 获取下一个Tab控件 */
		FWidget* getNextTabWidget(void) const;
		FWidget* getNextTabWidget(FWidget* widget) const;

		/** 锁定子控件的插入、删除操作 */
		void lockChildren(void);
		void unlockChildren(void);

	public:
		/** 获取鼠标下的控件 */
		virtual FWidget* widgetFromPoint(int xPos, int yPos) const;

		FOOINLINE FWidget* widgetFromPoint(const POINT point) const
		{ return widgetFromPoint(point.x, point.y); }

		FOOINLINE FWidget* findWidget(int xPos, int yPos) const
		{ return widgetFromPoint(xPos, yPos); }

		FOOINLINE FWidget* findWidget(const POINT& point) const
		{ return widgetFromPoint(point.x, point.y); }

	public:
		/** 获取鼠标下的滚动视图 */
		virtual FWidget* scrollViewFromPoint(int xPos, int yPos) const;
		/** 返回true表明是滚动视图 */
		virtual bool isScrollView(void) const;

		FOOINLINE FWidget* scrollViewFromPoint(const POINT point) const
		{ return scrollViewFromPoint(point.x, point.y); }

		FOOINLINE FWidget* findScrollView(const POINT point) const
		{ return scrollViewFromPoint(point.x, point.y); }

		FOOINLINE FWidget* findScrollView(int xPos, int yPos) const
		{ return scrollViewFromPoint(xPos, yPos); }

	public:
		/** 
			控件层处理 
		*/
		void setParent(FWidget* parent);
		/** 设置拥有者（与setParent的区别是不会插入到父控件的子控件列表中） */
		void setOwner(FWidget* parent);
		FWidget* getParent(void) const;
		FWidget* getTopLevelParent(void) const;

		/** 本控件前一个同级控件 */
		FWidget* prevSibling(void) const;
		/** 本控件下一个同级控件 */
		FWidget* nextSibling (void) const;

	public:
		/** 控件操作 */
		void show(void);
		void hide(void);
		void center(FWidget* pWidget = NULL);
		void enable(bool bEnable = true);
		bool isEnabled(void) const;
		void focus(void);
		bool isFocused(void) const;

	public:
		/** 状态设置 */
		void setVisible(bool bVisible = true);
		bool isVisible(void) const;

		void setEnabled(bool bEnabled = true);

		void setFocus(bool bFocuse = true);
		void setFocus(FWidget* pWidget);

		void setSelect(bool bSelected);
		bool isSelected(void) const;

		virtual void setCheck(bool bCheck);
		bool isChecked(void) const;

	public:
		void setTransparent(bool bTransparent);
		bool isTransparent(void) const;

		void skipTab(bool bSkip);
		bool isSkipTab(void) const;

		void skipMouse(bool bSkip);
		bool isSkipMouse(void) const;

		void skipKeyboard(bool bSkip);
		bool isSkipKeyboard(void) const;

		void skipFocus(bool bSkip);
		bool isSkipFocus(void) const;

		void reflectMouse(bool bReflect);
		bool isReflectMouse(void);

		void reflectKeyboard(bool bReflect);
		bool isReflectKeyboard(void);

	protected:
		void setStateNone(bool bRepaint = true);
		bool isStateNone(void) const;

		void setStateNormal(bool bRepaint = true);
		bool isStateNormal(void) const;

		void setStateHover(bool bRepaint = true);
		bool isStateHover(void) const;

		void setStatePushed(bool bRepaint = true);
		bool iStatePushed(void) const;

		void setStateDisabled(bool bRepaint = true);
		bool isStateDisabled(void) const;

		void setStateEnabled(bool bRepaint = true);
		bool isStateEnabled(void) const;

		void setStateInVisible(bool bRepaint = true);
		bool isStateInVisible(void) const;

		void setStateVisible(bool bRepaint = true);
		bool isStateVisible(void) const;

		void setStateUnFocused(bool bRepaint = true);
		bool isStateUnFocused(void) const;

		void setStateFocused(bool bRepaint = true);
		bool isStateFocused(void) const;

		void setStateUnSelected(bool bRepaint = true);
		bool isStateUnSelected(void) const;

		void setStateSelected(bool bRepaint = true);
		bool isStateSelected(void) const;

		void setStateUnChecked(bool bRepaint = true);
		bool isStateUnChecked(void) const;

		void setStateChecked(bool bRepaint = true);
		bool isStateChecked(void) const;

		void setStateNonEmpty(bool bRepaint = true);
		bool isStateNonEmpty(void) const;

		void setStateEmpty(bool bRepaint = true);
		bool isStateEmpty(void) const;

		void setStateDeActivated(bool bRepaint = true);
		bool isStateDeActivated(void) const;

		void setStateActivated(bool bRepaint = true);
		bool isStateActivated(void) const;

	public:
		/** 
			布局相关
		*/
		/** 重新布局（延迟） */
		void relayout(void);
		/** 重新布局（立即） */
		void updateLayout(void);
		/** 让父控件重新布局 */
		void relayoutParent(void);
		void updateLayoutParent(void);

		/** 创建滚动条 
			wBar取值：SB_BOTH、SB_CTL、SB_HORZ、SB_VERT
			bShow指示创建后是否立即显示
			如果创建成功或已经存在滚动条返回true
			否则返回false
		*/
		bool createScrollBar(int wBar, bool bShow = false);

		/** 显示滚动条
			wBar取值：SB_BOTH、SB_CTL、SB_HORZ、SB_VERT
			bShow为true显示滚动条， 为false隐藏滚动条
			如果不存在滚动条返回false
			否则返回true
		*/
		bool showScrollBar(int wBar, bool bShow);
		bool isShowScrollBar(int wBar) const;
		FScrollBar* getScrollBar(int wBar) const;

	public:
		/** 设置/返回布局位置 */
		/** 调用setLayoutRect()会立即产生layout()，
			如果需要重新布局请调用relayout()
		*/
		void setLayoutRect(const Core::FRect& rcLayout);
		const Core::FRect& getLayoutRect(void) const;

		void setLayoutRectReal(const Core::FRect& rcLayout);
		const Core::FRect& getLayoutRectReal(void) const;

		/** 请在layout()开始前调用 */
		void beginLayout(void);
		/** 务必在layout()结束前调用 */
		void endLayout(void);

	protected:
		void layout(void);
		/** 对子控件设定布局区域，请使用setLayoutRect */
		virtual void layout(const Core::FRect& rcLayout);

		/** 给滚动条布局 */
		void layoutScrollBar(void);

		/** 计算最佳位置 */
		void adjustLayoutRect(Core::FPoint& position, Core::FSize& size, const Core::FRect& rcUsable) const;

	public:

		/** 
			更新相关
		*/
		/** 更新，如果rect为NULL，更新当前控件区域（延迟） */
		void repaint(const RECT* rect = NULL);
		/** 更新，如果rect为NULL，更新当前控件区域（立即） */
		void update(const RECT* rect = NULL);

	public:
		/** 重新布局并更新，如果rect为NULL，更新当前控件区域 */
		void relayoutpaint(const RECT* rect = NULL);
		void relayoutpaintParent(const RECT* rect = NULL);
		void updatelayoutpaint(const RECT* rect = NULL);
		void updatelayoutpaintParent(const RECT* rect = NULL);
		
	public:
		/** 
			窗口相关
		*/
		bool createWindow(HWND hParentWnd = NULL);
		bool createWindow(
			DWORD dwExStyle,
			LPCTSTR lpClassName,
			LPCTSTR lpWindowName,
			DWORD dwStyle,
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int nWidth = CW_USEDEFAULT,
			int nHeight = CW_USEDEFAULT,
			HWND hWndParent = NULL,
			HMENU hMenu = NULL,
			LPVOID lpCreateParams = NULL
			);

		bool destroyWindow(void);

		UINT doModal(HWND hWndParent = NULL);
		UINT doModaless(HWND hWndParent = NULL);
		void endModal(UINT uRetCode = IDCANCEL);

		/** 返回当前控件窗口句柄，如果不是窗口将返回NULL */
		HWND getHwnd(void) const;
		/** 返回一个有效的窗口句柄
			（如果当前不是窗口，往上级控件查找，直到找到窗口句柄）
		*/
		HWND getSafeHwnd(void) const;
		/** 返回具有真实窗口句柄的控件
			如果当前控件是窗口，返回当前控件
			否则向上查找直到找到窗口控件
		*/
		FWidget* getHostWindow(void) const;
		HWND getHostWindowHwnd(void) const;
		/** 返回顶层窗口句柄 */
		HWND getTopLevelHwnd(void) const;
		/** 如果是窗口返回true，否则返回false */
		bool isWindow(void) const;

	public:
		/** 控件拥有的ToolTip 
			* 注意：如果该控件为autodelete会在析构时删除
			* 如果同一个tooltip设置了多处，请不要设置为autodelete
			* 以免出错
		*/
		FToolTip* m_tooltip;
		FToolTip* getToolTip(void) const;
		void replaceToolTip(FToolTip* tooltip);

		static FToolTip* getGlobalToolTip(void);
		static bool replaceGlobalToolTip(FToolTip* tooltip);

	public:
		/**
			消息处理
		*/
		/** WM_NCCREATE */
		virtual void createMessage(HWND hWnd) { }
		/** WM_NCDESTROY */
		virtual void finalMessage(HWND hWnd) { }
		/** 消息处理程序 */
		virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) { return 0; }
		/** 默认的消息处理程序 */
		LRESULT defWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	protected:
		/** 窗口消息过程 */
		LRESULT processWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		LRESULT makeWidgetMessage(UINT uMsg, WPARAM wParam = 0L, LPARAM lParam = 0L);
		LRESULT makeWidgetMessage(FWidget* pSender, UINT nID, UINT uMsg, WPARAM wParam = 0L, LPARAM lParam = 0L);
		LRESULT sendWidgetMessage(FWidget* pSender, UINT nID, UINT uMsg, WPARAM wParam = 0L, LPARAM lParam = 0L);

	protected:
		/** 控件消息处理程序 */
		virtual LRESULT handleWidgetMessage(FWidget* pSender, UINT nID, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	protected:
		/** event处理 */
		bool event(Event::FEvent* e);

		virtual void paintEvent(Event::FPaintEvent* e);
		virtual void showEvent(Event::FEvent* e);
		virtual void hideEvent(Event::FEvent* e);
		virtual void changedEvent(Event::FChangedEvent* e);
		virtual void layoutChangedEvent(Event::FLayoutChangedEvent* e);
		virtual void mousePressEvent(Event::FMouseEvent* e);
		virtual void mouseReleaseEvent(Event::FMouseEvent* e);
		virtual void mouseDoubleClickEvent(Event::FMouseEvent* e);
		virtual void mouseClickEvent(Event::FMouseEvent* e);
		virtual void mouseMoveEvent(Event::FMouseEvent* e);
		virtual void mouseWheelEvent(Event::FWheelEvent* e);
		virtual void mouseEnterEvent(Event::FEvent* e);
		virtual void mouseLeaveEvent(Event::FEvent* e);
		virtual void keyPressEvent(Event::FKeyEvent* e);
		virtual void keyReleaseEvent(Event::FKeyEvent* e);
		virtual void keyCharEvent(Event::FCharEvent* e);
		virtual void reflectEvent(Event::FReflectEvent* e);
		virtual void cursorEvent(Event::FCursorEvent* e);
		virtual void parentWindowCreatedEvent(Event::FParentWindowEvent* e);
		virtual void parentWindowSizedEvent(Event::FParentWindowEvent* e);
		virtual void windowPreCreateEvent(Event::FWindowPreCreateEvent* e);
		virtual void windowCreatedEvent(Event::FWindowCreatedEvent* e);
		virtual void customEvent(Event::FCustomEvent* e);

	protected:
		virtual void defaultPaint(Event::FPaintEvent* e) { }

	protected:
		/** markup */
		bool beginMarkup(Markup::FMarkup* pMarkup);
		bool setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue);
		bool endMarkup(Markup::FMarkup* pMarkup);

	public:
		virtual bool markupAddWidget(Markup::FMarkup* pMarkup, FWidget* pWidget);

	public:
		/** 当xml解析器创建控件时， 会先调用该函数 */
		virtual FWidget* createChildWidget(LPCTSTR pszName) const { return NULL; }
		static FWidget* createWidget(LPCTSTR pszName);

	protected:
		/** 控件名称和ID */
		Core::FString m_strName;
		int m_nID;

		/** 控件组 */
		Core::FString m_strGroup;

		/** 控件文本 */
		Core::FString m_strText;
		/** Tab索引 */
		int m_tabIndex;
		/** 控件属性 */
		DWORD m_dwFlags;
		/** 控件状态 */
		DWORD m_dwState;
		/** 停靠方式 */
		DWORD m_dwDock;
		/** 对齐方式 */
		DWORD m_dwAlign;
		/** 提示文本 */
		Core::FString m_strToolTip;
		/** 控件角色 */
		DWORD m_dwRole;
		/** 外边距 */
		Core::FEdge m_egMargin;
		/** 内边距 */
		Core::FEdge m_egPadding;
		/** 控件位置 */
		Core::FPoint m_position;
		bool m_bRelativePos;
		bool m_bFloatPos;
		bool m_bIsFixedXPosition;
		bool m_bIsFixedYPosition;

		/** 控件大小 */
		Core::FSize m_size;
		bool m_bIsFixedWidth;
		bool m_bIsFixedHeight;

		Core::FSize m_minsize;
		Core::FSize m_maxsize;

		/** 滚动条 */
		FScrollBar* m_pHScrollBar;
		FScrollBar* m_pVScrollBar;

		/** 资源管理 */
		Markup::FResourceManager m_resourceMgr;

		LPVOID m_pUserData;
		Core::FString m_strUserTag;

	protected:
		/** 鼠标样式 */
		Markup::FCursorResource m_cursor;
		/** 样式表 */
		Markup::FStyleSheetResource m_stylesheet;

	public:
		//////////////////////////////////////////////////////////////////////////
		// FWidgetLock
		/** 构造时调用：FWidget::lockChildren()
			析构时调用：FWidget::unlockChildren()
		*/
		class FOOUI_LIB FWidgetLock
		{
		public:
			explicit FWidgetLock(FWidget* widget);
			virtual ~FWidgetLock(void);

		private:
			FWidget* m_pWidget;
		};

	private:
		FWidgetPrivate* const m_pWidgetPrivate;
	};

}} // !namespace

#endif // !FWIDGET_H
