/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FAbstractList.cpp
* 描述：		FAbstractList
* 创建日期：	2013-07-25
*
*/

#include "StdAfx.h"
#include "widgets/FAbstractList.h"

#define FABSTRACTLIST_INLINE FOOINLINE

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FAbstractList
	FABSTRACTLIST_INLINE FAbstractList::FAbstractList(FWidget* parent/* = NULL*/) : 
		FLayout(FLayout::VertLayout, parent) 
		, m_pSelectedElement(NULL)
	{
	}

	FABSTRACTLIST_INLINE bool FAbstractList::selectElement(int nIndex)
	{
		FWidget *pWidget = getChild(nIndex);
		if (NULL == pWidget) return false;

		return selectElement((FAbstractListElement*)pWidget->getInterface(FOOWC_ABSTRACTLISTELEMENT));
	}

	FABSTRACTLIST_INLINE bool FAbstractList::selectElement(FAbstractListElement* element)
	{
		FAbstractListElement* pOldElement = m_pSelectedElement;
		m_pSelectedElement = element;

		if (pOldElement != element)
		{
			if (NULL != pOldElement)
			{
				pOldElement->setStateUnSelected(true);
			}

			if (NULL != element)
			{
				element->setStateSelected(true);
				makeWidgetMessage(FWM_ITEMSELECTED, (WPARAM)element->childIndex(), (LPARAM)element);
			}

			return true;
		}

		return false;
	}

	FABSTRACTLIST_INLINE FAbstractListElement* FAbstractList::getSelectedElement(void) const
	{
		return m_pSelectedElement;
	}

	FABSTRACTLIST_INLINE void FAbstractList::elementClickedEvent(FAbstractListElement* pSender)
	{
		FASSERT(NULL != pSender);
		selectElement(pSender);
	}

	//////////////////////////////////////////////////////////////////////////
	// FAbstractListElement
	FABSTRACTLIST_INLINE FAbstractListElement::FAbstractListElement(FWidget* parent/* = NULL*/) : 
		FWidget(parent)
	{
	}

	FABSTRACTLIST_INLINE FAbstractListElement::~FAbstractListElement(void)
	{
		FAbstractList *pAbstractList = getList();
		if (NULL != pAbstractList && this == pAbstractList->m_pSelectedElement)
		{
			pAbstractList->m_pSelectedElement = NULL;
		}
	}

	FAbstractList* FAbstractListElement::getList(void) const
	{
		FWidget *parent = getParent();
		while (NULL != parent)
		{
			FAbstractList *pAbstractList = static_cast<FAbstractList*>(parent->getInterface(FOOWC_ABSTRACTLIST));
			if (NULL != pAbstractList)
			{
				return pAbstractList;
			}

			parent = parent->getParent();
		}

		return NULL;
	}

	FABSTRACTLIST_INLINE bool FAbstractListElement::selectElement(void)
	{
		FAbstractList *pAbstractList = getList();
		if (NULL != pAbstractList)
		{
			pAbstractList->elementClickedEvent(this);
			return true;
		}

		return false;
	}

	FABSTRACTLIST_INLINE void FAbstractListElement::mouseClickEvent(Event::FMouseEvent* e)
	{
		if (LeftButton == e->button)
		{
			selectElement();
		}
	}

	FABSTRACTLIST_INLINE void FAbstractListElement::reflectEvent(Event::FReflectEvent* e)
	{
		switch (e->event->type())
		{
		case Event::FEvent::MouseButtonPress:
			{
				Event::FMouseEvent* mouseEvent = static_cast<Event::FMouseEvent*>(e->event);

				mouseClickEvent(mouseEvent);
			}
			break;

		default:
			__super::reflectEvent(e);
			break;
		}
	}

}} // ! namespace