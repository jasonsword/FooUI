/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FImage_gdi_p.cpp
* 描述：		FImage_gdi_p
* 创建日期：	2013-06-20
*
*/

#include "StdAfx.h"
#include "painting/gdi/FImage_gdi_p.h"

extern "C"
{
#include "painting/gdi/stb_image.c"
};

#ifdef FOOUI_SUPPORT_GDI
namespace FooUI { namespace Painting {

	//////////////////////////////////////////////////////////////////////////
	// FImagePrivate
	FImage::FImagePrivate::FImagePrivate(void) : 
		m_hDC(NULL) 
		, m_hBitmap(NULL) 
		, m_nSaveDC(0) 
		, m_bAlphaChannel(false) 
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
		if (!isNull() || NULL == pStream)
		{
			FASSERT(false);
			return false;
		}

		if (NULL == m_hDC)
		{
			m_hDC = ::CreateCompatibleDC(NULL);
			m_nSaveDC = ::SaveDC(m_hDC);
		}

		LPBYTE pImage = stbi_load_from_memory((stbi_uc const *)pStream, dwSize, (int*)&m_size.cx, (int*)&m_size.cy, NULL, 4);
		if (NULL == pImage)
		{
			FASSERT(false);
			return false;
		}

		BITMAPINFO bmi;
		::ZeroMemory(&bmi, sizeof(BITMAPINFO));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = m_size.cx;
		bmi.bmiHeader.biHeight = -m_size.cy;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = 0;

		LPBYTE pPixels = NULL;
		m_hBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pPixels, NULL, 0);
		::SelectObject(m_hDC, m_hBitmap);

		/*::SetDIBitsToDevice(m_hDC, 
		0, 0, m_size.cx, m_size.cy, 
		0, 0, 0, m_size.cy, 
		pImage, 
		&bmi, DIB_RGB_COLORS);*/

		m_bAlphaChannel = false;
		for( int i = 0; i < m_size.cx * m_size.cy; ++i) 
		{
			pPixels[i*4 + 3] = pImage[i*4 + 3];
			if( pPixels[i*4 + 3] < 255 )
			{
				pPixels[i * 4] = (BYTE)(DWORD(pImage[i * 4 + 2]) * pImage[i * 4 + 3] / 255);
				pPixels[i * 4 + 1] = (BYTE)(DWORD(pImage[i * 4 + 1]) * pImage[i * 4 + 3] / 255);
				pPixels[i * 4 + 2] = (BYTE)(DWORD(pImage[i * 4]) * pImage[i * 4 + 3] / 255); 
				m_bAlphaChannel = true;
			}
			else
			{
				pPixels[i * 4] = pImage[i * 4 + 2];
				pPixels[i * 4 + 1] = pImage[i * 4 + 1];
				pPixels[i * 4 + 2] = pImage[i * 4]; 
			}

			//if( *(DWORD*)(&pPixels[i*4]) == mask ) {
			//	pPixels[i * 4] = (BYTE)0;
			//	pPixels[i * 4 + 1] = (BYTE)0;
			//	pPixels[i * 4 + 2] = (BYTE)0; 
			//	pPixels[i * 4 + 3] = (BYTE)0;
			//	//bAlphaChannel = true;
			//}
		}

		stbi_image_free(pImage);
		pImage = NULL;

		return (NULL != m_hBitmap);
	}

	LPVOID FImage::FImagePrivate::getImage(void) const
	{
		return m_hDC;
	}

	int FImage::FImagePrivate::width(void) const
	{
		return m_size.cx;
	}

	int FImage::FImagePrivate::height(void) const
	{
		return m_size.cy;
	}

	bool FImage::FImagePrivate::isNull(void) const
	{
		return (NULL == m_hBitmap);
	}

	bool FImage::FImagePrivate::create(void)
	{
		FASSERT(NULL == m_hDC);

		m_hDC = ::CreateCompatibleDC(NULL);
		m_nSaveDC = ::SaveDC(m_hDC);

		return (NULL != m_hDC);
	}

	bool FImage::FImagePrivate::destroy(void)
	{
		if (NULL != m_hDC)
		{
			::RestoreDC(m_hDC, m_nSaveDC);
			fooSafeDeleteDC(m_hDC);
		}

		if (NULL != m_hBitmap)
		{
			fooSafeDeleteObject(m_hBitmap);
		}

		return true;
	}

}} // !namespace
#endif // !FOOUI_SUPPORT_GDI