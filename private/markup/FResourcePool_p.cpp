/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FResourcePool_p.cpp
* 描述：		FResourcePool_p
* 创建日期：	2013-06-20
*
*/

#include "StdAfx.h"
#include "markup/FResourcePool_p.h"

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FResourcePool
	FResourceManager::FResourcePool::FResourcePool(void)
	{
		FASSERT(this == &pool);
	}

	/*static*/ FResourceManager FResourceManager::FResourcePool::pump(LPCTSTR pszName)
	{
		if (NULL != pszName)
		{
			Core::FString strName = pszName;
			strName.makeLower();
			
			Core::FPtrHash::POSITION position = pool.m_hash.find(strName);
			if (NULL != position)
			{
				return FResourceManager((LPVOID)pool.m_hash.getAtPosition(position));
			}
			else
			{
				FResourceManager resourceMgr;
				pool.m_hash.insert(strName, (LPVOID)resourceMgr.m_resourceManagerPrivate);
				return resourceMgr;
			}
		}

		return FResourceManager();
	}

	/*static*/ bool FResourceManager::FResourcePool::remove(FResourceManagerPrivate* p)
	{
		Core::FPtrHash::POSITION position = pool.m_hash.findData(p);
		if (NULL != position)
		{
			return pool.m_hash.removeAtPosition(position);
		}

		return false;
	}

	FResourceManager::FResourcePool FResourceManager::FResourcePool::pool;

}} // !namespace