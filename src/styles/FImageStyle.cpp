/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FImageStyle.cpp
* 描述：		FImageStyle
* 创建日期：	2013-06-26
*
*/

#include "StdAfx.h"
#include "core/FObject.h"
#include "markup/FImageResource.h"
#include "markup/FAttributeScript.h"
#include "styles/FImageStyle.h"

#define FIMAGESTYLE_INLINE FOOINLINE

namespace FooUI { namespace Styles {

	//////////////////////////////////////////////////////////////////////////
	// FImageStylePrivate
	class FImageStyle::FImageStylePrivate : public Core::FReferenceObject, public Core::FNoncopyableObject
	{
	public:
		FImageStylePrivate(void);

	public:
		Markup::FImageResource m_resImage;
		FImageStyle::eFImageStyleFillStyle m_fillStyle;
		Painting::FPaint m_paint;
	};

	// 
	FImageStyle::FImageStylePrivate::FImageStylePrivate(void) : 
		m_fillStyle(FImageStyle::Stretch)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	// FImageStyle
	FIMAGESTYLE_INLINE FImageStyle::FImageStyle(void)
	{
		m_imageStylePrivate = new FImageStylePrivate;
		m_imageStylePrivate->addRef();
	}

	FIMAGESTYLE_INLINE FImageStyle::FImageStyle(const FImageStyle& style)
	{
		FASSERT(this != &style);
		m_imageStylePrivate = style.m_imageStylePrivate;
		m_imageStylePrivate->addRef();
	}

	FIMAGESTYLE_INLINE FImageStyle& FImageStyle::operator=(const FImageStyle& style)
	{
		if (m_imageStylePrivate != style.m_imageStylePrivate)
		{
			m_imageStylePrivate->release();
			m_imageStylePrivate = style.m_imageStylePrivate;
			m_imageStylePrivate->addRef();
		}

		return (*this);
	}

	FIMAGESTYLE_INLINE FImageStyle::~FImageStyle(void)
	{
		FASSERT(NULL != m_imageStylePrivate);
		m_imageStylePrivate->release();
		m_imageStylePrivate = NULL;
	}

	FIMAGESTYLE_INLINE bool FImageStyle::draw(Widgets::FWidget* pWidget, /** 控件对象 */
											Painting::FGraphics* pGraphics, /** 画布 */
											const Core::FRect* prcUpdate, /** 更新区域 */
											DWORD dwState, /** 状态 */
											const Core::FRect& rcDest, /** 绘图区域 */
											LPCTSTR pszType/* = NULL*/, 
											LPCTSTR pszPart/* = NULL*/, 
											FSTYLESTRUCT* fss/* = NULL*/)
	{
		FASSERT(NULL != m_imageStylePrivate);

		switch (m_imageStylePrivate->m_fillStyle)
		{
		case FImageStyle::Stretch:
			return drawStretch(pWidget, pGraphics, prcUpdate, dwState, rcDest, pszType, pszPart, fss);
			break;

		case FImageStyle::Tile:
			return drawTile(pWidget, pGraphics, prcUpdate, dwState, rcDest, pszType, pszPart, fss);
			break;

		case FImageStyle::HTile:
			return drawHTile(pWidget, pGraphics, prcUpdate, dwState, rcDest, pszType, pszPart, fss);
			break;

		case FImageStyle::VTile:
			return drawVTile(pWidget, pGraphics, prcUpdate, dwState, rcDest, pszType, pszPart, fss);
			break;

		default:
			break;
		}

		return drawNone(pWidget, pGraphics, prcUpdate, dwState, rcDest, pszType, pszPart, fss);
	}

	FIMAGESTYLE_INLINE bool FImageStyle::drawNone(Widgets::FWidget* pWidget, /** 控件对象 */
										Painting::FGraphics* pGraphics, /** 画布 */
										const Core::FRect* prcUpdate, /** 更新区域 */
										DWORD dwState, /** 状态 */
										const Core::FRect& rcDest, /** 绘图区域 */
										LPCTSTR pszType/* = NULL*/, 
										LPCTSTR pszPart/* = NULL*/, 
										FSTYLESTRUCT* fss/* = NULL*/)
	{
		return drawStretch(pWidget, pGraphics, prcUpdate, dwState, rcDest, pszType, pszPart, fss);
	}

	FIMAGESTYLE_INLINE bool FImageStyle::drawStretch(Widgets::FWidget* pWidget, /** 控件对象 */
												Painting::FGraphics* pGraphics, /** 画布 */
												const Core::FRect* prcUpdate, /** 更新区域 */
												DWORD dwState, /** 状态 */
												const Core::FRect& rcDest, /** 绘图区域 */
												LPCTSTR pszType/* = NULL*/, 
												LPCTSTR pszPart/* = NULL*/, 
												FSTYLESTRUCT* fss/* = NULL*/)
	{
		FASSERT(NULL != m_imageStylePrivate);

		if (!fooTestAttribute(dwState, getState())) return false;
		if (NULL != pszType && _tcscmp(pszType, getType()) != 0) return false;
		if (NULL != pszPart && _tcscmp(pszPart, getPart()) != 0) return false;

		Markup::FImageResource& resImage = m_imageStylePrivate->m_resImage;
		if (resImage.isNull())
		{
			return false;
		}

		Core::FRect rcDraw;
		Core::FRect rcClip;
		calcRect(rcDraw, rcClip, rcDest);
		if (rcDraw.isEmpty() || rcClip.isEmpty())
		{
			return false;
		}

		if (!rcClip.isIntersects(rcDraw))
		{
			return false;
		}

		/** 不在脏区域内 */
		if (NULL != prcUpdate 
			&& !prcUpdate->isIntersects(rcDraw))
		{
			return false;
		}

		Core::FRect rcSource = resImage.getSource();
		if (rcSource.isEmpty())
		{
			rcSource.setRect(0, 0, 
				resImage.getImage().width(), 
				resImage.getImage().height());
		}

		Painting::FAutoClipRect autoclip(pGraphics, rcClip);
		return pGraphics->draw9PatchImage(resImage.getImage(), rcSource, 
							rcDraw, resImage.getNPatch(), &m_imageStylePrivate->m_paint);
	}


	FIMAGESTYLE_INLINE bool FImageStyle::drawTile(Widgets::FWidget* pWidget, /** 控件对象 */
												Painting::FGraphics* pGraphics, /** 画布 */
												const Core::FRect* prcUpdate, /** 更新区域 */
												DWORD dwState, /** 状态 */
												const Core::FRect& rcDest, /** 绘图区域 */
												LPCTSTR pszType/* = NULL*/, 
												LPCTSTR pszPart/* = NULL*/, 
												FSTYLESTRUCT* fss/* = NULL*/)
	{
		FASSERT(NULL != m_imageStylePrivate);

		if (!fooTestAttribute(dwState, getState())) return false;
		if (NULL != pszType && _tcscmp(pszType, getType()) != 0) return false;
		if (NULL != pszPart && _tcscmp(pszPart, getPart()) != 0) return false;

		Markup::FImageResource& resImage = m_imageStylePrivate->m_resImage;
		if (resImage.isNull())
		{
			return false;
		}

		Core::FRect rcDraw;
		Core::FRect rcClip;
		calcRect(rcDraw, rcClip, rcDest);
		if (rcDraw.isEmpty() || rcClip.isEmpty())
		{
			return false;
		}

		if (!rcClip.isIntersects(rcDraw))
		{
			return false;
		}

		/** 不在脏区域内 */
		if (NULL != prcUpdate 
			&& !prcUpdate->isIntersects(rcDraw))
		{
			return false;
		}

		Core::FRect rcSource = resImage.getSource();
		if (rcSource.isEmpty())
		{
			rcSource.setRect(0, 0, 
				resImage.getImage().width(), 
				resImage.getImage().height());
		}

		Painting::FAutoClipRect autoclip(pGraphics, rcClip);
		bool bRet = false;
		for (int yPos = rcDraw.top; yPos < rcDraw.bottom; yPos += rcSource.height())
		{
			for (int xPos = rcDraw.left; xPos < rcDraw.right; xPos += rcSource.width())
			{
				Core::FRect rcFill(xPos, yPos, xPos	+ rcSource.width(), yPos + rcSource.height());
				if (!rcClip.isIntersects(rcFill) || (NULL != prcUpdate && !prcUpdate->isIntersects(rcFill)))
				{
					continue;
				}
				
				if (pGraphics->draw9PatchImage(resImage.getImage(), rcSource, 
								rcFill, resImage.getNPatch(), &m_imageStylePrivate->m_paint))
				{
					bRet = true;
				}
			}
		}

		return bRet;
	}


	FIMAGESTYLE_INLINE bool FImageStyle::drawHTile(Widgets::FWidget* pWidget, /** 控件对象 */
												Painting::FGraphics* pGraphics, /** 画布 */
												const Core::FRect* prcUpdate, /** 更新区域 */
												DWORD dwState, /** 状态 */
												const Core::FRect& rcDest, /** 绘图区域 */
												LPCTSTR pszType/* = NULL*/, 
												LPCTSTR pszPart/* = NULL*/, 
												FSTYLESTRUCT* fss/* = NULL*/)
	{
		FASSERT(NULL != m_imageStylePrivate);

		if (!fooTestAttribute(dwState, getState())) return false;
		if (NULL != pszType && _tcscmp(pszType, getType()) != 0) return false;
		if (NULL != pszPart && _tcscmp(pszPart, getPart()) != 0) return false;

		Markup::FImageResource& resImage = m_imageStylePrivate->m_resImage;
		if (resImage.isNull())
		{
			return false;
		}

		Core::FRect rcDraw;
		Core::FRect rcClip;
		calcRect(rcDraw, rcClip, rcDest);
		if (rcDraw.isEmpty() || rcClip.isEmpty())
		{
			return false;
		}

		if (!rcClip.isIntersects(rcDraw))
		{
			return false;
		}

		/** 不在脏区域内 */
		if (NULL != prcUpdate 
			&& !prcUpdate->isIntersects(rcDraw))
		{
			return false;
		}

		Core::FRect rcSource = resImage.getSource();
		if (rcSource.isEmpty())
		{
			rcSource.setRect(0, 0, 
				resImage.getImage().width(), 
				resImage.getImage().height());
		}

		Painting::FAutoClipRect autoclip(pGraphics, rcClip);
		bool bRet = false;
		for (int xPos = rcDraw.left; xPos < rcDraw.right; xPos += rcSource.width())
		{
			Core::FRect rcFill(xPos, rcDraw.top, xPos + rcSource.width(), rcDraw.bottom);
			if (!rcClip.isIntersects(rcFill) || (NULL != prcUpdate && !prcUpdate->isIntersects(rcFill)))
			{
				continue;
			}

			if (pGraphics->draw9PatchImage(resImage.getImage(), rcSource, 
							rcFill, resImage.getNPatch(), &m_imageStylePrivate->m_paint))
			{
				bRet = true;
			}
		}

		return bRet;
	}


	bool FImageStyle::drawVTile(Widgets::FWidget* pWidget, /** 控件对象 */
								Painting::FGraphics* pGraphics, /** 画布 */
								const Core::FRect* prcUpdate, /** 更新区域 */
								DWORD dwState, /** 状态 */
								const Core::FRect& rcDest, /** 绘图区域 */
								LPCTSTR pszType/* = NULL*/, 
								LPCTSTR pszPart/* = NULL*/, 
								FSTYLESTRUCT* fss/* = NULL*/)
	{
		FASSERT(NULL != m_imageStylePrivate);

		if (!fooTestAttribute(dwState, getState())) return false;
		if (NULL != pszType && _tcscmp(pszType, getType()) != 0) return false;
		if (NULL != pszPart && _tcscmp(pszPart, getPart()) != 0) return false;

		Markup::FImageResource& resImage = m_imageStylePrivate->m_resImage;
		if (resImage.isNull())
		{
			return false;
		}

		Core::FRect rcDraw;
		Core::FRect rcClip;
		calcRect(rcDraw, rcClip, rcDest);
		if (rcDraw.isEmpty() || rcClip.isEmpty())
		{
			return false;
		}

		if (!rcClip.isIntersects(rcDraw))
		{
			return false;
		}

		/** 不在脏区域内 */
		if (NULL != prcUpdate 
			&& !prcUpdate->isIntersects(rcDraw))
		{
			return false;
		}

		Core::FRect rcSource = resImage.getSource();
		if (rcSource.isEmpty())
		{
			rcSource.setRect(0, 0, 
				resImage.getImage().width(), 
				resImage.getImage().height());
		}

		Painting::FAutoClipRect autoclip(pGraphics, rcClip);
		bool bRet = false;
		for (int yPos = rcDraw.top; yPos < rcDraw.bottom; yPos += rcSource.height())
		{
			Core::FRect rcFill(rcDraw.left, yPos, rcDraw.right, yPos + rcSource.height());
			if (!rcClip.isIntersects(rcFill) || (NULL != prcUpdate && !prcUpdate->isIntersects(rcFill)))
			{
				continue;
			}

			if (pGraphics->draw9PatchImage(resImage.getImage(), rcSource, 
							rcFill, resImage.getNPatch(), &m_imageStylePrivate->m_paint))
			{
				bRet = true;
			}
		}

		return bRet;
	}

	FIMAGESTYLE_INLINE bool FImageStyle::setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		FASSERT(NULL != m_imageStylePrivate);
		FASSERT(NULL != pMarkup);
		FASSERT(NULL != pszName);
		FASSERT(NULL != pszValue);

		if (_tcscmp(pszName, _T("src")) == 0)
		{
			m_imageStylePrivate->m_resImage.parseAttribute(pMarkup, pszValue);
		}
		else if (_tcscmp(pszName, _T("fill")) == 0)
		{
			if (_tcscmp(pszValue, _T("stretch")) == 0)
				m_imageStylePrivate->m_fillStyle = FImageStyle::Stretch;
			else if (_tcscmp(pszValue, _T("tile")) == 0)
				m_imageStylePrivate->m_fillStyle = FImageStyle::Tile;
			else if (_tcscmp(pszValue, _T("htile")) == 0)
				m_imageStylePrivate->m_fillStyle = FImageStyle::HTile;
			else if (_tcscmp(pszValue, _T("vtile")) == 0)
				m_imageStylePrivate->m_fillStyle = FImageStyle::VTile;
			else
				m_imageStylePrivate->m_fillStyle = FImageStyle::None;
		}
		else if (_tcscmp(pszName, _T("alpha")) == 0)
		{
			m_imageStylePrivate->m_paint.setAlpha((unsigned char)min(255, _tcstoul(pszValue, NULL, 10)));
		}

		return __super::setAttribute(pMarkup, pszName, pszValue);
	}

}} // !namespace