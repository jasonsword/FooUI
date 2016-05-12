/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FGraphics.cpp
* 描述：		FGraphics
* 创建日期：	2013-06-06
*
*/

#include "StdAfx.h"

#include "painting/FGraphics.h"

#include "widgets/FWidget.h"

#include "F3rd_p.h"

#define FGRAPHICS_INLINE FOOINLINE

namespace FooUI { namespace Painting {

	//////////////////////////////////////////////////////////////////////////
	// FGraphics
	FGRAPHICS_INLINE FGraphics::FGraphics(void)
	{
		m_graphicsPrivate = new FGraphicsPrivate;
		m_graphicsPrivate->addRef();
	}

	FGRAPHICS_INLINE FGraphics::FGraphics(const FGraphics& graphics)
	{
		m_graphicsPrivate = graphics.m_graphicsPrivate;
		m_graphicsPrivate->addRef();
	}

	FGRAPHICS_INLINE FGraphics::FGraphics(int nWidth, int nHeight)
	{
		m_graphicsPrivate = new FGraphicsPrivate;
		m_graphicsPrivate->addRef();

		resize(nWidth, nHeight);
	}

	FGRAPHICS_INLINE FGraphics::FGraphics(const Core::FSize& initSize)
	{
		m_graphicsPrivate = new FGraphicsPrivate;
		m_graphicsPrivate->addRef();

		resize(initSize.cx, initSize.cy);
	}

	FGRAPHICS_INLINE FGraphics::FGraphics(const Core::FRect& initSize)
	{
		m_graphicsPrivate = new FGraphicsPrivate;
		m_graphicsPrivate->addRef();

		resize(initSize.width(), initSize.height());
	}

	FGRAPHICS_INLINE FGraphics& FGraphics::operator=(const FGraphics& graphics)
	{
		if (m_graphicsPrivate != graphics.m_graphicsPrivate)
		{
			m_graphicsPrivate->release();
			m_graphicsPrivate = graphics.m_graphicsPrivate;
			m_graphicsPrivate->addRef();
		}

		return (*this);
	}

	FGRAPHICS_INLINE FGraphics::~FGraphics(void)
	{
		FASSERT(NULL != m_graphicsPrivate);
		m_graphicsPrivate->release();
		m_graphicsPrivate = NULL;
	}

	FGRAPHICS_INLINE HDC FGraphics::getSafeHdc(void) const
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->getSafeHdc();
	}

	FGRAPHICS_INLINE LPVOID FGraphics::getGraphics(void) const
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->getGraphics();
	}

	FGRAPHICS_INLINE int FGraphics::width(void) const
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->width();
	}

	FGRAPHICS_INLINE int FGraphics::height(void) const
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->height();
	}

	FGRAPHICS_INLINE Core::FSize FGraphics::size(void) const
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->size();
	}

	FGRAPHICS_INLINE bool FGraphics::resize(int nWidth, int nHeight)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->resize(nWidth, nHeight);
	}

	FGRAPHICS_INLINE int FGraphics::save(void)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->save();
	}

	FGRAPHICS_INLINE bool FGraphics::restore(void)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->restore();
	}

	FGRAPHICS_INLINE bool FGraphics::restore(int nSave)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->restore(nSave);
	}

	FGRAPHICS_INLINE bool FGraphics::flush(void)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->flush();
	}

	FGRAPHICS_INLINE bool FGraphics::clipRect(const Core::FRect& rect, eClipMode mode)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->clipRect(rect, mode);
	}

	FGRAPHICS_INLINE bool FGraphics::clearClip(void)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->clearClip();
	}

	FGRAPHICS_INLINE int FGraphics::pushClipRect(const Core::FRect& rect, eClipMode mode)
	{
		FASSERT(NULL != m_graphicsPrivate);
		int nSave = m_graphicsPrivate->save();
		m_graphicsPrivate->clipRect(rect, mode);
		return nSave;
	}

	FGRAPHICS_INLINE bool FGraphics::popClipRect(int nPush)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->restore(nPush);
	}

	FGRAPHICS_INLINE bool FGraphics::popClipRect(void)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->restore();
	}

	FGRAPHICS_INLINE bool FGraphics::clear(void)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->clear();
	}

	FGRAPHICS_INLINE bool FGraphics::clear(const Core::FRect& rect)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->clear(rect);
	}

	FGRAPHICS_INLINE bool FGraphics::update(HDC hDestDC, const Core::FRect& rect) const
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->update(hDestDC, rect);
	}

	FGRAPHICS_INLINE bool FGraphics::updateLayered(HWND hWnd, const Core::FRect& rect) const
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->updateLayered(hWnd, rect);
	}

	FGRAPHICS_INLINE void FGraphics::setRenderMode(eRenderMode mode)
	{
		FASSERT(NULL != m_graphicsPrivate);
		m_graphicsPrivate->setRenderMode(mode);
	}

	FGRAPHICS_INLINE FGraphics::eRenderMode FGraphics::getRenderMode(void) const
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->getRenderMode();
	}

	FGRAPHICS_INLINE void FGraphics::setForeColor(const FColor& clr)
	{
		FASSERT(NULL != m_graphicsPrivate);
		m_graphicsPrivate->setForeColor(clr);
	}

	FGRAPHICS_INLINE const FColor& FGraphics::getForeColor(void) const
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->getForeColor();
	}

	FGRAPHICS_INLINE void FGraphics::setBackColor(const FColor& clr)
	{
		FASSERT(NULL != m_graphicsPrivate);
		m_graphicsPrivate->setBackColor(clr);
	}

	FGRAPHICS_INLINE const FColor& FGraphics::getBackColor(void) const
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->getBackColor();
	}

	FGRAPHICS_INLINE void FGraphics::setFont(const FFont& font)
	{
		FASSERT(NULL != m_graphicsPrivate);
		m_graphicsPrivate->setFont(font);
	}

	FGRAPHICS_INLINE const FFont& FGraphics::font(void) const
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->font();
	}

	FGRAPHICS_INLINE bool FGraphics::drawRect(const Core::FRect& rect, const FPaint* paint)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->drawRect(rect, paint);
	}

	FGRAPHICS_INLINE bool FGraphics::drawCircle(int xLeft, int yTop, int radius, const FPaint* paint)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->drawCircle(xLeft, yTop, radius, paint);
	}

	FGRAPHICS_INLINE bool FGraphics::drawCircle(const Core::FRect& rect, const FPaint* paint)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->drawCircle(rect, paint);
	}

	FGRAPHICS_INLINE bool FGraphics::drawEllipse(Core::FRect& rect, const FPaint* paint)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->drawEllipse(rect, paint);
	}

	FGRAPHICS_INLINE bool FGraphics::drawLine(int x0, int y0, int x1, int y1, const FPaint* paint)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->drawLine(x0, y0, x1, y1, paint);
	}

	FGRAPHICS_INLINE bool FGraphics::drawImage(const FImage& image, int xLeft, int yTop, const FPaint* paint)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->drawImage(image, xLeft, yTop, paint);
	}

	FGRAPHICS_INLINE bool FGraphics::drawImage(const FImage& image, const Core::FRect& rcDest, const FPaint* paint)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->drawImage(image, rcDest, paint);
	}

	FGRAPHICS_INLINE bool FGraphics::drawImage(const FImage& image, const Core::FRect& rcSource, const Core::FRect& rcDest, const FPaint* paint)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->drawImage(image, rcSource, rcDest, paint);
	}

	FGRAPHICS_INLINE bool FGraphics::draw9PatchImage(const FImage& image, const Core::FRect& rcDest, const Core::FEdge& egNinePatch, const FPaint* paint)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->draw9PatchImage(image, rcDest, egNinePatch, paint);
	}

	FGRAPHICS_INLINE bool FGraphics::draw9PatchImage(const FImage& image, const Core::FRect& rcSource, const Core::FRect& rcDest, const Core::FEdge& egNinePatch, const FPaint* paint)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->draw9PatchImage(image, rcSource, rcDest, egNinePatch, paint);
	}

	FGRAPHICS_INLINE bool FGraphics::drawText(LPCTSTR pString, int nCount, Core::FRect& rect, UINT uFormat, const FPaint* paint)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->drawText(pString, nCount, rect, uFormat, paint);
	}

	FGRAPHICS_INLINE bool FGraphics::drawFocusRect(const Core::FRect& rect)
	{
		FASSERT(NULL != m_graphicsPrivate);
		return m_graphicsPrivate->drawFocusRect(rect);
	}

	//////////////////////////////////////////////////////////////////////////
	// FAutoClipRect
	FGRAPHICS_INLINE FAutoClipRect::FAutoClipRect(FGraphics* pGraphics)
	{
		FASSERT(NULL != pGraphics);
		m_pGraphics = pGraphics;
		m_nSave = m_pGraphics->save();
	}

	FGRAPHICS_INLINE FAutoClipRect::FAutoClipRect(FGraphics* pGraphics, const Core::FRect& rcClip, FGraphics::eClipMode mode)
	{
		FASSERT(NULL != pGraphics);
		m_pGraphics = pGraphics;
		m_nSave = m_pGraphics->save();
		m_pGraphics->clipRect(rcClip, mode);
	}

	FGRAPHICS_INLINE FAutoClipRect::~FAutoClipRect(void)
	{
		FASSERT(NULL != m_pGraphics);
		FASSERT(0 != m_nSave);

		m_pGraphics->restore(m_nSave);
	}

	FGRAPHICS_INLINE bool FAutoClipRect::clipRect(const Core::FRect& rect, FGraphics::eClipMode mode)
	{
		FASSERT(NULL != m_pGraphics);
		return m_pGraphics->clipRect(rect, mode);
	}

}} // !namespace