/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FResourceManager.h
* ������		FResourceManager
* �������ڣ�	2013-06-20
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
		/** ��������һ����Դ������
			(����Դ���������Ҳ���������Դ��ʱ�����Ұ����� 
		*/
		void include(FResourceManager& resourceMgr);
		/** ���һ����Դ�������İ��� */
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