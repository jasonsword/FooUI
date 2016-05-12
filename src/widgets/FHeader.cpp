/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FHeader.cpp
* 描述：		FHeader, 列表头
* 创建日期：	2013-07-25
*
*/

#include "StdAfx.h"
#include "widgets/FHeader.h"

#define FHEADER_INLINE FOOINLINE

#define FHEADER_SIZEGRIP_DELTA 5

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FHeader
	FHEADER_INLINE FHeader::FHeader(FWidget* parent/* = NULL*/) : 
		FLayout(FLayout::HorzLayout, parent)
	{
		modifyFlag(FOOFLAG_TRANSPARENT, 0);
	}

	FHEADER_INLINE FHeader::~FHeader(void)
	{
	}

	FHEADER_INLINE FWidget* FHeader::widgetFromPoint(int xPos, int yPos) const
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

		FWidget *widgetLast = NULL;
		for (int i = 0; i < getChildrenCount(); ++i)
		{
			FWidget *widget = getChild(i);
			
			if (!widget->isVisible())
			{
				continue;
			}

			FHeaderElement *headerElement = static_cast<FHeaderElement*>(widget->getInterface(FOOWC_HEADERELEMENT));
			if (NULL != headerElement 
				&& headerElement->isSizable() 
				&& headerElement->getLayoutRect().isEmpty())
			{
				// 当列表头项宽度为0时，使能够得到改变宽度的机会（鼠标拉伸）
				if (NULL != widgetLast 
					&& xPos >= widgetLast->getLayoutRect().right
					&& xPos <= widgetLast->getLayoutRect().right + FHEADER_SIZEGRIP_DELTA )
				{
					return widget;
				}
				else if (xPos >= getLayoutRect().left
					&& xPos <= getLayoutRect().left + FHEADER_SIZEGRIP_DELTA)
				{
					return widget;
				}
			}
	
			FWidget *pFindWidget = widget->widgetFromPoint(xPos, yPos);
			if (NULL != pFindWidget)
			{
				return pFindWidget;
			}

			widgetLast = widget;
		}

		return NULL;
	}

	FHEADER_INLINE void FHeader::elementWidthChanged(FHeaderElement* element, int width)
	{
		relayoutpaintParent();
	}

	//////////////////////////////////////////////////////////////////////////
	// FHeaderElement
	FHEADER_INLINE FHeaderElement::FHeaderElement(FWidget* parent/* = NULL*/) : 
		FWidget(parent) 
		, m_bSizable(true)
	{
	}

	FHEADER_INLINE FHeaderElement::~FHeaderElement(void)
	{
	}

	void FHeaderElement::sizable(bool bSizable/* = true*/)
	{
		m_bSizable = bSizable;
	}

	bool FHeaderElement::isSizable(void) const
	{
		return m_bSizable;
	}

	FHEADER_INLINE void FHeaderElement::cursorEvent(Event::FCursorEvent* e)
	{
		if (isSizable() 
			&& (getLayoutRect().isEmpty() || e->mouse.x >= getLayoutRect().right - FHEADER_SIZEGRIP_DELTA))
		{
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			e->accpet();
			return;
		}

		__super::cursorEvent(e);
	}

	static bool FHeaderElement_SizeMode = false;
	static Core::FRect FHeaderElement_Rect(0, 0, 0, 0);
	static int FHeaderElement_MouseX = 0;
	FHEADER_INLINE void FHeaderElement::mousePressEvent(Event::FMouseEvent* e)
	{
		::ReleaseCapture();
		::SetCapture(getSafeHwnd());

		if (isSizable() 
			&& LeftButton == e->button 
			&& (getLayoutRect().isEmpty() || e->mouse.x >= getLayoutRect().right - FHEADER_SIZEGRIP_DELTA))
		{
			FHeaderElement_SizeMode = true;
			FHeaderElement_Rect = getLayoutRect();
			FHeaderElement_MouseX = e->mouse.x;
		}
		else
		{
			FHeaderElement_SizeMode = false;
		}

		__super::mousePressEvent(e);
	}

	FHEADER_INLINE void FHeaderElement::mouseMoveEvent(Event::FMouseEvent* e)
	{
		if (isSizable() 
			&& LeftButton == e->button 
			&& FHeaderElement_SizeMode)
		{
			int nWidth = e->mouse.x - FHeaderElement_Rect.left + (FHeaderElement_Rect.right - FHeaderElement_MouseX);
			if (nWidth < 0) nWidth = 0;
			if (getMinWidth() > nWidth) nWidth = getMinWidth();
			if (getMaxWidth() > 0 && getMaxWidth() < nWidth) nWidth = getMaxWidth();

			FHeader *pHeader = NULL;
			if (NULL != getParent())
			{
				pHeader = static_cast<FHeader*>(getParent()->getInterface(FOOWC_HEADER));
			}

			setWidth(nWidth);
			if (NULL != pHeader)
			{
				pHeader->elementWidthChanged(this, nWidth);
			}
		}

		__super::mouseMoveEvent(e);
	}

	FHEADER_INLINE void FHeaderElement::mouseReleaseEvent(Event::FMouseEvent* e)
	{
		::ReleaseCapture();
		__super::mouseReleaseEvent(e);
	}

	FHEADER_INLINE void FHeaderElement::mouseDoubleClickEvent(Event::FMouseEvent* e)
	{
		if (isSizable() 
			&& LeftButton == e->button )
		{
			FHeader *pHeader = NULL;
			if (NULL != getParent())
			{
				pHeader = static_cast<FHeader*>(getParent()->getInterface(FOOWC_HEADER));
			}

			emptyWidth();
			if (NULL != pHeader)
			{
				pHeader->elementWidthChanged(this, 0);
			}
		}

		__super::mouseDoubleClickEvent(e);
	}

	FHEADER_INLINE bool FHeaderElement::setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		if (_tcscmp(pszName, _T("sizable")) == 0)
		{
			sizable(Markup::fooMarkupParseBoolean(pszValue));
		}
		return __super::setAttribute(pMarkup, pszName, pszValue);
	}

}} // !namespace