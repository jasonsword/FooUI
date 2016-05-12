/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FCursorResource.h
* 描述：		FCursorResource
* 创建日期：	2013-06-21
*
*/

#include "StdAfx.h"
#include "core/FObject.h"
#include "core/FApplication.h"
#include "markup/FMarkup.h"
#include "markup/FCursorResource.h"

#define FCURSORRESOURCE_INLINE FOOINLINE

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FCursorResourcePrivate
	class FCursorResource::FCursorResourcePrivate : public Core::FReferenceObject, public Core::FNoncopyableObject
	{
	public:
		FCursorResourcePrivate(void);
		~FCursorResourcePrivate(void);

	public:
		bool setCursor(HCURSOR hCursor);
		bool loadSystem(LPCTSTR pszName);
		bool loadFromFile(LPCTSTR pszFileName);
		bool loadFromMemory(void* pBuffer, DWORD dwLength);
		bool destroy(void);

	public:
		HCURSOR m_hCursor;
	};

	// 
	FCURSORRESOURCE_INLINE FCursorResource::FCursorResourcePrivate::FCursorResourcePrivate(void) : 
		m_hCursor(NULL)
	{
	}

	FCURSORRESOURCE_INLINE FCursorResource::FCursorResourcePrivate::~FCursorResourcePrivate(void)
	{
		if (NULL != m_hCursor)
		{
			::DestroyCursor(m_hCursor);
			m_hCursor = NULL;
		}
	}

	FCURSORRESOURCE_INLINE bool FCursorResource::FCursorResourcePrivate::setCursor(HCURSOR hCursor)
	{
		FASSERT(NULL == m_hCursor);

		m_hCursor = hCursor;

		return (NULL != m_hCursor);
	}

	FCURSORRESOURCE_INLINE bool FCursorResource::FCursorResourcePrivate::loadSystem(LPCTSTR pszName)
	{
		FASSERT(NULL == m_hCursor);

#define CURSOR_HANDLER(_pszName, _nCursorID) \
		if (_tcscmp(pszName, _pszName) == 0) \
		{ \
			return setCursor(::LoadCursor(NULL, _nCursorID)); \
		}

		CURSOR_HANDLER(_T("IDC_ARROW"), IDC_ARROW);
		CURSOR_HANDLER(_T("IDC_IBEAM"), IDC_IBEAM);
		CURSOR_HANDLER(_T("IDC_WAIT"), IDC_WAIT);
		CURSOR_HANDLER(_T("IDC_CROSS"), IDC_CROSS);
		CURSOR_HANDLER(_T("IDC_UPARROW"), IDC_UPARROW);
		CURSOR_HANDLER(_T("IDC_SIZE"), IDC_SIZE);
		CURSOR_HANDLER(_T("IDC_ICON"), IDC_ICON);
		CURSOR_HANDLER(_T("IDC_SIZENWSE"), IDC_SIZENWSE);
		CURSOR_HANDLER(_T("IDC_SIZENESW"), IDC_SIZENESW);
		CURSOR_HANDLER(_T("IDC_SIZEWE"), IDC_SIZEWE);
		CURSOR_HANDLER(_T("IDC_SIZENS"), IDC_SIZENS);
		CURSOR_HANDLER(_T("IDC_SIZEALL"), IDC_SIZEALL);
		CURSOR_HANDLER(_T("IDC_NO"), IDC_NO);
		#if(WINVER >= 0x0500)
		CURSOR_HANDLER(_T("IDC_HAND"), IDC_HAND);
		#endif /* WINVER >= 0x0500 */
		CURSOR_HANDLER(_T("IDC_APPSTARTING"), IDC_APPSTARTING);
		#if(WINVER >= 0x0400)
		CURSOR_HANDLER(_T("IDC_HELP"), IDC_HELP);
		#endif /* WINVER >= 0x0400 */

		return false;
	}

	FCURSORRESOURCE_INLINE bool FCursorResource::FCursorResourcePrivate::loadFromFile(LPCTSTR pszFileName)
	{
		FASSERT(NULL == m_hCursor);
		return setCursor(::LoadCursorFromFile(pszFileName));
	}

	FCURSORRESOURCE_INLINE bool FCursorResource::FCursorResourcePrivate::loadFromMemory(void* pBuffer, DWORD dwLength)
	{
		FASSERT(NULL == m_hCursor);
		return setCursor(::CreateIconFromResource((PBYTE)pBuffer, dwLength, FALSE, 0x00030000));
	}

	FCURSORRESOURCE_INLINE bool FCursorResource::FCursorResourcePrivate::destroy(void)
	{
		FASSERT(NULL != m_hCursor);

		::DestroyCursor(m_hCursor);
		m_hCursor = NULL;

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// FCursorResource
	FCURSORRESOURCE_INLINE FCursorResource::FCursorResource(void)
	{
		m_cursorResPrivate = new FCursorResourcePrivate;
		m_cursorResPrivate->addRef();
	}

	FCURSORRESOURCE_INLINE FCursorResource::FCursorResource(const FCursorResource& res)
	{
		FASSERT(this != &res);
		m_cursorResPrivate = res.m_cursorResPrivate;
		m_cursorResPrivate->addRef();
	}

	FCURSORRESOURCE_INLINE FCursorResource& FCursorResource::operator=(const FCursorResource& res)
	{
		if (m_cursorResPrivate != res.m_cursorResPrivate)
		{
			m_cursorResPrivate->release();
			m_cursorResPrivate = res.m_cursorResPrivate;
			m_cursorResPrivate->addRef();
		}

		return (*this);
	}

	FCURSORRESOURCE_INLINE FCursorResource::~FCursorResource(void)
	{
		FASSERT(NULL != m_cursorResPrivate);
		m_cursorResPrivate->release();
		m_cursorResPrivate = NULL;
	}

	FCURSORRESOURCE_INLINE HCURSOR FCursorResource::getCursor(void) const
	{
		FASSERT(NULL != m_cursorResPrivate);
		return m_cursorResPrivate->m_hCursor;
	}

	FCURSORRESOURCE_INLINE bool FCursorResource::showCursor(void) const
	{
		FASSERT(NULL != m_cursorResPrivate);
		if (NULL == m_cursorResPrivate->m_hCursor)
		{
			return false;
		}

		::SetCursor(m_cursorResPrivate->m_hCursor);
		return true;
	}

	FCURSORRESOURCE_INLINE bool FCursorResource::isNull(void) const
	{
		FASSERT(NULL != m_cursorResPrivate);
		return (NULL == m_cursorResPrivate->m_hCursor);
	}

	FCURSORRESOURCE_INLINE bool FCursorResource::parseAttribute(FMarkup* pMarkup, LPCTSTR pszValue)
	{
		FASSERT(NULL != m_cursorResPrivate);
		FASSERT(NULL != pMarkup);
		FASSERT(NULL != pszValue);

		if (pszValue[0] == _T('#'))
		{
			FCursorResource *pCursorRes = (FCursorResource*)pMarkup->getResource(pszValue + 1, FResourceObject::FRESTYPE_CURSOR);
			if (NULL != pCursorRes)
			{
				*this = *pCursorRes;
				return true;
			}
		}

		return __super::parseAttribute(pMarkup, pszValue);
	}

	FCURSORRESOURCE_INLINE bool FCursorResource::setAttribute(FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		FASSERT(NULL != m_cursorResPrivate);
		FASSERT(NULL != pMarkup);
		FASSERT(NULL != pszName);
		FASSERT(NULL != pszValue);

		if (_tcscmp(pszName, _T("src")) == 0)
		{
			parseSource(pszValue);
		}

		return __super::setAttribute(pMarkup, pszName, pszValue);
	}

	FCURSORRESOURCE_INLINE bool FCursorResource::parseSource(LPCTSTR pszValue)
	{
		FASSERT(NULL != m_cursorResPrivate);
		FASSERT(NULL != pszValue);

		bool bRet = false;

		if (pszValue[0] == _T('#') || pszValue[0] == _T('$'))
		{
			bRet = m_cursorResPrivate->loadSystem(pszValue + 1);
		}

		if (!bRet)
		{
			if (FMarkup::isSkinPackageOpened())
			{
				/** 优先从zip打开 */
				int nNeedBuffer = FMarkup::unpackFile(pszValue, NULL, 0);
				if (nNeedBuffer > 0)
				{
					char *pBuffer = new char[nNeedBuffer];
					FMarkup::unpackFile(pszValue, pBuffer, nNeedBuffer);
					bRet = m_cursorResPrivate->loadFromMemory(pBuffer, nNeedBuffer);
					fooSafeDeleteArray(pBuffer);
				}
			}
		}

		if (!bRet)
		{
			/** 优先从XML根目录打开 */
			LPCTSTR pszFullPath = FPathHelper::pathHelper(FMarkup::getResourcePath(), pszValue);
			if (NULL != pszFullPath)
			{
				bRet = m_cursorResPrivate->loadFromFile(pszFullPath);
			}
		}

		if (!bRet)
		{
			/** 优先从实例根目录打开 */
			LPCTSTR pszFullPath = FPathHelper::pathHelper(Core::FApplication::getApplicationDirectory(), pszValue);
			if (NULL != pszFullPath)
			{
				bRet = m_cursorResPrivate->loadFromFile(pszFullPath);
			}
		}

		if (!bRet)
		{
			/** 尝试直接打开 */
			bRet = m_cursorResPrivate->loadFromFile(pszValue);
		}

		FASSERT(bRet);
		return bRet;
	}

}} // !namespace