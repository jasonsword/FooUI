/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FStringResource.h
* 描述：		FStringResource
* 创建日期：	2013-06-21
*
*/

#include "StdAfx.h"
#include "core/FObject.h"
#include "markup/FStringResource.h"
#include "markup/FMarkup.h"

#define FSTRINGRESOURCE_INLINE FOOINLINE
namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FStringResourcePrivate
	class FStringResource::FStringResourcePrivate : public Core::FReferenceObject, public Core::FNoncopyableObject
	{
	public:
		Core::FString m_string;
	};

	//////////////////////////////////////////////////////////////////////////
	// FStringResource
	FSTRINGRESOURCE_INLINE FStringResource::FStringResource(void)
	{
		m_stringResPrivate = new FStringResourcePrivate;
		m_stringResPrivate->addRef();
	}

	FSTRINGRESOURCE_INLINE FStringResource::FStringResource(const FStringResource& res)
	{
		FASSERT(this != &res);
		m_stringResPrivate = res.m_stringResPrivate;
		m_stringResPrivate->addRef();
	}

	FSTRINGRESOURCE_INLINE FStringResource& FStringResource::operator=(const FStringResource& res)
	{
		if (m_stringResPrivate != res.m_stringResPrivate)
		{
			m_stringResPrivate->release();
			m_stringResPrivate = res.m_stringResPrivate;
			m_stringResPrivate->addRef();
		}

		return (*this);
	}

	FSTRINGRESOURCE_INLINE FStringResource::~FStringResource(void)
	{
		FASSERT(NULL != m_stringResPrivate);
		m_stringResPrivate->release();
		m_stringResPrivate = NULL;
	}

	FSTRINGRESOURCE_INLINE void FStringResource::setString(LPCTSTR pszString)
	{
		FASSERT(NULL != m_stringResPrivate);
		FASSERT(NULL != pszString);
		m_stringResPrivate->m_string = pszString;
	}

	FSTRINGRESOURCE_INLINE const Core::FString& FStringResource::getString(void) const
	{
		FASSERT(NULL != m_stringResPrivate);
		return m_stringResPrivate->m_string;
	}

	FSTRINGRESOURCE_INLINE bool FStringResource::parseAttribute(FMarkup* pMarkup, LPCTSTR pszValue)
	{
		FASSERT(NULL != m_stringResPrivate);
		FASSERT(NULL != pMarkup);
		FASSERT(NULL != pszValue);

		if (pszValue[0] == _T('#'))
		{
			FStringResource *pStringRes = (FStringResource*)pMarkup->getResource(pszValue + 1, FResourceObject::FRESTYPE_STRING);
			if (NULL != pStringRes)
			{
				*this = *pStringRes;
				return true;
			}
		}
		if (!__super::parseAttribute(pMarkup, pszValue))
		{
			m_stringResPrivate->m_string = pszValue;
		}

		return true;
	}

	FSTRINGRESOURCE_INLINE bool FStringResource::setAttribute(FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		FASSERT(NULL != m_stringResPrivate);
		FASSERT(NULL != pMarkup);
		FASSERT(NULL != pszName);
		FASSERT(NULL != pszValue);

		if (_tcslen(pszName) == 0
			|| _tcscmp(pszName, _T("string")) == 0 
			|| _tcscmp(pszName, _T("text")) == 0)
		{
			m_stringResPrivate->m_string = pszValue;
		}

		return __super::setAttribute(pMarkup, pszName, pszValue);
	}

}} // !namespace