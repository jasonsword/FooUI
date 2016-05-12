/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FLayout.cpp
* ������		FLayout�����֣�layout����ˮƽ���֣�horzlayout������ֱ���֣�vertlayout��
* �������ڣ�	2013-06-30
*
*/

#include "StdAfx.h"
#include "core/FLock.h"
#include "widgets/FLayout.h"
#include "widgets/FWidgetPrivate_p.h"
#include "widgets/FScrollBar.h"

#define FLAYOUT_INLINE FOOINLINE
namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FLayout
	FLAYOUT_INLINE FLayout::FLayout(DWORD dwLayoutStyle, FWidget* parent) : 
		FWidget(parent) 
		, m_dwLayoutStyle(dwLayoutStyle) 
		, m_nCurSelected(0)
	{
		modifyFlag(0, FOOFLAG_TRANSPARENT | FOOFLAG_SKIPTAB | FOOFLAG_SKIPMOUSE | FOOFLAG_SKIPKEYBOARD | FOOFLAG_SKIPFOCUS);
	}

	FLAYOUT_INLINE FLayout::~FLayout(void)
	{
	}

	FLAYOUT_INLINE void FLayout::setLayoutStyle(DWORD dwLayoutStyle)
	{
		if (m_dwLayoutStyle == dwLayoutStyle)
		{
			return;
		}
		m_dwLayoutStyle = dwLayoutStyle;
		relayout();
	}

	FLAYOUT_INLINE DWORD FLayout::getLayoutStyle(void) const
	{
		return m_dwLayoutStyle;
	}

	void FLayout::setCurSelected(int nCurSelected)
	{
		if (getLayoutStyle() == TabLayout)
		{
			if (nCurSelected < 0 
				|| nCurSelected > getChildrenCount())
			{
				FASSERT(FALSE);
				return;
			}

			if (nCurSelected == m_nCurSelected)
			{
				return;
			}

			m_nCurSelected = nCurSelected;
			for (int i = 0; i < getChildrenCount(); ++i)
			{
				FWidget *pWidget = getChild(i);
				FASSERT(NULL != pWidget);
				pWidget->setVisible(m_nCurSelected == i);
			}
			repaint();
		}
		else
		{
			FASSERT(FALSE);
		}
	}

	int FLayout::getCurSelected(void) const
	{
		if (getLayoutStyle() == TabLayout)
		{
			return m_nCurSelected;
		}
		else
		{
			return -1;
		}
	}

	FLAYOUT_INLINE void FLayout::layout(const Core::FRect& rcLayout)
	{
		switch (getLayoutStyle())
		{
		case FLayout::HorzLayout:
			layoutHorz(rcLayout);
			break;

		case FLayout::VertLayout:
			layoutVert(rcLayout);
			break;

		/*case FLayout::FlowLayout:
			layoutFlow(rcLayout);
			break;*/

		default:
			__super::layout(rcLayout);
			break;
		}
	}

	FLAYOUT_INLINE void FLayout::layoutHorz(const Core::FRect& rcLayout)
	{
		beginLayout();

		/** �����ӿؼ�Ϊ0��״̬ */
		if (!hasChildren())
		{
			if (NULL != getScrollBar(SB_HORZ)) getScrollBar(SB_HORZ)->hide();
			if (NULL != getScrollBar(SB_VERT)) getScrollBar(SB_VERT)->hide();

			endLayout();
			return;
		}

		/** �ͻ�����С�������ڸ��ӿؼ����ֵ����� */
		Core::FRect rcClient;

		/** �������Ŀ���״̬ */
		bool bIsShowHScrollBar = false;
		bool bIsShowVScrollBar = false;

		/** ����������Ŀ���״̬ */
		bool bIsNeedShowHScrollBar = false;
		bool bIsNeedShowVScrollBar = false;

		/** ��Ҫ���������� */
		Core::FSize szNeedScroll;

		/** ��ȡ�ͻ�������Ч�������� */
		getValidLayoutRect(&rcClient);
		
		int cxUsable = rcClient.width();
		int nMeanSize = cxUsable;
		int nFixedSizeWidgetCount = 0;
		/** �ų��̶���С�Ŀؼ� */
		for (int i = 0; i < getChildrenCount(); ++i)
		{
			FWidget *pWidget = getChild(i);
			FASSERT(NULL != pWidget);

			if (pWidget->isVisible())
			{
				cxUsable -= pWidget->getWidth() + pWidget->getMargin().edgeWidth();
				if (cxUsable < 0)
				{
					cxUsable = 0;
					nMeanSize = 0;
				}
				if (pWidget->isFixedWidth()) nFixedSizeWidgetCount++;
			}
			else
			{
				nFixedSizeWidgetCount++;
			}
		}

		if (cxUsable > 0 
			&& nFixedSizeWidgetCount < getChildrenCount())
		{
			nMeanSize = cxUsable / (getChildrenCount() - nFixedSizeWidgetCount);
		}
		else
		{
			nMeanSize = 0;
		}

		/** ���������״̬�����仯���¿�ʼ���� */
DO_RELAYOUT:

		/** ��¼��ǰ�Ĺ���������״̬����������仯���²��֣� */
		layoutScrollBar();
		bIsShowHScrollBar = isShowScrollBar(SB_HORZ);
		bIsShowVScrollBar = isShowScrollBar(SB_VERT);

		/** ��ȡ�ͻ�������Ч�������� */
		getValidLayoutRect(&rcClient);

		Core::FRect rcUsable = rcClient;
		szNeedScroll.setEmpty();

		/** ���ӿؼ����� */
		for (int i = 0; i < getChildrenCount(); ++i)
		{
			FWidget *pWidget = getChild(i);
			FASSERT(NULL != pWidget);

			/*if (rcUsable.isEmpty())
			{
				pWidget->setLayoutRect(Core::FRect(0, 0, 0, 0));
				continue;
			}*/

			if (!pWidget->isVisible())
			{
				pWidget->setLayoutRect(Core::FRect(0, 0, 0, 0));
				continue;
			}

			Core::FPoint ptWidget;
			Core::FSize szWidget;
			
			/** ����ˮƽ��� */
			if (pWidget->isFixedWidth())
			{
				szWidget.cx = pWidget->getWidth();
			}
			else
			{
				szWidget.cx = nMeanSize;
				if (pWidget->getMinWidth() > szWidget.cx) 
					szWidget.cx = pWidget->getMinWidth();
				if (pWidget->getMaxWidth() > 0 && pWidget->getMaxWidth() < szWidget.cx)
					szWidget.cx = pWidget->getMaxWidth();
			}

			/** ���㴹ֱ�߶� */
			if (pWidget->isFixedHeight())
			{
				szWidget.cy = pWidget->getHeight();
			}
			else
			{
				szWidget.cy = rcUsable.height() - pWidget->getMargin().edgeHeight();
				if (pWidget->getMinHeight() > szWidget.cy) 
					szWidget.cy = pWidget->getMinHeight();
				if (pWidget->getMaxHeight() > 0 && pWidget->getMaxHeight() < szWidget.cy)
					szWidget.cy = pWidget->getMaxHeight();
			}

			/** ����ˮƽ���� */
			ptWidget.x = rcUsable.left + pWidget->getMargin().left;
			/** ���㴹ֱ���� */
			if (FOODOCK_BOTTOM == pWidget->getDock() 
				|| fooTestAttribute(pWidget->getAlign(), FOOALIGN_BOTTOM))
			{
				ptWidget.y = rcUsable.bottom - szWidget.cy - pWidget->getMargin().bottom;
			}
			else if (fooTestAttribute(pWidget->getAlign(), FOOALIGN_YCENTER))
			{
				ptWidget.y = rcUsable.top + (rcUsable.height() - szWidget.cy) / 2 + pWidget->getMargin().top;
			}
			else
			{
				ptWidget.y = rcUsable.top + pWidget->getMargin().top;
			}

			/** �ӿ��ò��������ȥ�������� */
			rcUsable.left += szWidget.cx + pWidget->getMargin().edgeWidth();

			/** ��Ҫ�����ķ�Χ */
			szNeedScroll.cx += szWidget.cx + pWidget->getMargin().edgeWidth();
			szNeedScroll.cy = max(szNeedScroll.cy, ptWidget.y - rcClient.top + szWidget.cy + pWidget->getMargin().bottom);

			/** �����Ҫ��ʾ�������� ���²��� */
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
			/** �����������Ҫ���ػ���ʾ�����²��� */
			/** �����Ҫ��ʾ�������� ���²��� */
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

	FLAYOUT_INLINE void FLayout::layoutVert(const Core::FRect& rcLayout)
	{
		beginLayout();

		/** �����ӿؼ�Ϊ0��״̬ */
		if (!hasChildren())
		{
			if (NULL != getScrollBar(SB_HORZ)) getScrollBar(SB_HORZ)->hide();
			if (NULL != getScrollBar(SB_VERT)) getScrollBar(SB_VERT)->hide();

			endLayout();
			return;
		}

		/** �ͻ�����С�������ڸ��ӿؼ����ֵ����� */
		Core::FRect rcClient;

		/** �������Ŀ���״̬ */
		bool bIsShowHScrollBar = false;
		bool bIsShowVScrollBar = false;

		/** ����������Ŀ���״̬ */
		bool bIsNeedShowHScrollBar = false;
		bool bIsNeedShowVScrollBar = false;

		/** ��Ҫ���������� */
		Core::FSize szNeedScroll;

		/** ��ȡ�ͻ�������Ч�������� */
		getValidLayoutRect(&rcClient);

		int cyUsable = rcClient.height();
		int nMeanSize = cyUsable;
		int nFixedSizeWidgetCount = 0;
		/** �ų��̶���С�Ŀؼ� */
		for (int i = 0; i < getChildrenCount(); ++i)
		{
			FWidget *pWidget = getChild(i);
			FASSERT(NULL != pWidget);

			if (pWidget->isVisible())
			{
				cyUsable -= pWidget->getHeight() + pWidget->getMargin().edgeHeight();
				if (cyUsable < 0)
				{
					cyUsable = 0;
					nMeanSize = 0;
					break;
				}
				if (pWidget->isFixedHeight()) nFixedSizeWidgetCount++;
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

		/** ���������״̬�����仯���¿�ʼ���� */
DO_RELAYOUT:	

		/** ��¼��ǰ�Ĺ���������״̬����������仯���²��֣� */
		layoutScrollBar();
		bIsShowHScrollBar = isShowScrollBar(SB_HORZ);
		bIsShowVScrollBar = isShowScrollBar(SB_VERT);

		/** ��ȡ�ͻ�������Ч�������� */
		getValidLayoutRect(&rcClient);

		Core::FRect rcUsable = rcClient;
		szNeedScroll.setEmpty();

		/** ���ӿؼ����� */
		for (int i = 0; i < getChildrenCount(); ++i)
		{
			FWidget *pWidget = getChild(i);
			FASSERT(NULL != pWidget);

			/*if (rcUsable.isEmpty())
			{
				pWidget->setLayoutRect(Core::FRect(0, 0, 0, 0));
				continue;
			}*/

			if (!pWidget->isVisible())
			{
				pWidget->setLayoutRect(Core::FRect(0, 0, 0, 0));
				continue;
			}

			Core::FPoint ptWidget;
			Core::FSize szWidget;

			/** ����ˮƽ��� */
			if (pWidget->isFixedWidth())
			{
				szWidget.cx = pWidget->getWidth();
			}
			else
			{
				szWidget.cx = rcUsable.width() - pWidget->getMargin().edgeWidth();
				if (pWidget->getMinWidth() > szWidget.cx) 
					szWidget.cx = pWidget->getMinWidth();
				if (pWidget->getMaxWidth() > 0 && pWidget->getMaxWidth() < szWidget.cx)
					szWidget.cx = pWidget->getMaxWidth();
			}

			/** ���㴹ֱ�߶� */
			if (pWidget->isFixedHeight())
			{
				szWidget.cy = pWidget->getHeight();
			}
			else
			{
				szWidget.cy = nMeanSize;
				if (pWidget->getMinHeight() > szWidget.cy) 
					szWidget.cy = pWidget->getMinHeight();
				if (pWidget->getMaxHeight() > 0 && pWidget->getMaxHeight() < szWidget.cy)
					szWidget.cy = pWidget->getMaxHeight();
			}

			/** ����ˮƽ���� */
			if (FOODOCK_RIGHT == pWidget->getDock() 
				|| fooTestAttribute(pWidget->getAlign(), FOOALIGN_RIGHT))
			{
				ptWidget.x = rcUsable.right - szWidget.cx - pWidget->getMargin().right;
			}
			else if (fooTestAttribute(pWidget->getAlign(), FOOALIGN_XCENTER))
			{
				ptWidget.x = rcUsable.left + (rcUsable.width() - szWidget.cx) / 2 + pWidget->getMargin().left;
			}
			else
			{
				ptWidget.x = rcUsable.left + pWidget->getMargin().left;
			}

			/** ���㴹ֱ���� */
			ptWidget.y = rcUsable.top + pWidget->getMargin().top;

			/** �ӿ��ò��������ȥ�������� */
			rcUsable.top += szWidget.cy + pWidget->getMargin().edgeHeight();

			/** ��Ҫ�����ķ�Χ */
			szNeedScroll.cx = max(szNeedScroll.cx, ptWidget.x - rcClient.left + szWidget.cx + pWidget->getMargin().right);
			szNeedScroll.cy += szWidget.cy + pWidget->getMargin().edgeWidth();

			/** �����������Ҫ���ػ���ʾ�����²��� */
			/** �����Ҫ��ʾ�������� ���²��� */
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
			/** �����������Ҫ���ػ���ʾ�����²��� */
			/** �����Ҫ��ʾ�������� ���²��� */
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

	FLAYOUT_INLINE void FLayout::layoutFlow(const Core::FRect& rcLayout)
	{

	}

	FLAYOUT_INLINE bool FLayout::setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		if (_tcscmp(pszName, _T("layout")) == 0 
			|| _tcscmp(pszName, _T("layoutstyle")) == 0)
		{
			if (_tcscmp(pszValue, _T("none")) == 0) setLayoutStyle(FLayout::NoneLayout);
			else if (_tcscmp(pszValue, _T("horz")) == 0) setLayoutStyle(FLayout::HorzLayout);
			else if (_tcscmp(pszValue, _T("vert")) == 0) setLayoutStyle(FLayout::VertLayout);
		}
		else if (_tcscmp(pszName, _T("cursel")) == 0 
			|| _tcscmp(pszName, _T("curselected")) == 0)
		{
			m_nCurSelected = _tcstol(pszValue, NULL, 10);
		}

		return __super::setAttribute(pMarkup, pszName, pszValue);
	}

	FLAYOUT_INLINE bool FLayout::endMarkup(Markup::FMarkup* pMarkup)
	{
		/** ����Ĭ��ѡ�� */
		if (m_nCurSelected < 0 || m_nCurSelected > getChildrenCount())
		{
			m_nCurSelected = 0;
		}

		if (getLayoutStyle() == FLayout::TabLayout)
		{
			for (int i = 0; i < getChildrenCount(); ++i)
			{
				FWidget *pWidget = getChild(i);
				FASSERT(NULL != pWidget);
				pWidget->setVisible(m_nCurSelected == i);
			}
		}

		return __super::endMarkup(pMarkup);
	}

}} // !namespace