/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FButton.h
* ������		FButton, ��ť��Button)����ѡ��ť(checkbutton)����ѡ��ť(radiobutton)
* �������ڣ�	2013-07-23
*
*/

#ifndef FBUTTON_H
#define FBUTTON_H

#include "core/FBase.h"
#include "widgets/FWidget.h"

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FButton
	#define FOOWC_BUTTON _T("button")
	#define FOOWC_CHECKBUTTON _T("checkbutton")
	#define FOOWC_RADIOBUTTON _T("radiobutton")
	class FOOUI_LIB FButton : public FWidget
	{
	public:
		enum eFButtonStyle
		{
			PushButton = 0, 
			CheckButton, 
			RadioButton, 
		};

		FButton(eFButtonStyle dwButtonStyle = FButton::PushButton, FWidget* parent = NULL);
		~FButton(void);
		FOODECLARE_CLASS(FOOWC_BUTTON)
		FOODECLARE_CONTAINER(true)

	public:
		void setButtonStyle(DWORD dwButtonStyle);
		DWORD getButtonStyle(void) const;
		bool isPushButton(void) const;
		bool isCheckButton(void) const;
		bool isRadioButton(void) const;

	public:
		void setCheck(bool bChecked);

	protected:
		void mouseClickEvent(Event::FMouseEvent* e);
		void keyPressEvent(Event::FKeyEvent* e);
		void keyReleaseEvent(Event::FKeyEvent* e);

	protected:
		bool setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue);

	private:
		DWORD m_dwButtonStyle;
	};

	#define BUTTONGROUPNAME(groupname) \
		(NULL != pSender->getInterface(FOOWC_BUTTON) && (_tcscmp(((FButton*)pSender)->getGroup(), groupname) == 0))

}} // !namespace

#endif