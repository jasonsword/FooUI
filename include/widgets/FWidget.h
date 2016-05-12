/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FWidget.h
* ������		FWidget
* �������ڣ�	2013-06-06
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
			�ؼ��������
		*/
		/** ����/����name��������Ψһ�� */
		void setName(LPCTSTR pszName);
		LPCTSTR getName(void) const;

		/** ����/����id��������Ψһ�� */
		void setID(int nID);
		int getID(void) const;

		/** �ؼ��� */
		void setGroup(LPCTSTR pszGroup);
		LPCTSTR getGroup(void) const;

		/** ����/����text */
		void setText(LPCTSTR pszText);
		LPCTSTR getText(void) const;

		/** ����/����Tab���� */
		void setTabIndex(int tabIndex);
		int getTabIndex(void) const;

		/** ����/����Attributes */
		void setFlags(DWORD dwAttribute);
		void modifyFlag(DWORD dwRemove, DWORD dwAdd);
		bool testFlag(DWORD dwAttribute) const;
		DWORD getFlags(void) const;

		/** ����/����״̬ */
		void setState(DWORD dwState, bool bRepaint = false);
		bool modifyState(DWORD dwRemove, DWORD dwAdd, bool bRepaint = false);
		bool testState(DWORD dwState) const;
		DWORD getState(void) const;

		/** ����/����Dock */
		void setDock(DWORD dwDock);
		DWORD getDock(void) const;

		/** ����/����Align */
		void setAlign(DWORD dwAlign);
		bool testAlign(DWORD dwAlign) const;
		DWORD getAlign(void) const;

		/** ����/���ÿؼ���ʾ�ı� */
		void setToolTipText(LPCTSTR pszToolTip);
		LPCTSTR getToolTipText(void) const;

		/** ����/���ÿؼ�Role */
		void setRole(DWORD dwRole);
		DWORD getRole(void) const;

		/** ��ȡ��Ч�Ĳ�������
			�ų��ڱ߾༰������
		*/
		void getValidLayoutRect(RECT* lpRect) const;

		/** ����/������߾� */
		void setMargin(const Core::FEdge& egMargin);
		void setMargin(int xLeft, int yTop, int xRight, int yBottom);
		const Core::FEdge& getMargin(void) const;

		/** ����/�����ڱ߾� */
		void setPadding(const Core::FEdge& egPadding);
		void setPadding(int xLeft, int yTop, int xRight, int yBottom);
		const Core::FEdge& getPadding(void) const;

		/** ����/���ÿؼ�λ�� */
		void setLeft(int nLeft);
		void setTop(int nTop);
		int getLeft(void) const;
		int getTop(void) const;
		void setPosition(const Core::FPoint& position);
		void setPosition(int xLeft, int yTop);
		/** ���ˮƽ�ʹ�ֱ���꣨���Զ������� */
		void emptyXPosition(void);
		void emptyYPosition(void);
		void emptyPosition(void);
		const Core::FPoint& getPosition(void) const;
		/** ����/���þ���λ�� */
		void relativePosition(bool bRelative);
		bool isRelativePosition(void) const;
		/** ����/���ø���λ�� */
		void floatPosition(bool bFloat);
		bool isFloatPosition(void) const;
		/** ����/�����Ƿ�̶�ˮƽ��ֱ���꣨������̶����Զ�����) */
		bool isFixedXPosition(void) const;
		bool isFixedYPosition(void) const;

		/** �������ÿؼ���С */
		void setWidth(int nWidth);
		void setHeight(int nHeight);
		int getWidth(void) const;
		int getHeight(void) const;
		void setSize(Core::FSize& size);
		void setSize(int cx, int cy);
		/** ��տ�Ⱥ͸߶ȣ����Զ������� */
		void emptyWidth(void);
		void emptyHeight(void);
		void emptySize(void);
		const Core::FSize& getSize(void) const;
		/** ����/�����Ƿ�̶���Ȼ�߶ȣ�������̶����Զ������� */
		bool isFixedWidth(void) const;
		bool isFixedHeight(void) const;

		/** �������ÿؼ���С��С */
		void setMinWidth(int nMinWidth);
		void setMinHeight(int nMinHeight);
		int getMinWidth(void) const;
		int getMinHeight(void) const;
		void setMinSize(Core::FSize& size);
		void setMinSize(int cx, int cy);
		void clearMinSize(void);
		const Core::FSize& getMinSize(void) const;

		/** �������ÿؼ�����С */
		void setMaxWidth(int nMaxWidth);
		void setMaxHeight(int nMaxHeight);
		int getMaxWidth(void) const;
		int getMaxHeight(void) const;
		void setMaxSize(Core::FSize& size);
		void setMaxSize(int cx, int cy);
		void clearMaxSize(void);
		const Core::FSize& getMaxSize(void) const;

		/** ����/������ʽ�� */
		void setStyle(const Markup::FStyleSheetResource& stylesheet);
		bool setStyle(LPCTSTR pszName);
		const Markup::FStyleSheetResource& getStyle(void) const;
		/** ����Ƿ����dwState1��dwState2״̬�ķ�� 
			��״̬�ı�ʱ�� ���ⲻ��Ҫ���ػ�
			��ʹ���˶������ʽ�������ظú���
			�������false����ʹ״̬�ı�Ҳ�������ػ档
		*/
		/** ����Ƿ����dwState����ʽ���ػ�ǰ��飬���������ػ棩 */
		virtual bool testStyle(DWORD dwState, LPCTSTR pszType = NULL, LPCTSTR pszPart = NULL) const;

		/** ����/������Դ������ */
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
			�ӿؼ�����
		*/
		/** ��pWidget���뵽��󣬳ɹ�����true��ʧ�ܷ���false */
		bool addChild(FWidget* pWidget);
		/** ��pWidget���뵽nIndexλ�ã��ɹ�����true��ʧ�ܷ���false */
		bool insertChild(int nIndex, FWidget* pWidget);
		/** �Ƴ�һ��Widget���ɹ�����true��ʧ�ܷ���false 
			*ע�⣬�κ��Ƴ�������������ɶ�������
		*/
		bool removeChild(int nIndex);
		bool removeChild(FWidget* pWidget);
		/** �������Widget���ɹ�����true��ʧ�ܷ���false */
		bool clearChildren(void);
		/** ��ȡWidget���� */
		FWidget* getChild(int nIndex) const;
		/** ��ȡ�˿ؼ��ڸ��ؼ���˳������ */
		int childIndex(void) const;
		/** ��ȡWidget��parent�е�˳������ */
		int indexOfChild(FWidget* widget) const;
		/** ��ȡWidget���� */
		int getChildrenCount(void) const;
		bool hasChildren(void) const;

		/** ���ݿؼ������ҿؼ� */
		FWidget* findWidget(LPCTSTR pszName) const;
		FWidget* findNextWidget(FWidget* pCurWidget, LPCTSTR pszName) const;

		/** ���ݿؼ�ID���ҿؼ� */
		FWidget* findWidget(int nID) const;
		FWidget* findNextWidget(FWidget* pCurWidget, int nID) const;

		/** ��ȡ��һ��Tab�ؼ� */
		FWidget* getNextTabWidget(void) const;
		FWidget* getNextTabWidget(FWidget* widget) const;

		/** �����ӿؼ��Ĳ��롢ɾ������ */
		void lockChildren(void);
		void unlockChildren(void);

	public:
		/** ��ȡ����µĿؼ� */
		virtual FWidget* widgetFromPoint(int xPos, int yPos) const;

		FOOINLINE FWidget* widgetFromPoint(const POINT point) const
		{ return widgetFromPoint(point.x, point.y); }

		FOOINLINE FWidget* findWidget(int xPos, int yPos) const
		{ return widgetFromPoint(xPos, yPos); }

		FOOINLINE FWidget* findWidget(const POINT& point) const
		{ return widgetFromPoint(point.x, point.y); }

	public:
		/** ��ȡ����µĹ�����ͼ */
		virtual FWidget* scrollViewFromPoint(int xPos, int yPos) const;
		/** ����true�����ǹ�����ͼ */
		virtual bool isScrollView(void) const;

		FOOINLINE FWidget* scrollViewFromPoint(const POINT point) const
		{ return scrollViewFromPoint(point.x, point.y); }

		FOOINLINE FWidget* findScrollView(const POINT point) const
		{ return scrollViewFromPoint(point.x, point.y); }

		FOOINLINE FWidget* findScrollView(int xPos, int yPos) const
		{ return scrollViewFromPoint(xPos, yPos); }

	public:
		/** 
			�ؼ��㴦�� 
		*/
		void setParent(FWidget* parent);
		/** ����ӵ���ߣ���setParent�������ǲ�����뵽���ؼ����ӿؼ��б��У� */
		void setOwner(FWidget* parent);
		FWidget* getParent(void) const;
		FWidget* getTopLevelParent(void) const;

		/** ���ؼ�ǰһ��ͬ���ؼ� */
		FWidget* prevSibling(void) const;
		/** ���ؼ���һ��ͬ���ؼ� */
		FWidget* nextSibling (void) const;

	public:
		/** �ؼ����� */
		void show(void);
		void hide(void);
		void center(FWidget* pWidget = NULL);
		void enable(bool bEnable = true);
		bool isEnabled(void) const;
		void focus(void);
		bool isFocused(void) const;

	public:
		/** ״̬���� */
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
			�������
		*/
		/** ���²��֣��ӳ٣� */
		void relayout(void);
		/** ���²��֣������� */
		void updateLayout(void);
		/** �ø��ؼ����²��� */
		void relayoutParent(void);
		void updateLayoutParent(void);

		/** ���������� 
			wBarȡֵ��SB_BOTH��SB_CTL��SB_HORZ��SB_VERT
			bShowָʾ�������Ƿ�������ʾ
			��������ɹ����Ѿ����ڹ���������true
			���򷵻�false
		*/
		bool createScrollBar(int wBar, bool bShow = false);

		/** ��ʾ������
			wBarȡֵ��SB_BOTH��SB_CTL��SB_HORZ��SB_VERT
			bShowΪtrue��ʾ�������� Ϊfalse���ع�����
			��������ڹ���������false
			���򷵻�true
		*/
		bool showScrollBar(int wBar, bool bShow);
		bool isShowScrollBar(int wBar) const;
		FScrollBar* getScrollBar(int wBar) const;

	public:
		/** ����/���ز���λ�� */
		/** ����setLayoutRect()����������layout()��
			�����Ҫ���²��������relayout()
		*/
		void setLayoutRect(const Core::FRect& rcLayout);
		const Core::FRect& getLayoutRect(void) const;

		void setLayoutRectReal(const Core::FRect& rcLayout);
		const Core::FRect& getLayoutRectReal(void) const;

		/** ����layout()��ʼǰ���� */
		void beginLayout(void);
		/** �����layout()����ǰ���� */
		void endLayout(void);

	protected:
		void layout(void);
		/** ���ӿؼ��趨����������ʹ��setLayoutRect */
		virtual void layout(const Core::FRect& rcLayout);

		/** ������������ */
		void layoutScrollBar(void);

		/** �������λ�� */
		void adjustLayoutRect(Core::FPoint& position, Core::FSize& size, const Core::FRect& rcUsable) const;

	public:

		/** 
			�������
		*/
		/** ���£����rectΪNULL�����µ�ǰ�ؼ������ӳ٣� */
		void repaint(const RECT* rect = NULL);
		/** ���£����rectΪNULL�����µ�ǰ�ؼ����������� */
		void update(const RECT* rect = NULL);

	public:
		/** ���²��ֲ����£����rectΪNULL�����µ�ǰ�ؼ����� */
		void relayoutpaint(const RECT* rect = NULL);
		void relayoutpaintParent(const RECT* rect = NULL);
		void updatelayoutpaint(const RECT* rect = NULL);
		void updatelayoutpaintParent(const RECT* rect = NULL);
		
	public:
		/** 
			�������
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

		/** ���ص�ǰ�ؼ����ھ����������Ǵ��ڽ�����NULL */
		HWND getHwnd(void) const;
		/** ����һ����Ч�Ĵ��ھ��
			�������ǰ���Ǵ��ڣ����ϼ��ؼ����ң�ֱ���ҵ����ھ����
		*/
		HWND getSafeHwnd(void) const;
		/** ���ؾ�����ʵ���ھ���Ŀؼ�
			�����ǰ�ؼ��Ǵ��ڣ����ص�ǰ�ؼ�
			�������ϲ���ֱ���ҵ����ڿؼ�
		*/
		FWidget* getHostWindow(void) const;
		HWND getHostWindowHwnd(void) const;
		/** ���ض��㴰�ھ�� */
		HWND getTopLevelHwnd(void) const;
		/** ����Ǵ��ڷ���true�����򷵻�false */
		bool isWindow(void) const;

	public:
		/** �ؼ�ӵ�е�ToolTip 
			* ע�⣺����ÿؼ�Ϊautodelete��������ʱɾ��
			* ���ͬһ��tooltip�����˶ദ���벻Ҫ����Ϊautodelete
			* �������
		*/
		FToolTip* m_tooltip;
		FToolTip* getToolTip(void) const;
		void replaceToolTip(FToolTip* tooltip);

		static FToolTip* getGlobalToolTip(void);
		static bool replaceGlobalToolTip(FToolTip* tooltip);

	public:
		/**
			��Ϣ����
		*/
		/** WM_NCCREATE */
		virtual void createMessage(HWND hWnd) { }
		/** WM_NCDESTROY */
		virtual void finalMessage(HWND hWnd) { }
		/** ��Ϣ������� */
		virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) { return 0; }
		/** Ĭ�ϵ���Ϣ������� */
		LRESULT defWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	protected:
		/** ������Ϣ���� */
		LRESULT processWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		LRESULT makeWidgetMessage(UINT uMsg, WPARAM wParam = 0L, LPARAM lParam = 0L);
		LRESULT makeWidgetMessage(FWidget* pSender, UINT nID, UINT uMsg, WPARAM wParam = 0L, LPARAM lParam = 0L);
		LRESULT sendWidgetMessage(FWidget* pSender, UINT nID, UINT uMsg, WPARAM wParam = 0L, LPARAM lParam = 0L);

	protected:
		/** �ؼ���Ϣ������� */
		virtual LRESULT handleWidgetMessage(FWidget* pSender, UINT nID, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	protected:
		/** event���� */
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
		/** ��xml�����������ؼ�ʱ�� ���ȵ��øú��� */
		virtual FWidget* createChildWidget(LPCTSTR pszName) const { return NULL; }
		static FWidget* createWidget(LPCTSTR pszName);

	protected:
		/** �ؼ����ƺ�ID */
		Core::FString m_strName;
		int m_nID;

		/** �ؼ��� */
		Core::FString m_strGroup;

		/** �ؼ��ı� */
		Core::FString m_strText;
		/** Tab���� */
		int m_tabIndex;
		/** �ؼ����� */
		DWORD m_dwFlags;
		/** �ؼ�״̬ */
		DWORD m_dwState;
		/** ͣ����ʽ */
		DWORD m_dwDock;
		/** ���뷽ʽ */
		DWORD m_dwAlign;
		/** ��ʾ�ı� */
		Core::FString m_strToolTip;
		/** �ؼ���ɫ */
		DWORD m_dwRole;
		/** ��߾� */
		Core::FEdge m_egMargin;
		/** �ڱ߾� */
		Core::FEdge m_egPadding;
		/** �ؼ�λ�� */
		Core::FPoint m_position;
		bool m_bRelativePos;
		bool m_bFloatPos;
		bool m_bIsFixedXPosition;
		bool m_bIsFixedYPosition;

		/** �ؼ���С */
		Core::FSize m_size;
		bool m_bIsFixedWidth;
		bool m_bIsFixedHeight;

		Core::FSize m_minsize;
		Core::FSize m_maxsize;

		/** ������ */
		FScrollBar* m_pHScrollBar;
		FScrollBar* m_pVScrollBar;

		/** ��Դ���� */
		Markup::FResourceManager m_resourceMgr;

		LPVOID m_pUserData;
		Core::FString m_strUserTag;

	protected:
		/** �����ʽ */
		Markup::FCursorResource m_cursor;
		/** ��ʽ�� */
		Markup::FStyleSheetResource m_stylesheet;

	public:
		//////////////////////////////////////////////////////////////////////////
		// FWidgetLock
		/** ����ʱ���ã�FWidget::lockChildren()
			����ʱ���ã�FWidget::unlockChildren()
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
