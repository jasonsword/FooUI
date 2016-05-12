/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FImage.h
* ������		FImage
* �������ڣ�	2013-06-06
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

		/** ����ͼ�� */
		bool destroy(void);

	private:
		class FImagePrivate;
		friend FImagePrivate;
		FImagePrivate* m_imagePrivate;
	};

}} // !namespace

#endif // !FIMAGE_H