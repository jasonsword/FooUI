/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FHotKey.h
* ������		FHotKey, �ȼ��ؼ�
* �������ڣ�	2013-08-04
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
		/** �ȼ����μ������ܰ�������һ������ֵ�����
			MOD_WIN 
			MOD_CONTROL 
			MOD_SHIFT 
			MOD_ALT 
		*/
		DWORD getVirtualKey(void) const;
		/** �����ֵ */
		DWORD getModifiers(void) const;

	protected:
		void keyPressEvent(Event::FKeyEvent* e);
		void keyReleaseEvent(Event::FKeyEvent* e);

	private:
		/** ���ֽ�Ϊmodifiers�����ֽ�Ϊvirtualkey */
		DWORD m_dwHotKey;
	};

}} // !namespace

#endif // !FHOTKEY_H