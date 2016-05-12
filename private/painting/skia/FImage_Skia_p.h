/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FImage_Skia_p.h
* 描述：		FImage_Skia_p
* 创建日期：	2013-06-06
*
*/

#ifndef FIMAGE_SKIA_P_H
#define FIMAGE_SKIA_P_H

#include "F3rd_p.h"
#ifdef FOOUI_SUPPORT_SKIA

#include "core/FObject.h"
#include "painting/FImage.h"
#include "painting/IPatting_p.h"

namespace FooUI { namespace Painting {

	class FImage::FImagePrivate : public IImage
	{
	public:
		FImagePrivate(void);
		~FImagePrivate(void);

	public:
		/** 载入图像 */
		bool load(LPCTSTR pszFileName);
		bool load(void* pStream, DWORD dwSize);

		/** 返回图像 */
		LPVOID getImage(void) const;

		/** 返回图像大小 */
		int width(void) const;
		int height(void) const;

		/** 如果返回true， 没有载入图像 */
		bool isNull(void) const;

	public:
		bool create(void);
		bool destroy(void);

	public:
		SkBitmap* m_pBitmap;
	};

}} // !namespace

#endif // !FOOUI_SUPPORT_SKIA
#endif // !FIMAGE_SKIA_P_H