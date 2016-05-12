/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FGraphics_skia_p.h
* 描述：		FGraphics_skia_p
* 创建日期：	2013-06-06
*
*/

#ifndef FGRAPHICS_SKIA_P_H
#define FGRAPHICS_SKIA_P_H

#include "F3rd_p.h"
#ifdef FOOUI_SUPPORT_SKIA

#include "core/FObject.h"
#include "painting/FGraphics.h"
#include "painting/IPatting_p.h"

namespace FooUI { namespace Painting {

	class FGraphics::FGraphicsPrivate : public IGraphics
	{
	public:
		FGraphicsPrivate(void);
		~FGraphicsPrivate(void);

	public:
		/** 返回设备上下文句柄 */
		HDC getSafeHdc(void) const;
		/** 获取画布对象（取决于渲染引擎）(canvas) */
		LPVOID getGraphics(void) const;

		/** 返回画布大小 */
		int width(void) const;
		int height(void) const;
		Core::FSize size(void) const;

		/** 调整画布大小 */
		bool resize(int nWidth, int nHeight);

		/** 储存画布状态 */
		int save(void);
		/** 恢复上一次储存的画布状态 */
		bool restore(void);
		/** 恢复某次储存的画布状态 */
		bool restore(int nSave);

		/** 强制执行所有挂起的图形操作并立即返回 */
		bool flush(void);

		/** 裁剪区域 */
		bool clipRect(const Core::FRect& rect, eClipMode mode);
		/** 清空所有裁剪操作（可能无法恢复） */
		bool clearClip(void);

		/** 清除画布内容 */
		bool clear(void);
		/** 清除画布区域内容 */
		bool clear(const Core::FRect& rect);

		/** 拷贝画布内容到hDest */
		bool update(HDC hDestDC, const Core::FRect& rect) const;
		bool updateLayered(HWND hWnd, const Core::FRect& rect) const;

	public:
		void setRenderMode(FGraphics::eRenderMode mode);
		FGraphics::eRenderMode getRenderMode(void) const;

		void setForeColor(const FColor& clr);
		const FColor& getForeColor(void) const;

		void setBackColor(const FColor& clr);
		const FColor& getBackColor(void) const;

		void setFont(const FFont& font);
		const FFont& font(void) const;

	public:
		/** 使用BackColor填充矩形 */
		bool drawRect(const Core::FRect& rect, const FPaint* paint = NULL);
		/** 使用BackColor画圆 */
		bool drawCircle(int xLeft, int yTop, int radius, const FPaint* paint = NULL);
		bool drawCircle(const Core::FRect& rect, const FPaint* paint = NULL);
		/** 画椭圆 */
		bool drawEllipse(Core::FRect& rect, const FPaint* paint = NULL);
		/** 画线 */
		bool drawLine(int x0, int y0, int x1, int y1, const FPaint* paint = NULL);
		/** 渲染图像 */
		bool drawImage(const FImage& image, int xLeft, int yTop, const FPaint* paint = NULL);
		bool drawImage(const FImage& image, const Core::FRect& rcDest, const FPaint* paint = NULL);
		bool drawImage(const FImage& image, const Core::FRect& rcSource, const Core::FRect& rcDest, const FPaint* paint = NULL);
		/** 渲染九宫格图像 */
		bool draw9PatchImage(const FImage& image, const Core::FRect& rcDest, const Core::FEdge& egNinePatch, const FPaint* paint = NULL);
		bool draw9PatchImage(const FImage& image, const Core::FRect& rcSource, const Core::FRect& rcDest, const Core::FEdge& egNinePatch, const FPaint* paint = NULL);
		/** 渲染文字 */
		bool drawText(LPCTSTR pString, int nCount, Core::FRect& rect, UINT uFormat, const FPaint* paint = NULL);
		/** 画焦点框 */
		bool drawFocusRect(const Core::FRect& rect);

	public:
		/** 设置/返回是否支持分层，如果支持将做一些优化 */
		void setLayered(bool bLayered) { m_bLayered = bLayered; }
		bool isLayered(void) const { return m_bLayered; }

	private:
		bool create(void);
		bool destroy(void);

	private:
		bool fixAlpha(const Core::FRect& rcFix);
		void calcGdiTextRect(Core::FRect& rcCalc, const Core::FRect& rcDest, long lTextWidth, long lTextHeight, UINT uFormat) const;

	private:
		SkBitmap* m_pBitmapDevice;
		SkCanvas* m_pCanvas;
		Core::FSize m_size;

		HDC m_hDC;
		HBITMAP m_hBitmap;
		int m_nSaveDC;

		FGraphics::eRenderMode m_renderMode;
		FColor m_clrFore;
		FColor m_clrBack;
		FFont m_font;

		bool m_bLayered;
	};

}} // !namespace

#endif // !FOOUI_SUPPORT_SKIA
#endif // !FGRAPHICS_SKIA_P_H
