/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FButton.cpp
* 描述：		FButton, 按钮（Button)、多选按钮(checkbutton)、单选按钮(radiobutton)
* 创建日期：	2013-07-23
*
*/

#include "StdAfx.h"
#include "widgets/FButton.h"

#define FBUTTON_INLINE FOOINLINE

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FButton
	FBUTTON_INLINE FButton::FButton(eFButtonStyle dwButtonStyle/* = PushButton*/, 
					FWidget* parent/* = NULL*/) : 
		FWidget(parent)
		, m_dwButtonStyle(dwButtonStyle) 
	{
	}

	FBUTTON_INLINE FButton::~FButton(void)
	{
	}

	FBUTTON_INLINE void FButton::setButtonStyle(DWORD dwButtonStyle)
	{
		if (m_dwButtonStyle == dwButtonStyle)
		{
			return;
		}

		m_dwButtonStyle = dwButtonStyle;
		repaint();
	}

	FBUTTON_INLINE DWORD FButton::getButtonStyle(void) const
	{
		return m_dwButtonStyle;
	}

	bool FButton::isPushButton(void) const
	{
		return (FButton::PushButton == m_dwButtonStyle);
	}

	bool FButton::isCheckButton(void) const
	{
		return (FButton::CheckButton == m_dwButtonStyle);
	}

	bool FButton::isRadioButton(void) const
	{
		return (FButton::RadioButton == m_dwButtonStyle);
	}

	void FButton::setCheck(bool bChecked)
	{
		if (isRadioButton() && bChecked)
		{
			if (isChecked())
			{
				return;
			}

			FWidget *parent = getParent();
			if (NULL == parent)
			{
				return;
			}

			for (int i = 0; i < parent->getChildrenCount(); ++i)
			{
				FWidget *pWidget = parent->getChild(i);
				if (_tcscmp(pWidget->getClass(), FOOWC_BUTTON) != 0)
				{
					continue;
				}

				FButton *pButton = static_cast<FButton*>(pWidget);
				if (!pButton->isRadioButton() 
					|| _tcscmp(pButton->getGroup(), getGroup()) != 0)
				{
					continue;
				}

				if (this != pButton)
				{
					pButton->setCheck(false);
				}
			}
		}

		__super::setCheck(bChecked);
	}

	void FButton::mouseClickEvent(Event::FMouseEvent* e)
	{
		if (LeftButton == e->button)
		{
			if (isCheckButton())
			{
				setCheck(!isChecked());
			}
			else if (isRadioButton())
			{
				setCheck(true);
			}
		}

		__super::mouseClickEvent(e);
	}

	void FButton::keyPressEvent(Event::FKeyEvent* e)
	{
		if (VK_SPACE == e->key)
		{
			setStatePushed(true);
		}

		if (VK_RETURN == e->key)
		{
			if (isCheckButton())
			{
				setCheck(!isChecked());
			}
			else if (isRadioButton())
			{
				setCheck(true);
			}
			else
			{
				makeWidgetMessage(FWM_MOUSECLICK, (WPARAM)MK_LBUTTON, 0L);
			}
		}

		__super::keyPressEvent(e);
	}

	void FButton::keyReleaseEvent(Event::FKeyEvent* e)
	{
		if (VK_SPACE == e->key)
		{
			if (isCheckButton())
			{
				setCheck(!isChecked());
			}
			else if (isRadioButton())
			{
				setCheck(true);
			}
			else
			{
				makeWidgetMessage(FWM_MOUSECLICK, (WPARAM)MK_LBUTTON, 0L);
			}
 
			POINT pt;
			::GetCursorPos(&pt);
			::ScreenToClient(getSafeHwnd(), &pt);
			if (findWidget(pt) == this) setStateHover(true);
			else setStateNormal(true);
		}
	 
		__super::keyReleaseEvent(e);
	}

	bool FButton::setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		return __super::setAttribute(pMarkup, pszName, pszValue);
	}

}} // !namespace