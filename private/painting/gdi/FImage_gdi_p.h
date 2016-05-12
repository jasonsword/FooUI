/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FImage_gdi_p.h
* ������		FImage_gdi_p
* �������ڣ�	2013-06-20
*
*/

#ifndef FIMAGE_GDI_P_H
#define FIMAGE_GDI_P_H

#include "F3rd_p.h"
#ifdef FOOUI_SUPPORT_GDI

#include "core/FObject.h"
#include "core/FTypes.h"
#include "core/FString.h"
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
		bool isAlphaChannel(void) const { return m_bAlphaChannel; }

	private:
		HDC m_hDC;
		HBITMAP m_hBitmap;
		int m_nSaveDC;

		bool m_bAlphaChannel;
		Core::FSize m_size;
	};

}} // !namespace

#endif // !FOOUI_SUPPORT_GDI
#endif // !FIMAGE_GDI_P_H