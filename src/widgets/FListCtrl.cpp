/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FListCtrl.cpp
* 描述：		FListCtrl
* 创建日期：	2013-07-28
*
*/

#include "StdAfx.h"
#include "widgets/FListCtrl.h"
#include "widgets/FScrollBar.h"

#define FLISTCTRL_INLINE FOOINLINE

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FListCtrlLayout
	#define FOOWC_LISTCTRLLAYOUT _T("listctrllayout")
	class FListCtrl::FListCtrlLayout : public FLayout
	{
		friend FListCtrl;
	public:
		explicit FListCtrlLayout(FListCtrl* pListCtrl);
		~FListCtrlLayout(void);
		FOODECLARE_CLASS(FOOWC_LISTCTRLLAYOUT)
		FOODECLARE_CONTAINER(true)

	public:
		FListCtrl* getListCtrl(void) const;

	protected:
		int calcHeight(const Core::FRect& rcLayout);

	private:
		FListCtrl* const m_pListCtrl;
	};

	// 
	FLISTCTRL_INLINE FListCtrl::FListCtrlLayout::FListCtrlLayout(FListCtrl* pListCtrl) : 
		FLayout(FLayout::VertLayout)
		, m_pListCtrl(pListCtrl)
	{
		FASSERT(NULL != m_pListCtrl);
	}

	FLISTCTRL_INLINE FListCtrl::FListCtrlLayout::~FListCtrlLayout(void)
	{
	}

	FLISTCTRL_INLINE FListCtrl* FListCtrl::FListCtrlLayout::getListCtrl(void) const
	{
		FASSERT(NULL != m_pListCtrl);
		return m_pListCtrl;
	}

	FLISTCTRL_INLINE int FListCtrl::FListCtrlLayout::calcHeight(const Core::FRect& rcLayout)
	{
		int cyUsable = rcLayout.height();
		int nMeanSize = cyUsable;
		int nFixedSizeWidgetCount = 0;

		for (int i = 0; i < getChildrenCount(); ++i)
		{
			FWidget *pWidget = getChild(i);
			FASSERT(NULL != pWidget);

			if (pWidget->isVisible())
			{
				cyUsable -= pWidget->getWidth() + pWidget->getMargin().edgeWidth();
				if (cyUsable < 0)
				{
					nFixedSizeWidgetCount = 0;
					nMeanSize = 0;
					break;
				}
				if (pWidget->isFixedWidth()) nFixedSizeWidgetCount++;
			}
			else
			{
				nFixedSizeWidgetCount++;
			}
		}

		if (cyUsable > 0 
			&& nFixedSizeWidgetCount < getChildrenCount())
		{
			nMeanSize = cyUsable / (getChildrenCount() - nFixedSizeWidgetCount);
		}
		else
		{
			nMeanSize = 0;
		}

		int yOffset = rcLayout.top;
		int cyNeedHeight = 0;
		for (int i = 0; i < getChildrenCount(); ++i)
		{
			FWidget *pWidget = getChild(i);
			FASSERT(NULL != pWidget);

			Core::FPoint ptWidget;
			Core::FSize szWidget;

			if (pWidget->isFixedHeight())
			{
				szWidget.cy = pWidget->getHeight();
			}
			else
			{
				szWidget.cy = nMeanSize;
			}

			ptWidget.y = yOffset + pWidget->getMargin().top;

			yOffset += ptWidget.y + szWidget.cy + pWidget->getMargin().edgeHeight();
			cyNeedHeight += szWidget.cy + pWidget->getMargin().edgeHeight();
		}

		return cyNeedHeight;
	}

	//////////////////////////////////////////////////////////////////////////
	// FListCtrl
	FLISTCTRL_INLINE FListCtrl::FListCtrl(FWidget* parent/* = NULL*/) : 
		FAbstractList(parent) 
		, m_pListCtrlLayout(NULL)
		, m_pHeader(NULL)
	{
		m_pListCtrlLayout = new FListCtrlLayout(this);
		m_pListCtrlLayout->autoDelete(true);
		m_pListCtrlLayout->setParent(this);

		m_pHeader = new FHeader(this);
		m_pHeader->autoDelete(true);
	}

	FLISTCTRL_INLINE FListCtrl::~FListCtrl(void)
	{
	}

	FLISTCTRL_INLINE int FListCtrl::addElement(FListCtrlElement* element)
	{
		FASSERT(NULL != m_pListCtrlLayout);
		if (NULL != m_pListCtrlLayout)
		{
			if (m_pListCtrlLayout->addChild(element))
			{
				repaint();
				return (m_pListCtrlLayout->getChildrenCount() - 1);
			}
		}

		return -1;
	}

	FLISTCTRL_INLINE int FListCtrl::insertElement(int nIndex, FListCtrlElement* element)
	{
		FASSERT(NULL != m_pListCtrlLayout);
		if (NULL != m_pListCtrlLayout)
		{
			if (m_pListCtrlLayout->insertChild(nIndex, element))
			{
				repaint();
				return nIndex;
			}
		}

		return -1;
	}

	FLISTCTRL_INLINE bool FListCtrl::removeElement(int nIndex)
	{
		FASSERT(NULL != m_pListCtrlLayout);
		if (NULL != m_pListCtrlLayout)
		{
			repaint();
			return m_pListCtrlLayout->removeChild(nIndex);
		}

		return false;
	}

	FLISTCTRL_INLINE bool FListCtrl::removeElement(FListCtrlElement* element)
	{
		FASSERT(NULL != m_pListCtrlLayout);
		if (NULL != m_pListCtrlLayout)
		{
			repaint();
			return m_pListCtrlLayout->removeChild(element);
		}

		return false;
	}

	FLISTCTRL_INLINE FListCtrlElement* FListCtrl::getElement(int nIndex) const
	{
		FASSERT(NULL != m_pListCtrlLayout);
		if (NULL != m_pListCtrlLayout)
		{
			return (FListCtrlElement*)m_pListCtrlLayout->getChild(nIndex);
		}

		return NULL;
	}

	FLISTCTRL_INLINE bool FListCtrl::clearElements(void)
	{
		FASSERT(NULL != m_pListCtrlLayout);
		if (NULL != m_pListCtrlLayout)
		{
			repaint();
			return m_pListCtrlLayout->clearChildren();
		}

		return false;
	}

	FLISTCTRL_INLINE int FListCtrl::getElementsCount(void) const
	{
		FASSERT(NULL != m_pListCtrlLayout);
		if (NULL != m_pListCtrlLayout)
		{
			return m_pListCtrlLayout->getChildrenCount();
		}

		return 0;
	}

	FLISTCTRL_INLINE bool FListCtrl::selectElement(int nIndex)
	{
		FASSERT(NULL != m_pListCtrlLayout);
		if (NULL != m_pListCtrlLayout)
		{
			FWidget *pWidget = m_pListCtrlLayout->getChild(nIndex);
			if (NULL == pWidget) return false;
			return __super::selectElement((FAbstractListElement*)pWidget->getInterface(FOOWC_ABSTRACTLISTELEMENT));
		}

		return false;
	}

	FLISTCTRL_INLINE FHeader* FListCtrl::getHeader(void) const
	{
		FASSERT(NULL != m_pHeader);
		return m_pHeader;
	}

	FLISTCTRL_INLINE void FListCtrl::layout(const Core::FRect& rcLayout)
	{
		beginLayout();

		/** 处理子控件为0的状态 */
		if (!hasChildren())
		{
			if (NULL != getScrollBar(SB_HORZ)) getScrollBar(SB_HORZ)->hide();
			if (NULL != getScrollBar(SB_VERT)) getScrollBar(SB_VERT)->hide();

			endLayout();
			return;
		}

		if (NULL == m_pListCtrlLayout 
			|| NULL == m_pHeader)
		{
			FASSERT(FALSE);
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

		/** 计算列表头区域及大小 */
		Core::FRect rcHeader;
		rcHeader.left = rcClient.left;
		rcHeader.top = rcClient.top;
		rcHeader.right = rcClient.right;

		if (m_pHeader->isFixedHeight())
		{
			rcHeader.bottom = rcHeader.top + m_pHeader->getHeight();
		}
		else
		{
			rcHeader.bottom = rcHeader.top + ::GetSystemMetrics(SM_CYHSCROLL);
		}

		m_pHeader->setLayoutRect(rcHeader);

		int cxHeader = 0;
		for (int i = 0; i < m_pHeader->getChildrenCount(); ++i)
		{
			FWidget *pHeaderElement = m_pHeader->getChild(i);
			FASSERT(NULL != pHeaderElement);

			if (pHeaderElement->getWidth() > 0)
			{
				cxHeader += pHeaderElement->getWidth();
			}
			else
			{
				cxHeader += pHeaderElement->getLayoutRect().width();
			}
		}

		if (cxHeader <= rcClient.width()) cxHeader = rcClient.width();
		rcHeader.right = rcHeader.left + cxHeader;
		if (isShowScrollBar(SB_HORZ))
		{
			rcHeader.offsetX(-getScrollBar(SB_HORZ)->sliderPosition());
		}
		m_pHeader->setLayoutRect(rcHeader);


		/** 列表项布局 */
		Core::FRect rcListLayout(rcHeader.left, rcHeader.bottom, rcHeader.right, rcClient.bottom);
		if (isShowScrollBar(SB_VERT))
		{
			rcListLayout.offsetY(-getScrollBar(SB_VERT)->sliderPosition());
		}
		int cyNeedHeight = m_pListCtrlLayout->calcHeight(rcListLayout);

		// 设置布局区域
		m_pListCtrlLayout->setLayoutRectReal(rcListLayout);
		rcListLayout.bottom = rcListLayout.top + cyNeedHeight;
		m_pListCtrlLayout->setLayoutRect(rcListLayout);

		szNeedScroll.cx = max(szNeedScroll.cx, rcHeader.width());
		szNeedScroll.cy = max(szNeedScroll.cy, rcHeader.height() + cyNeedHeight);

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

	FLISTCTRL_INLINE bool FListCtrl::markupAddWidget(Markup::FMarkup* pMarkup, FWidget* pWidget)
	{
		FASSERT(NULL != m_pListCtrlLayout);

		FHeader *pHeader = static_cast<FHeader*>(pWidget->getInterface(FOOWC_HEADER));
		if (NULL != pHeader)
		{
			if (pHeader != m_pHeader)
			{
				removeChild(m_pHeader);
				m_pHeader = NULL;
			}

			m_pHeader = pHeader;
			m_pHeader->setParent(this);
			return true;
		}

		return m_pListCtrlLayout->markupAddWidget(pMarkup, pWidget);
	}

	//////////////////////////////////////////////////////////////////////////
	// FListCtrlElement
	FLISTCTRL_INLINE FListCtrlElement::FListCtrlElement(FWidget* parent/* = NULL*/) : 
		FAbstractListElement(parent) 
	{
	}

	FLISTCTRL_INLINE FListCtrlElement::~FListCtrlElement(void)
	{
	}

	FLISTCTRL_INLINE void FListCtrlElement::layout(const Core::FRect& rcLayout)
	{
		beginLayout();

		/** 处理子控件为0的状态 */
		if (!hasChildren())
		{
			if (NULL != getScrollBar(SB_HORZ)) getScrollBar(SB_HORZ)->hide();
			if (NULL != getScrollBar(SB_VERT)) getScrollBar(SB_VERT)->hide();

			endLayout();
			return;
		}

		if (NULL == getParent() 
			|| NULL == getParent()->getParent())
		{
			FASSERT(FALSE);
			return;
		}

		FListCtrl *pListCtrl = static_cast<FListCtrl*>(getParent()->getParent()->getInterface(FOOWC_LISTCTRL));
		if (NULL == pListCtrl)
		{
			FASSERT(FALSE);
			return;
		}

		FHeader *pHeader = pListCtrl->getHeader();
		if (NULL == pHeader)
		{
			FASSERT(FALSE);
			return;
		}

		/** 获取客户区（有效布局区域） */
		Core::FRect rcClient;
		getValidLayoutRect(&rcClient);
		
		int xOffset = rcClient.left;
		int numOfHeaderElement = pHeader->getChildrenCount();
		for (int i = 0; i < getChildrenCount(); ++i)
		{
			FWidget *pWidget = getChild(i);
			FASSERT(NULL != pWidget);

			if (i >= numOfHeaderElement)
			{
				pWidget->setLayoutRect(Core::FRect(0, 0, 0, 0));
				continue;
			}
			else
			{
				Core::FPoint ptWidget;
				Core::FSize szWidget;

				FWidget *pHeaderElement = pHeader->getChild(i);
				FASSERT(NULL != pHeaderElement);

				szWidget.cx = pHeaderElement->getLayoutRect().width() - pWidget->getMargin().edgeWidth();
				if (pWidget->isFixedHeight())
				{
					szWidget.cy = pWidget->getHeight();
				}
				else
				{
					szWidget.cy = rcClient.height() - pWidget->getMargin().edgeHeight();
				}

				ptWidget.x = xOffset + pWidget->getMargin().left;
				/** 计算垂直坐标 */
				if (FOODOCK_BOTTOM == pWidget->getDock() 
					|| fooTestAttribute(pWidget->getAlign(), FOOALIGN_BOTTOM))
				{
					ptWidget.y = rcClient.bottom - szWidget.cy - pWidget->getMargin().bottom;
				}
				else if (fooTestAttribute(pWidget->getAlign(), FOOALIGN_YCENTER))
				{
					ptWidget.y = rcClient.top + (rcClient.height() - szWidget.cy) / 2 + pWidget->getMargin().top;
				}
				else
				{
					ptWidget.y = rcClient.top + pWidget->getMargin().top;
				}

				xOffset += szWidget.cx + pWidget->getMargin().edgeWidth();
				Core::FRect rcWidget(ptWidget, szWidget);
				pWidget->setLayoutRect(rcWidget);
			}
		}
	}

	FLISTCTRL_INLINE void FListCtrlElement::mousePressEvent(Event::FMouseEvent* e)
	{
		if (LeftButton == e->button)
		{
			selectElement();
		}
	}

}} // !namespace