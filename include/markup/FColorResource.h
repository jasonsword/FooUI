/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FColorResource.h
* 描述：		FColorResource
* 创建日期：	2013-06-21
*
*/

#ifndef FCOLORRESOURCE_H
#define FCOLORRESOURCE_H

#include "core/FBase.h"
#include "painting/FColor.h"
#include "markup/FResourceDef.h"

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FColorResource
	class FOOUI_LIB FColorResource : public FResourceObject
	{
	public:
		FColorResource(void);
		FColorResource(const FColorResource&);
		FColorResource& operator=(const FColorResource&);
		virtual ~FColorResource(void);
		FOOUI_DECLARE_RESTYPE(FResourceObject::FRESTYPE_COLOR)

	public:
		Painting::FColor& getColor(void);
		const Painting::FColor& getColor(void) const;
		/** "r, g, b" */
		bool parseRGB(LPCTSTR pszValue);
		/** "a, r, g, b" */
		bool parseARGB(LPCTSTR pszValue);
		/** "#AARRGGBB" */
		bool parseColor(LPCTSTR pszValue);

	public:
		bool parseAttribute(FMarkup* pMarkup, LPCTSTR pszValue);

	protected:
		bool setAttribute(FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue);

	private:
		class FColorResourcePrivate;
		FColorResourcePrivate* m_colorResPrivate;
	};

}} // !namespace

#endif // !FCOLORRESOURCE_H