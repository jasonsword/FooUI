/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FHotKey.cpp
* 描述：		FHotKey, 热键控件
* 创建日期：	2013-08-04
*
*/

#include "StdAfx.h"

#include "core/FString.h"
#include "widgets/FHotKey.h"

#define FHOTKEY_INLINE FOOINLINE

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FHotKey
	FHOTKEY_INLINE FHotKey::FHotKey(FWidget* parent/* = NULL*/) : 
		FWidget(parent) 
		, m_dwHotKey(0)
	{
		setText(_T("无"));
	}

	FHOTKEY_INLINE FHotKey::~FHotKey(void)
	{
	}

	FHOTKEY_INLINE void FHotKey::setHotKey(DWORD dwHotKey)
	{
		if (dwHotKey == m_dwHotKey)
		{
			return;
		}

		m_dwHotKey = dwHotKey;
		DWORD wModifiers = (DWORD)HIWORD(m_dwHotKey);
		DWORD wVirtualKeyCode = (DWORD)LOWORD(m_dwHotKey);

		switch (wVirtualKeyCode)
		{
		case VK_BACK:
		case VK_LWIN:
		case VK_RWIN:
		case VK_LCONTROL:
		case VK_RCONTROL:
		case VK_CONTROL:
		case VK_LSHIFT:
		case VK_RSHIFT:
		case VK_SHIFT:
		case VK_LMENU:
		case VK_RMENU:
		case VK_MENU:
			wVirtualKeyCode = 0;
			break;

		default:
			break;
		}

		if (wVirtualKeyCode == 0 
			&& wModifiers == 0)
		{
			m_dwHotKey = 0;
			setText(_T("无"));
		}
		else
		{
			Core::FString strText;

			/** wModifiers */
			if (fooTestAttribute(wModifiers, MOD_WIN))
			{
				strText += _T("Win + ");
			}
			if (fooTestAttribute(wModifiers, MOD_CONTROL))
			{
				strText += _T("Ctrl + ");
			}
			if (fooTestAttribute(wModifiers, MOD_SHIFT))
			{
				strText += _T("Shift + ");
			}
			if (fooTestAttribute(wModifiers, MOD_ALT))
			{
				strText += _T("Alt + ");
			}

			/** wVirtualKeyCode */
			switch (wVirtualKeyCode)
			{
			case VK_PAUSE:		strText += _T("Pause Break");	break;
			case VK_PRIOR:		strText += _T("Page Up");		break;
			case VK_NEXT:		strText += _T("Page Down");		break;
			case VK_END:		strText += _T("End");			break;
			case VK_HOME:		strText += _T("Home");			break;
			case VK_LEFT:		strText += _T("Left");			break;
			case VK_UP:			strText += _T("Up");			break;
			case VK_RIGHT:		strText += _T("Right");			break;
			case VK_DOWN:		strText += _T("Down");			break;
			case VK_SELECT:		strText += _T("Select");		break;
			case VK_PRINT:		strText += _T("Print");			break;
			case VK_EXECUTE:	strText += _T("Execute");		break;
			case VK_SNAPSHOT:	strText += _T("Snapshot");		break;
			case VK_INSERT:		strText += _T("Insert");		break;
			case VK_DELETE:		strText += _T("Delete");		break;
			case VK_HELP:		strText += _T("Help");			break;
			case VK_NUMLOCK:	strText += _T("Num Lock");		break;
			default:
				if (wVirtualKeyCode != 0)
				{
					TCHAR tchKeyName[255] = {0};
					::GetKeyNameText(MapVirtualKey(wVirtualKeyCode, 0) << 16, tchKeyName, 255);
					strText += tchKeyName;
				}
				break;
			}

			setText(strText);
		}

		makeWidgetMessage(FWM_CHANGED, CT_VALUE);
	}

	FHOTKEY_INLINE void FHotKey::setHotKey(DWORD wVirtualKeyCode, DWORD wModifiers)
	{
		setHotKey((DWORD)MAKELONG(wVirtualKeyCode, wModifiers));
	}

	FHOTKEY_INLINE DWORD FHotKey::getHotKey(void) const
	{
		return m_dwHotKey;
	}

	FHOTKEY_INLINE void FHotKey::getHotKey(DWORD& wVirtualKeyCode, DWORD& wModifiers) const
	{
		wVirtualKeyCode = getVirtualKey();
		wModifiers = getModifiers();
	}

	FHOTKEY_INLINE DWORD FHotKey::getVirtualKey(void) const
	{
		return (DWORD)LOWORD(m_dwHotKey);
	}

	FHOTKEY_INLINE DWORD FHotKey::getModifiers(void) const
	{
		return (DWORD)HIWORD(m_dwHotKey);
	}

	FHOTKEY_INLINE void FHotKey::keyPressEvent(Event::FKeyEvent* e)
	{
		DWORD wModifiers = 0;
		if (::GetKeyState(VK_LWIN) < 0 || ::GetKeyState(VK_RWIN) < 0) wModifiers |= MOD_WIN;
		if (::GetKeyState(VK_CONTROL) < 0) wModifiers |= MOD_CONTROL;
		if (::GetKeyState(VK_SHIFT) < 0) wModifiers |= MOD_SHIFT;
		if (::GetKeyState(VK_MENU) < 0) wModifiers |= MOD_ALT;

		if (VK_TAB == e->key 
			&& 0 == wModifiers)
		{
			// 按下Tab希望能正常使用Tab按键切换焦点
		}
		else if (0 == wModifiers && VK_BACK == e->key)
		{
			setHotKey(0);
		}
		else if (0 == wModifiers && VK_DELETE == e->key)
		{
			setHotKey(0);
		}
		else
		{
			//UINT uScanCode = (UINT)(e->flags & 0x0FF);
			//bool bExtended = (e->flags & 0x0100);
			
			//fooDebugf(_T("[%d] scancode:%d \r\n"), ::GetTickCount(), uScanCode);
			
			e->accpet(true);
			setHotKey(e->key, wModifiers);
		}
	}

	FHOTKEY_INLINE void FHotKey::keyReleaseEvent(Event::FKeyEvent* e)
	{
		if (getVirtualKey() == 0)
		{
			setHotKey(0, 0);
		}
	}

}} // !namespace