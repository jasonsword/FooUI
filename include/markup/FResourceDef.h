/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FResourceDef.h
* 描述：		FResourceDef
* 创建日期：	2013-06-21
*
*/

#ifndef FRESOURCEDEF_H
#define FRESOURCEDEF_H

#include "core/FBase.h"
#include "core/FString.h"

#include "markup/FMarkupDef.h"

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FResourceObject
	class FOOUI_LIB FResourceObject : public FMarkupObject
	{
	public:
		enum
		{
			FRESTYPE_UNKNOWN = 0, 
			FRESTYPE_STRING, 
			FRESTYPE_FONT, 
			FRESTYPE_COLOR, 
			FRESTYPE_IMAGE, 
			FRESTYPE_CURSOR, 
			FRESTYPE_STYLESHEET, 
		};
		virtual DWORD getType(void) const = 0;

	public:
		void setName(LPCTSTR pszName) { m_strName = pszName; }
		LPCTSTR getName(void) const { return m_strName; }
		virtual bool beginMarkup(FMarkup* pMarkup) { return true; }
		virtual bool setAttribute(FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
		{ if (_tcscmp(pszName, _T("name")) == 0) setName(pszValue); return true; }
		virtual bool endMarkup(FMarkup* pMarkup) { return true; }

	private:
		Core::FString m_strName;
	};

	#define FOOUI_DECLARE_RESTYPE(_type) \
			DWORD getType(void) const \
			{ \
				return (_type); \
			}

}} // !namespace

#endif // !FRESOURCEDEF_H