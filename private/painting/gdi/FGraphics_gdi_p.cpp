/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FGraphics_gdi_p.cpp
* 描述：		FGraphics_gdi_p
* 创建日期：	2013-06-20
*
*/

#include "StdAfx.h"
#include "painting/gdi/FGraphics_gdi_p.h"
#ifdef FOOUI_SUPPORT_GDI

#pragma comment(lib, "Msimg32.lib")
namespace FooUI { namespace Painting {

	//////////////////////////////////////////////////////////////////////////
	// FGraphicsPrivate
	FGraphics::FGraphicsPrivate::FGraphicsPrivate(void) : 
		m_hDC(NULL) 
		, m_hBitmap(NULL) 
		, m_nSaveDC(0) 
		, m_nLastSaveDC(0) 
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
		return m_hDC;
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
		if (NULL != m_hBitmap)
		{
			fooSafeDeleteObject(m_hBitmap);
		}

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

		m_size.cx = nWidth;
		m_size.cy = nHeight;

		return (NULL != m_hBitmap);
	}

	int FGraphics::FGraphicsPrivate::save(void)
	{
		FASSERT(NULL != m_hDC);
		m_nLastSaveDC= ::SaveDC(m_hDC);
		return m_nLastSaveDC;
	}

	bool FGraphics::FGraphicsPrivate::restore(void)
	{
		if (::RestoreDC(m_hDC, m_nLastSaveDC))
		{
			if (m_nLastSaveDC > 0) --m_nLastSaveDC;
			return true;
		}

		return false;
	}

	bool FGraphics::FGraphicsPrivate::restore(int nSave)
	{
		if (::RestoreDC(m_hDC, nSave))
		{
			if (nSave > 0) m_nLastSaveDC = (--nSave);
			return true;
		}

		FASSERT(false);
		return false;
	}

	bool FGraphics::FGraphicsPrivate::flush(void)
	{
		::GdiFlush();
		return true;
	}

	bool FGraphics::FGraphicsPrivate::clipRect(const Core::FRect& rect, eClipMode mode)
	{
		FASSERT(NULL != m_hDC);

		HRGN hRgnClip = ::CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
		HRGN hRgnBox = ::CreateRectRgn(0, 0, 0, 0);
		if (!::GetClipRgn(m_hDC, hRgnBox))
		{
			::SelectClipRgn(m_hDC, hRgnClip);
			fooSafeDeleteObject(hRgnBox);
			fooSafeDeleteObject(hRgnClip);
		}
		else
		{
			switch (mode)
			{
			case FooUI::Painting::FGraphics::clip_replace:
				break;
			case FooUI::Painting::FGraphics::clip_difference:
				::CombineRgn(hRgnBox, hRgnBox, hRgnClip, RGN_DIFF);
				break;
			case FooUI::Painting::FGraphics::clip_intersect:
				::CombineRgn(hRgnBox, hRgnBox, hRgnClip, RGN_AND);
				break;
			case FooUI::Painting::FGraphics::clip_union:
				::CombineRgn(hRgnBox, hRgnBox, hRgnClip, RGN_OR);
				break;
			case FooUI::Painting::FGraphics::clip_xor:
				::CombineRgn(hRgnBox, hRgnBox, hRgnClip, RGN_XOR);
				break;
			default:
				break;
			}

			::SelectClipRgn(m_hDC, hRgnBox);
			fooSafeDeleteObject(hRgnBox);
			fooSafeDeleteObject(hRgnClip);
		}

		return true;
	}

	bool FGraphics::FGraphicsPrivate::clearClip(void)
	{
		FASSERT(NULL != m_hDC);
		::SelectClipRgn(m_hDC, NULL);
		return true;
	}

	bool FGraphics::FGraphicsPrivate::clear(void)
	{
		FASSERT(NULL != m_hDC);

		return clear(Core::FRect(0, 0, m_size.cx, m_size.cy));
	}

	bool FGraphics::FGraphicsPrivate::clear(const Core::FRect& rect)
	{
		FASSERT(NULL != m_hDC);

		int nSaveDC = ::SaveDC(m_hDC);

		HRGN hRgn = ::CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
		::SelectClipRgn(m_hDC, hRgn);

		::FillRect(m_hDC, &rect, (HBRUSH)::GetStockObject(BLACK_BRUSH));

		::RestoreDC(m_hDC, nSaveDC);
		fooSafeDeleteObject(hRgn);

		return true;
	}

	bool FGraphics::FGraphicsPrivate::update(HDC hDestDC, const Core::FRect& rect) const
	{
		FASSERT(NULL != m_hDC);
		return (::BitBlt(hDestDC, rect.left, rect.top, rect.width(), rect.height(), m_hDC, rect.left, rect.top, SRCCOPY) == TRUE);
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
		return FGraphics::normal;
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
		HBRUSH hBrush = ::CreateSolidBrush(m_clrBack.toCOLORREF());
		::FillRect(m_hDC, &rect, hBrush);
		fooSafeDeleteObject(hBrush);

		if (m_bLayered)
		{
			Core::FRect rcFix;
			boundsFixRect(rcFix, rect);
			fixAlpha(rcFix);
		}

		return true;
	}

	bool FGraphics::FGraphicsPrivate::drawCircle(int xLeft, int yTop, int radius, const FPaint* paint)
	{
		int nSave = ::SaveDC(m_hDC);
		HPEN hPen = ::CreatePen(PS_SOLID, 1, m_clrBack.toCOLORREF());
		HBRUSH hBrush = ::CreateSolidBrush(m_clrBack.toCOLORREF());
		::SelectObject(m_hDC, hPen);
		::SelectObject(m_hDC, hBrush);
		::Ellipse(m_hDC, xLeft, yTop, xLeft + radius, yTop + radius);
		::RestoreDC(m_hDC, nSave);
		fooSafeDeleteObject(hPen);
		fooSafeDeleteObject(hBrush);
		return true;
	}

	bool FGraphics::FGraphicsPrivate::drawCircle(const Core::FRect& rect, const FPaint* paint)
	{
		HPEN hPen = ::CreatePen(PS_SOLID, 1, m_clrBack.toCOLORREF());
		HBRUSH hBrush = ::CreateSolidBrush(m_clrBack.toCOLORREF());
		HPEN hOldPen = (HPEN)::SelectObject(m_hDC, hPen);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(m_hDC, hBrush);
		int radius = min(rect.right, rect.bottom);
		::Ellipse(m_hDC, rect.left, rect.top, rect.left + radius, rect.top + radius);
		::SelectObject(m_hDC, hOldPen);
		::SelectObject(m_hDC, hOldBrush);
		fooSafeDeleteObject(hPen);
		fooSafeDeleteObject(hBrush);
		return true;
	}

	bool FGraphics::FGraphicsPrivate::drawEllipse(Core::FRect& rect, const FPaint* paint)
	{
		HPEN hPen = ::CreatePen(PS_SOLID, 1, m_clrBack.toCOLORREF());
		HBRUSH hBrush = ::CreateSolidBrush(m_clrBack.toCOLORREF());
		HPEN hOldPen = (HPEN)::SelectObject(m_hDC, hPen);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(m_hDC, hBrush);
		::Ellipse(m_hDC, rect.left, rect.top, rect.right, rect.bottom);
		::SelectObject(m_hDC, hOldPen);
		::SelectObject(m_hDC, hOldBrush);
		fooSafeDeleteObject(hPen);
		fooSafeDeleteObject(hBrush);
		return true;
	}

	bool FGraphics::FGraphicsPrivate::drawLine(int x0, int y0, int x1, int y1, const FPaint* paint)
	{
		FASSERT(NULL != m_hDC);
		HPEN hPen = ::CreatePen(PS_SOLID, 1, m_clrBack.toCOLORREF());
		HPEN hOldPen = (HPEN)::SelectObject(m_hDC, hPen);
		::MoveToEx(m_hDC, x0, y0, NULL);
		::LineTo(m_hDC, x1, y1);
		::SelectObject(m_hDC, hOldPen);
		fooSafeDeleteObject(hPen);
		return true;
	}

	bool FGraphics::FGraphicsPrivate::drawImage(const FImage& image, int xLeft, int yTop, const FPaint* paint)
	{
		FASSERT(NULL != m_hDC);
		return drawImage(image, Core::FRect(0, 0, image.width(), image.height()), 
								Core::FRect::makeXYWH(xLeft, yTop, image.width(), image.height()), paint);
	}

	bool FGraphics::FGraphicsPrivate::drawImage(const FImage& image, const Core::FRect& rcDest, const FPaint* paint)
	{
		FASSERT(NULL != m_hDC);
		return drawImage(image, Core::FRect(0, 0, image.width(), image.height()), rcDest, paint);
	}

	bool FGraphics::FGraphicsPrivate::drawImage(const FImage& image, const Core::FRect& rcSource, const Core::FRect& rcDest, const FPaint* paint)
	{
		FASSERT(NULL != m_hDC);
		FASSERT(!image.isNull());

		if (rcSource.isEmpty() || rcDest.isEmpty())
		{
			return false;
		}

		::SetStretchBltMode(m_hDC, COLORONCOLOR);

		BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
		::AlphaBlend(m_hDC, rcDest.left, rcDest.top, rcDest.width(), rcDest.height(), 
					(HDC)image.getImage(), rcSource.left, rcSource.top, rcSource.width(), rcSource.height(), bf);

		return true;
	}

	bool FGraphics::FGraphicsPrivate::draw9PatchImage(const FImage& image, const Core::FRect& rcDest, const Core::FEdge& egNinePatch, const FPaint* paint)
	{
		FASSERT(NULL != m_hDC);
		return draw9PatchImage(image, Core::FRect(0, 0, image.width(), image.height()), rcDest, egNinePatch, paint);
	}

	bool FGraphics::FGraphicsPrivate::draw9PatchImage(const FImage& image, const Core::FRect& rcSource, const Core::FRect& rcDest, const Core::FEdge& egNinePatch, const FPaint* paint)
	{
		FASSERT(NULL != m_hDC);
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
			rcPatchSrc[F9PATCH_BOTTOMLEFT].setRect(rcSource.left, rcSource.bottom - egNinePatch.bottom, rcSource.left + egNinePatch.left, rcSource.bottom);
			rcPatchDest[F9PATCH_BOTTOMLEFT].setRect(rcDest.left, rcDest.bottom - egNinePatch.bottom, rcDest.left + egNinePatch.left, rcDest.bottom);

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
		FASSERT(NULL != m_hDC);

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

			Core::FRect rcFixBouds;
			boundsFixRect(rcFixBouds, rcFix);
			if (!rcFix.isEmpty())
			{
				fixAlpha(rcFixBouds);
			}
		}

		::RestoreDC(hTextDC, nDCState);
		return true;
	}

	bool FGraphics::FGraphicsPrivate::drawFocusRect(const Core::FRect& rect)
	{
		FASSERT(NULL != m_hDC);

		if (rect.isEmpty())
		{
			return false;
		}

		::SetTextColor(m_hDC, RGB(0, 0, 0));
		::SetBkMode(m_hDC, TRANSPARENT);

		if (::DrawFocusRect(m_hDC, &rect))
		{
			return true;
		}

		return false;
	}

	bool FGraphics::FGraphicsPrivate::create(void)
	{
		FASSERT(NULL == m_hDC);
		FASSERT(NULL == m_hBitmap);

		m_hDC = ::CreateCompatibleDC(NULL);
		m_nSaveDC = ::SaveDC(m_hDC);

		return false;
	}

	bool FGraphics::FGraphicsPrivate::destroy(void)
	{
		if (NULL != m_hDC)
		{
			::RestoreDC(m_hDC, m_nSaveDC);
			fooSafeDeleteDC(m_hDC);
		}

		if (NULL != m_hBitmap)
		{
			fooSafeDeleteObject(m_hBitmap);
		}

		m_nSaveDC = m_nLastSaveDC = 0;

		return true;
	}

	bool FGraphics::FGraphicsPrivate::boundsFixRect(Core::FRect& rect, const Core::FRect& rcDraw) const
	{
		rect = rcDraw;

		HRGN hBoudsRgn = ::CreateRectRgn(0, 0, 0, 0);
		Core::FRect rcBoudsBox;
		if (::GetClipRgn(m_hDC, hBoudsRgn) 
			&& ::GetRgnBox(hBoudsRgn, &rcBoudsBox) 
			&& !rcBoudsBox.isEmpty())
		{
			rect.setRect(max(rcDraw.left, rcBoudsBox.left), 
						max(rcDraw.top, rcBoudsBox.top), 
						min(rcDraw.right, rcBoudsBox.right), 
						min(rcDraw.bottom, rcBoudsBox.bottom)
						);
		}
		fooSafeDeleteObject(hBoudsRgn);

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
		FASSERT(NULL != hFixDC); /** 可能是GDI句柄泄露了！ */
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
#endif // !FOOUI_SUPPORT_GDI