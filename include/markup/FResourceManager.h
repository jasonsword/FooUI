/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FResourceManager.h
* 描述：		FResourceManager
* 创建日期：	2013-06-20
*
*/

#ifndef FRESOURCEMANAGER_H
#define FRESOURCEMANAGER_H

#include "core/FBase.h"
#include "core/FPtrList.h"
#include "markup/FResourceDef.h"

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FResourceManager
	class FOOUI_LIB FResourceManager
	{
	public:
		FResourceManager(void);
		FResourceManager(const FResourceManager& resmgr);
		FResourceManager& operator=(const FResourceManager& resmgr);
		virtual ~FResourceManager(void);

	private:
		FResourceManager(LPVOID p);

	public:
		/** 包含另外一个资源管理器
			(在资源管理器中找不到所需资源的时候会查找包含） 
		*/
		void include(FResourceManager& resourceMgr);
		/** 解除一个资源管理器的包含 */
		void removeInclude(FResourceManager& resourceMgr);
		void removeAllIncludes(void);

	public:
		bool addResource(LPCTSTR pszName, DWORD dwType, FResourceObject* obj);
		bool testResource(LPCTSTR pszName, DWORD dwType) const;
		FResourceObject* getResource(LPCTSTR pszName, DWORD dwType) const;

	public:
		static FResourceManager pump(LPCTSTR pszName);

	private:
		class FResourcePool;
		class FResourceManagerPrivate;
		FResourceManagerPrivate* m_resourceManagerPrivate;
		Core::FPtrList m_includes;
	};

}} // !namespace

#endif // !FRESOURCEMANAGER_H