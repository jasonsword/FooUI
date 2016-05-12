/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FAbstractList.h
* 描述：		FAbstractList
* 创建日期：	2013-07-25
*
*/

#ifndef FABSTRACTLIST_H
#define FABSTRACTLIST_H

#include "core/FBase.h"
#include "widgets/FWidget.h"
#include "widgets/FLayout.h"

namespace FooUI { namespace Widgets {

	class FAbstractList;
	class FAbstractListElement;

	//////////////////////////////////////////////////////////////////////////
	// FAbstractList
	#define FOOWC_ABSTRACTLIST _T("abstractlist")
	class FOOUI_LIB FAbstractList : public FLayout
	{
		friend FAbstractListElement;
	public:
		explicit FAbstractList(FWidget* parent = NULL);
		FOODECLARE_CLASS(FOOWC_ABSTRACTLIST)
		FOODECLARE_CONTAINER(true);

	public:
		virtual bool selectElement(int nIndex);
		virtual bool selectElement(FAbstractListElement* element);
		virtual FAbstractListElement* getSelectedElement(void) const;

	protected:
		virtual void elementClickedEvent(FAbstractListElement* pSender);

	protected:
		FAbstractListElement* m_pSelectedElement;
	};

	//////////////////////////////////////////////////////////////////////////
	// FAbstractListElement
	#define FOOWC_ABSTRACTLISTELEMENT _T("abstractlistelement")
	class FOOUI_LIB FAbstractListElement : public FWidget
	{
		friend FAbstractList;
	public:
		FAbstractListElement(FWidget* parent = NULL);
		virtual ~FAbstractListElement(void);
		FOODECLARE_CLASS(FOOWC_ABSTRACTLISTELEMENT);
		FOODECLARE_CONTAINER(true);

	public:
		bool selectElement(void);
		FAbstractList* getList(void) const;

	protected:
		void mouseClickEvent(Event::FMouseEvent* e);
		void reflectEvent(Event::FReflectEvent* e);
	};

}} // !namespace

#endif // !FABSTRACTLIST_H