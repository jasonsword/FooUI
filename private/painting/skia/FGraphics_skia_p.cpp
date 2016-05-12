/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FGraphics_skia_p.cpp
* 描述：		FGraphics_skia_p
* 创建日期：	2013-06-13
*
*/

#include "StdAfx.h"
#include "painting/skia/FGraphics_skia_p.h"

#ifdef FOOUI_SUPPORT_SKIA
namespace FooUI { namespace Painting {

	//////////////////////////////////////////////////////////////////////////
	// FGraphicsPrivate
	FGraphics::FGraphicsPrivate::FGraphicsPrivate(void) : 
		m_pBitmapDevice(NULL) 
		, m_pCanvas(NULL) 
		, m_hDC(NULL) 
		, m_hBitmap(NULL) 
		, m_nSaveDC(0) 
		, m_renderMode(FGraphics::normal) 
		, m_bLayered(false)
	{
		create();
	}

	FGraphics::FGraphicsPrivate::~FGraphicsPrivate(void)
	{
		destroy();
	}

	HDC FGraphics::FGraphicsPrivate::getSafeHdc(void) const
	{
		return m_hDC;
	}

	LPVOID FGraphics::FGraphicsPrivate::getGraphics(void) const
	{
		return m_pCanvas;
	}

	int FGraphics::FGraphicsPrivate::width(void) const
	{
		return m_size.cx;
	}

	int FGraphics::FGraphicsPrivate::height(void) const
	{
		return m_size.cy;
	}

	Core::FSize FGraphics::FGraphicsPrivate::size(void) const
	{
		return m_size;
	}

	bool FGraphics::FGraphicsPrivate::resize(int nWidth, int nHeight)
	{
		fooSafeDeleteObject(m_hBitmap);
		fooSafeDelete(m_pCanvas);

		BITMAPINFO bmi;
		memset(&bmi, 0, sizeof(bmi));
		bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth       = nWidth;
		bmi.bmiHeader.biHeight      = -nHeight; 
		bmi.bmiHeader.biPlanes      = 1;
		bmi.bmiHeader.biBitCount    = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage   = 0;

		void *pPixel = NULL;
		m_hBitmap = ::CreateDIBSection(m_hDC, &bmi, DIB_RGB_COLORS, &pPixel, NULL, 0);
		::SelectObject(m_hDC, m_hBitmap);

		m_pBitmapDevice->setConfig(SkBitmap::kARGB_8888_Config, nWidth, nHeight);
		m_pBitmapDevice->setIsOpaque(false);
		m_pBitmapDevice->setPixels(pPixel);

		m_size.cx = nWidth;
		m_size.cy = nHeight;

		m_pCanvas = new SkCanvas(*m_pBitmapDevice);
		m_pCanvas->clear(0x00FFFFFF);

		return true;
	}

	int FGraphics::FGraphicsPrivate::save(void)
	{
		FASSERT(NULL != m_pCanvas);
		return m_pCanvas->save();
	}

	bool FGraphics::FGraphicsPrivate::restore(void)
	{
		FASSERT(NULL != m_pCanvas);
		m_pCanvas->restore();
		return true;
	}

	bool FGraphics::FGraphicsPrivate::restore(int nSave)
	{
		FASSERT(NULL != m_pCanvas);
		if (nSave < m_pCanvas->getSaveCount())
		{
			m_pCanvas->restoreToCount(nSave);
			return true;
		}

		return false;
	}

	bool FGraphics::FGraphicsPrivate::flush(void)
	{
		FASSERT(NULL != m_pCanvas);
		m_pCanvas->flush();
		return true;
	}

	bool FGraphics::FGraphicsPrivate::clipRect(const Core::FRect& rect, eClipMode mode)
	{
		FASSERT(NULL != m_pCanvas);

		SkRegion::Op op = SkRegion::kIntersect_Op;
		if (mode == clip_replace) op = SkRegion::kReplace_Op;
		else if (mode == clip_difference) op = SkRegion::kDifference_Op;
		else if (mode == clip_union) op = SkRegion::kUnion_Op;
		else if (mode == clip_xor) op = SkRegion::kXOR_Op;
		else op = SkRegion::kIntersect_Op;

		return m_pCanvas->clipRect(SkRect::MakeLTRB(
												(SkScalar)rect.left, 
												(SkScalar)rect.top, 
												(SkScalar)rect.right, 
												(SkScalar)rect.bottom), op);
	}

	bool FGraphics::FGraphicsPrivate::clearClip(void)
	{
		FASSERT(NULL != m_pCanvas);

		fooSafeDelete(m_pCanvas);

		m_pCanvas = new SkCanvas(*m_pBitmapDevice);

		return true;
	}

	bool FGraphics::FGraphicsPrivate::clear(void)
	{
		FASSERT(NULL != m_pCanvas);
		m_pCanvas->clear(0x00000000);
		return true;
	}

	bool FGraphics::FGraphicsPrivate::clear(const Core::FRect& rect)
	{
		FASSERT(NULL != m_pCanvas);

		SkPaint paint;
		paint.setColor(0x00000000);
		paint.setXfermodeMode(SkXfermode::kSrc_Mode);

		m_pCanvas->save();
		m_pCanvas->clipRect(SkRect::MakeLTRB(
											(SkScalar)rect.left, 
											(SkScalar)rect.top, 
											(SkScalar)rect.right, 
											(SkScalar)rect.bottom), SkRegion::kReplace_Op);
		m_pCanvas->drawPaint(paint);
		m_pCanvas->restore();

		return true;
	}

	bool FGraphics::FGraphicsPrivate::update(HDC hDestDC, const Core::FRect& rect) const
	{
		FASSERT(NULL != m_pBitmapDevice);

		BITMAPINFO bmi = {0};
		memset(&bmi, 0, sizeof(bmi));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = m_pBitmapDevice->width();
		bmi.bmiHeader.biHeight = -m_pBitmapDevice->height();
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = 0;

		FASSERT(m_pBitmapDevice->width() * m_pBitmapDevice->bytesPerPixel() == m_pBitmapDevice->rowBytes());

		m_pBitmapDevice->lockPixels();
		::SetDIBitsToDevice(hDestDC, 
			rect.left, 
			rect.top, 
			rect.width(), 
			rect.height(), 
			rect.left, 
			m_size.cy - rect.height(), 
			0, 
			m_pBitmapDevice->height(), 
			m_pBitmapDevice->getPixels(), 
			&bmi, DIB_RGB_COLORS);
		m_pBitmapDevice->unlockPixels();

		return true;
	}

	bool FGraphics::FGraphicsPrivate::updateLayered(HWND hWnd, const Core::FRect& rect) const
	{
		BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
		SIZE szWindow = {rect.width(), rect.height()};
#if (_WIN32_WINNT < 6000)
		::UpdateLayeredWindow(hWnd, 
			NULL, 
			NULL, 
			&szWindow, 
			m_hDC, 
			(POINT*)&rect, 
			0, 
			&blend, 
			ULW_ALPHA);
#else
		UPDATELAYEREDWINDOWINFO ulwInfo = {0};
		ulwInfo.cbSize = sizeof(UPDATELAYEREDWINDOWINFO);
		ulwInfo.dwFlags = ULW_ALPHA;
		ulwInfo.hdcSrc = m_hDC;
		ulwInfo.pblend = &blend;
		ulwInfo.psize = &szWindow;
		ulwInfo.pptSrc = (POINT*)&rect;
		ulwInfo.prcDirty = lpDstRect;
		::UpdateLayeredWindowIndirect(hWnd. &ulwinfo);
#endif
		return true;
	}

	void FGraphics::FGraphicsPrivate::setRenderMode(FGraphics::eRenderMode mode)
	{
		m_renderMode = mode;
	}

	FGraphics::eRenderMode FGraphics::FGraphicsPrivate::getRenderMode(void) const
	{
		return m_renderMode;
	}

	void FGraphics::FGraphicsPrivate::setForeColor(const FColor& clr)
	{
		m_clrFore = clr;
	}

	const FColor& FGraphics::FGraphicsPrivate::getForeColor(void) const
	{
		return m_clrFore;
	}

	void FGraphics::FGraphicsPrivate::setBackColor(const FColor& clr)
	{
		m_clrBack = clr;
	}

	const FColor& FGraphics::FGraphicsPrivate::getBackColor(void) const
	{
		return m_clrBack;
	}

	void FGraphics::FGraphicsPrivate::setFont(const FFont& font)
	{
		m_font = font;
	}

	const FFont& FGraphics::FGraphicsPrivate::font(void) const
	{
		return m_font;
	}

	bool FGraphics::FGraphicsPrivate::drawRect(const Core::FRect& rect, const FPaint* paint)
	{
		FASSERT(NULL != m_pCanvas);

		if (rect.isEmpty())
		{
			return false;
		}

		SkColor clr = SkColorSetARGBInline(m_clrBack.alpha(), m_clrBack.red(), m_clrBack.green(), m_clrBack.blue());
		m_pCanvas->save(SkCanvas::kClip_SaveFlag);
		m_pCanvas->clipRect(SkRect::MakeLTRB(
											(SkScalar)rect.left, 
											(SkScalar)rect.top, 
											(SkScalar)rect.right, 
											(SkScalar)rect.bottom
											), SkRegion::kIntersect_Op);
		m_pCanvas->drawColor(clr);
		m_pCanvas->restore();

		return true;
	}

	bool FGraphics::FGraphicsPrivate::drawCircle(int xLeft, int yTop, int radius, const FPaint* paint)
	{
		FASSERT(NULL != m_pCanvas);

		SkPaint paintCircle;
		paintCircle.setAntiAlias(getRenderMode() == FGraphics::quality);
		paintCircle.setDither(getRenderMode() == FGraphics::quality);
		paintCircle.setColor(SkColorSetARGBInline(m_clrBack.alpha(), m_clrBack.red(), m_clrBack.green(), m_clrBack.blue()));

		m_pCanvas->drawCircle((SkScalar)xLeft, (SkScalar)yTop, (SkScalar)radius, paintCircle);

		return true;
	}

	bool FGraphics::FGraphicsPrivate::drawCircle(const Core::FRect& rect, const FPaint* paint)
	{
		FASSERT(NULL != m_pCanvas);

		if (rect.isEmpty())
		{
			return false;
		}

		SkPaint paintCircle;
		paintCircle.setAntiAlias(getRenderMode() == FGraphics::quality);
		paintCircle.setDither(getRenderMode() == FGraphics::quality);
		paintCircle.setColor(SkColorSetARGBInline(m_clrBack.alpha(), m_clrBack.red(), m_clrBack.green(), m_clrBack.blue()));

		int radius = min(rect.width(), rect.height()) / 2;
		m_pCanvas->drawCircle((SkScalar)rect.left + rect.width() / 2, (SkScalar)rect.top + rect.height() / 2, (SkScalar)radius, paintCircle);

		return true;
	}

	bool FGraphics::FGraphicsPrivate::drawEllipse(Core::FRect& rect, const FPaint* paint)
	{
		FASSERT(NULL != m_pCanvas);

		if (rect.isEmpty())
		{
			return false;
		}

		SkPaint paintEllipse;
		paintEllipse.setAntiAlias(getRenderMode() == FGraphics::quality);
		paintEllipse.setDither(getRenderMode() == FGraphics::quality);
		paintEllipse.setColor(SkColorSetARGBInline(m_clrBack.alpha(), m_clrBack.red(), m_clrBack.green(), m_clrBack.blue()));
		m_pCanvas->drawOval(SkRect::MakeLTRB((SkScalar)rect.left, 
											(SkScalar)rect.top,
											(SkScalar)rect.right,
											(SkScalar)rect.bottom
											), paintEllipse);

		return false;
	}

	bool FGraphics::FGraphicsPrivate::drawLine(int x0, int y0, int x1, int y1, const FPaint* paint)
	{
		FASSERT(NULL != m_pCanvas);

		SkPaint paintLine;
		paintLine.setAntiAlias(getRenderMode() == FGraphics::quality);
		paintLine.setDither(getRenderMode() == FGraphics::quality);
		paintLine.setColor(SkColorSetARGBInline(m_clrBack.alpha(), m_clrBack.red(), m_clrBack.green(), m_clrBack.blue()));
		m_pCanvas->drawLine((SkScalar)x0, (SkScalar)y0, (SkScalar)x1, (SkScalar)y1, paintLine);

		return false;
	}

	bool FGraphics::FGraphicsPrivate::drawImage(const FImage& image, int xLeft, int yTop, const FPaint* paint)
	{
		FASSERT(NULL != m_pCanvas);
		return drawImage(image, Core::FRect(0, 0, image.width(), image.height()), 
						Core::FRect::makeXYWH(xLeft, yTop, image.width(), image.height()), paint);
	}

	bool FGraphics::FGraphicsPrivate::drawImage(const FImage& image, const Core::FRect& rcDest, const FPaint* paint)
	{
		FASSERT(NULL != m_pCanvas);
		return drawImage(image, Core::FRect(0, 0, image.width(), image.height()), rcDest, paint);
	}

	bool FGraphics::FGraphicsPrivate::drawImage(const FImage& image, const Core::FRect& rcSource, const Core::FRect& rcDest, const FPaint* paint)
	{
		FASSERT(NULL != m_pCanvas);
		FASSERT(!image.isNull());

		if (rcSource.isEmpty() || rcDest.isEmpty())
		{
			return false;
		}

		SkPaint paintImage;
		if (NULL != paint) paintImage.setAlpha(paint->getAlpha());
		m_pCanvas->save(SkCanvas::kClip_SaveFlag);
		m_pCanvas->clipRect(SkRect::MakeLTRB((SkScalar)rcDest.left, 
											(SkScalar)rcDest.top, 
											(SkScalar)rcDest.right, 
											(SkScalar)rcDest.bottom
											), SkRegion::kIntersect_Op);
		m_pCanvas->drawBitmapRect(*((SkBitmap*)image.getImage()), 
				&SkIRect::MakeLTRB(rcSource.left, rcSource.top, rcSource.right, rcSource.bottom), 
				SkRect::MakeLTRB((SkScalar)rcDest.left, (SkScalar)rcDest.top, (SkScalar)rcDest.right, (SkScalar)rcDest.bottom), 
				&paintImage);
		m_pCanvas->restore();
		return true;
	}

	bool FGraphics::FGraphicsPrivate::draw9PatchImage(const FImage& image, const Core::FRect& rcDest, const Core::FEdge& egNinePatch, const FPaint* paint)
	{
		FASSERT(NULL != m_pCanvas);
		return draw9PatchImage(image, Core::FRect(0, 0, image.width(), image.height()), rcDest, egNinePatch, paint);
	}

	bool FGraphics::FGraphicsPrivate::draw9PatchImage(const FImage& image, const Core::FRect& rcSource, const Core::FRect& rcDest, const Core::FEdge& egNinePatch, const FPaint* paint)
	{
		FASSERT(NULL != m_pCanvas);
		FASSERT(!image.isNull());

		enum eF9Patch
		{
			F9PATCH_MID = 0, 
			F9PATCH_TOPLEFT, 
			F9PATCH_TOP, 
			F9PATCH_TOPRIGHT, 
			F9PATCH_LEFT, 
			F9PATCH_RIGHT, 
			F9PATCH_BOTTOMLEFT, 
			F9PATCH_BOTTOM, 
			F9PATCH_BOTTOMRIGHT, 
		};

		if (egNinePatch.isZero())
		{
			return drawImage(image, rcSource, rcDest, paint);
		}
		else
		{
			Core::FRect rcPatchSrc[9];
			Core::FRect rcPatchDest[9];

			/** mid */
			if (egNinePatch.edgeWidth() < rcDest.width() 
				&& egNinePatch.edgeHeight() < rcDest.height())
			{
				rcPatchSrc[F9PATCH_MID].setRect(rcSource.left + egNinePatch.left, rcSource.top + egNinePatch.top, rcSource.right - egNinePatch.right, rcSource.bottom - egNinePatch.bottom);
				rcPatchDest[F9PATCH_MID].setRect(rcDest.left + egNinePatch.left, rcDest.top + egNinePatch.top, rcDest.right - egNinePatch.right, rcDest.bottom - egNinePatch.bottom);
			}

			/** top_left */
			rcPatchSrc[F9PATCH_TOPLEFT].setRect(rcSource.left, rcSource.top, rcSource.left + egNinePatch.left, rcSource.top + egNinePatch.top);
			rcPatchDest[F9PATCH_TOPLEFT].setRect(rcDest.left, rcDest.top, rcDest.left + egNinePatch.left, rcDest.top + egNinePatch.top);

			/** top */ 
			rcPatchSrc[F9PATCH_TOP].setRect(rcSource.left + egNinePatch.left, rcSource.top, rcSource.right - egNinePatch.right, rcSource.top + egNinePatch.top);
			rcPatchDest[F9PATCH_TOP].setRect(rcDest.left + egNinePatch.left, rcDest.top, rcDest.right - egNinePatch.right, rcDest.top + egNinePatch.top);


			/** left */
			rcPatchSrc[F9PATCH_LEFT].setRect(rcSource.left, rcSource.top + egNinePatch.top, rcSource.left + egNinePatch.left, rcSource.bottom - egNinePatch.bottom);
			rcPatchDest[F9PATCH_LEFT].setRect(rcDest.left, rcDest.top + egNinePatch.top, rcDest.left + egNinePatch.left, rcDest.bottom - egNinePatch.bottom);

			if (egNinePatch.left < rcDest.width())
			{
				/** right */
				rcPatchSrc[F9PATCH_RIGHT].setRect(rcSource.right - egNinePatch.right, rcSource.top + egNinePatch.top, rcSource.right, rcSource.bottom - egNinePatch.bottom);
				rcPatchDest[F9PATCH_RIGHT].setRect(rcDest.right - egNinePatch.right, rcDest.top + egNinePatch.top, rcDest.right, rcDest.bottom - egNinePatch.bottom);

				/** top_right */
				rcPatchSrc[F9PATCH_TOPRIGHT].setRect(rcSource.right - egNinePatch.right, rcSource.top, rcSource.right, rcSource.top + egNinePatch.top);
				rcPatchDest[F9PATCH_TOPRIGHT].setRect(rcDest.right - egNinePatch.right, rcDest.top, rcDest.right, rcDest.top + egNinePatch.top);
			}

			/** bottom_left */
			if (egNinePatch.top < rcDest.height())
			{
				rcPatchSrc[F9PATCH_BOTTOMLEFT].setRect(rcSource.left, rcSource.bottom - egNinePatch.bottom, rcSource.left + egNinePatch.left, rcSource.bottom);
				rcPatchDest[F9PATCH_BOTTOMLEFT].setRect(rcDest.left, rcDest.bottom - egNinePatch.bottom, rcDest.left + egNinePatch.left, rcDest.bottom);
			}

			if (egNinePatch.top < rcDest.height())
			{
				/** bottom */
				rcPatchSrc[F9PATCH_BOTTOM].setRect(rcSource.left + egNinePatch.left, rcSource.bottom - egNinePatch.bottom, rcSource.right - egNinePatch.right, rcSource.bottom);
				rcPatchDest[F9PATCH_BOTTOM].setRect(rcDest.left + egNinePatch.left, rcDest.bottom - egNinePatch.bottom, rcDest.right - egNinePatch.right, rcDest.bottom);

				/** bottom_right */
				rcPatchSrc[F9PATCH_BOTTOMRIGHT].setRect(rcSource.right - egNinePatch.right, rcSource.bottom - egNinePatch.bottom, rcSource.right, rcSource.bottom);
				rcPatchDest[F9PATCH_BOTTOMRIGHT].setRect(rcDest.right - egNinePatch.right, rcDest.bottom - egNinePatch.bottom, rcDest.right, rcDest.bottom);
			}

			for (int i = 0; i < 9; ++i)
			{
				drawImage(image, rcPatchSrc[i], rcPatchDest[i], paint);
			}

			return true;
		}

		return false;
	}

	bool FGraphics::FGraphicsPrivate::drawText(LPCTSTR pString, int nCount, Core::FRect& rect, UINT uFormat, const FPaint* paint)
	{
		FASSERT(NULL != m_pCanvas);

		/** 保存DC状态 */
		HDC hTextDC = getSafeHdc();
		int nDCState = ::SaveDC(hTextDC);

		/** 设置字体 */
		::SelectObject(hTextDC, (HFONT)m_font.font());

		/** 如果只是计算输出矩形 */
		if (fooTestAttribute(uFormat, DT_CALCRECT))
		{
			::DrawText(hTextDC, pString, nCount, &rect, uFormat);
			::RestoreDC(hTextDC, nDCState);
			return true;
		}

		/** clip绘图区域 */
		SkRect rcSkiaClipBounds;
		m_pCanvas->getClipBounds(&rcSkiaClipBounds);
		rcSkiaClipBounds.inset(1, 1); /** skia似乎返回的扩张了一个像素，所以收缩回去 */
		Core::FRect rcClipBounds(max((int)rcSkiaClipBounds.left(), rect.left), 
								max((int)rcSkiaClipBounds.top(), rect.top), 
								min((int)rcSkiaClipBounds.right(), rect.right), 
								min((int)rcSkiaClipBounds.bottom(), rect.bottom));
		HRGN hClipRgn = ::CreateRectRgn(rcClipBounds.left, rcClipBounds.top, rcClipBounds.right, rcClipBounds.bottom);
		::SelectClipRgn(hTextDC, hClipRgn);
		fooSafeDeleteObject(hClipRgn);

		/** 输出阴影 */
		if (NULL != paint 
			&& paint->isTextShadow())
		{
			Core::FRect rcTextShadow = rect;
			rcTextShadow.offset(paint->getTextShadowOffset().x, paint->getTextShadowOffset().y);
			::SetTextColor(hTextDC, paint->getTextShadowColor().toCOLORREF());
			::SetBkMode(hTextDC, TRANSPARENT);
			::DrawText(hTextDC, pString, nCount, &rcTextShadow, uFormat);
		}

		/** 设置文字颜色并输出 */
		::SetTextColor(hTextDC, m_clrFore.toCOLORREF());
		::SetBkMode(hTextDC, TRANSPARENT);

		Core::FRect rcCalc(0, 0, rect.width(), rect.height());
		if (isLayered())
		{
			/** 如果包含了DT_BOTTOM | DT_SINGLELINE计算的top无效，造成计算区域太大 */
			if (fooTestAttribute(uFormat, DT_SINGLELINE))
				::DrawText(hTextDC, pString, nCount, &rcCalc, DT_LEFT | DT_CALCRECT);
			else
				::DrawText(hTextDC, pString, nCount, &rcCalc, uFormat | DT_CALCRECT);
		}

		/** 输出文字 */
		::DrawText(hTextDC, pString, nCount, &rect, uFormat);

		/** 需要修正GDI::DrawText造成的alpha错误 */
		if (isLayered())
		{
			Core::FRect rcFix = rcCalc;
			calcGdiTextRect(rcFix, rect, rcCalc.right, rcCalc.bottom, uFormat);

			/** 将阴影部分计算进去 */
			if (NULL != paint 
				&& paint->isTextShadow())
			{
				if (paint->getTextShadowOffset().x < 0)
					rcFix.left += paint->getTextShadowOffset().x;
				else if (paint->getTextShadowOffset().x > 0)
					rcFix.right += paint->getTextShadowOffset().x;

				if (paint->getTextShadowOffset().y < 0)
					rcFix.top += paint->getTextShadowOffset().y;
				else if (paint->getTextShadowOffset().y > 0)
					rcFix.bottom += paint->getTextShadowOffset().y;
			}

			/** 将裁减区域计算进去，减少运算时间 */
			rcFix.left = max(rcFix.left, rcClipBounds.left);
			rcFix.top = max(rcFix.top, rcClipBounds.top);
			rcFix.right = min(rcFix.right, rcClipBounds.right);
			rcFix.bottom = min(rcFix.bottom, rcClipBounds.bottom);

			if (!rcFix.isEmpty())
			{
				fixAlpha(rcFix);
			}
		}

		::RestoreDC(hTextDC, nDCState);
		return true;
	}

	bool FGraphics::FGraphicsPrivate::drawFocusRect(const Core::FRect& rect)
	{
		FASSERT(NULL != m_pCanvas);

		/** 保存DC状态 */
		HDC hTextDC = getSafeHdc();
		int nDCState = ::SaveDC(hTextDC);

		/** clip绘图区域 */
		SkRect rcSkiaClipBounds;
		m_pCanvas->getClipBounds(&rcSkiaClipBounds);
		rcSkiaClipBounds.inset(1, 1); /** skia似乎返回的扩张了一个像素，所以收缩回去 */
		Core::FRect rcClipBounds(max((int)rcSkiaClipBounds.left(), rect.left), 
			max((int)rcSkiaClipBounds.top(), rect.top), 
			min((int)rcSkiaClipBounds.right(), rect.right), 
			min((int)rcSkiaClipBounds.bottom(), rect.bottom));
		HRGN hClipRgn = ::CreateRectRgn(rcClipBounds.left, rcClipBounds.top, rcClipBounds.right, rcClipBounds.bottom);
		::SelectClipRgn(hTextDC, hClipRgn);
		fooSafeDeleteObject(hClipRgn);

		/** 设置文字颜色并输出 */
		::SetTextColor(hTextDC, RGB(0, 0, 0));
		::SetBkMode(hTextDC, TRANSPARENT);

		BOOL bRet = ::DrawFocusRect(hTextDC, &rect);

		::RestoreDC(hTextDC, nDCState);

		return (TRUE == bRet);
	}

	bool FGraphics::FGraphicsPrivate::create(void)
	{
		if (NULL != m_pBitmapDevice 
			|| NULL != m_pCanvas 
			|| NULL != m_hDC 
			|| NULL != m_hBitmap)
		{
			FASSERT(false);
			return false;
		}

		m_pBitmapDevice = new SkBitmap;

		m_hDC = ::CreateCompatibleDC(NULL);
		m_nSaveDC = ::SaveDC(m_hDC);

		return true;
	}

	bool FGraphics::FGraphicsPrivate::destroy(void)
	{
		fooSafeDelete(m_pCanvas);
		fooSafeDelete(m_pBitmapDevice);

		if (NULL != m_hDC)
		{
			::RestoreDC(m_hDC, m_nSaveDC);
			::DeleteDC(m_hDC);

			m_hDC = NULL;
			m_nSaveDC = 0;
		}

		fooSafeDeleteObject(m_hBitmap);

		return true;
	}

	bool FGraphics::FGraphicsPrivate::fixAlpha(const Core::FRect& rcFix)
	{
		FASSERT(NULL != m_hDC);

		if (rcFix.isEmpty())
		{
			return false;
		}

		HDC hFixDC = ::CreateCompatibleDC(m_hDC);
		FASSERT(NULL != hFixDC);  /** 可能是GDI句柄泄露了！ */
		int nDCState = ::SaveDC(hFixDC);

		void* pFixDib = NULL;
		HBITMAP hFixBitmap = NULL;
		BITMAPINFO fixBitmap = {0};

		fixBitmap.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
		fixBitmap.bmiHeader.biWidth       = rcFix.width();
		fixBitmap.bmiHeader.biHeight      = -rcFix.height();
		fixBitmap.bmiHeader.biCompression = BI_RGB;
		fixBitmap.bmiHeader.biPlanes      = 1;
		fixBitmap.bmiHeader.biBitCount    = 32;
		hFixBitmap = ::CreateDIBSection(hFixDC, &fixBitmap, DIB_RGB_COLORS, &pFixDib, 0, 0);
		FASSERT(NULL != hFixBitmap);  /** 可能是GDI句柄泄露了！ */
		::SelectObject(hFixDC, hFixBitmap);

		::BitBlt(hFixDC, 0, 0, rcFix.width(), rcFix.height(), m_hDC,
			rcFix.left, rcFix.top, SRCCOPY);

		DWORD* pdwFixDib = (DWORD*)pFixDib;
		for(LONG i = rcFix.width() * rcFix.height() - 1; i >= 0; --i)
		{
			if (((BYTE*)(pdwFixDib + i))[3] == 0)
			{
				((BYTE*)(pdwFixDib + i))[3] = 0xFF;
			}
		}

		::SetDIBitsToDevice(m_hDC, 
			rcFix.left, 
			rcFix.top,
			rcFix.width(), 
			rcFix.height(), 
			0, 
			0, 
			0, 
			rcFix.height(),  
			pFixDib, 
			&fixBitmap, 
			DIB_RGB_COLORS);

		//////////////////////////////////////
		::RestoreDC(hFixDC, nDCState);
		fooSafeDeleteDC(hFixDC);
		fooSafeDeleteObject(hFixBitmap);

		return true;
	}

	void FGraphics::FGraphicsPrivate::calcGdiTextRect(Core::FRect& rcCalc, const Core::FRect& rcDest, 
													long lTextWidth, long lTextHeight, UINT uFormat) const
	{
		rcCalc = rcDest;

		/** 计算水平位置 */
		if (fooTestAttribute(uFormat, DT_CENTER))
			rcCalc.left = rcDest.left + (rcDest.width() - lTextWidth) / 2;
		else if (fooTestAttribute(uFormat, DT_RIGHT))
			rcCalc.left = rcDest.right - lTextWidth;

		/** 计算垂直位置 */
		if (fooTestAttribute(uFormat, DT_SINGLELINE))
		{
			if (fooTestAttribute(uFormat, DT_VCENTER))
				rcCalc.top = rcDest.top + (rcDest.height() - lTextHeight) / 2;
			else if (fooTestAttribute(uFormat, DT_BOTTOM))
				rcCalc.top = rcDest.bottom - lTextHeight;
		}

		rcCalc.right = rcCalc.left + lTextWidth;
		rcCalc.bottom = rcCalc.top + lTextHeight;

		/** 实际使用发现可能会存在一个像素的误差，扩张一个像素计算 */
		rcCalc.inflateRect(1, 1);
	}

}} // !namespace
#endif // !FOOUI_SUPPORT_SKIA