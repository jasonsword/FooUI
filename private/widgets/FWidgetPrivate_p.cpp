/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FWidgetPrivate_p.cpp
* 描述：		FWidgetPrivate_p
* 创建日期：	2013-06-06
*
*/

#include "StdAfx.h"

#include "core/FApplication.h"
#include "widgets/FWidgetPrivate_p.h"
#include "widgets/FWindowPrivate_p.h"
#include "widgets/FScrollBar.h"

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FWidgetPrivate
	FWidgetPrivate::FWidgetPrivate(FWidget* pWidget) : 
		m_pWidget(pWidget) 
		, m_pTopLevelWidget(NULL) 
		, m_pParentWidget(NULL) 
		, m_bNeedLayout(false) 
		, m_bNeedUpdate(false) 
		, m_pWindow(NULL) 
		, m_uRetCode(IDCANCEL) 
		, m_bExitModalLoop(false) 
	{
		m_rcLayoutReal.setEmpty();
	}
	

	FWidgetPrivate::~FWidgetPrivate(void)
	{
		fooSafeDelete(m_pWindow);

		clearChildren();
	}

	/** 
		子控件管理
	*/
	bool FWidgetPrivate::addChild(FWidget* pWidget)
	{
		FASSERT(NULL != m_pWidget);
		FASSERT(NULL != pWidget);

		Core::FAutoLock autolock(&m_lock);
		if (!m_pWidget->isContainer()) return false;

		if (m_children.add((LPVOID)pWidget))
		{
			m_pWidget->relayoutpaint();
			return true;
		}

		return false;
	}

	bool FWidgetPrivate::insertChild(int nIndex, FWidget* pWidget)
	{
		FASSERT(NULL != m_pWidget);
		FASSERT(NULL != pWidget);

		Core::FAutoLock autolock(&m_lock);
		if (!m_pWidget->isContainer()) return false;

		if (m_children.insert(nIndex, (LPVOID)pWidget))
		{
			m_pWidget->relayoutpaint();
			return true;
		}

		return false;
	}

	bool FWidgetPrivate::removeChild(int nIndex)
	{
		FASSERT(NULL != m_pWidget);
		if (removeChild((FWidget*)m_children.get(nIndex)))
		{
			m_pWidget->relayoutpaint();
			return true;
		}

		return false;
	}

	bool FWidgetPrivate::removeChild(FWidget* pWidget)
	{
		FASSERT(NULL != m_pWidget);
		FASSERT(NULL != pWidget);
		Core::FAutoLock autolock(&m_lock);

		if (pWidget->getParent() == m_pWidget)
		{
			bool bRet = m_children.remove((LPVOID)pWidget);
			if (bRet)
			{
				if (pWidget->isAutoDelete())
				{
					fooSafeDelete(pWidget);
				}

				m_pWidget->relayoutpaint();
				return true;
			}
		}
		else if (NULL != pWidget->getParent())
		{
			return pWidget->getParent()->removeChild(pWidget);
		}

		return false;
	}

	bool FWidgetPrivate::removeChildAndNoDel(int nIndex)
	{
		FASSERT(NULL != m_pWidget);
		Core::FAutoLock autolock(&m_lock);
		if (m_children.remove(nIndex))
		{
			m_pWidget->relayoutpaint();
			return true;
		}

		return false;
	}

	bool FWidgetPrivate::removeChildAndNoDel(FWidget* pWidget)
	{
		FASSERT(NULL != m_pWidget);
		Core::FAutoLock autolock(&m_lock);
		if (m_children.remove((LPVOID)pWidget))
		{
			m_pWidget->relayoutpaint();
			return true;
		}

		return false;
	}

	bool FWidgetPrivate::clearChildren(void)
	{
		FASSERT(NULL != m_pWidget);
		Core::FAutoLock autolock(&m_lock);

		for (int i = 0; i < m_children.size(); ++i)
		{
			FWidget *pWidget = FOBJECT_CAST(FWidget*, m_children.get(i));
			if (pWidget->isAutoDelete())
			{
				fooSafeDelete(pWidget);
			}
		}

		if (m_children.clear())
		{
			m_pWidget->relayoutpaint();
			return true;
		}

		return false;
	}

	FWidget* FWidgetPrivate::getChild(int nIndex) const
	{
		FASSERT(NULL != m_pWidget);
		Core::FAutoLock autolock(const_cast<Core::FCriticalSectionLock*>(&m_lock));
		return FOBJECT_CAST(FWidget*, m_children.get(nIndex));
	}

	int FWidgetPrivate::indexOfChild(FWidget* widget) const
	{
		FASSERT(NULL != m_pWidget);
		Core::FAutoLock autolock(const_cast<Core::FCriticalSectionLock*>(&m_lock));
		for (int i = 0; i < m_children.size(); ++i)
		{
			if (m_children.get(i) == widget)
			{
				return i;
			}
		}

		return -1;
	}

	int FWidgetPrivate::getChildrenCount(void) const
	{
		FASSERT(NULL != m_pWidget);
		Core::FAutoLock autolock(const_cast<Core::FCriticalSectionLock*>(&m_lock));
		return m_children.size();
	}

	void FWidgetPrivate::lockChildren(void)
	{
		m_lock.lock();
	}

	void FWidgetPrivate::unlockChildren(void)
	{
		m_lock.unlock();
	}

	FWidget* FWidgetPrivate::findWidget(LPCTSTR pszName) const
	{
		FASSERT(NULL != m_pWidget);
		FASSERT(NULL != pszName);

		Core::FAutoLock autolock(const_cast<Core::FCriticalSectionLock*>(&m_lock));

		for (int i = 0; i < getChildrenCount(); ++i)
		{
			FWidget* pWidget = getChild(i);
			if (_tcscmp(pWidget->getName(), pszName) == 0)
			{
				return pWidget;
			}

			pWidget = pWidget->findWidget(pszName);
			if (NULL != pWidget) return pWidget;
		}

		return NULL;
	}

	FWidget* FWidgetPrivate::findNextWidget(FWidget* pCurWidget, LPCTSTR pszName) const
	{
		FASSERT(NULL != m_pWidget);
		FASSERT(NULL != pCurWidget);
		FASSERT(NULL != pszName);

		Core::FAutoLock autolock(const_cast<Core::FCriticalSectionLock*>(&m_lock));

		FWidget *pParent = pCurWidget->getParent();

		/** 不是本控件子控件 */
		if (pParent != m_pWidget)
		{
			return pParent->findNextWidget(pCurWidget, pszName);
		}

		/** 查找控件位置 */
		Core::FPtrList::POSITION position = m_children.find(pCurWidget);
		if (NULL == position)
		{
			return NULL;
		}

		/** 一级一级查找 */
		position = m_children.nextPosition(position);
		while (NULL != position)
		{
			FWidget *pWidget = (FWidget*)m_children.getAtPosition(position);

			if (_tcscmp(pWidget->getName(), pszName) == 0)
			{
				return pWidget;
			}

			pWidget = pWidget->findWidget(pszName);
			if (NULL != pWidget) return pWidget;

			position = m_children.nextPosition(position);
		}

		/** 从上级开始继续查找 */
		if (NULL != pParent->getParent())
		{
			return pParent->getParent()->findNextWidget(pParent, pszName);
		}

		return NULL;
	}

	FWidget* FWidgetPrivate::findWidget(int nID) const
	{
		FASSERT(NULL != m_pWidget);

		Core::FAutoLock autolock(const_cast<Core::FCriticalSectionLock*>(&m_lock));

		for (int i = 0; i < getChildrenCount(); ++i)
		{
			FWidget* pWidget = getChild(i);
			if (pWidget->getID() == nID)
			{
				return pWidget;
			}

			pWidget = pWidget->findWidget(nID);
			if (NULL != pWidget) return pWidget;
		}

		return NULL;
	}

	FWidget* FWidgetPrivate::findNextWidget(FWidget* pCurWidget, int nID) const
	{
		FASSERT(NULL != m_pWidget);
		FASSERT(NULL != pCurWidget);

		Core::FAutoLock autolock(const_cast<Core::FCriticalSectionLock*>(&m_lock));

		FWidget *pParent = pCurWidget->getParent();

		/** 不是本控件子控件 */
		if (pParent != m_pWidget)
		{
			return pParent->findNextWidget(pCurWidget, nID);
		}

		/** 查找控件位置 */
		Core::FPtrList::POSITION position = m_children.find(pCurWidget);
		if (NULL == position)
		{
			return NULL;
		}

		/** 一级一级查找 */
		position = m_children.nextPosition(position);
		while (NULL != position)
		{
			FWidget *pWidget = (FWidget*)m_children.getAtPosition(position);

			if (pWidget->getID() == nID)
			{
				return pWidget;
			}

			pWidget = pWidget->findWidget(nID);
			if (NULL != pWidget) return pWidget;

			position = m_children.nextPosition(position);
		}

		/** 从上级开始继续查找 */
		if (NULL != pParent->getParent())
		{
			return pParent->getParent()->findNextWidget(pParent, nID);
		}

		return NULL;
	}

	bool FWidgetPrivate::initParent(FWidget* parent, FWidget* toplevel)
	{
		m_pParentWidget = parent;
		m_pTopLevelWidget = (NULL == toplevel) ? parent : toplevel;

		return true;
	}

	bool FWidgetPrivate::createWindow(HWND hParentWnd/* = NULL*/)
	{
		FASSERT(NULL != m_pWidget);
		if (NULL == m_pWindow) m_pWindow = new FWindowPrivate(this);

		Event::FWindowPreCreateEvent e;

		e.wci.classinfo.style |= CS_DBLCLKS;
		e.wci.wndinfo.dwStyle = m_wndstyle.dwStyle;
		e.wci.wndinfo.dwExStyle = m_wndstyle.dwExStyle;
		e.wci.wndinfo.x = m_pWidget->isFixedXPosition() ? m_pWidget->getLeft() : CW_USEDEFAULT;
		e.wci.wndinfo.y = m_pWidget->isFixedYPosition() ? m_pWidget->getTop() : CW_USEDEFAULT;
		e.wci.wndinfo.cx = m_pWidget->isFixedWidth() ? m_pWidget->getWidth() : CW_USEDEFAULT;
		e.wci.wndinfo.cy = m_pWidget->isFixedHeight() ? m_pWidget->getHeight() : CW_USEDEFAULT;

		e.wci.wndinfo.pszWindowText = m_pWidget->m_strText;
		e.wci.wndinfo.hwndParent = hParentWnd;

		m_pWidget->event(&e);

		return m_pWindow->create(e.wci);
	}

	bool FWidgetPrivate::createWindow(
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
									)
	{
		FASSERT(NULL != m_pWidget);

		if (NULL == m_pWindow) m_pWindow = new FWindowPrivate(this);

		Event::FWindowPreCreateEvent e;
		e.wci.classinfo.style |= CS_DBLCLKS;
		e.wci.wndinfo.dwExStyle = dwExStyle;
		e.wci.lpszClassName = lpClassName;
		e.wci.wndinfo.dwExStyle = dwExStyle;
		e.wci.wndinfo.pszWindowText = lpWindowName;
		e.wci.wndinfo.dwStyle = dwStyle;
		e.wci.wndinfo.x = x;
		e.wci.wndinfo.y = y;
		e.wci.wndinfo.cx = nWidth;
		e.wci.wndinfo.cy = nHeight;
		e.wci.wndinfo.hwndParent = hWndParent;
		e.wci.wndinfo.hMenu = hMenu;
		e.wci.wndinfo.lpCreateParams = lpCreateParams;

		m_pWidget->event(&e);

		return m_pWindow->create(e.wci);
	}

	bool FWidgetPrivate::destroyWindow(void)
	{
		if (NULL != m_pWindow)
		{
			if (NULL != m_pWindow->getHwnd() 
				&& ::IsWindow(m_pWindow->getHwnd()))
			{
				::DestroyWindow(m_pWindow->getHwnd());
				delete m_pWindow;
				m_pWindow = NULL;
			}

			return true;
		}

		return false;
	}

	UINT FWidgetPrivate::doModal(HWND hWndParent/* = NULL*/, bool bModaless/* = false*/)
	{
		bool bEnableParent = false;

		if (isWindow())
		{
			if (NULL == hWndParent) hWndParent = ::GetWindow(getHwnd(), GW_OWNER);
		}
		else
		{
			if (NULL == hWndParent) hWndParent = ::GetActiveWindow();

			/** 创建窗口 */
			if ( !createWindow(hWndParent))
			{
				/** 创建失败 */
				return IDCANCEL;
			}
		}

		HWND hWndLastActive = NULL;
		if (NULL != hWndParent)
		{
			hWndLastActive = ::SetActiveWindow(hWndParent);
			::ShowWindow(getHwnd(), SW_SHOW);
		}
		else
		{
			::ShowWindow(getHwnd(), SW_SHOW);
			::SetActiveWindow(getHwnd());
			::SetForegroundWindow(getHwnd());
		}

		if (!bModaless 
			&& NULL != hWndParent 
			&& hWndParent != ::GetDesktopWindow() 
			&& ::IsWindowEnabled(hWndParent))
		{
			::EnableWindow(hWndParent, FALSE);
			bEnableParent = true;
		}

		m_bExitModalLoop = false;
		m_uRetCode = IDCANCEL;

		/** 模态消息循环 */
		_modalMessageLoop();

		m_bExitModalLoop = false;

		/** 设置父窗口可用 */
		if (!bModaless && bEnableParent)
		{
			::EnableWindow(hWndParent, TRUE);
		}
		
		/** 隐藏窗口 */
		if (isWindow())
		{
			::SetWindowPos(getHwnd(), NULL, 0, 0, 0, 0, 
						 SWP_HIDEWINDOW | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		}
		
		/** 设置父窗口活动 */
		if (NULL != hWndParent)
		{
			::SetActiveWindow(hWndParent);
		}

		if (NULL != hWndLastActive)
		{
			::SetActiveWindow(hWndLastActive);
		}

		/** 销毁窗口 */
		if (isWindow())
		{
			::DestroyWindow(getHwnd());
		}

		return m_uRetCode;
	}

	void FWidgetPrivate::endModal(UINT uRetCode/* = IDCANCEL*/)
	{
		m_uRetCode = uRetCode;
		m_bExitModalLoop = true;

		//if (isWindow()) destroyWindow();
		if (isWindow())
		{
			::PostMessage(getHwnd(), WM_CLOSE, (WPARAM)uRetCode, 0L);
		}

		/** 防止卡死在_modalCreate */
		::PostThreadMessage(::GetCurrentThreadId(), WM_NULL, 0, 0);
	}

	HWND FWidgetPrivate::getHwnd(void) const
	{
		if (NULL == m_pWindow) return NULL;
		return m_pWindow->getHwnd();
	}

	bool FWidgetPrivate::isWindow(void) const
	{
		if (NULL == m_pWindow) return false;
		if (::IsWindow(m_pWindow->getHwnd())) return true;
		return false;
	}

	bool FWidgetPrivate::isLayeredWindow(void) const
	{
		if (NULL == m_pWindow) return false;
		return fooTestAttribute(m_pWindow->getStyleEx(), WS_EX_LAYERED);
	}

	/**
		消息处理
	*/
	/** WM_NCCREATE */
	void FWidgetPrivate::createMessage(HWND hWnd)
	{
		FASSERT(NULL != m_pWidget);
		m_pWidget->createMessage(hWnd);
	}

	/** WM_NCDESTROY */
	void FWidgetPrivate::finalMessage(HWND hWnd)
	{
		FASSERT(NULL != m_pWidget);
		if (NULL != m_pWindow)
		{
			fooSafeDelete(m_pWindow);
		}

		m_pWidget->finalMessage(hWnd);
	}

	void FWidgetPrivate::_modalMessageLoop(void)
	{
		FASSERT(NULL != m_pWindow);

		BOOL bRet;
		MSG msg;

		for (;;)
		{
			if (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				if (WM_QUIT == msg.message)
					break;

				if (WM_CLOSE == msg.message)
				{
					m_uRetCode = (UINT)msg.wParam;
					break;
				}
			}

			

			if (m_bExitModalLoop 
				|| NULL == m_pWindow 
				|| !isWindow())
			{
				break;
			}

			bRet = ::GetMessage(&msg, NULL, 0, 0);
			if (bRet == - 1)
			{
				continue;
			}
			else if (!bRet)
			{
				break;
			}

			if (NULL != Core::FApplication::getApplication())
			{
				if (Core::FApplication::getApplication()->translateMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam))
				{
					continue;
				}
			}

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	void FWidgetPrivate::broadcastPaintEvent(Event::FPaintEvent* e)
	{
		FASSERT(NULL != m_pWidget);

		if (!m_pWidget->isVisible())
		{
			return;
		}

		if (isNeedLayout())
		{
			m_pWidget->layout();
		}

		if (getLayoutRect().isEmpty())
		{
			return;
		}

		/** 裁剪控件区域 */
		Painting::FAutoClipRect autoclip(e->graphics(), getLayoutRect());

		/** 绘图 */
		if (!m_pWidget->isTransparent()) /** 透明的不重绘 */
		{
			m_pWidget->event(e);
		}

		if (m_pWidget->isShowScrollBar(SB_HORZ))
		{
			m_pWidget->m_pHScrollBar->event(e);
		}
		if (m_pWidget->isShowScrollBar(SB_VERT))
		{
			m_pWidget->m_pVScrollBar->event(e);
		}

		/** 裁剪客户区 */
		Core::FRect rcClient;
		m_pWidget->getValidLayoutRect(&rcClient);
		if (rcClient.isEmpty())
		{
			return;
		}
		autoclip.clipRect(rcClient);
		// 递归绘制图界面
		for (int i = 0; i < m_pWidget->getChildrenCount(); ++i)
		{
			FWidget *pChildWidget = m_pWidget->getChild(i);
			FASSERT(NULL != pChildWidget);

			if (!e->updateRect().isIntersects(pChildWidget->getLayoutRect()))
				continue;

			pChildWidget->m_pWidgetPrivate->broadcastPaintEvent(e);
		}
	}

	void FWidgetPrivate::broadcastEvent(Event::FEvent* e, bool broadSelf/* = false*/)
	{
		FASSERT(NULL != m_pWidget);

		if (broadSelf)
		{
			m_pWidget->event(e);
		}

		for (int i = 0; i < m_pWidget->getChildrenCount(); ++i)
		{
			FWidget *pChildWidget = m_pWidget->getChild(i);
			FASSERT(NULL != pChildWidget);

			pChildWidget->event(e);
			pChildWidget->m_pWidgetPrivate->broadcastEvent(e, true);
		}
	}

	void FWidgetPrivate::reflectMouseEvent(Event::FEvent* e)
	{
		FASSERT(NULL != m_pWidget);
		FASSERT(m_pWidget->isReflectMouse());

		FWidget *parent = m_pWidget->getParent();
		while (NULL != parent)
		{
			if (parent->isVisible() 
				&& !parent->isTransparent() 
				&& !parent->isSkipMouse())
			{
				parent->event(Event::FReflectEvent(Event::FEvent::reflectMouse, m_pWidget, e));
				break;
			}

			parent = parent->getParent();
		}
	}

	void FWidgetPrivate::reflectKeyboardEvent(Event::FEvent* e)
	{
		FASSERT(NULL != m_pWidget);
		FASSERT(m_pWidget->isReflectKeyboard());

		FWidget *parent = m_pWidget->getParent();
		while (NULL != parent)
		{
			if (parent->isVisible() 
				&& !parent->isTransparent() 
				&& !parent->isSkipKeyboard())
			{
				parent->event(Event::FReflectEvent(Event::FEvent::reflectKey, m_pWidget, e));
				break;
			}

			parent = parent->getParent();
		}
	}

	void FWidgetPrivate::parseWindowStyle(LPCTSTR pszValue)
	{
		/** 窗口风格 */
		if (Markup::fooMarkupCheckNameFlag(pszValue, _T("border")))
		{
			m_wndstyle.dwStyle |= WS_BORDER;
			m_wndstyle.border = true;
		}
		else if (Markup::fooMarkupCheckNameFlag(pszValue, _T("~border")))
		{
			m_wndstyle.dwStyle &= ~WS_BORDER;
			m_wndstyle.border = false;
		}

		if (Markup::fooMarkupCheckNameFlag(pszValue, _T("sizable")))
		{
			m_wndstyle.dwStyle |= WS_THICKFRAME;
		}
		else if (Markup::fooMarkupCheckNameFlag(pszValue, _T("~sizable")))
		{
			m_wndstyle.dwStyle &= ~WS_THICKFRAME;
		}

		if (Markup::fooMarkupCheckNameFlag(pszValue, _T("title")) 
			|| Markup::fooMarkupCheckNameFlag(pszValue, _T("caption")))
		{
			m_wndstyle.dwStyle |= WS_CAPTION;
		}
		else if (Markup::fooMarkupCheckNameFlag(pszValue, _T("~title")) 
			|| Markup::fooMarkupCheckNameFlag(pszValue, _T("~caption")))
		{
			m_wndstyle.dwStyle &= ~WS_CAPTION;
		}

		if (Markup::fooMarkupCheckNameFlag(pszValue, _T("minimizebox")))
		{
			m_wndstyle.dwStyle |= WS_MINIMIZEBOX;
		}
		else if (Markup::fooMarkupCheckNameFlag(pszValue, _T("~minimizebox")))
		{
			m_wndstyle.dwStyle &= ~WS_MINIMIZEBOX;
		}

		if (Markup::fooMarkupCheckNameFlag(pszValue, _T("maximizebox")))
		{
			m_wndstyle.dwStyle |= WS_MAXIMIZEBOX;
		}
		else if (Markup::fooMarkupCheckNameFlag(pszValue, _T("~maximizebox")))
		{
			m_wndstyle.dwStyle &= ~WS_MAXIMIZEBOX;
		}

		if (Markup::fooMarkupCheckNameFlag(pszValue, _T("center")))
		{
			m_wndstyle.center = true;
		}
		else if (Markup::fooMarkupCheckNameFlag(pszValue, _T("~center")))
		{
			m_wndstyle.center = false;
		}

		if (Markup::fooMarkupCheckNameFlag(pszValue, _T("layered")))
		{
			m_wndstyle.dwExStyle |= WS_EX_LAYERED;
		}
		else if (Markup::fooMarkupCheckNameFlag(pszValue, _T("~layered")))
		{
			m_wndstyle.dwExStyle &= ~WS_EX_LAYERED;
		}
	}

	bool FWidgetPrivate::hittest(const Core::FRect& rcWindow, const Core::FPoint& ptScreen, LRESULT& lResult)
	{
		if (NULL == m_pWindow) return false;

		bool bHitTestSize = true;
		if (!fooTestAttribute(m_pWindow->getStyle(), WS_THICKFRAME)) bHitTestSize = false;
		if (::IsZoomed(getHwnd())) bHitTestSize = false;
		if (::IsIconic(getHwnd())) bHitTestSize = false;
		if (m_wndstyle.windowncspace.isZero()) bHitTestSize = false;
		//if (!isLayeredWindow() && fooTestAttribute(m_pWindow->getStyle(), WS_CAPTION)) bHitTestSize = false;

		if (bHitTestSize)
		{
			if (ptScreen.x - rcWindow.left <= m_wndstyle.windowncspace.left 
				&& ptScreen.y - rcWindow.top <= m_wndstyle.windowncspace.top)
			{
				lResult = HTTOPLEFT;
				return true;
			}
			else if (rcWindow.right - ptScreen.x <= m_wndstyle.windowncspace.right 
				&& ptScreen.y - rcWindow.top <= m_wndstyle.windowncspace.top)
			{
				lResult = HTTOPRIGHT;
				return true;
			}
			else if (ptScreen.x - rcWindow.left <= m_wndstyle.windowncspace.left 
				&& rcWindow.bottom - ptScreen.y <= m_wndstyle.windowncspace.bottom)
			{
				lResult = HTBOTTOMLEFT;
				return true;
			}
			else if (rcWindow.right - ptScreen.x <= m_wndstyle.windowncspace.right 
				&& rcWindow.bottom - ptScreen.y <= m_wndstyle.windowncspace.bottom)
			{
				lResult = HTBOTTOMRIGHT;
				return true;
			}
			else if (ptScreen.x - rcWindow.left <= m_wndstyle.windowncspace.left)
			{
				lResult = HTLEFT;
				return true;
			}
			else if (ptScreen.y - rcWindow.top <= m_wndstyle.windowncspace.top)
			{
				lResult = HTTOP;
				return true;
			}
			else if (rcWindow.right - ptScreen.x <= m_wndstyle.windowncspace.right)
			{
				lResult = HTRIGHT;
				return true;
			}
			else if (rcWindow.bottom - ptScreen.y <= m_wndstyle.windowncspace.bottom)
			{
				lResult = HTBOTTOM;
				return true;
			}
		} // !if (bHitTestSize)

		return false;
	}

	/*static*/ FWidget* FWidgetPrivate::fFocusedWidget = NULL;
	/*static*/ FWidget* FWidgetPrivate::fHoverWidget = NULL;
	/*static*/ FWidget* FWidgetPrivate::fPushedWidget = NULL;
	/*static*/ FWidget* FWidgetPrivate::fActiveWidget = NULL;

	/*static*/ bool FWidgetPrivate::removeTabWidget(FWidget* widget)
	{
		if (NULL == FModule::tabwidgets())
		{
			return false;
		}

		if (NULL == widget)
		{
			FASSERT(FALSE);
			return false;
		}

		Core::FPtrList::POSITION position = FModule::tabwidgets()->find(widget);
		if (NULL != position)
		{
			return FModule::tabwidgets()->removeAtPosition(position);
		}

		return false;
	}

	/*static*/ bool FWidgetPrivate::setWidgetTabIndex(int nTabIndex, FWidget* widget)
	{
		if (NULL == FModule::tabwidgets())
		{
			FASSERT(FALSE);
			return false;
		}

		if (NULL == widget)
		{
			FASSERT(FALSE);
			return false;
		}

		Core::FPtrList::POSITION position = FModule::tabwidgets()->find(widget);
		if (NULL != position)
		{
			/** 先从Tab顺序链中删除 */
			FModule::tabwidgets()->removeAtPosition(position);
			position = NULL;
		}

		if (widget->isSkipFocus() 
			|| widget->isSkipTab())
		{
			return false;
		}

		if (FModule::tabwidgets()->size() == 0)
		{
			return FModule::tabwidgets()->add(widget);
		}
		else
		{
			for (int i = FModule::tabwidgets()->size() - 1; i >= 0; --i)
			{
				FWidget *pWidget = reinterpret_cast<FWidget*>(FModule::tabwidgets()->get(i));
				FASSERT(NULL != pWidget);
				if (pWidget->getTabIndex() <= nTabIndex)
				{
					return FModule::tabwidgets()->insert(i + 1, widget);
				}
			}

			return FModule::tabwidgets()->insert(0, widget);
		}

		return false;
	}

	/*static*/ FWidget* FWidgetPrivate::getNextTabWidget(FWidget* widget/* = NULL*/)
	{
		if (NULL == FModule::tabwidgets())
		{
			FASSERT(FALSE);
			return NULL;
		}

		/** 空链 */
		if (FModule::tabwidgets()->isEmpty())
		{
			return NULL;
		}

		FWidget *pNextWidget = NULL;
		if (NULL == widget)
		{
			int nIndex = 0;
			pNextWidget = reinterpret_cast<FWidget*>(FModule::tabwidgets()->get(nIndex));
			while (NULL != pNextWidget)
			{
				if (pNextWidget->isVisible() 
					&& !pNextWidget->isSkipFocus() 
					&& !pNextWidget->isSkipTab())
				{
					return pNextWidget;
				}

				if (nIndex >= FModule::tabwidgets()->size()) return NULL;
				pNextWidget = reinterpret_cast<FWidget*>(FModule::tabwidgets()->get(++nIndex));
			}

			return pNextWidget;
		}
		else
		{
			FWidget *pEndWidget = widget;
			Core::FPtrList::POSITION position = FModule::tabwidgets()->find(widget);
			if (NULL == position)
			{
				/** 没有找到， 从头开始，为防止进入死循环，以第一个作为参考结束 */
				pEndWidget = reinterpret_cast<FWidget*>(FModule::tabwidgets()->get(0));
				position = FModule::tabwidgets()->find(pEndWidget);
				if (NULL == position) return NULL;
				while (NULL != position)
				{
					pNextWidget = reinterpret_cast<FWidget*>(FModule::tabwidgets()->getAtPosition(position));
					if (pNextWidget->isVisible() 
						&& !pNextWidget->isSkipFocus() 
						&& !pNextWidget->isSkipTab())
					{
						return pNextWidget;
					}

					position = FModule::tabwidgets()->nextPosition(position);
				}

				/** 没有找到下一个widget只遍历一次 */
				return NULL; 
			}
			else 
			{
				position = FModule::tabwidgets()->nextPosition(position);
				if (NULL == position) position = FModule::tabwidgets()->find(FModule::tabwidgets()->get(0));
				while (NULL != position)
				{
					pNextWidget = reinterpret_cast<FWidget*>(FModule::tabwidgets()->getAtPosition(position));
					if (pNextWidget == widget)
					{
						/** 回到了起点 */
						return NULL;
					}

					if (pNextWidget->isVisible() 
						&& !pNextWidget->isSkipFocus() 
						&& !pNextWidget->isSkipTab())
					{
						return pNextWidget;
					}

					position = FModule::tabwidgets()->nextPosition(position);
				}
			}

			return pNextWidget;
		}

		return NULL;
	}

	/*static*/ bool FWidgetPrivate::isValidTabWidget(FWidget* widget)
	{
		if (NULL == FModule::tabwidgets())
		{
			FASSERT(FALSE);
			return false;
		}

		if (NULL == widget)
		{
			FASSERT(FALSE);
			return false;
		}

		if (!widget->isVisible() 
			|| widget->isSkipFocus() 
			|| widget->isSkipTab())
		{
			return false;
		}

		bool bValidTabWidget = false;
		HWND hActiveWindow = ::GetActiveWindow();
		FWidget *parent = widget->getParent();
		while (NULL != parent)
		{
			if (!parent->isVisible() 
				|| !parent->isEnabled())
			{
				return false;
			}

			if (parent->getHwnd() == hActiveWindow)
			{
				bValidTabWidget = true;
			}
			parent = parent->getParent();
		}

		return bValidTabWidget;
	}

}} // !namespace