/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FComboBox.h
* 描述：		FComboBox, 组合框
* 创建日期：	2013-07-24
*
*/

#ifndef FCOMBOBOX_H
#define FCOMBOBOX_H

#include "core/FBase.h"
#include "widgets/FWidget.h"
#include "widgets/FAbstractList.h"
#include "markup/FFontResource.h"

namespace FooUI { namespace Widgets {

	class FComboBox;
	class FComboBoxList;

	//////////////////////////////////////////////////////////////////////////
	// FComboBox
	#define FOOWC_COMBOBOX _T("combobox")
	class FOOUI_LIB FComboBox : public FWidget
	{
		friend FComboBoxList;
	public:
		enum eComboBoxStyle
		{
			DroplistComboBox = 0,
			DropdownComboBox, 
			SimpleComboBox, 
		};

		explicit FComboBox(DWORD dwComboBoxStyle = DroplistComboBox, FWidget* parent = NULL);
		~FComboBox(void);
		FOODECLARE_CLASS(FOOWC_COMBOBOX)
		FOODECLARE_CONTAINER(true)

	public:
		void setComboBoxStyle(DWORD dwComboBoxStyle);
		DWORD getComboBoxStyle(void) const;

	public:
		bool selectElement(int nIndex);
		bool selectElement(FAbstractListElement* element);
		FAbstractListElement* getSelectedElement(void) const;

	public:
		bool createWindow(HWND hParentWnd);
		void setPasswordChar(TCHAR);
		void emptyPasswordChar(void);

	protected:
		bool setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue);
		bool endMarkup(Markup::FMarkup* pMarkup);
		bool markupAddWidget(Markup::FMarkup* pMarkup, FWidget* pWidget);

	protected:
		LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
		LRESULT handleParentMessage(HWND hParentWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	protected:
		void paintEvent(Event::FPaintEvent* e);
		void showEvent(Event::FEvent* e);
		void hideEvent(Event::FEvent* e);
		void mousePressEvent(Event::FMouseEvent* e);
		void changedEvent(Event::FChangedEvent* e);
		void layoutChangedEvent(Event::FLayoutChangedEvent* e);
		void parentWindowCreatedEvent(Event::FParentWindowEvent* e);

	private:
		class FComboBoxPrivate;
		friend FComboBoxPrivate;
		FComboBoxPrivate* m_comboboxPrivate;
		TCHAR m_passwordChar;
		Markup::FFontResource m_resFont;
		FComboBoxList* m_comboboxList;
		DWORD m_dwComboBoxStyle;
	};

	//////////////////////////////////////////////////////////////////////////
	// FComboBoxList
	#define FOOWC_COMBOBOXLIST _T("comboboxlist")
	class FOOUI_LIB FComboBoxList : public FAbstractList
	{
		friend FComboBox;
	public:
		explicit FComboBoxList(FComboBox* parent = NULL);
		~FComboBoxList(void);
		FOODECLARE_CLASS(FOOWC_COMBOBOXLIST)
		FOODECLARE_CONTAINER(true)

	public:
		bool selectElement(int nIndex);
		bool selectElement(FAbstractListElement* element);

	protected:
		void dropdown(bool bDropdown = true);
		bool isDropdown(void) const;

	protected:
		void elementClickedEvent(FAbstractListElement* pSender);

	protected:
		LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	};

}}

#endif // !FCOMBOBOX_H