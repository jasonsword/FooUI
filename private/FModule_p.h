/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FModule_p.h
* ������		FModule_p��FooUIģ�飬
*				Ϊ�˱�֤����ȫ�ֱ���������˳���ʵ���Ӧ������ȫ�ֱ��������ģ��
* �������ڣ�	2013-08-04
*
*/

#ifndef FMODULE_H
#define FMODULE_H

#include "core/FPtrList.h"

#include "widgets/FWindowPrivate_p.h"
#include "widgets/FToolTip.h"
#include "markup/FMarkup.h"

namespace FooUI
{
	//////////////////////////////////////////////////////////////////////////
	// FModule
	class FModule
	{
	public:
		FModule(void);
		~FModule(void);

	public:
		/** ���ڴ���ģ�� */
		static Widgets::FWindowCreateHelper* window_create_helper(void);

		/** Tab˳���� */
		static Core::FPtrList* tabwidgets(void);

		/** ȫ�ֵ�Tooltip���� */
		static Widgets::FToolTip* tooltip(void);
		static bool replaceToolTip(Widgets::FToolTip* tooltip);

		/** MessageFilter */
		static Core::FPtrList* msgfilterlist(void);

		static Markup::CSkinPackage* setResourcePackage(Markup::CSkinPackage* pResoucePackage);
		static Markup::CSkinPackage* getSkinPackage(void);
		static void destroyResourcePackage(void);

	private:
		/** ���ڴ��������໯��ģ�� */
		Widgets::FWindowCreateHelper* m_pWindowCreateHelper;
		/** ����Tab�ؼ�˳�� */
		Core::FPtrList* m_tabWidgets;
		/** ȫ��Ĭ�ϵ�ToolTip���� */
		Widgets::FToolTip* m_pToolTip;
		/** FApplication::addMessageFilterʹ�� */
		Core::FPtrList* m_pMsgFilterList;

		/** Ƥ����Դ�� */
		Markup::CSkinPackage* m_pResoucePackage;
	};

} // !namespace

#endif // !FMODULE_H