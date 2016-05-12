/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FImage_Skia_p.cpp
* 描述：		FImage_Skia_p
* 创建日期：	2013-06-13
*
*/

#include "StdAfx.h"
#include "painting/skia/FImage_Skia_p.h"

#ifdef FOOUI_SUPPORT_SKIA
namespace FooUI { namespace Painting {

	//////////////////////////////////////////////////////////////////////////
	// FImagePrivate
	FImage::FImagePrivate::FImagePrivate(void) : m_pBitmap(NULL)
	{
		create();
	}

	FImage::FImagePrivate::~FImagePrivate(void)
	{
		destroy();
	}

	bool FImage::FImagePrivate::load(LPCTSTR pszFileName)
	{
		if (!isNull() || NULL == pszFileName)
		{
			FASSERT(false);
			return false;
		}

		HANDLE hFile = ::CreateFile(pszFileName, GENERIC_READ, 
			FILE_SHARE_READ, NULL, OPEN_EXISTING, 
			FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			FASSERT(false);
			return false;
		}

		DWORD dwSize = ::GetFileSize(hFile, NULL);
		char *pBuffer = new char[dwSize];
		DWORD dwReadBytes = 0;
		::ReadFile(hFile, pBuffer, dwSize, &dwReadBytes, NULL);
		FASSERT(dwReadBytes == dwSize);

		bool bRet = load(pBuffer, dwSize);

		fooSafeDeleteArray(pBuffer);
		fooSafeCloseHandle(hFile);

		return bRet;
	}

	bool FImage::FImagePrivate::load(void* pStream, DWORD dwSize)
	{
		if (!isNull() || NULL == pStream || 0 >= dwSize)
		{
			FASSERT(false);
			return false;
		}

		SkMemoryStream stream(pStream, (size_t)dwSize);
		SkImageDecoder *pDecoder = SkImageDecoder::Factory(&stream);
		if (NULL == pDecoder)
		{
			FASSERT(false);
			return false;
		}

		if (NULL == m_pBitmap) m_pBitmap = new SkBitmap;
		bool bRet = pDecoder->decode(&stream, m_pBitmap, 
									SkBitmap::kRGB_565_Config, 
									SkImageDecoder::kDecodePixels_Mode);
		sk_free(pDecoder);
		pDecoder = NULL;

		return (!isNull());
	}

	LPVOID FImage::FImagePrivate::getImage(void) const
	{
		FASSERT(NULL != m_pBitmap);
		return m_pBitmap;
	}

	int FImage::FImagePrivate::width(void) const
	{
		FASSERT(NULL != m_pBitmap);

		if (isNull())
		{
			FASSERT(false);
			return 0;
		}

		return m_pBitmap->width();
	}

	int FImage::FImagePrivate::height(void) const
	{
		FASSERT(NULL != m_pBitmap);

		if (isNull())
		{
			FASSERT(false);
			return 0;
		}

		return m_pBitmap->height();
	}

	bool FImage::FImagePrivate::isNull(void) const
	{
		FASSERT(NULL != m_pBitmap);
		return m_pBitmap->isNull();
	}

	bool FImage::FImagePrivate::create(void)
	{
		if (NULL != m_pBitmap)
		{
			FASSERT(false);
			return false;
		}

		m_pBitmap = new SkBitmap;

		return (NULL != m_pBitmap);
	}

	bool FImage::FImagePrivate::destroy(void)
	{
		FASSERT(NULL != m_pBitmap);

		fooSafeDelete(m_pBitmap);

		return (NULL == m_pBitmap);
	}

}} // !namespace
#endif // !FOOUI_SUPPORT_SKIA