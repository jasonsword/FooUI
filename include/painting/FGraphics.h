/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FGraphics.h
* 描述：		FGraphics
* 创建日期：	2013-06-06
*
*/

#ifndef FGRAPHICS_H
#define FGRAPHICS_H

#include "core/FBase.h"
#include "core/FObject.h"
#include "core/FTypes.h"

#include "painting/FColor.h"
#include "painting/FFont.h"
#include "painting/FImage.h"

namespace FooUI { namespace Widgets { class FWidget; }}

namespace FooUI { namespace Painting {

	//////////////////////////////////////////////////////////////////////////
	// FPaint
	class FOOUI_LIB FPaint
	{
	public:
		FPaint(void) : 
			m_alpha(255) 
			, m_bTextShadow(false) 
			, m_ptTextShadowOffset(0, 0) 
			, m_clrTextShadow(FColor::Transparent)
		{ }

	public:
		void setAlpha(unsigned char alpha) { m_alpha = alpha; }
		unsigned char getAlpha(void) const { return m_alpha; }

		void setTextShadow(bool bShadow) { m_bTextShadow = bShadow; }
		bool isTextShadow(void) const { return m_bTextShadow; }

		void setTextShadowOffset(const Core::FPoint& point) { m_ptTextShadowOffset = point; }
		void setTextShadowOffset(int x, int y) { m_ptTextShadowOffset.setPoint(x, y); }
		const Core::FPoint& getTextShadowOffset(void) const { return m_ptTextShadowOffset; }

		void setTextShadowColor(const FColor& color) { m_clrTextShadow = color; }
		const FColor& getTextShadowColor(void) const { return m_clrTextShadow; }

	private:
		/** 通用 */
		unsigned char m_alpha;

		/** 文字 */
		bool m_bTextShadow;
		Core::FPoint m_ptTextShadowOffset;
		FColor m_clrTextShadow;
	};

	//////////////////////////////////////////////////////////////////////////
	// FGraphics
	class FOOUI_LIB FGraphics
	{
	public:
		enum eRenderMode
		{
			normal = 0, 
			speed, 
			quality, 
		};

		enum eClipMode
		{
			clip_replace = 0, 
			clip_difference, 
			clip_intersect, 
			clip_union, 
			clip_xor, 
		};

		FGraphics(void);
		FGraphics(const FGraphics&);
		FGraphics(int nWidth, int nHeight);
		FGraphics(const Core::FSize&);
		FGraphics(const Core::FRect&);
		FGraphics& operator=(const FGraphics&);
		~FGraphics(void);

	public:
		/** 返回设备上下文句柄 */
		HDC getSafeHdc(void) const;
		/** 获取画布对象（取决于渲染引擎） */
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
		bool clipRect(const Core::FRect& rect, eClipMode mode = clip_intersect);
		/** 清空所有裁剪操作（可能无法恢复） */
		bool clearClip(void);
		/** 压入一个裁剪区域（返回裁剪前的save结果） */
		int pushClipRect(const Core::FRect& rect, eClipMode mode = clip_intersect);
		/** 调用restore(nPush) */
		bool popClipRect(int nPush);
		/** 调用restore() */
		bool popClipRect(void);

		/** 清除画布内容 */
		bool clear(void);
		/** 清除画布区域内容 */
		bool clear(const Core::FRect& rect);

		/** 拷贝画布内容到hDest */
		bool update(HDC hDestDC, const Core::FRect& rect) const;
		bool updateLayered(HWND hWnd, const Core::FRect& rect) const;

	public:
		void setRenderMode(eRenderMode mode);
		eRenderMode getRenderMode(void) const;

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
		/** 渲染HTML格式的文字 */
		bool drawHtmlText(LPCTSTR pString, int nCount, Core::FRect& rect, UINT uFormat, const FPaint* paint = NULL);
		/** 画焦点框 */
		bool drawFocusRect(const Core::FRect& rect);

	protected:
		class FGraphicsPrivate;
		friend FGraphicsPrivate;
		friend Widgets::FWidget;
		FGraphicsPrivate* m_graphicsPrivate;
	};

	//////////////////////////////////////////////////////////////////////////
	// FAutoClipRect
	class FOOUI_LIB FAutoClipRect : public Core::FNoncopyableObject
	{
	public:
		explicit FAutoClipRect(FGraphics* pGraphics);
		explicit FAutoClipRect(FGraphics* pGraphics, const Core::FRect& rcClip, FGraphics::eClipMode mode = FGraphics::clip_intersect);
		~FAutoClipRect(void);

	public:
		bool clipRect(const Core::FRect& rect, FGraphics::eClipMode mode = FGraphics::clip_intersect);

	private:
		FGraphics* m_pGraphics;
		int m_nSave;
	};

}} // !namespace

#endif // !FGRAPHICS_H
