/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FWidgetPrivate_p.h
* ������		FWidgetPrivate_p
* �������ڣ�	2013-06-06
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
	// FWidgetPrivate�� ��һЩ�������ⲿ�����ķ�����װ������
	class FWidgetPrivate
	{
		friend FWindowPrivate;
		friend FWidget;
	public:
		explicit FWidgetPrivate(FWidget* pWidget);
		~FWidgetPrivate(void);

	public:
		/** 
			�ӿؼ�����
		*/
		/** ��pWidget���뵽��󣬳ɹ�����true��ʧ�ܷ���false */
		bool addChild(FWidget* pWidget);
		/** ��pWidget���뵽nIndexλ�ã��ɹ�����true��ʧ�ܷ���false */
		bool insertChild(int nIndex, FWidget* pWidget);
		/** �Ƴ�һ��Widget���ɹ�����true��ʧ�ܷ���false */
		bool removeChild(int nIndex);
		bool removeChild(FWidget* pWidget);
		bool removeChildAndNoDel(int nIndex);
		bool removeChildAndNoDel(FWidget* pWidget);
		/** �������Widget���ɹ�����true��ʧ�ܷ���false */
		bool clearChildren(void);
		/** ��ȡWidget���� */
		FWidget* getChild(int nIndex) const;
		/** ��ȡWidget���� */
		int indexOfChild(FWidget* widget) const;
		/** ��ȡWidget���� */
		int getChildrenCount(void) const;

		/** �����ӿؼ��Ĳ��롢ɾ������ */
		void lockChildren(void);
		void unlockChildren(void);

		/** ���ݿؼ������ҿؼ� */
		FWidget* findWidget(LPCTSTR pszName) const;
		FWidget* findNextWidget(FWidget* pCurWidget, LPCTSTR pszName) const;

		/** ���ݿؼ�ID���ҿؼ� */
		FWidget* findWidget(int nID) const;
		FWidget* findNextWidget(FWidget* pCurWidget, int nID) const;

		/** ��ʼ�����ؼ������toplevelΪNULL��parent��������Ϊ�����е�parent */
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

		/** ����Ƿֲ㴰�ڷ���true */
		bool isLayeredWindow(void) const;

	protected:
		/**
			��Ϣ����
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

		/** ���ÿؼ���С*/
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
		/** ���ò���λ�� �ɼ����������ڴ��ڵĸ��²��� -- 2015/4/21 by hyh*/
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

		/** �㲥Paint�¼����ӿؼ���������ӿؼ���Ҫ�ػ棩 */
		void broadcastPaintEvent(Event::FPaintEvent* e);

		/** �㲥�¼��������ӿؼ� 
			e: �㲥���¼�
			broadSelf�� �㲥ǰ�Ƿ񴫵ݸ��Լ�һ��
		*/
		void broadcastEvent(Event::FEvent* e, bool broadSelf = false);
		
		void reflectMouseEvent(Event::FEvent* e);
		void reflectKeyboardEvent(Event::FEvent* e);

	public:
		/** ������Widget���� */
		FWidget* const m_pWidget;

		/** �ӿؼ��б� */
		Core::FPtrList m_children;
		Core::FCriticalSectionLock m_lock;

		/** ����ؼ��븸�ؼ� */
		FWidget* m_pTopLevelWidget;
		FWidget* m_pParentWidget;

		/** ����Ƿ���Ҫ���²��� */
		bool m_bNeedLayout;
		/** ����λ�� */
		Core::FRect m_rcLayout;

		/** ����λ�� �ɼ������� ���ڴ��ڵĸ��²��� -- 2015/4/21 by hyh*/
		Core::FRect m_rcLayoutReal;

		/** ����Ƿ���Ҫ���� */
		bool m_bNeedUpdate;

		/** doModal�ķ���ֵ */
		UINT m_uRetCode;
		bool m_bExitModalLoop;

		/** һЩ���ڵ�����д������ */
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
			bool border;	/** �����Ƿ���б߿� */
			bool center;	/** ���ڴ���ʱ�Ƿ������ʾ */
			Core::FEdge windowncspace; /** ���ڵĿ�����߾� */
		};
		FWindowPrivateStyle m_wndstyle;
		void parseWindowStyle(LPCTSTR pszValue);

		/** ������ֻ����windowncspace��
			������false��ʾû�д���windowncspace
		*/
		bool hittest(const Core::FRect& rcWindow, const Core::FPoint& ptMouse, LRESULT& lResult);

	public:
		/** �����˵�ǰ���㡢��������ѹ�ؼ� */
		static FWidget* fFocusedWidget;
		static FWidget* fHoverWidget;
		static FWidget* fPushedWidget;
		static FWidget* fActiveWidget;

	protected:
		/** ά��������Tab˳���� 
		*/
		/** ���ؼ�����Ӧ�����øú�����Tab˳������ɾ�� */
		static bool removeTabWidget(FWidget* widget);
		/** �ı�widget��tab��������������в���������� */
		static bool setWidgetTabIndex(int nTabIndex, FWidget* widget);
		/** ��ȡ��һ��Tab˳�����Ŀؼ������widgetΪNULL�����ص�һ��
			����б�Ϊ���򷵻�NULL
			�����!isVisible()��isSkipFocus()��isSkipTab()�Ŀؼ�
		*/
		static FWidget* getNextTabWidget(FWidget* widget = NULL);
		/** �������true��widget��һ����Ч��tab�ؼ� */
		static bool isValidTabWidget(FWidget* widget);

	protected:
		/** ���ڶ��� */
		FWindowPrivate* m_pWindow;
	};

}} // !namespace

#endif // !FWidgetPrivate_P_H