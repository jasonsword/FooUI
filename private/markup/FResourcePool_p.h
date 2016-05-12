/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FResourcePool_p.h
* 描述：		FResourcePool_p
* 创建日期：	2013-06-20
*
*/

#ifndef FRESOURCEPOOL_H
#define FRESOURCEPOOL_H

#include "core/FBase.h"
#include "core/FString.h"
#include "core/FPtrHash.h"
#include "markup/FResourceManager.h"
#include "markup/FResourceManagerPrivate_p.h"

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FResourcePool
	class FResourceManager::FResourcePool
	{
	public:
		FResourcePool(void);

	public:
		static FResourceManager pump(LPCTSTR pszName);
		static bool remove(FResourceManagerPrivate* p);

	private:
		Core::FPtrHash m_hash;
		static FResourcePool pool;
	};

}} // !namespace

#endif // !FRESOURCEPOOL_H