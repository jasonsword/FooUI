/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FStyleSheetResource.cpp
* 描述：		FStyleSheetResource
* 创建日期：	2013-06-26
*
*/

#include "StdAfx.h"
#include "core/FObject.h"
#include "markup/FAttributeScript.h"
#include "markup/FStyleSheetResource.h"
#include "styles/FStyleSheet.h"

#define FSTYLESHEETRESOURCE_INLINE FOOINLINE

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FStyleSheetResourcePrivate
	class FStyleSheetResource::FStyleSheetResourcePrivate : public Core::FReferenceObject, public Core::FNoncopyableObject
	{
	public:
		Styles::FStyleSheet m_stylesheet;
	};

	//////////////////////////////////////////////////////////////////////////
	// FStyleSheetResource
	FSTYLESHEETRESOURCE_INLINE FStyleSheetResource::FStyleSheetResource(void)
	{
		m_stylesheetResPrivate = new FStyleSheetResourcePrivate;
		m_stylesheetResPrivate->addRef();
	}

	FSTYLESHEETRESOURCE_INLINE FStyleSheetResource::FStyleSheetResource(const FStyleSheetResource& res)
	{
		FASSERT(this != &res);
		m_stylesheetResPrivate = res.m_stylesheetResPrivate;
		m_stylesheetResPrivate->addRef();
	}

	FSTYLESHEETRESOURCE_INLINE FStyleSheetResource& FStyleSheetResource::operator=(const FStyleSheetResource& res)
	{
		if (m_stylesheetResPrivate != res.m_stylesheetResPrivate)
		{
			m_stylesheetResPrivate->release();
			m_stylesheetResPrivate = res.m_stylesheetResPrivate;
			m_stylesheetResPrivate->addRef();
		}

		return (*this);
	}

	FSTYLESHEETRESOURCE_INLINE FStyleSheetResource::~FStyleSheetResource(void)
	{
		FASSERT(NULL != m_stylesheetResPrivate);
		m_stylesheetResPrivate->release();
		m_stylesheetResPrivate = NULL;
	}

	FSTYLESHEETRESOURCE_INLINE bool FStyleSheetResource::addStyle(Styles::FStyleObject* pStyle)
	{
		FASSERT(NULL != m_stylesheetResPrivate);
		return m_stylesheetResPrivate->m_stylesheet.addStyle(pStyle);
	}

	FSTYLESHEETRESOURCE_INLINE Styles::FStyleObject* FStyleSheetResource::getStyle(int nIndex) const
	{
		FASSERT(NULL != m_stylesheetResPrivate);
		return m_stylesheetResPrivate->m_stylesheet.getStyle(nIndex);
	}

	FSTYLESHEETRESOURCE_INLINE bool FStyleSheetResource::remove(int nIndex)
	{
		FASSERT(NULL != m_stylesheetResPrivate);
		return m_stylesheetResPrivate->m_stylesheet.remove(nIndex);
	}

	FSTYLESHEETRESOURCE_INLINE bool FStyleSheetResource::remove(Styles::FStyleObject* pStyle)
	{
		FASSERT(NULL != m_stylesheetResPrivate);
		return m_stylesheetResPrivate->m_stylesheet.remove(pStyle);
	}

	FSTYLESHEETRESOURCE_INLINE bool FStyleSheetResource::clear(void)
	{
		FASSERT(NULL != m_stylesheetResPrivate);
		return m_stylesheetResPrivate->m_stylesheet.clear();
	}

	FSTYLESHEETRESOURCE_INLINE int FStyleSheetResource::getStyleCount(void) const
	{
		FASSERT(NULL != m_stylesheetResPrivate);
		return m_stylesheetResPrivate->m_stylesheet.getStyleCount();
	}

	FSTYLESHEETRESOURCE_INLINE bool FStyleSheetResource::isNull(void) const
	{
		FASSERT(NULL != m_stylesheetResPrivate);
		return m_stylesheetResPrivate->m_stylesheet.isNull();
	}

	FSTYLESHEETRESOURCE_INLINE bool FStyleSheetResource::draw(Widgets::FWidget* pWidget, 
																Painting::FGraphics* pGraphics, 
																const Core::FRect* prcUpdate, 
																DWORD dwState, 
																const Core::FRect& rcDest, 
																LPCTSTR pszType/* = NULL*/, 
																LPCTSTR pszPart/* = NULL*/, 
																Styles::FSTYLESTRUCT* fss/* = NULL*/) const
	{
		FASSERT(NULL != m_stylesheetResPrivate);
		return m_stylesheetResPrivate->m_stylesheet.draw(pWidget, pGraphics, 
									prcUpdate, dwState, rcDest, pszType, pszPart, fss);
	}

	bool FStyleSheetResource::createWindowRegion(HWND hWnd, DWORD dwState, int xLeft, int yTop, int xRight, int yBottom, 
		LPCTSTR pszType/* = NULL*/, LPCTSTR pszPart/* = NULL*/) const
	{
		FASSERT(NULL != m_stylesheetResPrivate);
		return m_stylesheetResPrivate->m_stylesheet.createWindowRegion(hWnd, dwState, xLeft, yTop, 
																	xRight, yBottom, pszType, pszPart);
	}

	FSTYLESHEETRESOURCE_INLINE bool FStyleSheetResource::testStyle(DWORD dwState, 
									LPCTSTR pszType/* = NULL*/, LPCTSTR pszPart/* = NULL*/) const
	{
		FASSERT(NULL != m_stylesheetResPrivate);
		return m_stylesheetResPrivate->m_stylesheet.testStyle(dwState, pszType, pszPart);
	}

	FSTYLESHEETRESOURCE_INLINE bool FStyleSheetResource::parseAttribute(FMarkup* pMarkup, LPCTSTR pszValue)
	{
		if (pszValue[0] == _T('#'))
		{
			FStyleSheetResource *pStyleSheet = (FStyleSheetResource*)pMarkup->getResource(pszValue + 1, 
																			FResourceObject::FRESTYPE_STYLESHEET);
			if (NULL != pStyleSheet)
			{
				*this = *pStyleSheet;
				return true;
			}
			FASSERT(false); /** 没有找到资源，警告 */
		}

		return parseAttributeScript(pMarkup, pszValue);
	}

	FSTYLESHEETRESOURCE_INLINE bool FStyleSheetResource::parseAttributeScript(FMarkup* pMarkup, LPCTSTR pszAttributeScript)
	{
		Markup::FAttributeScript attrScript(pszAttributeScript);
		Markup::FATTRIBUTESCRIPT *pAttr = attrScript.firstAttribute();

		if (NULL != pAttr)
		{
			beginMarkup(pMarkup);
			while (NULL != pAttr)
			{
				Styles::FStyleObject *pStyleObject = Styles::FStyleObject::createStyle(pAttr->name);
				if (NULL != pStyleObject)
				{
					pStyleObject->beginMarkup(pMarkup);

					/** 解析子属性 */
					do
					{
						Markup::FATTRIBUTESCRIPT *pChildAttr = pAttr->children;
						while (NULL != pChildAttr)
						{
							pStyleObject->setAttribute(pMarkup, pChildAttr->name, pChildAttr->value);
							pChildAttr = pChildAttr->next;
						}
					} while(0);

					pStyleObject->endMarkup(pMarkup);

					if (!addStyle(pStyleObject))
					{
						fooSafeDeleteObject(pStyleObject);
					}
				}
				pAttr = pAttr->next;
			}
			endMarkup(pMarkup);

			return true;
		}

		return false;
	}

	FSTYLESHEETRESOURCE_INLINE bool FStyleSheetResource::setAttribute(FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		FASSERT(NULL != m_stylesheetResPrivate);
		FASSERT(NULL != pMarkup);
		FASSERT(NULL != pszName);
		FASSERT(NULL != pszValue);

		return __super::setAttribute(pMarkup, pszName, pszValue);
	}

}} // !namespace