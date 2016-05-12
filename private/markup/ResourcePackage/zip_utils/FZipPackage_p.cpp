/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FZipPackage_p.cpp
* 描述：		FZipPackage_p， ZIP资源包
* 创建日期：	2013-08-19
*
*/

#include "StdAfx.h"
#include "markup/ResourcePackage/zip_utils/FZipPackage_p.h"

#pragma warning(push)
#pragma warning(disable:4996)

#pragma warning(pop)

///////////////////////////////////////////////////////////////////////////////
DECLARE_HANDLE(HZIP);	// An HZIP identifies a zip file that has been opened
typedef DWORD ZRESULT;
typedef struct
{ 
	int index;                 // index of this file within the zip
	char name[MAX_PATH];       // filename within the zip
	DWORD attr;                // attributes, as in GetFileAttributes.
	FILETIME atime,ctime,mtime;// access, create, modify filetimes
	long comp_size;            // sizes of item, compressed and uncompressed. These
	long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRY;
typedef struct
{ 
	int index;                 // index of this file within the zip
	TCHAR name[MAX_PATH];      // filename within the zip
	DWORD attr;                // attributes, as in GetFileAttributes.
	FILETIME atime,ctime,mtime;// access, create, modify filetimes
	long comp_size;            // sizes of item, compressed and uncompressed. These
	long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRYW;
#define OpenZip OpenZipU
#define CloseZip(hz) CloseZipU(hz)
extern HZIP OpenZipU(void *z,unsigned int len,DWORD flags);
extern ZRESULT CloseZipU(HZIP hz);
#ifdef _UNICODE
#define ZIPENTRY ZIPENTRYW
#define GetZipItem GetZipItemW
#define FindZipItem FindZipItemW
#else
#define GetZipItem GetZipItemA
#define FindZipItem FindZipItemA
#endif
extern ZRESULT GetZipItemA(HZIP hz, int index, ZIPENTRY *ze);
extern ZRESULT GetZipItemW(HZIP hz, int index, ZIPENTRYW *ze);
extern ZRESULT FindZipItemA(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRY *ze);
extern ZRESULT FindZipItemW(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRYW *ze);
extern ZRESULT UnzipItem(HZIP hz, int index, void *dst, unsigned int len, DWORD flags);



HZIP m_hZip;
namespace FooUI { namespace Markup {
	
	//////////////////////////////////////////////////////////////////////////
	// FZipPackage
	FZipPackage::FZipPackage(void) 
	{
	}

	FZipPackage::~FZipPackage(void)
	{
		if (isOpened())
		{
			close();
		}
	}

	bool FZipPackage::open(LPCTSTR pszFileName)
	{
		FASSERT(NULL != pszFileName);
		FASSERT(NULL == m_hZip);
		int len = 0;
		if (NULL != pszFileName  )
		{ 

			m_hZip = ::OpenZip( _tcsdup(pszFileName),0 , 2); 
		} 
		return (NULL != m_hZip);
	}

	bool FZipPackage::open(void* pStream, int nStreamLength, LPCTSTR pszPassword, int nPasswordLength)
	{
		FASSERT(NULL != pStream && nStreamLength > 0);
		FASSERT(NULL == m_hZip);

		if (NULL != pszPassword && _tcslen(pszPassword) > 0)
		{
			m_hZip = ::OpenZip(pStream, nStreamLength, NULL);
		}
		else
		{
			m_hZip = ::OpenZip(pStream, nStreamLength, 3);
		}

		return (NULL != m_hZip);
		return true;
	}

	bool FZipPackage::isOpened(void) const
	{
		return (NULL != m_hZip);
	}

	bool FZipPackage::findFile(LPCTSTR pszFileName) const
	{
		ZIPENTRY ze;
		int i;
		if (0 == ::FindZipItem(m_hZip, pszFileName, true, &i, &ze))
		{
			return true;
		}

		return false;
	}

	/** 从资源包中获取文件
		返回值：大于0，所需缓冲区大小
				小于0，没有找到这个文件或者发生错误
	*/
	int FZipPackage::unpackFile(LPCTSTR pszFileName, void* buffer, int bufferLength) const
	{
		FASSERT(NULL != m_hZip);

		ZIPENTRY ze;
		int i;
		if (0 != ::FindZipItem(m_hZip, pszFileName, true, &i, &ze))
		{
			return -1;
		}

		if (NULL == buffer || bufferLength <= 0)
		{
			return ze.unc_size;
		}
		else
		{
			/*ZRESULT zr = */::UnzipItem(m_hZip, i, buffer, bufferLength, 3);
			return ze.unc_size;
		}

		return -1;
	}

	bool FZipPackage::close(void)
	{
		FASSERT(NULL != m_hZip);

		if (m_hZip)
		{
			::CloseZip(m_hZip);
			m_hZip = NULL;

			return true;
		}

		return false;
	}

}} // !namespace