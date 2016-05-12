/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FImage_Skia_p.h
* ������		FImage_Skia_p
* �������ڣ�	2013-06-06
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
		/** ����ͼ�� */
		bool load(LPCTSTR pszFileName);
		bool load(void* pStream, DWORD dwSize);

		/** ����ͼ�� */
		LPVOID getImage(void) const;

		/** ����ͼ���С */
		int width(void) const;
		int height(void) const;

		/** �������true�� û������ͼ�� */
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