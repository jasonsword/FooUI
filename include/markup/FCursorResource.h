/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FCursorResource.h
* 描述：		FCursorResource
* 创建日期：	2013-06-21
*
*/

#ifndef FCURSORRESOURCE_H
#define FCURSORRESOURCE_H

#include "core/FBase.h"
#include "markup/FResourceDef.h"

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FCursorResource
	class FOOUI_LIB FCursorResource : public FResourceObject
	{
	public:
		FCursorResource(void);
		FCursorResource(const FCursorResource&);
		FCursorResource& operator=(const FCursorResource&);
		virtual ~FCursorResource(void);
		FOOUI_DECLARE_RESTYPE(FResourceObject::FRESTYPE_CURSOR)

	public:
		HCURSOR getCursor(void) const;
		bool showCursor(void) const;
		bool isNull(void) const;

	public:
		bool parseAttribute(FMarkup* pMarkup, LPCTSTR pszValue);

	protected:
		bool setAttribute(FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue);

	private:
		bool parseSource(LPCTSTR pszValue);

	private:
		class FCursorResourcePrivate;
		FCursorResourcePrivate* m_cursorResPrivate;
	};

}} // !namespace

#endif // !FCURSORRESOURCE_H