/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FResourceManagerPrivate_p.cpp
* 描述：		FResourceManagerPrivate_p
* 创建日期：	2013-06-20
*
*/

#include "StdAfx.h"

#include "markup/FResourceManagerPrivate_p.h"
#include "markup/FResourcePool_p.h"

#include "markup/FStringResource.h"
#include "markup/FFontResource.h"
#include "markup/FColorResource.h"
#include "markup/FImageResource.h"
#include "markup/FCursorResource.h"
#include "markup/FStyleSheetResource.h"

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FResourceManagerPrivate
	FResourceManager::FResourceManagerPrivate::FResourceManagerPrivate(void)
	{
		m_hashStrings.setRemoveCallBack(removeCallBack, (LPVOID)FResourceObject::FRESTYPE_STRING);
		m_hashFonts.setRemoveCallBack(removeCallBack, (LPVOID)FResourceObject::FRESTYPE_FONT);
		m_hashColors.setRemoveCallBack(removeCallBack, (LPVOID)FResourceObject::FRESTYPE_COLOR);
		m_hashImages.setRemoveCallBack(removeCallBack, (LPVOID)FResourceObject::FRESTYPE_IMAGE);
		m_hashCursors.setRemoveCallBack(removeCallBack, (LPVOID)FResourceObject::FRESTYPE_CURSOR);
		m_hashStyleSheets.setRemoveCallBack(removeCallBack, (LPVOID)FResourceObject::FRESTYPE_STYLESHEET);
	}

	FResourceManager::FResourceManagerPrivate::~FResourceManagerPrivate(void)
	{
		FResourcePool::remove(this);
	}

	bool FResourceManager::FResourceManagerPrivate::addResource(LPCTSTR pszName, DWORD dwType, FResourceObject* obj)
	{
		FASSERT(NULL != pszName);
		FASSERT(NULL != obj);

		if (obj->getType() != dwType)
		{
			FASSERT(false);
			return false;
		}

		/** 如果已经存在相同名字的资源终止插入 */
		if (NULL != getResource(pszName, dwType))
		{
			return false;
		}

		switch (dwType)
		{
		case FResourceObject::FRESTYPE_STRING:
			return m_hashStrings.insert(pszName, (LPVOID)obj);
			break;
		case FResourceObject::FRESTYPE_FONT:
			return m_hashFonts.insert(pszName, (LPVOID)obj);
			break;
		case FResourceObject::FRESTYPE_COLOR:
			return m_hashColors.insert(pszName, (LPVOID)obj);
			break;
		case FResourceObject::FRESTYPE_IMAGE:
			return m_hashImages.insert(pszName, (LPVOID)obj);
			break;
		case FResourceObject::FRESTYPE_CURSOR:
			return m_hashCursors.insert(pszName, (LPVOID)obj);
			break;
		case FResourceObject::FRESTYPE_STYLESHEET:
			return m_hashStyleSheets.insert(pszName, (LPVOID)obj);
			break;
		default:
			break;
		}

		FASSERT(false);
		return false;
	}

	bool FResourceManager::FResourceManagerPrivate::testResource(LPCTSTR pszName, DWORD dwType) const
	{
		switch (dwType)
		{
		case FResourceObject::FRESTYPE_STRING:
			return (m_hashStrings.find(pszName) != NULL);
			break;
		case FResourceObject::FRESTYPE_FONT:
			return (m_hashFonts.find(pszName) != NULL);
			break;
		case FResourceObject::FRESTYPE_COLOR:
			return (m_hashColors.find(pszName) != NULL);
			break;
		case FResourceObject::FRESTYPE_IMAGE:
			return (m_hashImages.find(pszName) != NULL);
			break;
		case FResourceObject::FRESTYPE_CURSOR:
			return (m_hashCursors.find(pszName) != NULL);
			break;
		case FResourceObject::FRESTYPE_STYLESHEET:
			return (m_hashStyleSheets.find(pszName) != NULL);
			break;
		default:
			break;
		}

		return FALSE;
	}

	FResourceObject* FResourceManager::FResourceManagerPrivate::getResource(LPCTSTR pszName, DWORD dwType) const
	{
		switch (dwType)
		{
		case FResourceObject::FRESTYPE_STRING:
			return (FResourceObject*)m_hashStrings.get(pszName);
			break;
		case FResourceObject::FRESTYPE_FONT:
			return (FResourceObject*)m_hashFonts.get(pszName);
			break;
		case FResourceObject::FRESTYPE_COLOR:
			return (FResourceObject*)m_hashColors.get(pszName);
			break;
		case FResourceObject::FRESTYPE_IMAGE:
			return (FResourceObject*)m_hashImages.get(pszName);
			break;
		case FResourceObject::FRESTYPE_CURSOR:
			return (FResourceObject*)m_hashCursors.get(pszName);
			break;
		case FResourceObject::FRESTYPE_STYLESHEET:
			return (FResourceObject*)m_hashStyleSheets.get(pszName);
			break;
		default:
			break;
		}

		FASSERT(false);
		return NULL;
	}

	/*static*/ void CALLBACK FResourceManager::FResourceManagerPrivate::removeCallBack(LPVOID pVal, LPVOID lpParameter)
	{
		switch ((DWORD)lpParameter)
		{
		case FResourceObject::FRESTYPE_STRING:
			fooSafeDeleteT(FStringResource*, pVal);
			break;
		case FResourceObject::FRESTYPE_FONT:
			fooSafeDeleteT(FFontResource*, pVal);
			break;
		case FResourceObject::FRESTYPE_COLOR:
			fooSafeDeleteT(FColorResource*, pVal);
			break;
		case FResourceObject::FRESTYPE_IMAGE:
			fooSafeDeleteT(FImageResource*, pVal);
			break;
		case FResourceObject::FRESTYPE_CURSOR:
			fooSafeDeleteT(FCursorResource*, pVal);
			break;
		case FResourceObject::FRESTYPE_STYLESHEET:
			fooSafeDeleteT(FStyleSheetResource*, pVal);
			break;
		default:
			FASSERT(false);
			break;
		}
	}

}} // !namespace