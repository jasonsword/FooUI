/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		IPatting_p.h
* 描述：		IPatting_p
* 创建日期：	2013-06-20
*
*/

#ifndef IPATTING_P_H
#define IPATTING_P_H

#include "core/FObject.h"
#include "painting/FGraphics.h"

namespace FooUI { namespace Painting {

	class IPainting : public Core::FReferenceObject, public Core::FNoncopyableObject
	{
	};

	//////////////////////////////////////////////////////////////////////////
	// IFont
	class IFont : public IPainting
	{
	public:
		virtual bool create(void) = 0;
		virtual bool create(const LOGFONT* lplf) = 0;
		virtual bool create(LPCTSTR pszFaceName, int nSize, bool bBold, bool bItalic, bool bUnderLine, bool bStrickeOut) = 0;
		virtual bool copy(HFONT hFont) = 0;
		virtual bool destroy(void) = 0;
		virtual bool isNull(void) const = 0;
		virtual LPVOID font(void) const = 0;
		virtual HFONT handle(void) const = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	// IImage
	class IImage : public IPainting
	{
	public:
		virtual bool load(LPCTSTR pszFileName) = 0;
		virtual bool load(void* pStream, DWORD dwSize) = 0;
		virtual LPVOID getImage(void) const = 0;
		virtual int width(void) const = 0;
		virtual int height(void) const = 0;
		virtual bool isNull(void) const = 0;
		virtual bool destroy(void) = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	// IGraphics
	class IGraphics : public IPainting
	{
	public:
		virtual void setLayered(bool bLayered) = 0;
		virtual bool isLayered(void) const = 0;

	public:
		virtual HDC getSafeHdc(void) const= 0;
		virtual LPVOID getGraphics(void) const= 0;
		virtual int width(void) const= 0;
		virtual int height(void) const= 0;
		virtual Core::FSize size(void) const= 0;
		virtual bool resize(int nWidth, int nHeight)= 0;
		virtual int save(void)= 0;
		virtual bool restore(void)= 0;
		virtual bool restore(int nSave)= 0;
		virtual bool flush(void)= 0;
		virtual bool clipRect(const Core::FRect& rect, FGraphics::eClipMode mode)= 0;
		virtual bool clearClip(void)= 0;
		virtual bool clear(void)= 0;
		virtual bool clear(const Core::FRect& rect)= 0;
		virtual bool update(HDC hDestDC, const Core::FRect& rect) const = 0;
		virtual bool updateLayered(HWND hWnd, const Core::FRect& rect) const = 0;

	public:
		virtual void setRenderMode(FGraphics::eRenderMode mode)= 0;
		virtual FGraphics::eRenderMode getRenderMode(void) const= 0;

		virtual void setForeColor(const FColor& clr)= 0;
		virtual const FColor& getForeColor(void) const= 0;

		virtual void setBackColor(const FColor& clr)= 0;
		virtual const FColor& getBackColor(void) const= 0;

		virtual void setFont(const FFont& font)= 0;
		virtual const FFont& font(void) const= 0;

	public:
		virtual bool drawRect(const Core::FRect& rect, const FPaint* paint)= 0;
		virtual bool drawCircle(int xLeft, int yTop, int radius, const FPaint* paint)= 0;
		virtual bool drawCircle(const Core::FRect& rect, const FPaint* paint)= 0;
		virtual bool drawEllipse(Core::FRect& rect, const FPaint* paint)= 0;
		virtual bool drawLine(int x0, int y0, int x1, int y1, const FPaint* paint)= 0;
		virtual bool drawImage(const FImage& image, int xLeft, int yTop, const FPaint* paint)= 0;
		virtual bool drawImage(const FImage& image, const Core::FRect& rcDest, const FPaint* paint)= 0;
		virtual bool drawImage(const FImage& image, const Core::FRect& rcSource, const Core::FRect& rcDest, const FPaint* paint)= 0;
		virtual bool draw9PatchImage(const FImage& image, const Core::FRect& rcDest, const Core::FEdge& egNinePatch, const FPaint* paint)= 0;
		virtual bool draw9PatchImage(const FImage& image, const Core::FRect& rcSource, const Core::FRect& rcDest, const Core::FEdge& egNinePatch, const FPaint* paint)= 0;
		virtual bool drawText(LPCTSTR pString, int nCount, Core::FRect& rect, UINT uFormat, const FPaint* paint)= 0;
		virtual bool drawFocusRect(const Core::FRect& rect) = 0;
	};

}} // !namespace

#endif // !IPATTING_P_H
