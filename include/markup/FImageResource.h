/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FImageResource.h
* 描述：		FImageResource
* 创建日期：	2013-06-21
*
*/

#ifndef FIMAGERESOURCE_H
#define FIMAGERESOURCE_H

#include "core/FBase.h"
#include "core/FTypes.h"
#include "painting/FImage.h"
#include "markup/FResourceDef.h"

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FImageResource
	class FOOUI_LIB FImageResource : public FResourceObject
	{
	public:
		FImageResource(void);
		FImageResource(const FImageResource&);
		FImageResource& operator=(const FImageResource&);
		virtual ~FImageResource(void);
		FOOUI_DECLARE_RESTYPE(FResourceObject::FRESTYPE_IMAGE)

	public:
		const Painting::FImage& getImage(void) const;
		const Core::FRect& getSource(void) const;
		const Core::FEdge& getNPatch(void) const;
		bool isNull(void) const;

	public:
		bool parseAttribute(FMarkup* pMarkup, LPCTSTR pszValue);

	protected:
		bool setAttribute(FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue);

	private:
		bool parseSource(LPCTSTR pszValue);

	private:
		class FImageResourceCacheMgr; 
		class FImageResourceCache; 
		class FImageResourcePrivate;
		FImageResourcePrivate* m_imageResPrivate;
	};

}} // !namespace

#endif // !FIMAGERESOURCE_H