/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FMenu.h
* 描述：		FMenu, 菜单
* 创建日期：	2015-05-17
*
*/

#ifndef FEDIT_H
#define FEDIT_H

#include "core/FBase.h"
#include "widgets/FWidget.h"
#include "markup/FFontResource.h"

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FEdit
	#define FOOWC_EDIT _T("fmenu")
	class FOOUI_LIB FEdit : public FWidget
	{
	public:
		FEdit(FWidget* parent = NULL);
		~FEdit(void);
		FOODECLARE_CLASS(FOOWC_EDIT)
		FOODECLARE_CONTAINER(true)

	public:
		bool createWindow(HWND hParentWnd);

		void setPasswordChar(TCHAR);
		void clearPasswordChar(void);
		TCHAR getPasswordChar(void) const;

		void setLimitText(UINT nMax);
		void clearLimitText(UINT nMax);
		UINT getLimitText(void) const;

		void setNumberOnly(bool bNumberOnly = true);
		bool isNumberOnly(void) const;

		void setReadOnly(bool bReadOnly = true);
		bool isReadOnly(void) const;

		void setSel(int nStartChar, int nEndChar);
		void selectAll(void);
		int getSelStart(void) const;
		int getSelLength(void) const;

	protected:
		bool setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue);
		bool endMarkup(Markup::FMarkup* pMarkup);

	protected:
		LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
		LRESULT handleParentMessage(HWND hParentWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	protected:
		void paintEvent(Event::FPaintEvent* e);
		void showEvent(Event::FEvent* e);
		void hideEvent(Event::FEvent* e);
		void changedEvent(Event::FChangedEvent* e);
		void layoutChangedEvent(Event::FLayoutChangedEvent* e);
		void parentWindowCreatedEvent(Event::FParentWindowEvent* e);

	protected:
		class FEditPrivate;
		friend FEditPrivate;
		FEditPrivate* const m_editPrivate;
		Markup::FFontResource m_resFont;
	};

}} // !namespace

#endif // !FEDIT_H