/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FResourceManagerPrivate_p.h
* 描述：		FResourceManagerPrivate_p
* 创建日期：	2013-06-20
*
*/

#ifndef FResourceManagerPrivate_P_H
#define FResourceManagerPrivate_P_H

#include "core/FBase.h"
#include "core/FObject.h"
#include "core/FPtrHash.h"
#include "markup/FResourceManager.h"

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FResourceManagerPrivate
	class FResourceManager::FResourceManagerPrivate : public Core::FReferenceObject, public Core::FNoncopyableObject
	{
	public:
		FResourceManagerPrivate(void);
		~FResourceManagerPrivate(void);

	public:
		bool addResource(LPCTSTR pszName, DWORD dwType, FResourceObject* obj);
		bool testResource(LPCTSTR pszName, DWORD dwType) const;
		FResourceObject* getResource(LPCTSTR pszName, DWORD dwType) const;

	private:
		static void CALLBACK removeCallBack(LPVOID pVal, LPVOID lpParameter);

	private:
		Core::FPtrHash m_hashStrings;
		Core::FPtrHash m_hashFonts;
		Core::FPtrHash m_hashColors;
		Core::FPtrHash m_hashImages;
		Core::FPtrHash m_hashCursors;
		Core::FPtrHash m_hashStyleSheets;
	};

}} // !namespace

#endif // !FResourceManagerPrivate_P_H