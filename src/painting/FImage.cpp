/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FImage.cpp
* 描述：		FImage
* 创建日期：	2013-06-06
*
*/

#include "StdAfx.h"
#include "painting/FImage.h"

#include "F3rd_p.h"

#define FIMAGE_INLINE FOOINLINE

namespace FooUI { namespace Painting {

	//////////////////////////////////////////////////////////////////////////
	// FImage
	FIMAGE_INLINE FImage::FImage(void)
	{
		m_imagePrivate = new FImagePrivate;
		m_imagePrivate->addRef();
	}

	FIMAGE_INLINE FImage::FImage(const FImage& image)
	{
		m_imagePrivate = image.m_imagePrivate;
		m_imagePrivate->addRef();
	}

	FIMAGE_INLINE FImage& FImage::operator=(const FImage& image)
	{
		if (m_imagePrivate != image.m_imagePrivate)
		{
			m_imagePrivate->release();
			m_imagePrivate = image.m_imagePrivate;
			m_imagePrivate->addRef();
		}

		return (*this);
	}

	FIMAGE_INLINE FImage::~FImage(void)
	{
		FASSERT(NULL != m_imagePrivate);
		m_imagePrivate->release();
		m_imagePrivate = NULL;
	}

	FIMAGE_INLINE bool FImage::load(LPCTSTR pszFileName)
	{
		FASSERT(NULL != m_imagePrivate);
		return m_imagePrivate->load(pszFileName);
	}

	FIMAGE_INLINE bool FImage::load(void* pStream, DWORD dwSize)
	{
		FASSERT(NULL != m_imagePrivate);
		return m_imagePrivate->load(pStream, dwSize);
	}

	FIMAGE_INLINE LPVOID FImage::getImage(void) const
	{
		FASSERT(NULL != m_imagePrivate);
		return m_imagePrivate->getImage();
	}

	FIMAGE_INLINE int FImage::width(void) const
	{
		FASSERT(NULL != m_imagePrivate);
		return m_imagePrivate->width();
	}

	FIMAGE_INLINE int FImage::height(void) const
	{
		FASSERT(NULL != m_imagePrivate);
		return m_imagePrivate->height();
	}

	FIMAGE_INLINE bool FImage::isNull(void) const
	{
		FASSERT(NULL != m_imagePrivate);
		return m_imagePrivate->isNull();
	}

	FIMAGE_INLINE bool FImage::destroy(void)
	{
		FASSERT(NULL != m_imagePrivate);
		return m_imagePrivate->destroy();
	}

}} // !namespace