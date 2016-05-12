/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FImageResource.h
* 描述：		FImageResource
* 创建日期：	2013-06-21
*
*/

#include "StdAfx.h"
#include "core/FObject.h"
#include "core/FPtrHash.h"
#include "core/FString.h"
#include "core/FApplication.h"
#include "markup/FMarkup.h"
#include "markup/FImageResource.h"

#define FIMAGERESOURCE_INLINE FOOINLINE

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FImageResourceCacheMgr
	class FImageResource::FImageResourceCacheMgr
	{
	public:
		FImageResourceCacheMgr(void);

	public:
		static bool push(LPCTSTR pszKey, const FImageResourceCache* pCache);
		static FImageResourceCache* get(LPCTSTR pszKey);
		static bool pop(const FImageResourceCache* pCache);

	private:
		static Core::FPtrHash hashCache;
		static FImageResourceCacheMgr cacheMgr;
	};

	/*static*/ Core::FPtrHash FImageResource::FImageResourceCacheMgr::hashCache;
	/*static*/ FImageResource::FImageResourceCacheMgr FImageResource::FImageResourceCacheMgr::cacheMgr;

	FIMAGERESOURCE_INLINE FImageResource::FImageResourceCacheMgr::FImageResourceCacheMgr(void)
	{
		FASSERT(this == &cacheMgr);
	}

	FIMAGERESOURCE_INLINE /*static*/ bool FImageResource::FImageResourceCacheMgr::push(LPCTSTR pszKey, const FImageResourceCache* pCache)
	{
		FASSERT(NULL != pszKey);
		FASSERT(NULL != pCache);
		FASSERT(NULL == hashCache.get(pszKey));

		return hashCache.insert(pszKey, (LPVOID)pCache);
	}

	FIMAGERESOURCE_INLINE /*static*/ FImageResource::FImageResourceCache* FImageResource::FImageResourceCacheMgr::get(LPCTSTR pszKey)
	{
		FASSERT(NULL != pszKey);

		return (FImageResource::FImageResourceCache*)hashCache.get(pszKey);
	}

	FIMAGERESOURCE_INLINE /*static*/ bool FImageResource::FImageResourceCacheMgr::pop(const FImageResource::FImageResourceCache* pCache)
	{
		Core::FPtrHash::POSITION position = hashCache.findData((LPVOID)pCache);
		if (NULL != position)
		{
			return hashCache.removeAtPosition(position);
		}

		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// FImageResourceCache
	class FImageResource::FImageResourceCache : public Core::FReferenceObject, public Core::FNoncopyableObject
	{
	public:
		~FImageResourceCache(void);

	public:
		Painting::FImage m_image;
	};

	// 
	FIMAGERESOURCE_INLINE FImageResource::FImageResourceCache::~FImageResourceCache(void)
	{
		FImageResource::FImageResourceCacheMgr::pop(this);
	}

	//////////////////////////////////////////////////////////////////////////
	// FImageResourcePrivate
	class FImageResource::FImageResourcePrivate : public Core::FReferenceObject, public Core::FNoncopyableObject
	{
	public:
		FImageResourcePrivate(void);
		~FImageResourcePrivate(void);

	public:
		/** 从本地文件加载图像 */
		bool load(LPCTSTR pszFileName);
		/** 从zip加载图像，并根据图像名加入缓存 */
		bool load(LPCTSTR pszFileName, void* pStream, DWORD dwSize);
		const Painting::FImage& getImage(void) const;
		bool isNull(void) const;

	private:
		FImageResourceCache* m_pImageCache;

	public:
		Core::FRect m_rcSource;
		Core::FEdge m_egNPatch;
	};

	FIMAGERESOURCE_INLINE FImageResource::FImageResourcePrivate::FImageResourcePrivate(void) : 
		m_pImageCache(NULL)
	{
	}

	FIMAGERESOURCE_INLINE FImageResource::FImageResourcePrivate::~FImageResourcePrivate(void)
	{
		if (NULL != m_pImageCache)
		{
			m_pImageCache->release();
			m_pImageCache = NULL;
		}
	}

	FIMAGERESOURCE_INLINE bool FImageResource::FImageResourcePrivate::load(LPCTSTR pszFileName)
	{
		FASSERT(NULL != pszFileName);
		FASSERT(NULL == m_pImageCache);

		Core::FString strFileName = pszFileName;
		strFileName.makeLower();

		FImageResourceCache* pImageCache = FImageResourceCacheMgr::get(strFileName);
		if (NULL != pImageCache)
		{
			m_pImageCache = pImageCache;
			m_pImageCache->addRef();

			return true;
		}
		else
		{
			m_pImageCache = new FImageResourceCache;
			m_pImageCache->addRef();

			bool bRet = m_pImageCache->m_image.load(pszFileName);
			if (!bRet)
			{
				fooSafeDelete(m_pImageCache);

				return false;
			}

			FImageResourceCacheMgr::push(strFileName, m_pImageCache);

			return true;
		}

		return false;
	}

	FIMAGERESOURCE_INLINE bool FImageResource::FImageResourcePrivate::load(LPCTSTR pszFileName, void* pStream, DWORD dwSize)
	{
		FASSERT(NULL != pszFileName);
		FASSERT(NULL == m_pImageCache);

		Core::FString strFileName = pszFileName;
		strFileName.makeLower();

		FImageResourceCache* pImageCache = FImageResourceCacheMgr::get(strFileName);
		if (NULL != pImageCache)
		{
			m_pImageCache = pImageCache;
			m_pImageCache->addRef();

			return true;
		}
		else
		{
			m_pImageCache = new FImageResourceCache;
			m_pImageCache->addRef();

			bool bRet = m_pImageCache->m_image.load(pStream, dwSize);
			if (!bRet)
			{
				fooSafeDelete(m_pImageCache);

				return false;
			}

			FImageResourceCacheMgr::push(strFileName, m_pImageCache);

			return true;
		}

		return false;
	}

	FIMAGERESOURCE_INLINE const Painting::FImage& FImageResource::FImageResourcePrivate::getImage(void) const
	{
		FASSERT(NULL != m_pImageCache);
		return m_pImageCache->m_image;
	}

	FIMAGERESOURCE_INLINE bool FImageResource::FImageResourcePrivate::isNull(void) const
	{
		FASSERT(NULL != m_pImageCache);
		return m_pImageCache->m_image.isNull();
	}

	//////////////////////////////////////////////////////////////////////////
	// FImageResource
	FIMAGERESOURCE_INLINE FImageResource::FImageResource(void)
	{
		m_imageResPrivate = new FImageResourcePrivate;
		m_imageResPrivate->addRef();
	}

	FIMAGERESOURCE_INLINE FImageResource::FImageResource(const FImageResource& res)
	{
		FASSERT(this != &res);
		m_imageResPrivate = res.m_imageResPrivate;
		m_imageResPrivate->addRef();
	}

	FIMAGERESOURCE_INLINE FImageResource& FImageResource::operator=(const FImageResource& res)
	{
		if (m_imageResPrivate != res.m_imageResPrivate)
		{
			m_imageResPrivate->release();
			m_imageResPrivate = res.m_imageResPrivate;
			m_imageResPrivate->addRef();
		}

		return (*this);
	}

	FIMAGERESOURCE_INLINE FImageResource::~FImageResource(void)
	{
		FASSERT(NULL != m_imageResPrivate);
		m_imageResPrivate->release();
		m_imageResPrivate = NULL;
	}

	FIMAGERESOURCE_INLINE const Painting::FImage& FImageResource::getImage(void) const
	{
		FASSERT(NULL != m_imageResPrivate);
		return m_imageResPrivate->getImage();
	}

	FIMAGERESOURCE_INLINE const Core::FRect& FImageResource::getSource(void) const
	{
		FASSERT(NULL != m_imageResPrivate);
		return m_imageResPrivate->m_rcSource;
	}

	FIMAGERESOURCE_INLINE const Core::FEdge& FImageResource::getNPatch(void) const
	{
		FASSERT(NULL != m_imageResPrivate);
		return m_imageResPrivate->m_egNPatch;
	}

	FIMAGERESOURCE_INLINE bool FImageResource::isNull(void) const
	{
		FASSERT(NULL != m_imageResPrivate);
		return m_imageResPrivate->isNull();
	}

	FIMAGERESOURCE_INLINE bool FImageResource::parseAttribute(FMarkup* pMarkup, LPCTSTR pszValue)
	{
		FASSERT(NULL != m_imageResPrivate);
		FASSERT(NULL != pMarkup);
		FASSERT(NULL != pszValue);

		if (pszValue[0] == _T('#'))
		{
			FImageResource *pImageRes = (FImageResource*)pMarkup->getResource(pszValue + 1, FResourceObject::FRESTYPE_IMAGE);
			if (NULL != pImageRes)
			{
				*this = *pImageRes;
				return true;
			}
		}

		if (__super::parseAttribute(pMarkup, pszValue))
		{
			return true;
		}

		if (parseSource(pszValue))
		{
			return true;
		}

		return false;
	}

	FIMAGERESOURCE_INLINE bool FImageResource::setAttribute(FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		FASSERT(NULL != m_imageResPrivate);
		FASSERT(NULL != pMarkup);
		FASSERT(NULL != pszName);
		FASSERT(NULL != pszValue);

		if (_tcscmp(pszName, _T("src")) == 0)
		{
			parseSource(pszValue);
		}
		else if (_tcscmp(pszName, _T("source")) == 0)
		{
			fooMarkupParseRectWH(&m_imageResPrivate->m_rcSource, pszValue);
		}
		else if (_tcscmp(pszName, _T("sourcerect")) == 0)
		{
			fooMarkupParseRect(&m_imageResPrivate->m_rcSource, pszValue);
		}
		else if (_tcscmp(pszName, _T("npatch")) == 0)
		{
			fooMarkupParseEdge(&m_imageResPrivate->m_egNPatch, pszValue);
		}

		return __super::setAttribute(pMarkup, pszName, pszValue);
	}

	FIMAGERESOURCE_INLINE bool FImageResource::parseSource(LPCTSTR pszValue)
	{
		FASSERT(NULL != m_imageResPrivate);
		FASSERT(NULL != pszValue);

		bool bRet = false;

		if (FMarkup::isSkinPackageOpened())
		{
			/** 优先从zip打开 */
			int nNeedBuffer = FMarkup::unpackFile(pszValue, NULL, 0);
			if (nNeedBuffer > 0)
			{
				char *pBuffer = new char[nNeedBuffer];
				FMarkup::unpackFile(pszValue, pBuffer, nNeedBuffer);
				bRet = m_imageResPrivate->load(pszValue, pBuffer, nNeedBuffer);
				fooSafeDeleteArray(pBuffer);
			}
		}

		if (!bRet)
		{
			/** 优先从XML根目录打开 */
			LPCTSTR pszFullPath = FPathHelper::pathHelper(FMarkup::getResourcePath(), pszValue);
			if (NULL != pszFullPath)
			{
				bRet = m_imageResPrivate->load(pszFullPath);
			}
		}

		if (!bRet)
		{
			/** 优先从实例根目录打开 */
			LPCTSTR pszFullPath = FPathHelper::pathHelper(Core::FApplication::getApplicationDirectory(), pszValue);
			if (NULL != pszFullPath)
			{
				bRet = m_imageResPrivate->load(pszFullPath);
			}
		}

		if (!bRet)
		{
			/** 尝试直接打开 */
			bRet = m_imageResPrivate->load(pszValue);
		}

		FASSERT(bRet);
		return bRet;
	}

}} // !namespace