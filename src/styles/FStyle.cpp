/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FStyle.cpp
* 描述：		FStyle
* 创建日期：	2013-06-19
*
*/

#include "StdAfx.h"

#include "core/FObject.h"
#include "core/FPtrList.h"

#include "markup/FMarkup.h"
#include "markup/FMarkupDef.h"

#include "styles/FStyle.h"
#include "styles/FStyleDef.h"

#include "styles/FIncludeStyle.h"
#include "styles/FColorStyle.h"
#include "styles/FImageStyle.h"
#include "styles/FTextStyle.h"
#include "styles/FFocusRectStyle.h"
#include "styles/FWindowRegionStyle.h"

#include "widgets/FWidget.h"
#include "widgets/FWidgetPrivate_p.h"

#define FSTYLE_INLINE FOOINLINE

namespace FooUI { namespace Styles {

	//////////////////////////////////////////////////////////////////////////
	// 
	class FStyleSheet::FStyleSheetPrivate : public Core::FReferenceObject, public Core::FNoncopyableObject
	{
	public:
		FStyleSheetPrivate(void);

	public:
		bool addStyle(FStyleObject* pStyle);
		FStyleObject* getStyle(int nIndex) const;
		FStyleObject* getStyle(LPCTSTR pszName) const;
		bool remove(int nIndex);
		bool remove(FStyleObject* pStyle);
		bool clear(void);
		int getStyleCount(void) const;
		bool isNull(void) const;

	public:
		bool draw(Widgets::FWidget* pWidget, /** 控件对象 */
			Painting::FGraphics* pGraphics, /** 画布 */
			const Core::FRect* prcUpdate, /** 更新区域 */
			DWORD dwState, /** 状态 */
			const Core::FRect& rcDest, /** 绘图区域 */
			LPCTSTR pszType/* = NULL*/, 
			LPCTSTR pszPart/* = NULL*/, 
			FSTYLESTRUCT* fss/* = NULL*/) const;
		bool createWindowRegion(HWND hWnd, DWORD dwState, int xLeft, int yTop, int xRight, int yBottom, 
			LPCTSTR pszType = NULL, LPCTSTR pszPart = NULL) const;
		bool testStyle(DWORD dwState, LPCTSTR pszType = NULL, LPCTSTR pszPart = NULL) const;

	private:
		Core::FPtrList m_listStyles;
		Core::FPtrList m_listRegions;
		static void CALLBACK removeCallBack(LPVOID pVal, LPVOID lpParameter);
	};

	FSTYLE_INLINE FStyleSheet::FStyleSheetPrivate::FStyleSheetPrivate(void)
	{
		m_listStyles.setRemoveCallBack(FStyleSheet::FStyleSheetPrivate::removeCallBack, NULL);
		m_listRegions.setRemoveCallBack(FStyleSheet::FStyleSheetPrivate::removeCallBack, NULL);
	}

	FSTYLE_INLINE bool FStyleSheet::FStyleSheetPrivate::addStyle(FStyleObject* pStyle)
	{
		if (FStyleObject::WindowRegionStyle == pStyle->getStyleType())
		{
			return m_listRegions.add((LPVOID)pStyle);
		}
		else
		{
			return m_listStyles.add((LPVOID)pStyle);
		}
	}

	FSTYLE_INLINE FStyleObject* FStyleSheet::FStyleSheetPrivate::getStyle(int nIndex) const
	{
		return (FStyleObject*)m_listStyles.get(nIndex);
	}

	FSTYLE_INLINE FStyleObject* FStyleSheet::FStyleSheetPrivate::getStyle(LPCTSTR pszName) const
	{
		for (int i = 0; i < getStyleCount(); ++i)
		{
			FStyleObject* pStyleObject = reinterpret_cast<FStyleObject*>(m_listStyles.get(i));
			FASSERT(NULL != pStyleObject);

			if (_tcscmp(pStyleObject->getName(), pszName) == 0)
			{
				return pStyleObject;
			}
		}

		return NULL;
	}

	FSTYLE_INLINE bool FStyleSheet::FStyleSheetPrivate::remove(int nIndex)
	{
		return m_listStyles.remove(nIndex);
	}

	FSTYLE_INLINE bool FStyleSheet::FStyleSheetPrivate::remove(FStyleObject* pStyle)
	{
		Core::FPtrList::POSITION position = m_listStyles.find((LPVOID)pStyle);
		if (NULL != position)
		{
			return m_listStyles.removeAtPosition(position);
		}

		return false;
	}

	FSTYLE_INLINE bool FStyleSheet::FStyleSheetPrivate::clear(void)
	{
		return m_listStyles.clear();
	}

	FSTYLE_INLINE int FStyleSheet::FStyleSheetPrivate::getStyleCount(void) const
	{
		return m_listStyles.size();
	}

	FSTYLE_INLINE bool FStyleSheet::FStyleSheetPrivate::isNull(void) const
	{
		return m_listStyles.isEmpty();
	}

	FSTYLE_INLINE bool FStyleSheet::FStyleSheetPrivate::draw(Widgets::FWidget* pWidget, /** 控件对象 */
													Painting::FGraphics* pGraphics, /** 画布 */
													const Core::FRect* prcUpdate, /** 更新区域 */
													DWORD dwState, /** 状态 */
													const Core::FRect& rcDest, /** 绘图区域 */
													LPCTSTR pszType/* = NULL*/, 
													LPCTSTR pszPart/* = NULL*/, 
													FSTYLESTRUCT* fss/* = NULL*/) const
	{
		if (NULL == pWidget || NULL == pGraphics)
		{
			FASSERT(FALSE);
			return false;
		}

		if (m_listStyles.isEmpty()) return false;
		
		if (pWidget != Widgets::FWidgetPrivate::fActiveWidget)
		{
			Widgets::FWidget *parent = pWidget->getParent();
			while (NULL != parent)
			{
				if (parent == Widgets::FWidgetPrivate::fActiveWidget)
				{
					dwState &= ~FOOSTATE_DEACTIVATED;
					dwState |= FOOSTATE_ACTIVATED;
					break;
				}

				parent = parent->getParent();
			}
		}

		bool bRet = false;
		for (int i = 0; i < m_listStyles.size(); ++i)
		{
			FStyleObject *pStyleObject = (FStyleObject*)m_listStyles.get(i);
			FASSERT(NULL != pStyleObject);

			if (pStyleObject->draw(pWidget, pGraphics, prcUpdate, dwState, rcDest, pszType, pszPart, fss))
			{
				bRet = true;
			}
		}

		return bRet;
	}

	bool FStyleSheet::FStyleSheetPrivate::createWindowRegion(HWND hWnd, DWORD dwState, int xLeft, int yTop, int xRight, int yBottom, 
		LPCTSTR pszType/* = NULL*/, LPCTSTR pszPart/* = NULL*/) const
	{
		if (m_listStyles.isEmpty()) return false;

		HRGN hRgn = NULL;

		bool bRet = false;
		for (int i = 0; i < m_listRegions.size(); ++i)
		{
			FWindowRegionStyle *pWindowRegionStyle = (FWindowRegionStyle*)m_listRegions.get(i);

			if (!fooTestAttribute(dwState, pWindowRegionStyle->getState()))
			{
				continue;
			}

			if (NULL != pszType 
				&& _tcscmp(pszType, pWindowRegionStyle->getType()) != 0)
			{
				continue;
			}

			if (NULL != pszPart 
				&& _tcscmp(pszPart, pWindowRegionStyle->getPart()) != 0)
			{
				continue;
			}

			HRGN hStyleRgn = NULL;

			Core::FSize szStyle(xRight- xLeft, yBottom - yTop);
			if (pWindowRegionStyle->getSize().cx > 0) szStyle.cx = pWindowRegionStyle->getSize().cx;
			if (pWindowRegionStyle->getSize().cy > 0) szStyle.cy = pWindowRegionStyle->getSize().cy;
			Core::FRect rcStyle(xLeft, yTop, xLeft + szStyle.cx, yTop + szStyle.cy);
			rcStyle.deflateRect(pWindowRegionStyle->deflaterect());

			switch (pWindowRegionStyle->shape())
			{
			case FWindowRegionStyle::RoundRectShape:
				hStyleRgn = ::CreateRoundRectRgn(rcStyle.left, rcStyle.top, rcStyle.right + 1, rcStyle.bottom + 1, 
					pWindowRegionStyle->roundsize().cx, 
					pWindowRegionStyle->roundsize().cy);
				break;

			case FWindowRegionStyle::RectAngleShape:
				hStyleRgn = ::CreateRectRgn(rcStyle.left, rcStyle.top, rcStyle.right + 1, rcStyle.bottom + 1);
				break;

			}

			if (NULL != hStyleRgn)
			{
				if (NULL == hRgn)
				{
					hRgn = hStyleRgn;
				}
				else
				{
					::CombineRgn(hRgn, hRgn, hStyleRgn, pWindowRegionStyle->combine());
					fooSafeDeleteObject(hStyleRgn);
				}
				bRet = true;
			}
		}

		if (NULL != hRgn)
		{
			::SetWindowRgn(hWnd, hRgn, TRUE);
			fooSafeDeleteObject(hRgn);
		}

		return bRet;
	}

	FSTYLE_INLINE bool FStyleSheet::FStyleSheetPrivate::testStyle(DWORD dwState, 
								LPCTSTR pszType/* = NULL*/, LPCTSTR pszPart/* = NULL*/) const
	{
		if (m_listStyles.isEmpty()) return false;

		for (int i = 0; i < m_listStyles.size(); ++i)
		{
			FStyleObject *pStyleObject = (FStyleObject*)m_listStyles.get(i);

			if (pStyleObject->testStyle(dwState, pszType, pszPart))
			{
				return true;
			}
		}

		return false;
	}

	FSTYLE_INLINE /*static*/ void CALLBACK FStyleSheet::FStyleSheetPrivate::removeCallBack(LPVOID pVal, LPVOID /*lpParameter*/)
	{
		FASSERT(NULL != pVal);
		fooSafeDeleteT(FStyleObject*, pVal);
	}

	//////////////////////////////////////////////////////////////////////////
	// FStyleSheet
	FSTYLE_INLINE FStyleSheet::FStyleSheet(void)
	{
		m_stylesheetPrivate = new FStyleSheetPrivate;
		m_stylesheetPrivate->addRef();
	}

	FSTYLE_INLINE FStyleSheet::FStyleSheet(const FStyleSheet& ss)
	{
		FASSERT(this != &ss);
		m_stylesheetPrivate = ss.m_stylesheetPrivate;
		m_stylesheetPrivate->addRef();
	}

	FSTYLE_INLINE FStyleSheet& FStyleSheet::operator=(const FStyleSheet& ss)
	{
		if (m_stylesheetPrivate != ss.m_stylesheetPrivate)
		{
			m_stylesheetPrivate->release();
			m_stylesheetPrivate = ss.m_stylesheetPrivate;
			m_stylesheetPrivate->addRef();
		}

		return (*this);
	}

	FSTYLE_INLINE FStyleSheet::~FStyleSheet(void)
	{
		FASSERT(NULL != m_stylesheetPrivate);
		m_stylesheetPrivate->release();
		m_stylesheetPrivate = NULL;
	}

	FSTYLE_INLINE bool FStyleSheet::addStyle(FStyleObject* pStyle)
	{
		FASSERT(NULL != m_stylesheetPrivate);
		return m_stylesheetPrivate->addStyle(pStyle);
	}

	FSTYLE_INLINE FStyleObject* FStyleSheet::getStyle(int nIndex) const
	{
		FASSERT(NULL != m_stylesheetPrivate);
		return m_stylesheetPrivate->getStyle(nIndex);
	}

	FSTYLE_INLINE FStyleObject* FStyleSheet::getStyle(LPCTSTR pszName) const
	{
		FASSERT(NULL != m_stylesheetPrivate);
		return m_stylesheetPrivate->getStyle(pszName);
	}

	FSTYLE_INLINE bool FStyleSheet::remove(int nIndex)
	{
		FASSERT(NULL != m_stylesheetPrivate);
		return m_stylesheetPrivate->remove(nIndex);
	}

	FSTYLE_INLINE bool FStyleSheet::remove(FStyleObject* pStyle)
	{
		FASSERT(NULL != m_stylesheetPrivate);
		return m_stylesheetPrivate->remove(pStyle);
	}

	FSTYLE_INLINE bool FStyleSheet::clear(void)
	{
		FASSERT(NULL != m_stylesheetPrivate);
		return m_stylesheetPrivate->clear();
	}

	FSTYLE_INLINE int FStyleSheet::getStyleCount(void) const
	{
		FASSERT(NULL != m_stylesheetPrivate);
		return m_stylesheetPrivate->getStyleCount();
	}

	FSTYLE_INLINE bool FStyleSheet::isNull(void) const
	{
		FASSERT(NULL != m_stylesheetPrivate);
		return m_stylesheetPrivate->isNull();
	}

	bool FStyleSheet::draw(Widgets::FWidget* pWidget, 
			Painting::FGraphics* pGraphics, 
			const Core::FRect* prcUpdate, 
			DWORD dwState, 
			const Core::FRect& rcDest, 
			LPCTSTR pszType/* = NULL*/, 
			LPCTSTR pszPart/* = NULL*/, 
			FSTYLESTRUCT* fss/* = NULL*/) const
	{
		FASSERT(NULL != m_stylesheetPrivate);
		return m_stylesheetPrivate->draw(pWidget, pGraphics, prcUpdate, dwState, rcDest, pszType, pszPart, fss);
	}

	bool FStyleSheet::createWindowRegion(HWND hWnd, DWORD dwState, int xLeft, int yTop, int xRight, int yBottom, 
		LPCTSTR pszType/* = NULL*/, LPCTSTR pszPart/* = NULL*/) const
	{
		FASSERT(NULL != m_stylesheetPrivate);
		return m_stylesheetPrivate->createWindowRegion(hWnd, dwState, xLeft, yTop, xRight, yBottom, pszType, pszPart);
	}

	bool FStyleSheet::testStyle(DWORD dwState, LPCTSTR pszType/* = NULL*/, LPCTSTR pszPart/* = NULL*/) const
	{
		FASSERT(NULL != m_stylesheetPrivate);
		return m_stylesheetPrivate->testStyle(dwState, pszType, pszPart);
	}

	//////////////////////////////////////////////////////////////////////////
	// FStyleObject
	FSTYLE_INLINE FStyleObject::FStyleObject(void) : 
		m_strName(_T("")) 
		, m_strPart(_T("")) 
		, m_dwState(FOOSTATE_ALL) 
		, m_dwAlign(FOOALIGN_LEFT | FOOALIGN_TOP) 
		, m_egMargin(0, 0, 0, 0) 
		, m_size(0, 0) 
		, m_ptOffset(0, 0) 
	{
	}

	FSTYLE_INLINE LPCTSTR FStyleObject::getName(void) const
	{
		return m_strName;
	}

	FSTYLE_INLINE LPCTSTR FStyleObject::getType(void) const
	{
		return m_strType;
	}

	FSTYLE_INLINE LPCTSTR FStyleObject::getPart(void) const
	{
		return m_strPart;
	}

	FSTYLE_INLINE DWORD FStyleObject::getState(void) const
	{
		return m_dwState;
	}

	FSTYLE_INLINE DWORD FStyleObject::getAlign(void) const
	{
		return m_dwAlign;
	}

	FSTYLE_INLINE const Core::FEdge& FStyleObject::getMargin(void) const
	{
		return m_egMargin;
	}

	FSTYLE_INLINE const Core::FSize& FStyleObject::getSize(void) const
	{
		return m_size;
	}

	FSTYLE_INLINE const Core::FPoint& FStyleObject::getOffset(void) const
	{
		return m_ptOffset;
	}

	bool FStyleObject::testStyle(DWORD dwState, LPCTSTR pszType/* = NULL*/, LPCTSTR pszPart/* = NULL*/) const
	{
		if (NULL != pszType && _tcscmp(getType(), pszType) != 0) return false;
		if (NULL != pszPart && _tcscmp(getPart(), pszPart) != 0) return false;

		if (dwState != 0 && (getState() & dwState))
		{
			return true;
		}

		return false;
	}

	FSTYLE_INLINE bool FStyleObject::setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		FASSERT(NULL != pMarkup);
		FASSERT(NULL != pszName);
		FASSERT(NULL != pszValue);

		if (_tcscmp(pszName, _T("name")) == 0)
		{
			m_strName = pszValue;
		}
		else if (_tcscmp(pszName, _T("type")) == 0)
		{
			m_strType = pszValue;
		}
		else if (_tcscmp(pszName, _T("part")) == 0)
		{
			m_strPart = pszValue;
		}
		else if (_tcscmp(pszName, _T("state")) == 0)
		{
			m_dwState = Markup::fooMarkupParseState(pszValue);
		}
		else if (_tcscmp(pszName, _T("align")) == 0)
		{
			m_dwAlign = Markup::fooMarkupParseAlign(pszValue);
		}
		else if (_tcscmp(pszName, _T("margin")) == 0)
		{
			Markup::fooMarkupParseEdge(&m_egMargin, pszValue);
		}
		else if (_tcscmp(pszName, _T("padding")) == 0)
		{
			Markup::fooMarkupParseEdge(&m_egPadding, pszValue);
		}
		else if (_tcscmp(pszName, _T("size")) == 0)
		{
			Markup::fooMarkupParseSize(&m_size, pszValue);
		}
		else if (_tcscmp(pszName, _T("minsize")) == 0)
		{
			Markup::fooMarkupParseSize(&m_minsize, pszValue);
		}
		else if (_tcscmp(pszName, _T("maxsize")) == 0)
		{
			Markup::fooMarkupParseSize(&m_maxsize, pszValue);
		}
		else if (_tcscmp(pszName, _T("width")) == 0)
		{
			m_size.cx = _tcstol(pszValue, NULL, 10);
		}
		else if (_tcscmp(pszName, _T("height")) == 0)
		{
			m_size.cy = _tcstol(pszValue, NULL, 10);
		}
		else if (_tcscmp(pszName, _T("minwidth")) == 0)
		{
			m_minsize.cx = _tcstol(pszValue, NULL, 10);
		}
		else if (_tcscmp(pszName, _T("minheight")) == 0)
		{
			m_minsize.cy = _tcstol(pszValue, NULL, 10);
		}
		else if (_tcscmp(pszName, _T("maxwidth")) == 0)
		{
			m_maxsize.cx = _tcstol(pszValue, NULL, 10);
		}
		else if (_tcscmp(pszName, _T("maxheight")) == 0)
		{
			m_maxsize.cy = _tcstol(pszValue, NULL, 10);
		}
		else if (_tcscmp(pszName, _T("offset")) == 0)
		{
			Markup::fooMarkupParsePoint(&m_ptOffset, pszValue);
		}

		return true;
	}

	/*static*/ FStyleObject* FStyleObject::createStyle(LPCTSTR pszName)
	{
		FStyleObject *pStyleObject = NULL;

		/** 加do{}while(0)是为了防止漏掉FSTYLE_END_MAP() */
#define FSTYLE_BEGIN_MAP() \
		do \
		{ \
			if (false) \
			{ \
			}
#define FSTYLE_HANDLER(_foosc, _class) \
			else if (_tcscmp(pszName, _foosc) == 0) \
			{ \
				pStyleObject = new _class; \
			}
#define FSTYLE_END_MAP() \
			else \
			{ \
				FASSERT(FALSE); \
				return NULL; \
			} \
		} while (0);

		// 
		FSTYLE_BEGIN_MAP()
			FSTYLE_HANDLER(FOOSC_INCLUDE, FIncludeStyle)
			FSTYLE_HANDLER(FOOSC_COLOR, FColorStyle)
			FSTYLE_HANDLER(FOOSC_IMAGE, FImageStyle)
			FSTYLE_HANDLER(FOOSC_TEXT, FTextStyle)
			FSTYLE_HANDLER(FOOSC_STRING, FStringStyle)
			FSTYLE_HANDLER(FOOSC_FOCUSRECT, FFocusRectStyle)
			FSTYLE_HANDLER(FOOSC_WINDOWREGION, FWindowRegionStyle)
		FSTYLE_END_MAP()

		pStyleObject->autoDelete(true);
		return pStyleObject;
	}

	void FStyleObject::calcRect(Core::FRect& rcDraw, Core::FRect& rcClip, const Core::FRect& rcDest) const
	{
		Core::FRect rcClient = rcDest;
		rcClient.deflateRect(m_egMargin);

		rcClip = rcClient;
		rcDraw = rcClient;

		/** 计算大小 */
		int nWidth = (m_size.cx > 0) ? m_size.cx : rcDraw.width();
		int nHeight = (m_size.cy > 0) ? m_size.cy : rcDraw.height();
		if (m_minsize.cx > 0 && nWidth < m_minsize.cx) nWidth = m_minsize.cx;
		if (m_minsize.cy > 0 && nHeight < m_minsize.cy) nHeight = m_minsize.cy;
		if (m_maxsize.cx > 0 && nWidth > m_maxsize.cx) nWidth = m_maxsize.cx;
		if (m_maxsize.cy > 0 && nHeight > m_maxsize.cy) nHeight = m_maxsize.cy;

		/** 计算水平位置 */
		if (fooTestAttribute(m_dwAlign, FOOALIGN_XCENTER))
		{
			rcDraw.left = rcClient.left + (rcClient.width() - nWidth) / 2;
			rcDraw.right = rcDraw.left + nWidth;
		}
		else if (fooTestAttribute(m_dwAlign, FOOALIGN_RIGHT))
		{
			rcDraw.left = rcClient.right - nWidth;
			rcDraw.right = rcDraw.left + nWidth;
		}
		else
		{
			rcDraw.right = rcDraw.left + nWidth;
		}

		/** 计算垂直位置 */
		if (fooTestAttribute(m_dwAlign, FOOALIGN_YCENTER))
		{
			rcDraw.top = rcClient.top + (rcClient.height() - nHeight) / 2;
			rcDraw.bottom = rcDraw.top + nHeight;
		}
		else if (fooTestAttribute(m_dwAlign, FOOALIGN_BOTTOM))
		{
			rcDraw.top = rcClient.bottom - nHeight;
			rcDraw.bottom = rcDraw.top + nHeight;
		}
		else
		{
			rcDraw.bottom = rcDraw.top + nHeight;
		}

		rcDraw.deflateRect(m_egPadding);
		rcClip.deflateRect(m_egPadding);

		rcDraw.offset(m_ptOffset);
	}

}} // !namespace