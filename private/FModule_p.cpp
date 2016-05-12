/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FModule_p.cpp
* ������		FModule_p��FooUIģ�飬
*				Ϊ�˱�֤����ȫ�ֱ���������˳���ʵ���Ӧ������ȫ�ֱ��������ģ��
* �������ڣ�	2013-08-04
*
*/

#include "StdAfx.h"
#include "core/FBase.h"
#include "FModule_p.h"

namespace FooUI
{
	//////////////////////////////////////////////////////////////////////////
	// FModule
	static FModule* fModule = NULL;
	FModule::FModule(void) : 
		m_pWindowCreateHelper(new Widgets::FWindowCreateHelper) 
		, m_tabWidgets(new Core::FPtrList)
		, m_pToolTip(NULL) 
		, m_pMsgFilterList(new Core::FPtrList)
	{
		FASSERT(NULL == fModule);
		fModule = this;

		m_pToolTip = new Widgets::FCommToolTip;
		m_pToolTip->autoDelete(true);
	}

	FModule::~FModule(void)
	{
		FASSERT(NULL != fModule);
		fModule = NULL;

		/** ɾ��MsgFilter */
		fooSafeDelete(m_pMsgFilterList);

		/** ɾ��ȫ�ֵ�ToolTip */
		if (m_pToolTip->isAutoDelete())
		{
			fooSafeDelete(m_pToolTip);
		}
		m_pToolTip = NULL;

		/** ɾ��Tab˳���� */
		fooSafeDelete(m_tabWidgets);

		/** ɾ�����ڴ���ģ�� */
		fooSafeDelete(m_pWindowCreateHelper);

		/** ɾ��Ƥ����Դ�� */
		fooSafeDelete(m_pResoucePackage);
	}

	/*static*/ Widgets::FWindowCreateHelper* FModule::window_create_helper(void)
	{
		//FASSERT(NULL != fModule);
		//FASSERT(NULL != fModule->m_pWindowCreateHelper);
		//return fModule->m_pWindowCreateHelper;

		if (NULL != fModule)
		{
			return fModule->m_pWindowCreateHelper;
		}

		return NULL;
	}

	/*static*/ Core::FPtrList* FModule::tabwidgets(void)
	{
		if (NULL == fModule 
			|| NULL == fModule->m_tabWidgets)
		{
			return NULL;
		}

		return fModule->m_tabWidgets;
	}

	/*static*/ Widgets::FToolTip* FModule::tooltip(void)
	{
		FASSERT(NULL != fModule);
		FASSERT(NULL != fModule->m_pToolTip);
		return fModule->m_pToolTip;
	}

	/*static*/ bool FModule::replaceToolTip(Widgets::FToolTip* tooltip)
	{
		FASSERT(NULL != tooltip);
		FASSERT(NULL != fModule);

		if (NULL != fModule->m_pToolTip 
			&& fModule->m_pToolTip->isAutoDelete())
		{
			fooSafeDelete(fModule->m_pToolTip);
			fModule->m_pToolTip = NULL;
		}

		fModule->m_pToolTip = tooltip;

		return true;
	}

	/*static*/ Core::FPtrList* FModule::msgfilterlist(void)
	{
		FASSERT(NULL != fModule);
		FASSERT(NULL != fModule->m_pToolTip);
		return fModule->m_pMsgFilterList;
	}

	/*static*/ Markup::CSkinPackage* FModule::setResourcePackage(Markup::CSkinPackage* pResoucePackage)
	{
		FASSERT(NULL != fModule);
		Markup::CSkinPackage* pOldResoucePackage = fModule->m_pResoucePackage;
		fModule->m_pResoucePackage = pResoucePackage;
		return pOldResoucePackage;
	}

	/*static*/ Markup::CSkinPackage* FModule::getSkinPackage(void)
	{
		FASSERT(NULL != fModule);
		return fModule->m_pResoucePackage;
	}

	/*static*/ void FModule::destroyResourcePackage(void)
	{
		FASSERT(NULL != fModule);
		if (NULL != fModule->m_pResoucePackage)
		{
			delete fModule->m_pResoucePackage;
			fModule->m_pResoucePackage = NULL;
		}
	}

} // !namespace