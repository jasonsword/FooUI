/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FHotKey.h
* 描述：		FHotKey, 热键控件
* 创建日期：	2013-08-04
*
*/

#ifndef FHOTKEY_H
#define FHOTKEY_H

#include "core/FBase.h"

#include "widgets/FWidget.h"

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FHotKey
	#define FOOWC_HOTKEY _T("hotkey")
	class FOOUI_LIB FHotKey : public FWidget
	{
	public:
		explicit FHotKey(FWidget* parent = NULL);
		~FHotKey(void);
		FOODECLARE_CLASS(FOOWC_HOTKEY)
		FOODECLARE_CONTAINER(true)

	public:
		void setHotKey(DWORD dwHotKey);
		void setHotKey(DWORD wVirtualKeyCode, DWORD wModifiers);
		DWORD getHotKey(void) const;
		void getHotKey(DWORD& wVirtualKeyCode, DWORD& wModifiers) const;
		/** 热键修饰键，可能包含以下一个或多个值的组合
			MOD_WIN 
			MOD_CONTROL 
			MOD_SHIFT 
			MOD_ALT 
		*/
		DWORD getVirtualKey(void) const;
		/** 虚拟键值 */
		DWORD getModifiers(void) const;

	protected:
		void keyPressEvent(Event::FKeyEvent* e);
		void keyReleaseEvent(Event::FKeyEvent* e);

	private:
		/** 高字节为modifiers、低字节为virtualkey */
		DWORD m_dwHotKey;
	};

}} // !namespace

#endif // !FHOTKEY_H