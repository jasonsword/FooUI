/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FListCtrl.h
* 描述：		FListCtrl
* 创建日期：	2013-07-28
*
*/

#ifndef FLISTCTRL_H
#define FLISTCTRL_H

#include "core/FBase.h"
#include "widgets/FAbstractList.h"
#include "widgets/FHeader.h"

namespace FooUI { namespace Widgets {

	class FListCtrl;
	class FListCtrlElement;

	//////////////////////////////////////////////////////////////////////////
	// FListCtrl
	#define FOOWC_LISTCTRL _T("listctrl")
	class FOOUI_LIB FListCtrl : public FAbstractList
	{
		friend FListCtrlElement;
	public:
		explicit FListCtrl(FWidget* parent = NULL);
		~FListCtrl(void);
		FOODECLARE_CLASS(FOOWC_LISTCTRL)
		FOODECLARE_CONTAINER(true)

	public:
		int addElement(FListCtrlElement* element);
		int insertElement(int nIndex, FListCtrlElement* element);
		bool removeElement(int nIndex);
		bool removeElement(FListCtrlElement* element);
		FListCtrlElement* getElement(int nIndex) const;
		bool clearElements(void);
		int getElementsCount(void) const;

		bool selectElement(int nIndex);

	public:
		FHeader* getHeader(void) const;

	protected:
		void layout(const Core::FRect& rcLayout);

	protected:
		bool markupAddWidget(Markup::FMarkup* pMarkup, FWidget* pWidget);

	private:
		class FListCtrlLayout;
		FListCtrlLayout* m_pListCtrlLayout;

		FHeader* m_pHeader;
	};

	//////////////////////////////////////////////////////////////////////////
	// 
	#define FOOWC_LISTCTRLELEMENT _T("listctrlelement")
	class FOOUI_LIB FListCtrlElement : public FAbstractListElement
	{
		friend FListCtrl;
	public:
		explicit FListCtrlElement(FWidget* parent = NULL);
		~FListCtrlElement(void);
		FOODECLARE_CLASS(FOOWC_LISTCTRLELEMENT)
		FOODECLARE_CONTAINER(true)

	protected:
		void layout(const Core::FRect& rcLayout);

	protected:
		void mousePressEvent(Event::FMouseEvent* e);
	};

}} // !namespace

#endif // !FLISTCTRL_H