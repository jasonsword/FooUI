/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FImage.h
* 描述：		FImage
* 创建日期：	2013-06-06
*
*/

#ifndef FIMAGE_H
#define FIMAGE_H

#include "core/FBase.h"

namespace FooUI { namespace Painting {

	//////////////////////////////////////////////////////////////////////////
	// FImage
	class FOOUI_LIB FImage
	{
	public:
		FImage(void);
		FImage(const FImage&);
		FImage& operator=(const FImage&);
		~FImage(void);

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

		/** 销毁图像 */
		bool destroy(void);

	private:
		class FImagePrivate;
		friend FImagePrivate;
		FImagePrivate* m_imagePrivate;
	};

}} // !namespace

#endif // !FIMAGE_H