/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FFontResource.h
* 描述：		FFontResource
* 创建日期：	2013-06-21
*
*/

#ifndef FFONTRESOURCE_H
#define FFONTRESOURCE_H

#include "core/FBase.h"
#include "painting/FFont.h"
#include "markup/FResourceDef.h"

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FFontResource
	class FOOUI_LIB FFontResource : public FResourceObject
	{
	public:
		FFontResource(void);
		FFontResource(const FFontResource&);
		FFontResource& operator=(const FFontResource&);
		virtual ~FFontResource(void);
		FOOUI_DECLARE_RESTYPE(FResourceObject::FRESTYPE_FONT)

	public:
		const Painting::FFont& font(void) const;
		bool isNull(void) const;

	public:
		bool parseAttribute(FMarkup* pMarkup, LPCTSTR pszValue);

	protected:
		bool setAttribute(FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue);
		bool endMarkup(FMarkup* pMarkup);

	private:
		class FFontResourcePrivate;
		FFontResourcePrivate* m_fontResPrivate;
	};

}} // !namespace

#endif // !FFONTRESOURCE_H