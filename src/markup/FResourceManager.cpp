/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FResourceManager.cpp
* 描述：		FResourceManager
* 创建日期：	2013-06-20
*
*/

#include "StdAfx.h"
#include "markup/FResourceManager.h"
#include "markup/FResourceManagerPrivate_p.h"
#include "markup/FResourcePool_p.h"

#define FRESOURCEMANAGER_INLINE FOOINLINE

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FResourceManager
	FRESOURCEMANAGER_INLINE FResourceManager::FResourceManager(void)
	{
		m_resourceManagerPrivate = new FResourceManagerPrivate;
		m_resourceManagerPrivate->addRef();
	}

	FRESOURCEMANAGER_INLINE FResourceManager::FResourceManager(const FResourceManager& resmgr)
	{
		FASSERT(this != &resmgr);
		m_resourceManagerPrivate = resmgr.m_resourceManagerPrivate;
		m_resourceManagerPrivate->addRef();
	}

	FRESOURCEMANAGER_INLINE FResourceManager& FResourceManager::operator=(const FResourceManager& resmgr)
	{
		if (m_resourceManagerPrivate != resmgr.m_resourceManagerPrivate)
		{
			m_resourceManagerPrivate->release();
			m_resourceManagerPrivate = resmgr.m_resourceManagerPrivate;
			m_resourceManagerPrivate->addRef();
		}

		return (*this);
	}

	FRESOURCEMANAGER_INLINE FResourceManager::FResourceManager(LPVOID p)
	{
		FASSERT(NULL != p);
		m_resourceManagerPrivate = (FResourceManagerPrivate*)p;
		m_resourceManagerPrivate->addRef();
	}

	FRESOURCEMANAGER_INLINE FResourceManager::~FResourceManager(void)
	{
		FASSERT(NULL != m_resourceManagerPrivate);
		m_resourceManagerPrivate->release();
		m_resourceManagerPrivate = NULL;

		removeAllIncludes();
	}

	FRESOURCEMANAGER_INLINE void FResourceManager::include(FResourceManager& resourceMgr)
	{
		if (m_resourceManagerPrivate == resourceMgr.m_resourceManagerPrivate)
		{
			return;
		}

		resourceMgr.m_resourceManagerPrivate->addRef();
		m_includes.add((LPVOID)resourceMgr.m_resourceManagerPrivate);
	}

	FRESOURCEMANAGER_INLINE void FResourceManager::removeInclude(FResourceManager& resourceMgr)
	{
		Core::FPtrList::POSITION position = m_includes.find((LPVOID)resourceMgr.m_resourceManagerPrivate);
		if (NULL != position)
		{
			resourceMgr.m_resourceManagerPrivate->release();
			m_includes.removeAtPosition(position);
		}
	}

	FRESOURCEMANAGER_INLINE void FResourceManager::removeAllIncludes(void)
	{
		for (int i = 0; i < m_includes.size(); ++i)
		{
			FResourceManagerPrivate *pData = (FResourceManagerPrivate*)m_includes.get(i);
			pData->release();
		}
		m_includes.clear();
	}

	FRESOURCEMANAGER_INLINE bool FResourceManager::addResource(LPCTSTR pszName, DWORD dwType, FResourceObject* obj)
	{
		FASSERT(NULL != m_resourceManagerPrivate);
		FASSERT(NULL != pszName);
		FASSERT(NULL != obj);
		return m_resourceManagerPrivate->addResource(pszName, dwType, obj);
	}

	bool FResourceManager::testResource(LPCTSTR pszName, DWORD dwType) const
	{
		FASSERT(NULL != m_resourceManagerPrivate);
		FASSERT(NULL != pszName);
		return m_resourceManagerPrivate->testResource(pszName, dwType);
	}

	FRESOURCEMANAGER_INLINE FResourceObject* FResourceManager::getResource(LPCTSTR pszName, DWORD dwType) const
	{
		FASSERT(NULL != m_resourceManagerPrivate);
		FASSERT(NULL != pszName);

		FResourceObject *pObj = m_resourceManagerPrivate->getResource(pszName, dwType);
		if (NULL != pObj) return pObj;

		for (int i = 0; i < m_includes.size(); ++i)
		{
			FResourceManagerPrivate *pData = (FResourceManagerPrivate*)m_includes.get(i);
			pObj = pData->getResource(pszName, dwType);
			if (NULL != pObj) return pObj;
		}

		return NULL;
	}

	FRESOURCEMANAGER_INLINE /*static*/ FResourceManager FResourceManager::pump(LPCTSTR pszName)
	{
		return FResourcePool::pump(pszName);
	}

}} // !namespace