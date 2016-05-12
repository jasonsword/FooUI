/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FWidget.cpp
* 描述：		FWidget
* 创建日期：	2013-06-06
*
*/

#include "StdAfx.h"
#include "widgets/FWidgetPrivate_p.h"
#include "widgets/FWindowPrivate_p.h"
#include "widgets/FWidget.h"
#include "widgets/FScrollBar.h"

#include "F3rd_p.h"
#include "painting/FGraphics.h"

#include "markup/FResourceManager.h"

#include "widgets/FScrollBar.h"
#include "widgets/FToolTip.h"
#include "widgets/FLayout.h"
#include "widgets/FLabel.h"
#include "widgets/FButton.h"
#include "widgets/FEdit.h"
#include "widgets/FAbstractList.h"
#include "widgets/FComboBox.h"
#include "widgets/FProgressBar.h"
#include "widgets/FHeader.h"
#include "widgets/FListCtrl.h"
#include "widgets/FTreeCtrl.h"
#include "widgets/FHotKey.h"
#include "widgets/FActivex.h" 
#include "widgets/FWebBrowser.h"

#define FWIDGET_INLINE FOOINLINE

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FWidget
	FWIDGET_INLINE FWidget::FWidget(FWidget* parent) : 
		m_pWidgetPrivate(new FWidgetPrivate(this)) 
		, m_strName(_T("")) 
		, m_nID(0) 
		, m_strText(_T("")) 
		, m_tabIndex(0) 
		, m_dwFlags(FOOFLAG_NONE) 
		, m_dwState(FOOSTATE_NONE | FOOSTATE_NORMAL | FOOSTATE_ENABLED 
					| FOOSTATE_VISIBLE | FOOSTATE_UNFOCUSED | FOOSTATE_UNCHECKED 
					| FOOSTATE_UNSELECTED | FOOSTATE_DEACTIVATED) 
		, m_dwDock(FOODOCK_NONE) 
		, m_dwAlign(FOOALIGN_NONE) 
		, m_strToolTip(_T("")) 
		, m_dwRole(FOOROLE_NONE) 
		, m_egMargin(0, 0, 0, 0) 
		, m_egPadding(0, 0, 0, 0) 
		, m_position(0, 0) 
		, m_bRelativePos(true) 
		, m_bFloatPos(false) 
		, m_bIsFixedXPosition(false) 
		, m_bIsFixedYPosition(false) 
		, m_size(0, 0) 
		, m_bIsFixedWidth(false) 
		, m_bIsFixedHeight(false) 
		, m_minsize(0, 0) 
		, m_maxsize(0, 0) 
		, m_pHScrollBar(NULL) 
		, m_pVScrollBar(NULL) 
		, m_tooltip(NULL)
	{
		if (NULL != parent) setParent(parent);
	}

	FWIDGET_INLINE FWidget::~FWidget(void)
	{
		FASSERT(NULL != m_pWidgetPrivate);
		if (m_pWidgetPrivate->isWindow())
		{
			m_pWidgetPrivate->destroyWindow();
		}

		/** 从Tab顺序链中移除 */
		FWidgetPrivate::removeTabWidget(this);

		if (this == FWidgetPrivate::fFocusedWidget)
		{
			FWidgetPrivate::fFocusedWidget = NULL;
		}

		if (this == FWidgetPrivate::fHoverWidget)
		{
			FWidgetPrivate::fHoverWidget = NULL;
		}

		if (this == FWidgetPrivate::fPushedWidget)
		{
			FWidgetPrivate::fPushedWidget = NULL;
		}

		if (this == FWidgetPrivate::fActiveWidget)
		{
			FWidgetPrivate::fActiveWidget = NULL;
		}

		if (NULL != m_pHScrollBar 
			&& m_pHScrollBar->isAutoDelete())
		{
			fooSafeDelete(m_pHScrollBar);
		}

		if (NULL != m_pVScrollBar 
			&& m_pVScrollBar->isAutoDelete())
		{
			fooSafeDelete(m_pVScrollBar);
		}

		if (NULL != m_tooltip 
			&& m_tooltip->isAutoDelete())
		{
			fooSafeDelete(m_tooltip);
		}

		delete m_pWidgetPrivate;
	}

	/** 
	控件属性相关
	*/
	FWIDGET_INLINE void FWidget::setName(LPCTSTR pszName)
	{
		FASSERT(NULL != pszName);
		m_strName = pszName;
	}

	FWIDGET_INLINE LPCTSTR FWidget::getName(void) const
	{
		return m_strName;
	}

	FWIDGET_INLINE void FWidget::setID(int nID)
	{
		m_nID = nID;
	}

	FWIDGET_INLINE int FWidget::getID(void) const
	{
		return m_nID;
	}

	FWIDGET_INLINE void FWidget::setGroup(LPCTSTR pszGroup)
	{
		m_strGroup = (NULL == pszGroup) ? _T("") : pszGroup;
	}

	FWIDGET_INLINE LPCTSTR FWidget::getGroup(void) const
	{
		return m_strGroup;
	}

	FWIDGET_INLINE void FWidget::setText(LPCTSTR pszText)
	{
		FASSERT(NULL != pszText);

		if (_tcscmp(m_strText, pszText) == 0)
		{
			return;
		}

		m_strText = pszText;
		if (isWindow())
		{
			::SetWindowText(getHwnd(), pszText);
		}

		event(Event::FChangedEvent(CT_TEXT));

		repaint();
	}

	FWIDGET_INLINE LPCTSTR FWidget::getText(void) const
	{
		return m_strText;
	}

	FWIDGET_INLINE void FWidget::setTabIndex(int tabIndex)
	{
		if (m_tabIndex == tabIndex)
		{
			return;
		}

		m_tabIndex = tabIndex;
	
		FWidgetPrivate::setWidgetTabIndex(getTabIndex(), this);
	}

	FWIDGET_INLINE int FWidget::getTabIndex(void) const
	{
		return m_tabIndex;
	}

	FWIDGET_INLINE void FWidget::setFlags(DWORD dwAttribute)
	{
		m_dwFlags = dwAttribute;
	}

	FWIDGET_INLINE void FWidget::modifyFlag(DWORD dwRemove, DWORD dwAdd)
	{
		fooModifyAttribute<DWORD>(m_dwFlags, dwRemove, dwAdd);
	}

	FWIDGET_INLINE bool FWidget::testFlag(DWORD dwAttribute) const
	{
		return fooTestAttribute(m_dwFlags, dwAttribute);
	}

	FWIDGET_INLINE DWORD FWidget::getFlags(void) const
	{
		return m_dwFlags;
	}

	FWIDGET_INLINE void FWidget::setState(DWORD dwState, bool bRepaint)
	{
		if (dwState == m_dwState) return;
		m_dwState = dwState;
		if (bRepaint) repaint();
	}

	FWIDGET_INLINE bool FWidget::modifyState(DWORD dwRemove, DWORD dwAdd, bool bRepaint)
	{
		bool bRet = fooModifyAttribute<DWORD>(m_dwState, dwRemove, dwAdd);

		if (bRepaint && bRet)
		{
			if (testStyle(dwRemove, _T(""), _T("")) 
				|| testStyle(dwAdd, _T(""), _T(""))
				)
			{
				repaint();
			}
		}

		return bRet;
	}

	FWIDGET_INLINE bool FWidget::testState(DWORD dwState) const
	{
		return fooTestAttribute(m_dwState, dwState);
	}

	FWIDGET_INLINE DWORD FWidget::getState(void) const
	{
		return m_dwState;
	}

	FWIDGET_INLINE void FWidget::setDock(DWORD dwDock)
	{
		if (m_dwDock == dwDock)
		{
			return;
		}

		m_dwDock = dwDock;
		relayoutpaintParent();
	}

	FWIDGET_INLINE DWORD FWidget::getDock(void) const
	{
		return m_dwDock;
	}

	FWIDGET_INLINE void FWidget::setAlign(DWORD dwAlign)
	{
		if (m_dwAlign == dwAlign)
		{
			return;
		}

		m_dwAlign = dwAlign;
		relayoutpaintParent();
	}

	FWIDGET_INLINE bool FWidget::testAlign(DWORD dwAlign) const
	{
		return fooTestAttribute(m_dwAlign, dwAlign);
	}

	FWIDGET_INLINE DWORD FWidget::getAlign(void) const
	{
		return m_dwAlign;
	}

	FWIDGET_INLINE void FWidget::setToolTipText(LPCTSTR pszToolTip)
	{
		m_strToolTip = pszToolTip;
	}

	FWIDGET_INLINE LPCTSTR FWidget::getToolTipText(void) const
	{
		return m_strToolTip;
	}

	FWIDGET_INLINE void FWidget::setRole(DWORD dwRole)
	{
		m_dwRole = dwRole;
	}

	FWIDGET_INLINE DWORD FWidget::getRole(void) const
	{
		return m_dwRole;
	}

	FWIDGET_INLINE void FWidget::getValidLayoutRect(RECT* lpRect) const
	{
		FASSERT(NULL != m_pWidgetPrivate);
		FASSERT(NULL != lpRect);

		Core::FRect rcClient = getLayoutRect();
		rcClient.deflateRect(getPadding());

		/** 这里排除滚动条大小 */
		if (NULL != m_pHScrollBar 
			&& m_pHScrollBar->isVisible())
		{
			rcClient.bottom -= m_pHScrollBar->getHeight() + m_pHScrollBar->getMargin().edgeHeight();
		}

		if (NULL != m_pVScrollBar 
			&& m_pVScrollBar->isVisible())
		{
			rcClient.right -= m_pVScrollBar->getWidth() + m_pVScrollBar->getMargin().edgeWidth();
		}

		*lpRect = rcClient;
	}

	FWIDGET_INLINE void FWidget::setMargin(const Core::FEdge& egMargin)
	{
		setMargin(egMargin.left, egMargin.top, egMargin.right, egMargin.bottom);
	}

	FWIDGET_INLINE void FWidget::setMargin(int xLeft, int yTop, int xRight, int yBottom)
	{
		m_egMargin.setRect(xLeft, yTop, xRight, yBottom);
		relayoutpaintParent();
	}

	FWIDGET_INLINE const Core::FEdge& FWidget::getMargin(void) const
	{
		return m_egMargin;
	}

	FWIDGET_INLINE void FWidget::setPadding(const Core::FEdge& egPadding)
	{
		setPadding(egPadding.left, egPadding.top, egPadding.right, egPadding.bottom);
	}

	FWIDGET_INLINE void FWidget::setPadding(int xLeft, int yTop, int xRight, int yBottom)
	{
		m_egPadding.setRect(xLeft, yTop, xRight, yBottom);
		relayoutpaintParent();
	}

	FWIDGET_INLINE const Core::FEdge& FWidget::getPadding(void) const
	{
		return m_egPadding;
	}

	FWIDGET_INLINE void FWidget::setLeft(int nLeft)
	{
		if (m_position.x != nLeft 
			|| !m_bIsFixedXPosition)
		{
			m_position.x = nLeft;
			m_bIsFixedXPosition = true;
			relayoutpaintParent();
		}
	}

	FWIDGET_INLINE void FWidget::setTop(int nTop)
	{
		if (m_position.y != nTop 
			|| !m_bIsFixedYPosition)
		{
			m_position.y = nTop;
			m_bIsFixedYPosition = true;
			relayoutpaintParent();
		}
	}

	FWIDGET_INLINE int FWidget::getLeft(void) const
	{
		return m_position.x;
	}

	FWIDGET_INLINE int FWidget::getTop(void) const
	{
		return m_position.y;
	}

	FWIDGET_INLINE void FWidget::setPosition(const Core::FPoint& position)
	{
		setPosition(position.x, position.y);
	}

	FWIDGET_INLINE void FWidget::setPosition(int xLeft, int yTop)
	{
		FASSERT(NULL != m_pWidgetPrivate);
		if (m_position.x != xLeft 
			|| m_position.y != yTop 
			|| !m_bIsFixedXPosition
			|| !m_bIsFixedYPosition)
		{
			m_position.setPoint(xLeft, yTop);
			m_bIsFixedXPosition = true;
			m_bIsFixedYPosition = true;
			relayoutpaintParent();
		}
	}

	FWIDGET_INLINE void FWidget::emptyXPosition(void)
	{
		if (m_position.x != 0 
			|| m_bIsFixedXPosition)
		{
			m_position.x = 0;
			m_bIsFixedXPosition = false;
			relayoutpaintParent();
		}
	}

	FWIDGET_INLINE void FWidget::emptyYPosition(void)
	{
		if (m_position.y != 0 
			|| m_bIsFixedYPosition)
		{
			m_position.y = 0;
			m_bIsFixedYPosition = false;
			relayoutpaintParent();
		}
	}

	FWIDGET_INLINE void FWidget::emptyPosition(void)
	{
		if (!m_position.isEmpty() 
			|| m_bIsFixedXPosition 
			|| m_bIsFixedYPosition)
		{
			m_position.setEmpty();
			m_bIsFixedXPosition = false;
			m_bIsFixedYPosition = false;
			relayoutpaintParent();
		}
	}

	FWIDGET_INLINE const Core::FPoint& FWidget::getPosition(void) const
	{
		return m_position;
	}

	FWIDGET_INLINE void FWidget::relativePosition(bool bRelative)
	{
		m_bRelativePos = bRelative;
		relayoutpaintParent();
	}

	FWIDGET_INLINE bool FWidget::isRelativePosition(void) const
	{
		return m_bRelativePos;
	}

	FWIDGET_INLINE void FWidget::floatPosition(bool bFloat)
	{
		m_bFloatPos = bFloat;
		relayoutpaintParent();
	}

	FWIDGET_INLINE bool FWidget::isFloatPosition(void) const
	{
		return m_bFloatPos;
	}

	FWIDGET_INLINE bool FWidget::isFixedXPosition(void) const
	{
		return m_bIsFixedXPosition;
	}

	FWIDGET_INLINE bool FWidget::isFixedYPosition(void) const
	{
		return m_bIsFixedYPosition;
	}

	FWIDGET_INLINE void FWidget::setWidth(int nWidth)
	{
		if (m_size.cx != nWidth 
			|| !m_bIsFixedWidth)
		{
			m_size.cx = nWidth;
			m_bIsFixedWidth = true;
			relayoutpaintParent();
		}
	}

	FWIDGET_INLINE void FWidget::setHeight(int nHeight)
	{
		if (m_size.cy != nHeight 
			|| !m_bIsFixedHeight)
		{
			m_size.cy = nHeight;
			m_bIsFixedHeight = true;
			relayoutpaintParent();
		}
	}

	FWIDGET_INLINE int FWidget::getWidth(void) const
	{
		return m_size.cx;
	}

	FWIDGET_INLINE int FWidget::getHeight(void) const
	{
		return m_size.cy;
	}

	FWIDGET_INLINE void FWidget::setSize(Core::FSize& size)
	{
		setSize(size.cx, size.cy);
	}

	FWIDGET_INLINE void FWidget::setSize(int cx, int cy)
	{
		FASSERT(NULL != m_pWidgetPrivate);
		if (m_size.cx != cx 
			|| m_size.cy != cy 
			|| !m_bIsFixedWidth 
			|| !m_bIsFixedHeight)
		{
			m_size.setSize(cx, cy);
			m_bIsFixedWidth = true;
			m_bIsFixedHeight = true;
			relayoutpaintParent();
		}
	}

	void FWidget::emptyWidth(void)
	{
		if (m_size.cx != 0 
			&& m_bIsFixedWidth)
		{
			m_size.cx = 0;
			m_bIsFixedWidth = false;
			relayoutpaintParent();
		}
	}

	void FWidget::emptyHeight(void)
	{
		if (m_size.cy != 0 
			&& m_bIsFixedHeight)
		{
			m_size.cy = 0;
			m_bIsFixedHeight = false;
			relayoutpaintParent();
		}
	}

	FWIDGET_INLINE void FWidget::emptySize(void)
	{
		if (!m_size.isEmpty() 
			|| m_bIsFixedWidth 
			|| m_bIsFixedHeight)
		{
			m_size.setEmpty();
			m_bIsFixedWidth = false;
			m_bIsFixedHeight = false;
			relayoutpaintParent();
		}
	}

	FWIDGET_INLINE const Core::FSize& FWidget::getSize(void) const
	{
		return m_size;
	}

	bool FWidget::isFixedWidth(void) const
	{
		return m_bIsFixedWidth;
	}

	bool FWidget::isFixedHeight(void) const
	{
		return m_bIsFixedHeight;
	}

	FWIDGET_INLINE void FWidget::setMinWidth(int nMinWidth)
	{
		setMinSize(nMinWidth, m_minsize.cy);
	}

	FWIDGET_INLINE void FWidget::setMinHeight(int nMinHeight)
	{
		setMinSize(m_minsize.cx, nMinHeight);
	}

	FWIDGET_INLINE int FWidget::getMinWidth(void) const
	{
		return m_minsize.cx;
	}

	FWIDGET_INLINE int FWidget::getMinHeight(void) const
	{
		return m_minsize.cy;
	}

	FWIDGET_INLINE void FWidget::setMinSize(Core::FSize& size)
	{
		setMinSize(size.cx, size.cy);
	}

	FWIDGET_INLINE void FWidget::setMinSize(int cx, int cy)
	{
		if (m_minsize.cx == cx && m_minsize.cy == cy)
		{
			return;
		}

		m_minsize.setSize(cx, cy);
		relayoutpaintParent();
	}

	FWIDGET_INLINE void FWidget::clearMinSize(void)
	{
		setMinSize(0, 0);
	}

	FWIDGET_INLINE const Core::FSize& FWidget::getMinSize(void) const
	{
		return m_minsize;
	}

	FWIDGET_INLINE void FWidget::setMaxWidth(int nMaxWidth)
	{
		setMaxSize(nMaxWidth, m_maxsize.cy);
	}

	FWIDGET_INLINE void FWidget::setMaxHeight(int nMaxHeight)
	{
		setMaxSize(m_maxsize.cx, nMaxHeight);
	}

	FWIDGET_INLINE int FWidget::getMaxWidth(void) const
	{
		return m_maxsize.cx;
	}

	FWIDGET_INLINE int FWidget::getMaxHeight(void) const
	{
		return m_maxsize.cy;
	}

	FWIDGET_INLINE void FWidget::setMaxSize(Core::FSize& size)
	{
		setMaxSize(size.cx, size.cy);
	}

	FWIDGET_INLINE void FWidget::setMaxSize(int cx, int cy)
	{
		if (m_maxsize.cx == cx && m_maxsize.cy == cy)
		{
			return;
		}

		m_maxsize.setSize(cx, cy);
		relayoutpaintParent();
	}

	FWIDGET_INLINE void FWidget::clearMaxSize(void)
	{
		setMaxSize(0, 0);
	}

	FWIDGET_INLINE const Core::FSize& FWidget::getMaxSize(void) const
	{
		return m_maxsize;
	}

	FWIDGET_INLINE void FWidget::setStyle(const Markup::FStyleSheetResource& stylesheet)
	{
		m_stylesheet = stylesheet;
		repaint();
	}

	FWIDGET_INLINE bool FWidget::setStyle(LPCTSTR pszName)
	{
		Markup::FStyleSheetResource *pStyleObject = (Markup::FStyleSheetResource*)getResource(pszName, 
			Markup::FResourceObject::FRESTYPE_STYLESHEET);
		if (NULL != pStyleObject)
		{
			setStyle(*pStyleObject);
			return true;
		}

		FASSERT(false); /** 没有找到资源，警告 */
		return false;
	}

	FWIDGET_INLINE const Markup::FStyleSheetResource& FWidget::getStyle(void) const
	{
		return m_stylesheet;
	}

	FWIDGET_INLINE bool FWidget::testStyle(DWORD dwState, LPCTSTR pszType/* = NULL*/, LPCTSTR pszPart/* = NULL*/) const
	{
		return m_stylesheet.testStyle(dwState, pszType, pszPart);
	}

	FWIDGET_INLINE void FWidget::setResourceManager(const Markup::FResourceManager& resourceMgr)
	{
		m_resourceMgr = resourceMgr;
	}

	FWIDGET_INLINE Markup::FResourceManager& FWidget::resourceManager(void)
	{
		return m_resourceMgr;
	}

	FWIDGET_INLINE const Markup::FResourceManager& FWidget::resourceManager(void) const
	{
		return m_resourceMgr;
	}

	FWIDGET_INLINE Markup::FResourceObject* FWidget::getResource(LPCTSTR pszName, DWORD dwType) const
	{
		Markup::FResourceObject *pResrouceObect = m_resourceMgr.getResource(pszName, dwType);
		if (NULL != pResrouceObect)
		{
			return pResrouceObect;
		}

		if (NULL != getParent())
		{
			return getParent()->getResource(pszName, dwType);
		}

		return NULL;
	}

	void FWidget::setUserData(LPVOID lpData)
	{
		m_pUserData = lpData;
	}

	LPVOID FWidget::getUserData(void) const
	{
		return m_pUserData;
	}

	void FWidget::setUserTag(LPCTSTR pszUserTag)
	{
		m_strUserTag = (NULL == pszUserTag) ? _T("") : pszUserTag;
	}

	LPCTSTR FWidget::getUserTag(void) const
	{
		return m_strUserTag;
	}

	/** 
	子控件管理
	*/
	FWIDGET_INLINE bool FWidget::addChild(FWidget* pWidget)
	{
		FASSERT(NULL != pWidget);
		FASSERT(NULL != m_pWidgetPrivate);

		if (NULL != pWidget->getParent())
		{
			pWidget->getParent()->m_pWidgetPrivate->removeChildAndNoDel(pWidget);
		}

		if (!pWidget->m_pWidgetPrivate->initParent(this, getTopLevelParent()))
		{
			return false;
		}

		return m_pWidgetPrivate->addChild(pWidget);
	}

	FWIDGET_INLINE bool FWidget::insertChild(int nIndex, FWidget* pWidget)
	{
		FASSERT(NULL != pWidget);
		FASSERT(NULL != m_pWidgetPrivate);

		if (NULL != pWidget->getParent())
		{
			pWidget->getParent()->m_pWidgetPrivate->removeChildAndNoDel(pWidget);
		}

		if (!pWidget->m_pWidgetPrivate->initParent(this, getTopLevelParent()))
		{
			return false;
		}

		return m_pWidgetPrivate->insertChild(nIndex, pWidget);
	}

	FWIDGET_INLINE bool FWidget::removeChild(int nIndex)
	{
		FASSERT(NULL != m_pWidgetPrivate);
		return m_pWidgetPrivate->removeChild(nIndex);
	}

	FWIDGET_INLINE bool FWidget::removeChild(FWidget* pWidget)
	{
		FASSERT(NULL != pWidget);
		FASSERT(NULL != m_pWidgetPrivate);

		if (NULL == pWidget->getParent())
		{
			/** 该控件没有父窗口，因此无法从父窗口子控件列表移除 */
			FASSERT(false);
			return false;
		}

		if (this == pWidget->getParent())
		{
			return m_pWidgetPrivate->removeChild(pWidget);
		}
		else
		{
			return pWidget->getParent()->removeChild(pWidget);
		}
	}

	FWIDGET_INLINE bool FWidget::clearChildren(void)
	{
		FASSERT(NULL != m_pWidgetPrivate);
		return m_pWidgetPrivate->clearChildren();
	}

	FWIDGET_INLINE FWidget* FWidget::getChild(int nIndex) const
	{
		FASSERT(NULL != m_pWidgetPrivate);
		return m_pWidgetPrivate->getChild(nIndex);
	}

	FWIDGET_INLINE int FWidget::childIndex(void) const
	{
		FWidget *parent = getParent();
		if (NULL != parent)
		{
			for (int i = 0; i < parent->getChildrenCount(); ++i)
			{
				FWidget *widget = parent->getChild(i);
				if (this == widget)
				{
					return i;
				}
			}
		}

		return 0;
	}

	FWIDGET_INLINE int FWidget::indexOfChild(FWidget* widget) const
	{
		FASSERT(NULL != m_pWidgetPrivate);
		return m_pWidgetPrivate->indexOfChild(widget);
	}

	FWIDGET_INLINE int FWidget::getChildrenCount(void) const
	{
		FASSERT(NULL != m_pWidgetPrivate);
		return m_pWidgetPrivate->getChildrenCount();
	}

	FWIDGET_INLINE bool FWidget::hasChildren(void) const
	{
		FASSERT(NULL != m_pWidgetPrivate);
		return (m_pWidgetPrivate->getChildrenCount() > 0);
	}

	FWIDGET_INLINE FWidget* FWidget::findWidget(LPCTSTR pszName) const
	{
		FASSERT(NULL != m_pWidgetPrivate);
		return m_pWidgetPrivate->findWidget(pszName);
	}

	FWIDGET_INLINE FWidget* FWidget::findNextWidget(FWidget* pCurWidget, LPCTSTR pszName) const
	{
		FASSERT(NULL != m_pWidgetPrivate);
		return m_pWidgetPrivate->findNextWidget(pCurWidget, pszName);
	}

	FWIDGET_INLINE FWidget* FWidget::findWidget(int nID) const
	{
		FASSERT(NULL != m_pWidgetPrivate);
		return m_pWidgetPrivate->findWidget(nID);
	}

	FWIDGET_INLINE FWidget* FWidget::findNextWidget(FWidget* pCurWidget, int nID) const
	{
		FASSERT(NULL != m_pWidgetPrivate);
		return m_pWidgetPrivate->findNextWidget(pCurWidget, nID);
	}

	FWIDGET_INLINE FWidget* FWidget::getNextTabWidget(void) const
	{
		return getNextTabWidget(FWidgetPrivate::fFocusedWidget);
	}

	FWIDGET_INLINE FWidget* FWidget::getNextTabWidget(FWidget* widget) const
	{
		FWidget* pNextWidget = FWidgetPrivate::getNextTabWidget(widget);
		while (NULL != pNextWidget)
		{
			if (FWidgetPrivate::isValidTabWidget(pNextWidget))
			{
				return pNextWidget;
			}
			pNextWidget = FWidgetPrivate::getNextTabWidget(pNextWidget);
		}

		return NULL;
	}

	void FWidget::lockChildren(void)
	{
		FASSERT(NULL != m_pWidgetPrivate);

		m_pWidgetPrivate->lockChildren();
	}

	void FWidget::unlockChildren(void)
	{
		FASSERT(NULL != m_pWidgetPrivate);

		m_pWidgetPrivate->unlockChildren();
	}

	FWIDGET_INLINE FWidget* FWidget::widgetFromPoint(int xPos, int yPos) const
	{
		/** 此控件不可见 */
		if (!isVisible())
		{
			return NULL;
		}

		/** 此控件不在坐标范围 */
		if (!getLayoutRect().isIntersects(xPos, yPos))
		{
			return NULL;
		}

		/** 检测水平滚动条 */
		FScrollBar *pHScrollBar = getScrollBar(SB_HORZ);
		if (NULL != pHScrollBar 
			&& pHScrollBar->getLayoutRect().isIntersects(xPos, yPos) 
			&& pHScrollBar->isVisible() 
			&& !pHScrollBar->isTransparent() 
			&& !pHScrollBar->isSkipMouse())
		{
			return pHScrollBar;
		}

		/** 检测垂直滚动条 */
		FScrollBar *pVScrollBar = getScrollBar(SB_VERT);
		if (NULL != pVScrollBar 
			&& pVScrollBar->getLayoutRect().isIntersects(xPos, yPos) 
			&& pVScrollBar->isVisible() 
			&& !pVScrollBar->isTransparent() 
			&& !pVScrollBar->isSkipMouse())
		{
			return pVScrollBar;
		}

		Core::FRect rcClient;
		getValidLayoutRect(rcClient);

		if (rcClient.isIntersects(xPos, yPos))
		{
			for (int i = getChildrenCount() - 1; i >= 0; --i)
			{
				FWidget *pWidget = getChild(i);
				FASSERT(NULL != pWidget);

				if (!pWidget->isVisible())
				{
					continue;
				}

				FWidget *pFindWidget = pWidget->widgetFromPoint(xPos, yPos);
				if (NULL != pFindWidget)
				{
					return pFindWidget;
				}
			}
		} // !if (rcClient.isIntersects(xPos, yPos))

		/** 非透明与接收鼠标消息则返回自身 */
		if (!isTransparent() 
			&& !isSkipMouse())
		{
			return const_cast<FWidget*>(this);
		}

		return NULL;
	}

	FWIDGET_INLINE FWidget* FWidget::scrollViewFromPoint(int xPos, int yPos) const
	{	
		/** 不可见 */
		if (!isVisible())
		{
			/** 控件式不可见的 */
			return NULL;
		}

		if (!isWindow() 
			&& !getLayoutRect().isIntersects(xPos, yPos))
		{
			/** 不在xPos和yPos中(由于窗口的坐标不是布局坐标，所以不能直接判断窗口） */
			return NULL;
		}

		/** 检测水平滚动条 */
		FScrollBar *pHScrollBar = getScrollBar(SB_HORZ);
		if (NULL != pHScrollBar 
			&& pHScrollBar->getLayoutRect().isIntersects(xPos, yPos) 
			&& pHScrollBar->isVisible() 
			&& !pHScrollBar->isTransparent() 
			&& !pHScrollBar->isSkipMouse())
		{
			return const_cast<FWidget*>(this);
		}

		/** 检测垂直滚动条 */
		FScrollBar *pVScrollBar = getScrollBar(SB_VERT);
		if (NULL != pVScrollBar 
			&& pVScrollBar->getLayoutRect().isIntersects(xPos, yPos) 
			&& pVScrollBar->isVisible() 
			&& !pVScrollBar->isTransparent() 
			&& !pVScrollBar->isSkipMouse())
		{
			return const_cast<FWidget*>(this);
		}

		Core::FRect rcClient;
		getValidLayoutRect(rcClient);

		if (rcClient.isIntersects(xPos, yPos))
		{
			for (int i = getChildrenCount() - 1; i >= 0; --i)
			{
				FWidget *pWidget = getChild(i);
				if (!pWidget->isVisible() 
					|| !pWidget->getLayoutRect().isIntersects(xPos, yPos))
				{
					continue;
				}

				FWidget *pFindWidget = pWidget->scrollViewFromPoint(xPos, yPos);
				if (NULL != pFindWidget)
				{
					return pFindWidget;
				}
			}
		} // !if (rcClient.isIntersects(xPos, yPos))

		if (isScrollView())
		{
			return const_cast<FWidget*>(this);
		}

		return NULL;
	}

	FWIDGET_INLINE bool FWidget::isScrollView(void) const
	{
		/** 只判断了垂直滚动条 */
		if (NULL != m_pVScrollBar 
			&& m_pVScrollBar->isVisible() 
			&& m_pVScrollBar->isEnabled())
		{
			return true;
		}

		return false;
	}

	/** 
	控件层处理 
	*/
	FWIDGET_INLINE void FWidget::setParent(FWidget* parent)
	{
		FASSERT(NULL != m_pWidgetPrivate);
		if (getParent() == parent)
		{
			return;
		}

		if (NULL != getParent())
		{
			getParent()->m_pWidgetPrivate->removeChildAndNoDel(this);
			m_pWidgetPrivate->initParent(NULL, NULL);
		}

		if (NULL != parent)
		{
			parent->addChild(this);
		}
	}

	FWIDGET_INLINE void FWidget::setOwner(FWidget* parent)
	{
		FASSERT(NULL != m_pWidgetPrivate);
		if (getParent() == parent)
		{
			m_pWidgetPrivate->initParent(parent, parent->getTopLevelParent());
			return;
		}

		if (NULL != getParent())
		{
			getParent()->m_pWidgetPrivate->removeChildAndNoDel(this);
			m_pWidgetPrivate->initParent(NULL, NULL);
		}

		m_pWidgetPrivate->initParent(parent, parent->getTopLevelParent());
	}

	FWIDGET_INLINE FWidget* FWidget::getParent(void) const
	{
		FASSERT(NULL != m_pWidgetPrivate);

		return m_pWidgetPrivate->m_pParentWidget;
	}

	FWIDGET_INLINE FWidget* FWidget::getTopLevelParent(void) const
	{
		FASSERT(NULL != m_pWidgetPrivate);

		return m_pWidgetPrivate->m_pTopLevelWidget;
	}

	
	FWIDGET_INLINE FWidget* FWidget::prevSibling(void) const
	{
		FASSERT(NULL != m_pWidgetPrivate);
		FWidgetLock lock(const_cast<FWidget*>(this));

		Core::FPtrList::POSITION position = m_pWidgetPrivate->m_children.find((LPVOID)this);
		if (NULL == position)
		{
			FASSERT(FALSE);
			return NULL;
		}

		position = m_pWidgetPrivate->m_children.previousPosition(position);
		if (NULL == position)
		{
			return NULL;
		}

		return reinterpret_cast<FWidget*>(m_pWidgetPrivate->m_children.getAtPosition(position));
	}

	FWIDGET_INLINE FWidget* FWidget::nextSibling (void) const
	{
		FASSERT(NULL != m_pWidgetPrivate);
		FWidgetLock lock(const_cast<FWidget*>(this));

		Core::FPtrList::POSITION position = m_pWidgetPrivate->m_children.find((LPVOID)this);
		if (NULL == position)
		{
			FASSERT(FALSE);
			return NULL;
		}

		position = m_pWidgetPrivate->m_children.nextPosition(position);
		if (NULL == position)
		{
			return NULL;
		}

		return reinterpret_cast<FWidget*>(m_pWidgetPrivate->m_children.getAtPosition(position));
	}

	/** 控件操作 */
	FWIDGET_INLINE void FWidget::show(void)
	{
		setVisible(true);
	}

	FWIDGET_INLINE void FWidget::hide(void)
	{
		setVisible(false);
	}

	FWIDGET_INLINE void FWidget::center(FWidget* pWidget)
	{
		if (isWindow())
		{
			FASSERT(NULL != m_pWidgetPrivate->m_pWindow);
			HWND hWndCenter = NULL;
			if (NULL != pWidget) hWndCenter = pWidget->getSafeHwnd();
			m_pWidgetPrivate->m_pWindow->centerWindow(hWndCenter);
		}
	}

	FWIDGET_INLINE void FWidget::enable(bool bEnable)
	{
		setEnabled(bEnable);
	}

	FWIDGET_INLINE bool FWidget::isEnabled(void) const
	{
		return isStateEnabled();
	}

	FWIDGET_INLINE void FWidget::focus(void)
	{
		setFocus(true);
	}

	FWIDGET_INLINE bool FWidget::isFocused(void) const
	{
		return fooTestAttribute(m_dwState, FOOSTATE_FOCUSED);
	}

	/** 状态设置 */
	FWIDGET_INLINE void FWidget::setVisible(bool bVisible)
	{
		if (bVisible)
		{
			setStateVisible();
			if (isWindow()) ::ShowWindow(getHwnd(), SW_SHOW);
		}
		else
		{
			setStateInVisible();
			if (isWindow()) ::ShowWindow(getHwnd(), SW_HIDE);
		}
	}

	FWIDGET_INLINE bool FWidget::isVisible(void) const
	{
		return isStateVisible();
	}

	FWIDGET_INLINE void FWidget::setEnabled(bool bEnabled)
	{
		if (bEnabled)
		{
			setStateEnabled();
			if (isWindow()) ::EnableWindow(getHwnd(), TRUE);
		}
		else
		{
			setStateDisabled();
			if (isWindow()) ::EnableWindow(getHwnd(), FALSE);
		}
	}

	FWIDGET_INLINE void FWidget::setFocus(bool bFocuse)
	{
		if (bFocuse)
		{
			if (FWidgetPrivate::fFocusedWidget == this)
			{
				return;
			}

			if (NULL != FWidgetPrivate::fFocusedWidget)
			{
				FWidgetPrivate::fFocusedWidget->setStateUnFocused(true);
				FWidgetPrivate::fFocusedWidget->event(Event::FEvent(Event::FEvent::FocusOut));
			}

			setStateFocused();
			event(Event::FEvent(Event::FEvent::FocusIn));

			FWidgetPrivate::fFocusedWidget = this;

			::SetFocus(getSafeHwnd());
		}
		else
		{
			if (FWidgetPrivate::fFocusedWidget == this)
			{
				FWidgetPrivate::fFocusedWidget = NULL;
			}
			setStateUnFocused();
		}
	}

	FWIDGET_INLINE void FWidget::setFocus(FWidget* pWidget)
	{
		FASSERT(NULL != pWidget);
		pWidget->setFocus(true);
	}

	FWIDGET_INLINE void FWidget::setSelect(bool bSelected)
	{
		if (bSelected)
			setStateSelected();
		else
			setStateUnSelected();
	}

	FWIDGET_INLINE bool FWidget::isSelected(void) const
	{
		return isStateSelected();
	}

	FWIDGET_INLINE void FWidget::setCheck(bool bCheck)
	{
		if (bCheck)
			setStateChecked();
		else
			setStateUnChecked();
	}

	FWIDGET_INLINE bool FWidget::isChecked(void) const
	{
		return isStateChecked();
	}

	FWIDGET_INLINE void FWidget::setTransparent(bool bTransparent)
	{
		if (bTransparent)
			fooModifyAttribute<DWORD>(m_dwFlags, 0, FOOFLAG_TRANSPARENT);
		else
			fooModifyAttribute<DWORD>(m_dwFlags, FOOFLAG_TRANSPARENT, 0);
	}

	FWIDGET_INLINE bool FWidget::isTransparent(void) const
	{
		return fooTestAttribute(m_dwFlags, FOOFLAG_TRANSPARENT);
	}

	FWIDGET_INLINE void FWidget::skipTab(bool bSkip)
	{
		bool bNeedUpdate = false;
		if (bSkip)
			bNeedUpdate = fooModifyAttribute<DWORD>(m_dwFlags, 0, FOOFLAG_SKIPTAB);
		else
			bNeedUpdate = fooModifyAttribute<DWORD>(m_dwFlags, FOOFLAG_SKIPTAB, 0);

		if (bNeedUpdate)
		{
			FWidgetPrivate::setWidgetTabIndex(getTabIndex(), this);
		}
	}

	FWIDGET_INLINE bool FWidget::isSkipTab(void) const
	{
		return fooTestAttribute(m_dwFlags, FOOFLAG_SKIPTAB);
	}

	FWIDGET_INLINE void FWidget::skipMouse(bool bSkip)
	{
		if (bSkip)
			fooModifyAttribute<DWORD>(m_dwFlags, 0, FOOFLAG_SKIPMOUSE);
		else
			fooModifyAttribute<DWORD>(m_dwFlags, FOOFLAG_SKIPMOUSE, 0);
	}

	FWIDGET_INLINE bool FWidget::isSkipMouse(void) const
	{
		return fooTestAttribute(m_dwFlags, FOOFLAG_SKIPMOUSE);
	}

	FWIDGET_INLINE void FWidget::skipKeyboard(bool bSkip)
	{
		if (bSkip)
			fooModifyAttribute<DWORD>(m_dwFlags, 0, FOOFLAG_SKIPKEYBOARD);
		else
			fooModifyAttribute<DWORD>(m_dwFlags, FOOFLAG_SKIPKEYBOARD, 0);
	}

	FWIDGET_INLINE bool FWidget::isSkipKeyboard(void) const
	{
		return fooTestAttribute(m_dwFlags, FOOFLAG_SKIPKEYBOARD);
	}

	FWIDGET_INLINE void FWidget::skipFocus(bool bSkip)
	{
		bool bNeedUpdate = false;
		if (bSkip)
			bNeedUpdate = fooModifyAttribute<DWORD>(m_dwFlags, 0, FOOFLAG_SKIPFOCUS);
		else
			bNeedUpdate = fooModifyAttribute<DWORD>(m_dwFlags, FOOFLAG_SKIPFOCUS, 0);

		if (bNeedUpdate)
		{
			FWidgetPrivate::setWidgetTabIndex(getTabIndex(), this);
		}
	}

	FWIDGET_INLINE bool FWidget::isSkipFocus(void) const
	{
		return fooTestAttribute(m_dwFlags, FOOFLAG_SKIPFOCUS);
	}

	FWIDGET_INLINE void FWidget::reflectMouse(bool bReflect)
	{
		if (bReflect)
			fooModifyAttribute<DWORD>(m_dwFlags, 0, FOOFLAG_REFLECTMOUSE);
		else
			fooModifyAttribute<DWORD>(m_dwFlags, FOOFLAG_REFLECTMOUSE, 0);
	}

	FWIDGET_INLINE bool FWidget::isReflectMouse(void)
	{
		return fooTestAttribute(m_dwFlags, FOOFLAG_REFLECTMOUSE);
	}

	FWIDGET_INLINE void FWidget::reflectKeyboard(bool bReflect)
	{
		if (bReflect)
			fooModifyAttribute<DWORD>(m_dwFlags, 0, FOOFLAG_REFLECTKEYBOARD);
		else
			fooModifyAttribute<DWORD>(m_dwFlags, FOOFLAG_REFLECTKEYBOARD, 0);
	}

	FWIDGET_INLINE bool FWidget::isReflectKeyboard(void)
	{
		return fooTestAttribute(m_dwFlags, FOOFLAG_REFLECTKEYBOARD);
	}

	FWIDGET_INLINE void FWidget::setStateNone(bool bRepaint)
	{
		if (FOOSTATE_NONE != m_dwState)
		{
			m_dwState = FOOSTATE_NONE;
			if (bRepaint) repaint();
		}
	}

	FWIDGET_INLINE bool FWidget::isStateNone(void) const
	{
		return (FOOSTATE_NONE == m_dwState);
	}

	FWIDGET_INLINE void FWidget::setStateNormal(bool bRepaint)
	{
		modifyState(FOOSTATE_HOVER | FOOSTATE_PUSHED, FOOSTATE_NORMAL, bRepaint);
	}

	FWIDGET_INLINE bool FWidget::isStateNormal(void) const
	{
		return fooTestAttribute(m_dwState, FOOSTATE_NORMAL);
	}

	FWIDGET_INLINE void FWidget::setStateHover(bool bRepaint)
	{
		modifyState(FOOSTATE_NORMAL | FOOSTATE_PUSHED, FOOSTATE_HOVER, bRepaint);
	}

	FWIDGET_INLINE bool FWidget::isStateHover(void) const
	{
		return fooTestAttribute(m_dwState, FOOSTATE_HOVER);
	}

	FWIDGET_INLINE void FWidget::setStatePushed(bool bRepaint)
	{
		modifyState(FOOSTATE_NORMAL | FOOSTATE_HOVER, FOOSTATE_PUSHED, bRepaint);
	}

	FWIDGET_INLINE bool FWidget::iStatePushed(void) const
	{
		return fooTestAttribute(m_dwState, FOOSTATE_PUSHED);
	}

	FWIDGET_INLINE void FWidget::setStateDisabled(bool bRepaint)
	{
		modifyState(FOOSTATE_ENABLED, FOOSTATE_DISABLED, bRepaint);
	}

	FWIDGET_INLINE bool FWidget::isStateDisabled(void) const
	{
		return fooTestAttribute(m_dwState, FOOSTATE_DISABLED);
	}

	FWIDGET_INLINE void FWidget::setStateEnabled(bool bRepaint)
	{
		modifyState(FOOSTATE_DISABLED, FOOSTATE_ENABLED, bRepaint);
	}

	FWIDGET_INLINE bool FWidget::isStateEnabled(void) const
	{
		return fooTestAttribute(m_dwState, FOOSTATE_ENABLED);
	}

	FWIDGET_INLINE void FWidget::setStateInVisible(bool bRepaint)
	{
		bool bNeedRepaint = fooModifyAttribute<DWORD>(m_dwState, FOOSTATE_VISIBLE, FOOSTATE_INVISIBLE);
		if (bNeedRepaint)
		{
			m_pWidgetPrivate->broadcastEvent(Event::FEvent(Event::FEvent::Hide), true);
		}

		if (bNeedRepaint && bRepaint)
		{
			relayoutpaintParent();
		}
	}

	FWIDGET_INLINE bool FWidget::isStateInVisible(void) const
	{
		return fooTestAttribute(m_dwState, FOOSTATE_INVISIBLE);
	}

	FWIDGET_INLINE void FWidget::setStateVisible(bool bRepaint)
	{
		bool bNeedRepaint = fooModifyAttribute<DWORD>(m_dwState, FOOSTATE_INVISIBLE, FOOSTATE_VISIBLE);
		if (bNeedRepaint)
		{
			m_pWidgetPrivate->broadcastEvent(Event::FEvent(Event::FEvent::Show), true);
		}

		if (bNeedRepaint && bRepaint)
		{
			relayoutpaintParent();
		}
	}

	FWIDGET_INLINE bool FWidget::isStateVisible(void) const
	{
		return fooTestAttribute(m_dwState, FOOSTATE_VISIBLE);
	}

	FWIDGET_INLINE void FWidget::setStateUnFocused(bool bRepaint)
	{
		modifyState(FOOSTATE_FOCUSED, FOOSTATE_UNFOCUSED, bRepaint);
	}

	FWIDGET_INLINE bool FWidget::isStateUnFocused(void) const
	{
		return fooTestAttribute(m_dwState, FOOSTATE_UNFOCUSED);
	}

	FWIDGET_INLINE void FWidget::setStateFocused(bool bRepaint)
	{
		modifyState(FOOSTATE_UNFOCUSED, FOOSTATE_FOCUSED, bRepaint);
	}

	FWIDGET_INLINE bool FWidget::isStateFocused(void) const
	{
		return fooTestAttribute(m_dwState, FOOSTATE_FOCUSED);
	}

	FWIDGET_INLINE void FWidget::setStateUnSelected(bool bRepaint)
	{
		modifyState(FOOSTATE_SELECTED, FOOSTATE_UNSELECTED, bRepaint);
	}

	FWIDGET_INLINE bool FWidget::isStateUnSelected(void) const
	{
		return fooTestAttribute(m_dwState, FOOSTATE_UNSELECTED);
	}

	FWIDGET_INLINE void FWidget::setStateSelected(bool bRepaint)
	{
		modifyState(FOOSTATE_UNSELECTED, FOOSTATE_SELECTED, bRepaint);
	}

	FWIDGET_INLINE bool FWidget::isStateSelected(void) const
	{
		return fooTestAttribute(m_dwState, FOOSTATE_SELECTED);
	}

	FWIDGET_INLINE void FWidget::setStateUnChecked(bool bRepaint)
	{
		if (modifyState(FOOSTATE_CHECKED, FOOSTATE_UNCHECKED, bRepaint))
		{
			makeWidgetMessage(FWM_CHECKED, (WPARAM)FALSE, 0L);
		}
	}

	FWIDGET_INLINE bool FWidget::isStateUnChecked(void) const
	{
		return fooTestAttribute(m_dwState, FOOSTATE_UNCHECKED);
	}

	FWIDGET_INLINE void FWidget::setStateChecked(bool bRepaint)
	{
		if (modifyState(FOOSTATE_UNCHECKED, FOOSTATE_CHECKED, bRepaint))
		{
			makeWidgetMessage(FWM_CHECKED, (WPARAM)TRUE, 0L);
		}
	}

	FWIDGET_INLINE bool FWidget::isStateChecked(void) const
	{
		return fooTestAttribute(m_dwState, FOOSTATE_CHECKED);
	}

	FWIDGET_INLINE void FWidget::setStateNonEmpty(bool bRepaint)
	{
		modifyState(FOOSTATE_EMPTY, FOOSTATE_NONEMPTY, bRepaint);
	}

	FWIDGET_INLINE bool FWidget::isStateNonEmpty(void) const
	{
		return fooTestAttribute(m_dwState, FOOSTATE_NONEMPTY);
	}

	FWIDGET_INLINE void FWidget::setStateEmpty(bool bRepaint)
	{
		modifyState(FOOSTATE_NONEMPTY, FOOSTATE_EMPTY, bRepaint);
	}

	FWIDGET_INLINE bool FWidget::isStateEmpty(void) const
	{
		return fooTestAttribute(m_dwState, FOOSTATE_EMPTY);
	}

	FWIDGET_INLINE void FWidget::setStateDeActivated(bool bRepaint/* = true*/)
	{
		modifyState(FOOSTATE_ACTIVATED, FOOSTATE_DEACTIVATED, bRepaint);
	}

	FWIDGET_INLINE bool FWidget::isStateDeActivated(void) const
	{
		return fooTestAttribute(m_dwState, FOOSTATE_DEACTIVATED);
	}

	FWIDGET_INLINE void FWidget::setStateActivated(bool bRepaint/* = true*/)
	{
		modifyState(FOOSTATE_DEACTIVATED, FOOSTATE_ACTIVATED, bRepaint);
	}

	FWIDGET_INLINE bool FWidget::isStateActivated(void) const
	{
		return fooTestAttribute(m_dwState, FOOSTATE_ACTIVATED);
	}

	/** 
	布局相关
	*/
	FWIDGET_INLINE void FWidget::relayout(void)
	{
		FASSERT(NULL != m_pWidgetPrivate);

		m_pWidgetPrivate->needLayout(true);
		::InvalidateRect(getSafeHwnd(), m_pWidgetPrivate->getLayoutRect(), false);
	}

	FWIDGET_INLINE void FWidget::updateLayout(void)
	{
		FASSERT(NULL != m_pWidgetPrivate);

		m_pWidgetPrivate->needLayout(true);
		::InvalidateRect(getSafeHwnd(), m_pWidgetPrivate->getLayoutRect(), false);
		::UpdateWindow(getSafeHwnd());
	}

	FWIDGET_INLINE void FWidget::relayoutParent(void)
	{
		if (NULL != getParent())
		{
			getParent()->relayout();
		}
	}

	FWIDGET_INLINE void FWidget::updateLayoutParent(void)
	{
		if (NULL != getParent())
		{
			getParent()->updateLayout();
		}
	}

	FWIDGET_INLINE bool FWidget::createScrollBar(int wBar, bool bShow)
	{
		if (SB_HORZ == wBar || SB_BOTH == wBar)
		{
			if (NULL == m_pHScrollBar)
			{
				m_pHScrollBar = new FScrollBar(FScrollBar::Horz);
				m_pHScrollBar->m_pWidgetPrivate->initParent(this, getTopLevelParent());
				m_pHScrollBar->autoDelete(true);
				m_pHScrollBar->setHeight(::GetSystemMetrics(SM_CYHSCROLL));
				m_pHScrollBar->show();
			}
			m_pHScrollBar->setVisible(bShow);
		}

		if (SB_VERT == wBar || SB_BOTH == wBar)
		{
			if (NULL == m_pVScrollBar)
			{
				m_pVScrollBar = new FScrollBar(FScrollBar::Vert);
				m_pVScrollBar->m_pWidgetPrivate->initParent(this, getTopLevelParent());
				m_pVScrollBar->autoDelete(true);
				m_pVScrollBar->setWidth(::GetSystemMetrics(SM_CXVSCROLL));
			}
			m_pVScrollBar->setVisible(bShow);
		}

		if (SB_BOTH == wBar)
		{
			return (NULL != m_pHScrollBar && NULL != m_pVScrollBar);
		}
		else if (SB_HORZ == wBar)
		{
			return (NULL != m_pHScrollBar);
		}
		else if (SB_VERT == wBar)
		{
			return (NULL != m_pVScrollBar);
		}

		return false;
	}

	FWIDGET_INLINE bool FWidget::showScrollBar(int wBar, bool bShow)
	{
		if ((SB_HORZ == wBar || SB_BOTH == wBar) 
			&& NULL != m_pHScrollBar)
		{
			m_pHScrollBar->setVisible(bShow);
		}

		if ((SB_VERT == wBar || SB_BOTH == wBar) 
			&& NULL != m_pVScrollBar)
		{
			m_pVScrollBar->setVisible(bShow);
		}

		if (SB_BOTH == wBar)
		{
			return (NULL != m_pHScrollBar && NULL != m_pVScrollBar);
		}
		else if (SB_HORZ == wBar)
		{
			return (NULL != m_pHScrollBar);
		}
		else if (SB_VERT == wBar)
		{
			return (NULL != m_pVScrollBar);
		}

		return false;
	}

	FWIDGET_INLINE bool FWidget::isShowScrollBar(int wBar) const
	{
		switch (wBar)
		{
		case SB_BOTH:
			return (isShowScrollBar(SB_HORZ) && isShowScrollBar(SB_VERT));
			break;

		case SB_HORZ:
			if (NULL != m_pHScrollBar && m_pHScrollBar->isVisible())
			{
				return true;
			}
			break;

		case SB_VERT:
			if (NULL != m_pVScrollBar && m_pVScrollBar->isVisible())
			{
				return true;
			}
			break;

		default:
			break;
		}

		return false;
	}

	FWIDGET_INLINE FScrollBar* FWidget::getScrollBar(int wBar) const
	{
		switch (wBar)
		{
		case SB_HORZ:
			return m_pHScrollBar;
			break;

		case SB_VERT:
			return m_pVScrollBar;
			break;

		default:
			break;
		}

		return NULL;
	}

	void FWidget::setLayoutRect(const Core::FRect& rcLayout)
	{
		FASSERT(NULL != m_pWidgetPrivate);

		/*if (m_pWidgetPrivate->getLayoutRect() == rcLayout 
			&& !m_pWidgetPrivate->isNeedLayout())
		{
			return;
		}*/

		m_pWidgetPrivate->setLayoutRect(rcLayout);

		layout(rcLayout);
	}
	void FWidget::setLayoutRectReal(const Core::FRect& rcLayout)
	{
		FASSERT(NULL != m_pWidgetPrivate);
		m_pWidgetPrivate->setLayoutRectReal(rcLayout);
	}

	FWIDGET_INLINE const Core::FRect& FWidget::getLayoutRect(void) const
	{
		FASSERT(NULL != m_pWidgetPrivate);
		return m_pWidgetPrivate->getLayoutRect();
	}

	FWIDGET_INLINE const Core::FRect& FWidget::getLayoutRectReal(void) const
	{
		FASSERT(NULL != m_pWidgetPrivate);
		return m_pWidgetPrivate->getLayoutRectReal();
	}

	FWIDGET_INLINE void FWidget::layout(void)
	{
		layout(getLayoutRect());
	}

	FWIDGET_INLINE void FWidget::beginLayout(void)
	{
		FASSERT(NULL != m_pWidgetPrivate);

		m_pWidgetPrivate->needLayout(false);
	}

	FWIDGET_INLINE void FWidget::endLayout(void)
	{
		FASSERT(NULL != m_pWidgetPrivate);
	}

	/** layout：对子控件重新布局
	位置优先级：
	1：dock,参考的值（margin、size(minsize、maxsize)
	2: position，参考的值（size(minsize、maxsize))
	3: align: 参考margin、align、size(minsize、maxsize)
	*/
	FWIDGET_INLINE void FWidget::layout(const Core::FRect& /*rcLayout*/)
	{
		FASSERT(NULL != m_pWidgetPrivate);

		beginLayout();

		/** 处理子控件为0的状态 */
		if (!hasChildren())
		{
			if (NULL != getScrollBar(SB_HORZ)) getScrollBar(SB_HORZ)->hide();
			if (NULL != getScrollBar(SB_VERT)) getScrollBar(SB_VERT)->hide();

			endLayout();
			return;
		}

		/** 客户区大小（可用于给子控件布局的区域） */
		Core::FRect rcClient;

		/** 滚动条的可视状态 */
		bool bIsShowHScrollBar = false;
		bool bIsShowVScrollBar = false;

		/** 滚动条所需的可视状态 */
		bool bIsNeedShowHScrollBar = false;
		bool bIsNeedShowVScrollBar = false;

		/** 需要滚动的区域 */
		Core::FSize szNeedScroll;

		/** 如果滚动条状态发生变化重新开始布局 */
DO_RELAYOUT:	

		/** 记录当前的滚动条可视状态（如果发生变化重新布局） */
		layoutScrollBar();
		bIsShowHScrollBar = isShowScrollBar(SB_HORZ);
		bIsShowVScrollBar = isShowScrollBar(SB_VERT);

		/** 获取客户区（有效布局区域） */
		getValidLayoutRect(&rcClient);

		Core::FRect rcUsable = rcClient;
		szNeedScroll.setEmpty();

		/** 给子控件布局 */
		for (int i = 0; i < getChildrenCount(); ++i)
		{
			FWidget *pWidget = getChild(i);
			FASSERT(NULL != pWidget);

			if (rcUsable.isEmpty())
			{
				pWidget->setLayoutRect(Core::FRect(0, 0, 0, 0));
				continue;
			}

			if (!pWidget->isVisible())
			{
				pWidget->setLayoutRect(Core::FRect(0, 0, 0, 0));
				continue;
			}

			Core::FPoint ptWidget;
			Core::FSize szWidget;
			pWidget->adjustLayoutRect(ptWidget, szWidget, rcUsable);

			switch (pWidget->getDock())
			{
			case FOODOCK_LEFT:
				rcUsable.left += szWidget.cx + pWidget->getMargin().edgeWidth();
				break;
			case FOODOCK_RIGHT:
				rcUsable.right -= szWidget.cx + pWidget->getMargin().edgeWidth();
				break;
			case FOODOCK_TOP:
				rcUsable.top += szWidget.cy + pWidget->getMargin().edgeHeight();
				break;
			case FOODOCK_BOTTOM:
				rcUsable.bottom -= szWidget.cy + pWidget->getMargin().edgeHeight();
				break;
			default:
				break;
			}

			/** 需要滚动的范围 */
			szNeedScroll.cx = max(szNeedScroll.cx, ptWidget.x - rcClient.left + szWidget.cx + pWidget->getMargin().right);
			szNeedScroll.cy = max(szNeedScroll.cy, ptWidget.y - rcClient.top + szWidget.cy + pWidget->getMargin().bottom);

			/** 如果需要显示滚动条， 重新布局 */
			bIsNeedShowHScrollBar = szNeedScroll.cx > rcClient.width();
			bIsNeedShowVScrollBar = szNeedScroll.cy > rcClient.height();

			if ((bIsNeedShowHScrollBar && !bIsShowHScrollBar)
				|| (bIsNeedShowVScrollBar && !bIsShowVScrollBar))
			{
				bool bNeedReLayout = false;
				if ((bIsNeedShowHScrollBar && !bIsShowHScrollBar) 
					&& NULL != getScrollBar(SB_HORZ))
				{
					showScrollBar(SB_HORZ, bIsNeedShowHScrollBar);
					bNeedReLayout = true;
				}

				if ((bIsNeedShowVScrollBar && !bIsShowVScrollBar) 
					&& NULL != getScrollBar(SB_VERT))
				{
					showScrollBar(SB_VERT, bIsNeedShowVScrollBar);
					bNeedReLayout = true;
				}

				if (bNeedReLayout)
				{
					goto DO_RELAYOUT;
				}
			}

			Core::FRect rcWidget(ptWidget, szWidget);

			if (isShowScrollBar(SB_HORZ))
			{
				rcWidget.offsetX(-getScrollBar(SB_HORZ)->sliderPosition());
			}

			if (isShowScrollBar(SB_VERT))
			{
				rcWidget.offsetY(-getScrollBar(SB_VERT)->sliderPosition());
			}

			pWidget->setLayoutRect(rcWidget);
		} // !for (int i = 0; i < getChildrenCount(); ++i)

		if (!rcUsable.isEmpty())
		{
			/** 如果滚动条需要隐藏或显示，重新布局 */
			/** 如果需要显示滚动条， 重新布局 */
			bIsNeedShowHScrollBar = szNeedScroll.cx > rcClient.width();
			bIsNeedShowVScrollBar = szNeedScroll.cy > rcClient.height();

			if (bIsNeedShowHScrollBar != bIsShowHScrollBar
				|| bIsNeedShowVScrollBar != bIsShowVScrollBar)
			{
				bool bNeedReLayout = false;
				if (bIsNeedShowHScrollBar != bIsShowHScrollBar 
					&& NULL != getScrollBar(SB_HORZ))
				{
					showScrollBar(SB_HORZ, bIsNeedShowHScrollBar);
					bNeedReLayout = true;
				}

				if (bIsNeedShowVScrollBar != bIsShowVScrollBar
					&& NULL != getScrollBar(SB_VERT))
				{
					showScrollBar(SB_VERT, bIsNeedShowVScrollBar);
					bNeedReLayout = true;
				}

				if (bNeedReLayout)
				{
					goto DO_RELAYOUT;
				}
			}
		} // !if (!rcUsable.isEmpty())

		if (NULL != getScrollBar(SB_HORZ))
		{
			getScrollBar(SB_HORZ)->setRange(0, szNeedScroll.cx - rcClient.width());
			getScrollBar(SB_HORZ)->setPage(rcClient.width());
		}
		if (NULL != getScrollBar(SB_VERT))
		{
			getScrollBar(SB_VERT)->setRange(0, szNeedScroll.cy - rcClient.height());
			getScrollBar(SB_VERT)->setPage(rcClient.height());
		}

		endLayout();
	}

	void FWidget::layoutScrollBar(void)
	{
		bool bIsShowHScrollBar = isShowScrollBar(SB_HORZ);
		bool bIsShowVScrollBar = isShowScrollBar(SB_VERT);

		FScrollBar *pHScrollBar = getScrollBar(SB_HORZ);
		FScrollBar *pVScrollBar = getScrollBar(SB_VERT);

		Core::FRect rcUsable = getLayoutRect();
		rcUsable.deflateRect(getPadding());

		Core::FRect rcHScroll;
		Core::FRect rcVScroll;

		if (bIsShowHScrollBar && bIsShowVScrollBar)
		{
			rcHScroll.setRect(rcUsable.left + pHScrollBar->getMargin().left, 
				rcUsable.bottom - pHScrollBar->getHeight() - pHScrollBar->getMargin().bottom, 
				rcUsable.right - pHScrollBar->getMargin().right - pVScrollBar->getWidth() - pVScrollBar->getMargin().right, 
				rcUsable.bottom - pHScrollBar->getMargin().bottom);

			rcVScroll.setRect(rcUsable.right - pVScrollBar->getWidth() - pVScrollBar->getMargin().right, 
				rcUsable.top + pVScrollBar->getMargin().top, 
				rcUsable.right - pVScrollBar->getMargin().right, 
				rcUsable.bottom - pVScrollBar->getMargin().bottom - pHScrollBar->getHeight() - pHScrollBar->getMargin().bottom);
		}
		else if (bIsShowHScrollBar)
		{
			rcHScroll.setRect(rcUsable.left + pHScrollBar->getMargin().left, 
				rcUsable.bottom - pHScrollBar->getHeight() - pHScrollBar->getMargin().bottom, 
				rcUsable.right - pHScrollBar->getMargin().right, 
				rcUsable.bottom - pHScrollBar->getMargin().bottom);
		}
		else if (bIsShowVScrollBar)
		{
			rcVScroll.setRect(rcUsable.right - pVScrollBar->getWidth() - pVScrollBar->getMargin().right, 
				rcUsable.top + pVScrollBar->getMargin().top, 
				rcUsable.right - pVScrollBar->getMargin().right, 
				rcUsable.bottom - pVScrollBar->getMargin().bottom);
		}

		if (bIsShowHScrollBar) pHScrollBar->setLayoutRect(rcHScroll);
		if (bIsShowVScrollBar) pVScrollBar->setLayoutRect(rcVScroll);
	}

	void FWidget::adjustLayoutRect(Core::FPoint& position, Core::FSize& size, const Core::FRect& rcUsable) const
	{
		/** 计算水平宽度 */
		if (isFixedWidth())
		{
			size.cx = m_size.cx;
		}
		else
		{
			size.cx = rcUsable.width() - getMargin().edgeWidth();
			if (m_minsize.cx > size.cx) size.cx = m_minsize.cx;
			if (m_maxsize.cx > 0 && m_maxsize.cx < size.cx) size.cx = m_maxsize.cx;
		}

		/** 计算垂直高度 */
		if (isFixedHeight())
		{
			size.cy = m_size.cy;
		}
		else
		{
			size.cy = rcUsable.height() - getMargin().edgeHeight();
			if (m_minsize.cy > size.cy) size.cy = m_minsize.cy;
			if (m_maxsize.cy > 0 && m_maxsize.cy < size.cy) size.cy = m_maxsize.cy;
		}

		/** 计算水平坐标 */
		if (isFixedXPosition())
		{
			if (isRelativePosition())
				position.x = rcUsable.left + getPosition().x + getMargin().left;
			else
				position.x = getPosition().x + getMargin().left;
		}
		else if (FOODOCK_RIGHT == getDock() 
			|| fooTestAttribute(getAlign(), FOOALIGN_RIGHT))
		{
			position.x = rcUsable.right - size.cx - getMargin().right;
		}
		else if (fooTestAttribute(getAlign(), FOOALIGN_XCENTER))
		{
			position.x = rcUsable.left + (rcUsable.width() - size.cx) / 2 + getMargin().left;
		}
		else
		{
			position.x = rcUsable.left + getMargin().left;
		}

		/** 计算垂直坐标 */
		if (isFixedYPosition())
		{
			if (isRelativePosition())
				position.y = rcUsable.top + getPosition().y + getMargin().top;
			else
				position.y = getPosition().y + getMargin().top;
		}
		else if (FOODOCK_BOTTOM == getDock() 
			|| fooTestAttribute(getAlign(), FOOALIGN_BOTTOM))
		{
			position.y = rcUsable.bottom - size.cy - getMargin().bottom;
		}
		else if (fooTestAttribute(getAlign(), FOOALIGN_YCENTER))
		{
			position.y = rcUsable.top + (rcUsable.height() - size.cy) / 2 + getMargin().top;
		}
		else
		{
			position.y = rcUsable.top + getMargin().top;
		}
	}

	/** 
		更新相关
	*/
	FWIDGET_INLINE void FWidget::repaint(const RECT* rect/* = NULL*/)
	{
		FWidget *pWindowHost = getHostWindow();
		//FASSERT(NULL != pWindowHost);

		if (NULL == rect)
		{
			/** 不可见 */
			if (!isVisible())
			{
				return;
			}

			if (isTransparent() 
				&& getChildrenCount() == 0)
			{
				return;
			}
		}

		if (NULL != pWindowHost)
		{
			pWindowHost->m_pWidgetPrivate->needUpdate(true);
			::InvalidateRect(pWindowHost->getHwnd(), NULL == rect ? &getLayoutRect() : rect, true);
		}
		
		if (this != pWindowHost && isWindow())
		{
			::InvalidateRect(getHwnd(), NULL == rect ? &getLayoutRect() : rect, true);
		}
	}

	FWIDGET_INLINE void FWidget::update(const RECT* rect/* = NULL*/)
	{
		FWidget *pWindowHost = getHostWindow();
		//FASSERT(NULL != pWindowHost);

		if (NULL == rect)
		{
			if (!isVisible() || isTransparent())
				return;
		}

		if (NULL != pWindowHost)
		{
			pWindowHost->m_pWidgetPrivate->needUpdate(true);
			::InvalidateRect(pWindowHost->getHwnd(), NULL == rect ? &getLayoutRect() : rect, true);
			::UpdateWindow(pWindowHost->getHwnd());
		}

		if (this != pWindowHost && isWindow())
		{
			::InvalidateRect(getHwnd(), NULL == rect ? &getLayoutRect() : rect, true);
			::UpdateWindow(getHwnd());
		}
	}

	FWIDGET_INLINE void FWidget::relayoutpaint(const RECT* rect/* = NULL*/)
	{
		FWidget *pWindowHost = getHostWindow();
		//FASSERT(NULL != pWindowHost);

		if (NULL != pWindowHost)
		{
			pWindowHost->m_pWidgetPrivate->needLayout(true);
			pWindowHost->m_pWidgetPrivate->needUpdate(true);
			m_pWidgetPrivate->needLayout(true);
			m_pWidgetPrivate->needUpdate(true);

			::InvalidateRect(pWindowHost->getHwnd(), NULL == rect ? &getLayoutRectReal() : rect, true);
		}
		
		if (this != pWindowHost && isWindow())
		{
			::InvalidateRect(getHwnd(), NULL == rect ? &getLayoutRectReal() : rect, true);
		}
	}

	FWIDGET_INLINE void FWidget::relayoutpaintParent(const RECT* rect/* = NULL*/)
	{
		if (NULL != getParent())
		{
			getParent()->relayoutpaint(rect);
		}
	}

	FWIDGET_INLINE void FWidget::updatelayoutpaint(const RECT* rect/* = NULL*/)
	{
		if (NULL != getParent())
		{
			getParent()->updatelayoutpaint(rect);
		}
	}

	FWIDGET_INLINE void FWidget::updatelayoutpaintParent(const RECT* rect/* = NULL*/)
	{
		FWidget *pWindowHost = getHostWindow();
		//FASSERT(NULL != pWindowHost);

		if (NULL != pWindowHost)
		{
			pWindowHost->m_pWidgetPrivate->needLayout(true);
			pWindowHost->m_pWidgetPrivate->needUpdate(true);
			m_pWidgetPrivate->needLayout(true);
			::InvalidateRect(pWindowHost->getHwnd(), NULL == rect ? &getLayoutRect() : rect, true);
			::UpdateWindow(pWindowHost->getHwnd());
		}
		
		if (this != pWindowHost && isWindow())
		{
			::InvalidateRect(getHwnd(), NULL == rect ? &getLayoutRect() : rect, true);
			::UpdateWindow(getHwnd());
		}
	}

	/** 
		窗口相关
	*/
	FWIDGET_INLINE bool FWidget::createWindow(HWND hParentWnd/* = NULL*/)
	{
		FASSERT(NULL != m_pWidgetPrivate);
		return m_pWidgetPrivate->createWindow(hParentWnd);
	}

	FWIDGET_INLINE bool FWidget::createWindow(
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
		FASSERT(NULL != m_pWidgetPrivate);
		return m_pWidgetPrivate->createWindow(dwExStyle, 
			lpClassName, 
			lpWindowName, 
			dwStyle, 
			x, 
			y, 
			nWidth, 
			nHeight, 
			hWndParent, 
			hMenu, 
			lpCreateParams);
	}

	FWIDGET_INLINE bool FWidget::destroyWindow(void)
	{
		FASSERT(NULL != m_pWidgetPrivate);
		return m_pWidgetPrivate->destroyWindow();
	}

	FWIDGET_INLINE UINT FWidget::doModal(HWND hWndParent/* = NULL*/)
	{
		FASSERT(NULL != m_pWidgetPrivate);
		return m_pWidgetPrivate->doModal(hWndParent);
	}

	FWIDGET_INLINE UINT FWidget::doModaless(HWND hWndParent/* = NULL*/)
	{
		FASSERT(NULL != m_pWidgetPrivate);
		return m_pWidgetPrivate->doModal(hWndParent, true);
	}

	FWIDGET_INLINE void FWidget::endModal(UINT uRetCode)
	{
		FASSERT(NULL != m_pWidgetPrivate);
		m_pWidgetPrivate->endModal(uRetCode);
	}

	FWIDGET_INLINE HWND FWidget::getHwnd(void) const
	{
		FASSERT(NULL != m_pWidgetPrivate);
		return m_pWidgetPrivate->getHwnd();
	}

	FWIDGET_INLINE HWND FWidget::getSafeHwnd(void) const
	{
		if (NULL != isWindow()) return getHwnd();

		FWidget *parent = getParent();
		while (NULL != parent)
		{
			if (NULL != parent->isWindow())
			{
				return parent->getHwnd();
			}

			parent = parent->getParent();
		}
		return NULL;
	}

	FWIDGET_INLINE FWidget* FWidget::getHostWindow(void) const
	{
		FASSERT(NULL != m_pWidgetPrivate);

		FWidget *parent = getParent();
		while (NULL != parent)
		{
			if (NULL != parent->isWindow())
			{
				return parent;
			}

			parent = parent->getParent();
		}

		if (isWindow()) return (FWidget*)this;
		return NULL;
	}

	FWIDGET_INLINE HWND FWidget::getHostWindowHwnd(void) const
	{
		FASSERT(NULL != m_pWidgetPrivate);

		FWidget *pWindowHost = getHostWindow();
		if (NULL != pWindowHost)
		{
			return pWindowHost->getHwnd();
		}
		else
		{
			return NULL;
		}
	}

	FWIDGET_INLINE HWND FWidget::getTopLevelHwnd(void) const
	{
		if (NULL != getTopLevelParent())
		{
			return getTopLevelParent()->getHwnd();
		}

		return getHwnd();
	}

	FWIDGET_INLINE bool FWidget::isWindow(void) const
	{
		FASSERT(NULL != m_pWidgetPrivate);
		return m_pWidgetPrivate->isWindow();
	}

	FWIDGET_INLINE FToolTip* FWidget::getToolTip(void) const
	{
		return m_tooltip;
	}

	FWIDGET_INLINE void FWidget::replaceToolTip(FToolTip* tooltip)
	{
		if (NULL != m_tooltip 
			&& m_tooltip->isAutoDelete())
		{
			fooSafeDelete(m_tooltip);
		}

		m_tooltip = tooltip;
	}

	FWIDGET_INLINE /*static*/ FToolTip* FWidget::getGlobalToolTip(void)
	{
		return FModule::tooltip();
	}

	FWIDGET_INLINE /*static*/ bool FWidget::replaceGlobalToolTip(FToolTip* tooltip)
	{
		return FModule::replaceToolTip(tooltip);
	}

	/**
	消息处理
	*/
	FWIDGET_INLINE LRESULT FWidget::defWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		FASSERT(NULL != m_pWidgetPrivate);
		if (NULL != m_pWidgetPrivate->m_pWindow)
		{
			return m_pWidgetPrivate->m_pWindow->defWindowProc(uMsg, wParam, lParam);
		}
		else
		{
			return 0;
		}
	}

	FWIDGET_INLINE LRESULT FWidget::processWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		FASSERT(NULL != m_pWidgetPrivate);
		FASSERT(NULL != m_pWidgetPrivate->m_pWindow);

		bool bHandled = false;
		LRESULT lResult = handleMessage(uMsg, wParam, lParam, bHandled);

		switch (uMsg)
		{
		case WM_CREATE:
			{
				Event::FWindowCreatedEvent windowCreatedEvent((LPCREATESTRUCT)lParam);
				event(FOBJECT_CAST(Event::FEvent*, &windowCreatedEvent));
				if (m_pWidgetPrivate->m_wndstyle.center) center();

				m_pWidgetPrivate->broadcastEvent(Event::FParentWindowEvent(Event::FEvent::ParentWindowCreated, getHwnd()));
			
				/*FWidget *pNextTabWidget = getNextTabWidget();
				if (NULL != pNextTabWidget)
				{
					pNextTabWidget->setFocus();
				}*/

				DWORD dwStyle = ::GetWindowLong(getHwnd(), GWL_STYLE);
				DWORD dwNewStyle = dwStyle;
				if ((m_pWidgetPrivate->m_wndstyle.dwStyle & WS_CAPTION) == 0) dwNewStyle &= ~WS_CAPTION;
				if (dwNewStyle != dwStyle)
				{
					::SetWindowLong(getHwnd(), GWL_STYLE, dwNewStyle);
					::SetWindowPos(getHwnd(), NULL, 0, 0, 0, 0, 
								SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_FRAMECHANGED);
				}
			}
			break;

		case WM_ERASEBKGND:
			if (!bHandled)
			{
				bHandled = true;
				return 0;
			}
			break;

		case WM_PAINT:
			if (!bHandled)
			{
				Core::FRect rcClient;
				::GetClientRect(getHwnd(), &rcClient);

				Core::FRect rcUpdate;
				::GetUpdateRect(getHwnd(), &rcUpdate, NULL);
				if (!rcUpdate.isEmpty() && m_pWidgetPrivate->isNeedUpdate())
				{
					/** 设置是否是分层窗口 */
					m_pWidgetPrivate->m_pWindow->m_graphics.flush();
					m_pWidgetPrivate->m_pWindow->m_graphics.m_graphicsPrivate->setLayered(m_pWidgetPrivate->isLayeredWindow());
					m_pWidgetPrivate->m_pWindow->m_graphics.clear(rcUpdate);
					m_pWidgetPrivate->m_pWindow->m_graphics.clipRect(rcUpdate);

					/** paint_event */
					Event::FPaintEvent e(&m_pWidgetPrivate->m_pWindow->m_graphics, rcUpdate);
					m_pWidgetPrivate->broadcastPaintEvent(&e);

					m_pWidgetPrivate->m_pWindow->m_graphics.clearClip();
					m_pWidgetPrivate->endUpdate();
				}

				PAINTSTRUCT ps;
				::BeginPaint(getHwnd(), &ps);

				if (m_pWidgetPrivate->isLayeredWindow())
				{
					m_pWidgetPrivate->m_pWindow->m_graphics.updateLayered(getHwnd(), rcClient);
				}
				else
				{
					m_pWidgetPrivate->m_pWindow->m_graphics.update(ps.hdc, rcClient);
				}

				::EndPaint(getHwnd(), &ps);
				bHandled = true;
				return 0;
			}
			break;

		case WM_NCCALCSIZE:
			if (!m_pWidgetPrivate->m_wndstyle.border 
				|| m_pWidgetPrivate->isLayeredWindow())
			{
				bHandled = true;
				return 0;
			}
			break;

		case WM_NCACTIVATE:
			{
				if (wParam)
				{
					setStateFocused(true);
					setStateActivated(true);
					FWidgetPrivate::fActiveWidget = this;
				}
				else
				{
					setStateUnFocused(true);
					setStateDeActivated(true);

					FWidgetPrivate::fActiveWidget = NULL;
				}

				if (!m_pWidgetPrivate->m_wndstyle.border 
					|| m_pWidgetPrivate->isLayeredWindow())
				{
					bHandled = true;
					return wParam ? FALSE : TRUE;
				}
			}
			break;

		case WM_NCHITTEST:
			if (!bHandled)
			{
				Core::FRect rcWindow;
				::GetWindowRect(getHwnd(), &rcWindow);
				Core::FPoint ptScreen(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

				LRESULT lResult = 0;
				if (m_pWidgetPrivate->hittest(rcWindow, ptScreen, lResult))
				{
					bHandled = true;
					return lResult;
				}

				Core::FPoint ptMouse = ptScreen;
				::ScreenToClient(getHwnd(), &ptMouse);
				FWidget *pMouseWidget = findWidget(ptMouse);
				if (NULL != pMouseWidget)
				{
					if (FOOROLE_SIZEGRIP == pMouseWidget->getRole() 
						&& fooTestAttribute(m_pWidgetPrivate->m_pWindow->getStyle(), WS_THICKFRAME))
					{
						bHandled = true;
						return HTBOTTOMRIGHT;
					}
				}

				// 如果没有边框， 返回HTCLIENT，防止系统处理为非客户区事件
				if (!m_pWidgetPrivate->m_wndstyle.border)
				{
					bHandled = true;
					return HTCLIENT;
				}
			}
			break;

		case WM_SETCURSOR:
			if (LOWORD(lParam) == HTCLIENT)
			{
				Core::FPoint ptCursor;
				::GetCursorPos(&ptCursor);

				Core::FPoint ptMouse(ptCursor);
				::ScreenToClient(getHwnd(), &ptMouse);
				FWidget *pWidget = findWidget(ptMouse);

				if (NULL != pWidget 
					&& pWidget->isEnabled())
				{
					Event::FCursorEvent e(ptMouse, ptCursor);
					pWidget->event(&e);
					if (e.isAccept())
					{
						bHandled = true;
						return TRUE;
					}
				}
				else
				{
					Event::FCursorEvent e(ptMouse, ptCursor);
					event(&e);
					if (e.isAccept())
					{
						bHandled = true;
						return TRUE;
					}
				}
			}
			break;

		case WM_GETMINMAXINFO:
			{
				LPMINMAXINFO lpMinMaxInfo = (LPMINMAXINFO)lParam;
				FASSERT(NULL != lpMinMaxInfo);

				if (getMinWidth() > 0) lpMinMaxInfo->ptMinTrackSize.x = getMinWidth();
				if (getMinHeight() > 0) lpMinMaxInfo->ptMinTrackSize.y = getMinHeight();
				if (getMaxWidth() > 0) lpMinMaxInfo->ptMaxTrackSize.x = getMaxWidth();
				if (getMaxHeight() > 0) lpMinMaxInfo->ptMaxTrackSize.y = getMaxHeight();
			}
			break;

		case WM_SIZE:
			if (SIZE_MINIMIZED != wParam)
			{
				Core::FRect rcWindow;
				::GetWindowRect(getHwnd(), &rcWindow);
				m_stylesheet.createWindowRegion(getHwnd(), getState(), 
					0, 0, rcWindow.width(), rcWindow.height(), _T(""), _T(""));

				Core::FRect rcClient;
				::GetClientRect(getHwnd(), &rcClient);
				m_pWidgetPrivate->m_pWindow->m_graphics.resize(rcClient.width(), rcClient.height());
				m_pWidgetPrivate->m_rcLayout = rcClient;
				m_pWidgetPrivate->needLayout(true);
				m_pWidgetPrivate->needUpdate(true);
				::InvalidateRect(getHwnd(), &rcClient, FALSE);
				//relayoutpaint(rcClient);
			}
			m_pWidgetPrivate->broadcastEvent(Event::FParentWindowEvent(Event::FEvent::ParentWindowSized, getHwnd()));
			break;

			/** 鼠标相关消息 */
		case WM_MOUSEHOVER:
			{
				if (!isSkipMouse())
				{
					event(Event::FEvent(Event::FEvent::MouseEnter));
				}

				Core::FPoint ptMouse( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
				FWidget *pWidget = findWidget(ptMouse);

				UINT button = NoButton;
				if (fooTestAttribute(wParam, MK_LBUTTON)) button = LeftButton;
				else if (fooTestAttribute(wParam, MK_LBUTTON)) button = LeftButton;
				else if (fooTestAttribute(wParam, MK_MBUTTON)) button = MidButton;
				else if (fooTestAttribute(wParam, MK_RBUTTON)) button = RightButton;
				else if (fooTestAttribute(wParam, MK_XBUTTON1)) button = XButton1;
				else if (fooTestAttribute(wParam, MK_XBUTTON2)) button = XButton2;

				FToolTip *pToolTip = getToolTip();
				if (NULL == pToolTip) pToolTip = getGlobalToolTip();
				FASSERT(NULL != pToolTip);
				if (pToolTip != getGlobalToolTip())
				{
					getGlobalToolTip()->hideToolTip();
				}

				if (NULL != pWidget 
					&& pWidget->isEnabled() 
					&& _tcslen(pWidget->getToolTipText()) > 0 
					&& NoButton == button)
				{
					FToolTip *pToolTip = pWidget->getToolTip();
					if (NULL == pToolTip) pToolTip = getGlobalToolTip();

					FASSERT(NULL != pToolTip);
					if (pToolTip != getGlobalToolTip()) getGlobalToolTip()->hideToolTip();

					pToolTip->showToolTip(pWidget->getToolTipText(), pWidget);
				}
				else
				{
					if (NULL != getGlobalToolTip()) getGlobalToolTip()->hideToolTip();
				}

				m_pWidgetPrivate->m_pWindow->endTrackMouseEvent();
			}
			break;

		case WM_MOUSELEAVE:
			{
				m_pWidgetPrivate->m_pWindow->endTrackMouseEvent();

				if (NULL != getGlobalToolTip()) getGlobalToolTip()->hideToolTip();
				if (NULL != FWidgetPrivate::fHoverWidget
					&& NULL != FWidgetPrivate::fHoverWidget->getToolTip())
				{
					FWidgetPrivate::fHoverWidget->getToolTip()->hideToolTip();
				}

				if (NULL != FWidgetPrivate::fHoverWidget)
				{
					FWidgetPrivate::fHoverWidget->event(Event::FEvent(Event::FEvent::MouseLeave));
				}

				if (this != FWidgetPrivate::fHoverWidget)
				{
					event(Event::FEvent(Event::FEvent::MouseLeave));
				}

				FWidgetPrivate::fHoverWidget = NULL;
			}
			break;

		case WM_MOUSEWHEEL:
			{
				int nFlag = GET_KEYSTATE_WPARAM(wParam);
				int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

				Core::FPoint ptMouse(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				::ScreenToClient(getHwnd(), &ptMouse);
				FWidget *pMouseWidget = findScrollView(ptMouse);
				if (NULL != pMouseWidget)
				{
					int nButton = 0;
					if (fooTestAttribute(nFlag, MK_LBUTTON)) nButton |= MK_LBUTTON;
					if (fooTestAttribute(nFlag, MK_RBUTTON)) nButton |= MK_RBUTTON;
					if (fooTestAttribute(nFlag, MK_MBUTTON)) nButton |= MK_MBUTTON;
					if (fooTestAttribute(nFlag, MK_XBUTTON1)) nButton |= MK_XBUTTON1;
					if (fooTestAttribute(nFlag, MK_XBUTTON2)) nButton |= MK_XBUTTON2;
					pMouseWidget->event(Event::FWheelEvent(ptMouse, nButton, zDelta));
				}

			}
			break;

		case WM_MOUSEMOVE:
			{
				m_pWidgetPrivate->m_pWindow->trackMouseEvent(TME_HOVER | TME_LEAVE, 400);

				Core::FPoint ptMouse(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				Core::FPoint ptScreen;
				::GetCursorPos(&ptScreen);
				UINT uButton = (UINT)wParam;

				if (uButton == NoButton)
				{
					/** 鼠标没有按下按键在控件上滑动 */
					FWidget *pMouseWidget = findWidget(ptMouse);
					if (pMouseWidget != FWidgetPrivate::fHoverWidget)
					{
						/** 滑动到新控件上 */
						if (NULL != FWidgetPrivate::fHoverWidget)
						{
							FWidgetPrivate::fHoverWidget->event(Event::FEvent(Event::FEvent::MouseLeave));
						}

						if (NULL != pMouseWidget)
						{
							pMouseWidget->event(Event::FEvent(Event::FEvent::MouseEnter));
						}

						if (NULL != getGlobalToolTip()) getGlobalToolTip()->hideToolTip();
						if (NULL != FWidgetPrivate::fHoverWidget
							&& NULL != FWidgetPrivate::fHoverWidget->getToolTip())
						{
							FWidgetPrivate::fHoverWidget->getToolTip()->hideToolTip();
						}

						FWidgetPrivate::fHoverWidget = pMouseWidget;
					}
					else if (NULL != FWidgetPrivate::fHoverWidget 
						&& FWidgetPrivate::fHoverWidget->isEnabled())
					{
						/** 在旧的控件上滑动 */
						FWidgetPrivate::fHoverWidget->event(Event::FMouseEvent(Event::FEvent::MouseMove, uButton, ptMouse, ptScreen));
					}
				}
				else if (NULL != FWidgetPrivate::fPushedWidget 
					&& FWidgetPrivate::fPushedWidget->isEnabled()) /** 鼠标按下在控件上滑动 */
				{
					FWidgetPrivate::fPushedWidget->event(Event::FMouseEvent(Event::FEvent::MouseMove, uButton, ptMouse, ptScreen));
				}
			}
			break;

		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_XBUTTONDBLCLK:
			{
				Core::FPoint ptMouse(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				Core::FPoint ptScreen;
				::GetCursorPos(&ptScreen);
				UINT uButton = NoButton;
				if (WM_LBUTTONDBLCLK == uMsg) uButton = LeftButton;
				if (WM_MBUTTONDBLCLK == uMsg) uButton = MidButton;
				if (WM_RBUTTONDBLCLK == uMsg) uButton = RightButton;
				if (WM_XBUTTONDBLCLK == uMsg && fooTestAttribute(wParam, MK_XBUTTON1)) uButton = XButton1;
				if (WM_XBUTTONDBLCLK == uMsg && fooTestAttribute(wParam, MK_XBUTTON2)) uButton = XButton2;

				FWidget *pMouseWidget = findWidget(ptMouse);
				if (NULL != pMouseWidget 
					&& pMouseWidget->isEnabled())
				{
					pMouseWidget->event(Event::FMouseEvent(Event::FEvent::MouseButtonDblClick, uButton, ptMouse, ptScreen));
				
					/** 控件角色作用 */
					if (LeftButton == uButton 
						&& FOOROLE_CAPTION == pMouseWidget->getRole())
					{
						::ReleaseCapture();
						::SendMessage(getHwnd(), WM_NCLBUTTONDBLCLK, (WPARAM)HTCAPTION, 0L);
					}
				}

				FWidgetPrivate::fPushedWidget = pMouseWidget;
			}
			break;

		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_XBUTTONDOWN:
			{
				if (NULL != getGlobalToolTip()) getGlobalToolTip()->hide();
				if (NULL != getToolTip()) getToolTip()->hideToolTip();

				Core::FPoint ptMouse(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				Core::FPoint ptScreen;
				::GetCursorPos(&ptScreen);
				UINT uButton = NoButton;
				if (WM_LBUTTONDOWN == uMsg) uButton = LeftButton;
				if (WM_MBUTTONDOWN == uMsg) uButton = MidButton;
				if (WM_RBUTTONDOWN == uMsg) uButton = RightButton;
				if (WM_XBUTTONDOWN == uMsg && fooTestAttribute(wParam, MK_XBUTTON1)) uButton = XButton1;
				if (WM_XBUTTONDOWN == uMsg && fooTestAttribute(wParam, MK_XBUTTON2)) uButton = XButton2;

				FWidget *pMouseWidget = findWidget(ptMouse);
				if (NULL != pMouseWidget 
					&& pMouseWidget->isEnabled())
				{
					if (this != pMouseWidget
						&& !pMouseWidget->isSkipFocus())
					{
						pMouseWidget->focus();
					}

					pMouseWidget->event(Event::FMouseEvent(Event::FEvent::MouseButtonPress, uButton, ptMouse, ptScreen));

					/** 控件角色作用 */
					if (LeftButton == uButton 
						&& FOOROLE_CAPTION == pMouseWidget->getRole())
					{
						::ReleaseCapture();
						::SendMessage(getHwnd(), WM_NCLBUTTONDOWN, (WPARAM)HTCAPTION, 0L);
					}
				}

				FWidgetPrivate::fPushedWidget = pMouseWidget;
			}
			break;

		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_XBUTTONUP:
			{
				Core::FPoint ptMouse(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				Core::FPoint ptScreen;
				::GetCursorPos(&ptScreen);
				UINT uButton = NoButton;
				if (WM_LBUTTONUP == uMsg) uButton = LeftButton;
				if (WM_MBUTTONUP == uMsg) uButton = MidButton;
				if (WM_RBUTTONUP == uMsg) uButton = RightButton;
				if (WM_XBUTTONUP == uMsg && fooTestAttribute(wParam, MK_XBUTTON1)) uButton = XButton1;
				if (WM_XBUTTONUP == uMsg && fooTestAttribute(wParam, MK_XBUTTON2)) uButton = XButton2;

				FWidget *pMouseWidget = findWidget(ptMouse);

				if (NULL != FWidgetPrivate::fPushedWidget 
					&& FWidgetPrivate::fPushedWidget->isEnabled())
				{
					FWidgetPrivate::fPushedWidget->event(Event::FMouseEvent(Event::FEvent::MouseButtonRelease, uButton, ptMouse, ptScreen));
				}

				if (NULL != pMouseWidget 
					&& pMouseWidget->isEnabled() 
					&& pMouseWidget == FWidgetPrivate::fPushedWidget)
				{
					pMouseWidget->event(Event::FMouseEvent(Event::FEvent::MouseEnter, uButton, ptMouse, ptScreen));
					pMouseWidget->event(Event::FMouseEvent(Event::FEvent::MouseButtonClick, uButton, ptMouse, ptScreen));
				}

				::PostMessage(getHwnd(), WM_MOUSEMOVE, 0, lParam);

				FWidgetPrivate::fPushedWidget = NULL;
			}
			break;

			/** 键盘相关消息 */
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			{
				Event::FKeyEvent e(Event::FEvent::KeyPress, wParam, lParam);
				if (NULL != FWidgetPrivate::fFocusedWidget 
					&& FWidgetPrivate::fFocusedWidget->getSafeHwnd() == getHwnd())
				{
					FWidgetPrivate::fFocusedWidget->event(&e);
				}
				else
				{
					event(&e);
				}
			}
			break;

		case WM_CHAR:
			{
				Event::FCharEvent e(Event::FEvent::KeyChar, (UINT)wParam);
				if (NULL != FWidgetPrivate::fFocusedWidget 
					&& FWidgetPrivate::fFocusedWidget->getSafeHwnd() == getHwnd())
				{
					FWidgetPrivate::fFocusedWidget->event(&e);
				}
				else
				{
					event(&e);
				}

				if (e.isAccept())
				{
					bHandled = true;
					return TRUE;
				}

				if (VK_RETURN == wParam)
				{
					FWNMHDR hdr;
					hdr.bHandled = false;
					makeWidgetMessage(FWM_NOTIFY, NC_RETURN, (LPARAM)&hdr);
					if (hdr.bHandled)
					{
						bHandled = hdr.bHandled;
						return TRUE;
					}
				}
				else if (VK_TAB == wParam)
				{
					FWidget *pNextTabWidget = getNextTabWidget();
					if (NULL != pNextTabWidget)
					{
						pNextTabWidget->setFocus();
						bHandled = true;
						return TRUE;
					}
				}
			}
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			{
				Event::FKeyEvent e(Event::FEvent::KeyRelease, wParam, lParam);
				if (NULL != FWidgetPrivate::fFocusedWidget 
					&& FWidgetPrivate::fFocusedWidget->getSafeHwnd() == getHwnd())
				{
					FWidgetPrivate::fFocusedWidget->event(&e);
				}
				else
				{
					event(&e);
				}
			}
			break;

		default:
			break;
		}

		if (bHandled) 
			return lResult;
		else
			return defWindowProc(uMsg, wParam, lParam);
	}

	FWIDGET_INLINE LRESULT FWidget::makeWidgetMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return makeWidgetMessage((FWidget*)this, getID(), uMsg, wParam, lParam);
	}

	FWIDGET_INLINE LRESULT FWidget::makeWidgetMessage(FWidget* pSender, UINT nID, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lResult = 0;
		bool bHandled = false;

		if (NULL == getParent() 
			&& NULL == getTopLevelParent())
		{
			/** 没有顶层和父控件，交由自己处理 */
			lResult = handleWidgetMessage(pSender, nID, uMsg, wParam, lParam, bHandled);
			if (bHandled) return lResult;
		}
		else
		{
			/** 交给父控件处理 */
			if (NULL != getParent())
			{
				lResult = getParent()->handleWidgetMessage(pSender, nID, uMsg, wParam, lParam, bHandled);
			}
			if (bHandled) return lResult;

			/** 交给顶层控件处理 */
			if (NULL != getTopLevelParent() 
				&& getParent() != getTopLevelParent())
			{
				lResult = getTopLevelParent()->handleWidgetMessage(pSender, nID, uMsg, wParam, lParam, bHandled);
			}
			if (bHandled) return lResult;
		}

		return 0;
	}

	FWIDGET_INLINE LRESULT FWidget::sendWidgetMessage(FWidget* pSender, UINT nID, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		bool bHandled = false;
		LRESULT lResult = handleWidgetMessage(pSender, nID, uMsg, wParam, lParam, bHandled);
		if (bHandled) return lResult;

		return 0;
	}

	FWIDGET_INLINE LRESULT FWidget::handleWidgetMessage(FWidget* pSender, UINT nID, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
	{
		switch (uMsg)
		{
		case FWM_HSCROLL:
		case FWM_VSCROLL:
			if (pSender == m_pHScrollBar || pSender == m_pVScrollBar)
			{
				FScrollBar *pScrollBar = FOBJECT_CAST(FScrollBar*, pSender);
				if (wParam == FAbstractSlider::Track)
				{
					pScrollBar->setSliderPosition(lParam);
					relayoutpaint();
				}
				else if (wParam == FAbstractSlider::Minimum)
				{
					pScrollBar->setSliderPosition(pScrollBar->minimum());
					relayoutpaint();
				}
				else if (wParam == FAbstractSlider::Sub)
				{
					pScrollBar->setSliderPosition(lParam - (int)(pScrollBar->page() * 0.5f));
					relayoutpaint();
				}
				else if (wParam == FAbstractSlider::Maximum)
				{
					pScrollBar->setSliderPosition(pScrollBar->maximum());
					relayoutpaint();
				}
				else if (wParam == FAbstractSlider::Add)
				{
					pScrollBar->setSliderPosition(lParam + (int)(pScrollBar->page() * 0.5f));
					relayoutpaint();
				}
				else if (wParam == FAbstractSlider::PageSub)
				{
					pScrollBar->setSliderPosition(lParam - pScrollBar->page());
					relayoutpaint();
				}
				else if (wParam == FAbstractSlider::PageAdd)
				{
					pScrollBar->setSliderPosition(lParam + pScrollBar->page());
					relayoutpaint();
				}
			}
			break;

		case FWM_MOUSECLICK:
			if (!bHandled 
				&& LeftButton == wParam)
			{
				switch (pSender->getRole())
				{
				case FOOROLE_SYSMIN:
					::PostMessage(getHwnd(), WM_SYSCOMMAND, SC_MINIMIZE, 0);
					break;
				case FOOROLE_SYSMAX:
					::PostMessage(getHwnd(), WM_SYSCOMMAND, SC_MAXIMIZE, 0);
					break;
				case FOOROLE_SYSRESTORE:
					::PostMessage(getHwnd(), WM_SYSCOMMAND, SC_RESTORE, 0);
					break;
				case FOOROLE_SYSCLOSE:
					//::PostMessage(getHwnd(), WM_SYSCOMMAND, SC_CLOSE, 0);
					endModal(IDCANCEL);
					break;
				default:
					break;
				}
			}
			break;

		default:
			break;
		}

		return 0;
	}

	FWIDGET_INLINE bool FWidget::event(Event::FEvent* e)
	{
		if (NULL == e)
		{
			FASSERT(false);
			return false;
		}

#define FEVENT_BEGIN_MAP() switch (e->type()) {
#define FEVENT_HANDLER(_et, _func, _pt) case _et: _func(FOBJECT_CAST(_pt, e)); break;
#define FEVENT_END_MAP() default: break; }

		FEVENT_BEGIN_MAP()
			FEVENT_HANDLER(Event::FEvent::Paint, paintEvent, Event::FPaintEvent*);
			FEVENT_HANDLER(Event::FEvent::Show, showEvent, Event::FEvent*);
			FEVENT_HANDLER(Event::FEvent::Hide, hideEvent, Event::FEvent*);
			FEVENT_HANDLER(Event::FEvent::Changed, changedEvent, Event::FChangedEvent*);
			FEVENT_HANDLER(Event::FEvent::LayoutChanged, layoutChangedEvent, Event::FLayoutChangedEvent*);
			FEVENT_HANDLER(Event::FEvent::MouseButtonPress, mousePressEvent, Event::FMouseEvent*);
			FEVENT_HANDLER(Event::FEvent::MouseButtonRelease, mouseReleaseEvent, Event::FMouseEvent*);
			FEVENT_HANDLER(Event::FEvent::MouseButtonDblClick, mouseDoubleClickEvent, Event::FMouseEvent*);
			FEVENT_HANDLER(Event::FEvent::MouseButtonClick, mouseClickEvent, Event::FMouseEvent*);
			FEVENT_HANDLER(Event::FEvent::MouseMove, mouseMoveEvent, Event::FMouseEvent*);
			FEVENT_HANDLER(Event::FEvent::MouseWheel, mouseWheelEvent, Event::FWheelEvent*);
			FEVENT_HANDLER(Event::FEvent::MouseEnter, mouseEnterEvent, Event::FEvent*);
			FEVENT_HANDLER(Event::FEvent::MouseLeave, mouseLeaveEvent, Event::FEvent*);
			FEVENT_HANDLER(Event::FEvent::KeyPress, keyPressEvent, Event::FKeyEvent*);
			FEVENT_HANDLER(Event::FEvent::KeyRelease, keyReleaseEvent, Event::FKeyEvent*);
			FEVENT_HANDLER(Event::FEvent::KeyChar, keyCharEvent, Event::FCharEvent*);
			FEVENT_HANDLER(Event::FEvent::reflectMouse, reflectEvent, Event::FReflectEvent*);
			FEVENT_HANDLER(Event::FEvent::reflectKey, reflectEvent, Event::FReflectEvent*);
			FEVENT_HANDLER(Event::FEvent::Cursor, cursorEvent, Event::FCursorEvent*);
			FEVENT_HANDLER(Event::FEvent::ParentWindowCreated, parentWindowCreatedEvent, Event::FParentWindowEvent*);
			FEVENT_HANDLER(Event::FEvent::ParentWindowSized, parentWindowSizedEvent, Event::FParentWindowEvent*);
			FEVENT_HANDLER(Event::FEvent::WindowPreCreate, windowPreCreateEvent, Event::FWindowPreCreateEvent*);
			FEVENT_HANDLER(Event::FEvent::WindowCreated, windowCreatedEvent, Event::FWindowCreatedEvent*);
			FEVENT_HANDLER(Event::FEvent::Custom, customEvent, Event::FCustomEvent*);
		FEVENT_END_MAP()

		switch (e->type())
		{
		case Event::FEvent::MouseButtonPress:
		case Event::FEvent::MouseButtonRelease:
		case Event::FEvent::MouseButtonDblClick:
		case Event::FEvent::MouseButtonClick:
		case Event::FEvent::MouseMove:
		case Event::FEvent::MouseEnter:
		case Event::FEvent::MouseLeave:
			if (isReflectMouse())
			{
				m_pWidgetPrivate->reflectMouseEvent(e);
			}
			break;

		case Event::FEvent::KeyPress:
		case Event::FEvent::KeyRelease:
		case Event::FEvent::KeyChar:
			if (isReflectKeyboard())
			{
				m_pWidgetPrivate->reflectKeyboardEvent(e);
			}
			break;

		default:
			break;
		}

		return e->isAccept();
	}

	FWIDGET_INLINE void FWidget::paintEvent(Event::FPaintEvent* e)
	{
		if (m_stylesheet.draw(this, e->graphics(), 
									&e->updateRect(), 
									getState(), 
									getLayoutRect(), 
									_T(""), 
									_T("")))
		{
			e->accpet(true);
		}
		else
		{
			defaultPaint(e);
		}
	}

	FWIDGET_INLINE void FWidget::showEvent(Event::FEvent* e)
	{

	}

	FWIDGET_INLINE void FWidget::hideEvent(Event::FEvent* e)
	{

	}

	FWIDGET_INLINE void FWidget::changedEvent(Event::FChangedEvent* e)
	{
	}

	FWIDGET_INLINE void FWidget::layoutChangedEvent(Event::FLayoutChangedEvent* e)
	{
	}

	FWIDGET_INLINE void FWidget::mousePressEvent(Event::FMouseEvent* e)
	{
		if (LeftButton == e->button)
			setStatePushed(true);
	}

	FWIDGET_INLINE void FWidget::mouseReleaseEvent(Event::FMouseEvent* e)
	{
		setStateNormal(true);
	}

	FWIDGET_INLINE void FWidget::mouseDoubleClickEvent(Event::FMouseEvent* e)
	{
		if (LeftButton == e->button)
			setStatePushed(true);
	}

	FWIDGET_INLINE void FWidget::mouseClickEvent(Event::FMouseEvent* e)
	{
		setStateHover(true);
		makeWidgetMessage(FWM_MOUSECLICK, (WPARAM)e->button, MAKELPARAM(e->mouse.x, e->mouse.y));
	}

	FWIDGET_INLINE void FWidget::mouseMoveEvent(Event::FMouseEvent* e)
	{
	}

	FWIDGET_INLINE void FWidget::mouseWheelEvent(Event::FWheelEvent* e)
	{
		if (NULL != m_pVScrollBar)
		{
			if (e->delta > 0) // UP
			{
				m_pVScrollBar->makeWidgetMessage(FWM_VSCROLL, FAbstractSlider::Sub, m_pVScrollBar->sliderPosition());
			}
			else // DOWN
			{
				m_pVScrollBar->makeWidgetMessage(FWM_VSCROLL, FAbstractSlider::Add, m_pVScrollBar->sliderPosition());
			}
		}
	}

	FWIDGET_INLINE void FWidget::mouseEnterEvent(Event::FEvent* e)
	{
		setStateHover(true);
	}

	FWIDGET_INLINE void FWidget::mouseLeaveEvent(Event::FEvent* e)
	{
		setStateNormal(true);
	}

	FWIDGET_INLINE void FWidget::keyPressEvent(Event::FKeyEvent* e)
	{
	}

	FWIDGET_INLINE void FWidget::keyReleaseEvent(Event::FKeyEvent* e)
	{
	}

	FWIDGET_INLINE void FWidget::keyCharEvent(Event::FCharEvent* e)
	{
		if (isReflectKeyboard())
		{
			m_pWidgetPrivate->reflectKeyboardEvent(e);
		}
	}

	FWIDGET_INLINE void FWidget::reflectEvent(Event::FReflectEvent* e)
	{
		switch (e->event->type())
		{
		case Event::FEvent::MouseEnter:
			setStateHover(true);
			break;

		case Event::FEvent::MouseLeave:
			setStateNormal(true);
			break;

		}
	}

	FWIDGET_INLINE void FWidget::cursorEvent(Event::FCursorEvent* e)
	{
		FASSERT(NULL != e);
		e->accpet(m_cursor.showCursor());
	}

	FWIDGET_INLINE void FWidget::parentWindowSizedEvent(Event::FParentWindowEvent* e)
	{
		switch (getRole())
		{
		case FOOROLE_SYSMAX:
			setVisible(::IsZoomed(e->hWnd) == FALSE);
			break;

		case FOOROLE_SYSRESTORE:
			setVisible(::IsZoomed(e->hWnd) == TRUE);
			break;
		}
	}

	FWIDGET_INLINE void FWidget::parentWindowCreatedEvent(Event::FParentWindowEvent* e)
	{
	}

	FWIDGET_INLINE void FWidget::windowPreCreateEvent(Event::FWindowPreCreateEvent* e)
	{
	}

	FWIDGET_INLINE void FWidget::windowCreatedEvent(Event::FWindowCreatedEvent* e)
	{
	}

	FWIDGET_INLINE void FWidget::customEvent(Event::FCustomEvent* e)
	{

	}

	FWIDGET_INLINE bool FWidget::beginMarkup(Markup::FMarkup* pMarkup)
	{
		return true;
	}

	FWIDGET_INLINE bool FWidget::setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		if (_tcscmp(pszName, _T("name")) == 0)
		{
			m_strName = pszValue;
		}
		else if (_tcscmp(pszName, _T("id")) == 0)
		{
			setID((int)_tcstol(pszValue, NULL, 10));
		}
		else if (_tcscmp(pszName, _T("group")) == 0)
		{
			m_strGroup = pszValue;
		}
		else if (_tcscmp(pszName, _T("text")) == 0)
		{
			setText(pszValue);
		}
		else if (_tcscmp(pszName, _T("tabindex")) == 0)
		{
			m_tabIndex = _tcstol(pszValue, NULL, 10);
		}
		else if (_tcscmp(pszName, _T("dock")) == 0)
		{
			setDock(Markup::fooMarkupParseDock(pszValue));
		}
		else if (_tcscmp(pszName, _T("align")) == 0)
		{
			setAlign(Markup::fooMarkupParseAlign(pszValue));
		}
		else if (_tcscmp(pszName, _T("tooltip")) == 0)
		{
			setToolTipText(pszValue);
		}
		else if (_tcscmp(pszName, _T("role")) == 0)
		{
			setRole(Markup::fooMarkupParseRole(pszValue));
		}
		else if (_tcscmp(pszName, _T("visible")) == 0)
		{
			setVisible(Markup::fooMarkupParseBoolean(pszValue));
		}
		else if (_tcscmp(pszName, _T("enabled")) == 0)
		{
			setEnabled(Markup::fooMarkupParseBoolean(pszValue));
		}
		else if (_tcscmp(pszName, _T("checked")) == 0)
		{
			setCheck(Markup::fooMarkupParseBoolean(pszValue));
		}
		else if (_tcscmp(pszName, _T("selected")) == 0)
		{
			setSelect(Markup::fooMarkupParseBoolean(pszValue));
		}
		else if (_tcscmp(pszName, _T("transparent")) == 0)
		{
			setTransparent(Markup::fooMarkupParseBoolean(pszValue));
		}
		else if (_tcscmp(pszName, _T("skiptab")) == 0)
		{
			skipTab(Markup::fooMarkupParseBoolean(pszValue));
		}
		else if (_tcscmp(pszName, _T("skipmouse")) == 0)
		{
			skipMouse(Markup::fooMarkupParseBoolean(pszValue));
		}
		else if (_tcscmp(pszName, _T("skipkeyboard")) == 0)
		{
			skipKeyboard(Markup::fooMarkupParseBoolean(pszValue));
		}
		else if (_tcscmp(pszName, _T("skipfocus")) == 0)
		{
			skipFocus(Markup::fooMarkupParseBoolean(pszValue));
		}
		else if (_tcscmp(pszName, _T("skipfocus")) == 0)
		{
			skipFocus(Markup::fooMarkupParseBoolean(pszValue));
		}
		else if (_tcscmp(pszName, _T("reflectmouse")) == 0)
		{
			reflectMouse(Markup::fooMarkupParseBoolean(pszValue));
		}
		else if (_tcscmp(pszName, _T("reflectkeyboard")) == 0)
		{
			reflectKeyboard(Markup::fooMarkupParseBoolean(pszValue));
		}
		else if (_tcscmp(pszName, _T("margin")) == 0)
		{
			Markup::fooMarkupParseEdge(&m_egMargin, pszValue);
		}
		else if (_tcscmp(pszName, _T("padding")) == 0)
		{
			Markup::fooMarkupParseEdge(&m_egPadding, pszValue);
		}
		else if (_tcscmp(pszName, _T("x")) == 0 
			|| _tcscmp(pszName, _T("left")) == 0)
		{
			setLeft(_tcstol(pszValue, NULL, 10));
		}
		else if (_tcscmp(pszName, _T("y")) == 0 
			|| _tcscmp(pszName, _T("top")) == 0)
		{
			setTop(_tcstol(pszValue, NULL, 10));
		}
		else if (_tcscmp(pszName, _T("pos")) == 0 
			|| _tcscmp(pszName, _T("position")) == 0)
		{
			Core::FPoint position;
			Markup::fooMarkupParsePoint(&position, pszValue);
			setPosition(position);
		}
		else if (_tcscmp(pszName, _T("relativepos")) == 0)
		{
			relativePosition(Markup::fooMarkupParseBoolean(pszValue));
		}
		else if (_tcscmp(pszName, _T("floatpos")) == 0)
		{
			floatPosition(Markup::fooMarkupParseBoolean(pszValue));
		}
		else if (_tcscmp(pszName, _T("cx")) == 0 
			|| _tcscmp(pszName, _T("width")) == 0)
		{
			setWidth(_tcstol(pszValue, NULL, 10));
		}
		else if (_tcscmp(pszName, _T("cy")) == 0 
			|| _tcscmp(pszName, _T("height")) == 0)
		{
			setHeight(_tcstol(pszValue, NULL, 10));
		}
		else if (_tcscmp(pszName, _T("size")) == 0)
		{
			Core::FSize	size;
			Markup::fooMarkupParseSize(&size, pszValue);
			setSize(size);
		}
		else if (_tcscmp(pszName, _T("minwidth")) == 0)
		{
			setMinWidth(_tcstol(pszValue, NULL, 10));
		}
		else if (_tcscmp(pszName, _T("minheight")) == 0)
		{
			setMinHeight(_tcstol(pszValue, NULL, 10));
		}
		else if (_tcscmp(pszName, _T("minsize")) == 0)
		{
			Markup::fooMarkupParseSize(&m_minsize, pszValue);
		}
		else if (_tcscmp(pszName, _T("maxwidth")) == 0)
		{
			setMaxWidth(_tcstol(pszValue, NULL, 10));
		}
		else if (_tcscmp(pszName, _T("maxheight")) == 0)
		{
			setMaxHeight(_tcstol(pszValue, NULL, 10));
		}
		else if (_tcscmp(pszName, _T("maxsize")) == 0)
		{
			Markup::fooMarkupParseSize(&m_maxsize, pszValue);
		}
		else if (_tcscmp(pszName, _T("cursor")) == 0)
		{
			m_cursor.parseAttribute(pMarkup, pszValue);
		}
		else if (_tcscmp(pszName, _T("style")) == 0)
		{
			m_stylesheet.parseAttribute(pMarkup, pszValue);
		}
		else if (_tcscmp(pszName, _T("hscroll")) == 0)
		{
			createScrollBar(SB_HORZ, false);
			FASSERT(NULL != m_pHScrollBar);
			m_pHScrollBar->parseAttribute(pMarkup, pszValue);
		}
		else if (_tcscmp(pszName, _T("vscroll")) == 0)
		{
			createScrollBar(SB_VERT, false);
			FASSERT(NULL != m_pVScrollBar);
			m_pVScrollBar->parseAttribute(pMarkup, pszValue);
		}
		else if (_tcscmp(pszName, _T("windowstyle")) == 0)
		{
			m_pWidgetPrivate->parseWindowStyle(pszValue);
		}
		else if (_tcscmp(pszName, _T("windowncspace")) == 0) /** 窗口外观相关属性 */
		{
			Markup::fooMarkupParseEdge(&m_pWidgetPrivate->m_wndstyle.windowncspace, pszValue);
		}

		return true;
	}

	FWIDGET_INLINE bool FWidget::endMarkup(Markup::FMarkup* pMarkup)
	{
		FWidgetPrivate::setWidgetTabIndex(getTabIndex(), this);
		return true;
	}

	FWIDGET_INLINE bool FWidget::markupAddWidget(Markup::FMarkup* pMarkup, FWidget* pWidget)
	{
		return addChild(pWidget);
	}

	FWIDGET_INLINE /** static */ FWidget* FWidget::createWidget(LPCTSTR pszName)
	{
		FWidget* pWidgetObject = NULL;

		/** 加do{}while(0)是为了防止漏掉FWIDGET_END_MAP() */
#define FWIDGET_BEGIN_MAP() \
		do \
		{ \
			if (false) \
			{ \
			}
#define FWIDGET_HANDLER(_foowc, _class) \
			else if (_tcscmp(pszName, _foowc) == 0) \
			{ \
				pWidgetObject = new _class; \
			}
#define FWIDGET_END_MAP() \
			else \
			{ \
				FASSERT(FALSE); \
				return NULL; \
			} \
		} while (0);

		// 
		FWIDGET_BEGIN_MAP()
			FWIDGET_HANDLER(FOOWC_WIDGET, FWidget)
			FWIDGET_HANDLER(FOOWC_SCROLLBAR, FScrollBar)
			FWIDGET_HANDLER(FOOWC_HORZSCROLLBAR, FScrollBar(FScrollBar::Horz))
			FWIDGET_HANDLER(FOOWC_VERTSCROLLBAR, FScrollBar(FScrollBar::Vert))
			FWIDGET_HANDLER(FOOWC_LAYOUT, FLayout)
			FWIDGET_HANDLER(FOOWC_TABLAYOUT, FLayout(FLayout::TabLayout))
			FWIDGET_HANDLER(FOOWC_HORZLAYOUT, FLayout(FLayout::HorzLayout))
			FWIDGET_HANDLER(FOOWC_VERTLAYOUT, FLayout(FLayout::VertLayout))
			FWIDGET_HANDLER(FOOWC_LABEL, FLabel)
			FWIDGET_HANDLER(FOOWC_BUTTON, FButton)
			FWIDGET_HANDLER(FOOWC_CHECKBUTTON, FButton(FButton::CheckButton))
			FWIDGET_HANDLER(FOOWC_RADIOBUTTON, FButton(FButton::RadioButton))
			FWIDGET_HANDLER(FOOWC_EDIT, FEdit)
			FWIDGET_HANDLER(FOOWC_ABSTRACTLIST, FAbstractList)
			FWIDGET_HANDLER(FOOWC_ABSTRACTLISTELEMENT, FAbstractListElement)
			FWIDGET_HANDLER(FOOWC_COMBOBOX, FComboBox)
			FWIDGET_HANDLER(FOOWC_COMBOBOXLIST, FComboBoxList)
			FWIDGET_HANDLER(FOOWC_PROGRESSBAR, FProgressBar)
			FWIDGET_HANDLER(FOOWC_HORZPROGRESSBAR, FProgressBar(FProgressBar::HorzProgressBar))
			FWIDGET_HANDLER(FOOWC_VERTPROGRESSBAR, FProgressBar(FProgressBar::VertProgressBar))
			FWIDGET_HANDLER(FOOWC_HEADER, FHeader)
			FWIDGET_HANDLER(FOOWC_HEADERELEMENT, FHeaderElement)
			FWIDGET_HANDLER(FOOWC_LISTCTRL, FListCtrl)
			FWIDGET_HANDLER(FOOWC_LISTCTRLELEMENT, FListCtrlElement)
			FWIDGET_HANDLER(FOOWC_TREECTRL, FTreeCtrl)
			FWIDGET_HANDLER(FOOWC_TREECTRLELEMENT, FTreeCtrlElement)
			FWIDGET_HANDLER(FOOWC_HOTKEY, FHotKey) 
			FWIDGET_HANDLER(FOOWC_ACTIVEX, FActivex)
			FWIDGET_HANDLER(FOOWC_WEBBROWSER, FWebBrowser)
		FWIDGET_END_MAP()

		pWidgetObject->autoDelete(true);
		return pWidgetObject;
	}

	//////////////////////////////////////////////////////////////////////////
	// FWidgetLock
	FWIDGET_INLINE FWidget::FWidgetLock::FWidgetLock(FWidget* widget) : 
		m_pWidget(widget)
	{
		FASSERT(NULL != m_pWidget);

		m_pWidget->lockChildren();
	}

	FWIDGET_INLINE FWidget::FWidgetLock::~FWidgetLock(void)
	{
		FASSERT(NULL != m_pWidget);

		m_pWidget->unlockChildren();
	}

}} // !namespace