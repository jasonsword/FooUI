/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FIncludeStyle.cpp
* ������		FIncludeStyle
* �������ڣ�	2013-07-29
*
*/

#include "StdAfx.h"
#include "core/FObject.h"
#include "core/FPtrList.h"
#include "markup/FStyleSheetResource.h"
#include "markup/FAttributeScript.h"
#include "styles/FIncludeStyle.h"

#define FINCLUDESTYLE_INLINE FOOINLINE

namespace FooUI { namespace Styles {

	//////////////////////////////////////////////////////////////////////////
	// FIncludeStylePrivate
	class FIncludeStyle::FIncludeStylePrivate : public Core::FReferenceObject, public Core::FNoncopyableObject
	{
	public:
		FIncludeStylePrivate(void);
		~FIncludeStylePrivate(void);

	public:
		Markup::FStyleSheetResource m_resStyle;
	};

	// 
	FIncludeStyle::FIncludeStylePrivate::FIncludeStylePrivate(void)
	{
	}

	FIncludeStyle::FIncludeStylePrivate::~FIncludeStylePrivate(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	// FIncludeStyle
	FINCLUDESTYLE_INLINE FIncludeStyle::FIncludeStyle(void)
	{
		m_includeStylePrivate = new FIncludeStylePrivate;
		m_includeStylePrivate->addRef();
	}

	FINCLUDESTYLE_INLINE FIncludeStyle::FIncludeStyle(const FIncludeStyle& style)
	{
		FASSERT(this != &style);

		m_includeStylePrivate = style.m_includeStylePrivate;
		m_includeStylePrivate->addRef();
	}

	FINCLUDESTYLE_INLINE FIncludeStyle& FIncludeStyle::operator=(const FIncludeStyle& style)
	{
		if (m_includeStylePrivate != style.m_includeStylePrivate)
		{
			m_includeStylePrivate->release();
			m_includeStylePrivate = style.m_includeStylePrivate;
			m_includeStylePrivate->addRef();
		}

		return (*this);
	}

	FINCLUDESTYLE_INLINE FIncludeStyle::~FIncludeStyle(void)
	{
		FASSERT(NULL != m_includeStylePrivate);
		m_includeStylePrivate->release();
		m_includeStylePrivate = NULL;
	}

	FINCLUDESTYLE_INLINE bool FIncludeStyle::setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		FASSERT(NULL != m_includeStylePrivate);
		if (_tcscmp(pszName, _T("src")) == 0)
		{
			m_includeStylePrivate->m_resStyle.parseAttribute(pMarkup, pszValue);
		}

		return __super::setAttribute(pMarkup, pszName, pszValue);
	}

	FINCLUDESTYLE_INLINE bool FIncludeStyle::draw(Widgets::FWidget* pWidget, /** �ؼ����� */
												Painting::FGraphics* pGraphics, /** ���� */
												const Core::FRect* prcUpdate, /** �������� */
												DWORD dwState, /** ״̬ */
												const Core::FRect& rcDest, /** ��ͼ���� */
												LPCTSTR pszType/* = NULL*/, 
												LPCTSTR pszPart/* = NULL*/, 
												FSTYLESTRUCT* fss/* = NULL*/)
	{
		FASSERT(NULL != m_includeStylePrivate);
		return m_includeStylePrivate->m_resStyle.draw(pWidget, pGraphics, 
									prcUpdate, dwState, rcDest, pszType, pszPart, fss);
	}

	FINCLUDESTYLE_INLINE bool FIncludeStyle::testStyle(DWORD dwState, 
							LPCTSTR pszType/* = NULL*/, LPCTSTR pszPart/* = NULL*/) const
	{
		FASSERT(NULL != m_includeStylePrivate);
		return m_includeStylePrivate->m_resStyle.testStyle(dwState, pszType, pszPart);
	}

}} // !namespace