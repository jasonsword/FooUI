/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FModule_p.h
* 描述：		FModule_p，FooUI模块，
*				为了保证所有全局变量析构的顺序适当，应尽量将全局变量放入此模块
* 创建日期：	2013-08-04
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
		/** 窗口创建模块 */
		static Widgets::FWindowCreateHelper* window_create_helper(void);

		/** Tab顺序链 */
		static Core::FPtrList* tabwidgets(void);

		/** 全局的Tooltip窗口 */
		static Widgets::FToolTip* tooltip(void);
		static bool replaceToolTip(Widgets::FToolTip* tooltip);

		/** MessageFilter */
		static Core::FPtrList* msgfilterlist(void);

		static Markup::CSkinPackage* setResourcePackage(Markup::CSkinPackage* pResoucePackage);
		static Markup::CSkinPackage* getSkinPackage(void);
		static void destroyResourcePackage(void);

	private:
		/** 窗口创建（子类化）模块 */
		Widgets::FWindowCreateHelper* m_pWindowCreateHelper;
		/** 储存Tab控件顺序 */
		Core::FPtrList* m_tabWidgets;
		/** 全局默认的ToolTip窗口 */
		Widgets::FToolTip* m_pToolTip;
		/** FApplication::addMessageFilter使用 */
		Core::FPtrList* m_pMsgFilterList;

		/** 皮肤资源包 */
		Markup::CSkinPackage* m_pResoucePackage;
	};

} // !namespace

#endif // !FMODULE_H