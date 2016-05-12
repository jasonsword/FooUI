/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FStringResource.h
* 描述：		FStringResource
* 创建日期：	2013-06-21
*
*/

#ifndef FSTRINGRESOURCE_H
#define FSTRINGRESOURCE_H

#include "core/FBase.h"
#include "core/FString.h"
#include "markup/FResourceDef.h"

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FFontResource
	class FOOUI_LIB FStringResource : public FResourceObject
	{
	public:
		FStringResource(void);
		FStringResource(const FStringResource&);
		FStringResource& operator=(const FStringResource&);
		virtual ~FStringResource(void);
		FOOUI_DECLARE_RESTYPE(FResourceObject::FRESTYPE_STRING)

	public:
		void setString(LPCTSTR pszString);
		const Core::FString& getString(void) const;

	public:
		bool parseAttribute(FMarkup* pMarkup, LPCTSTR pszValue);

	protected:
		bool setAttribute(FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue);

	private:
		class FStringResourcePrivate;
		FStringResourcePrivate* m_stringResPrivate;
	};

}} // !namespace

#endif // !FSTRINGRESOURCE_H