/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FGraphics.h
* ������		FGraphics
* �������ڣ�	2013-06-06
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
		/** ͨ�� */
		unsigned char m_alpha;

		/** ���� */
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
		/** �����豸�����ľ�� */
		HDC getSafeHdc(void) const;
		/** ��ȡ��������ȡ������Ⱦ���棩 */
		LPVOID getGraphics(void) const;

		/** ���ػ�����С */
		int width(void) const;
		int height(void) const;
		Core::FSize size(void) const;

		/** ����������С */
		bool resize(int nWidth, int nHeight);

		/** ���滭��״̬ */
		int save(void);
		/** �ָ���һ�δ���Ļ���״̬ */
		bool restore(void);
		/** �ָ�ĳ�δ���Ļ���״̬ */
		bool restore(int nSave);

		/** ǿ��ִ�����й����ͼ�β������������� */
		bool flush(void);

		/** �ü����� */
		bool clipRect(const Core::FRect& rect, eClipMode mode = clip_intersect);
		/** ������вü������������޷��ָ��� */
		bool clearClip(void);
		/** ѹ��һ���ü����򣨷��زü�ǰ��save����� */
		int pushClipRect(const Core::FRect& rect, eClipMode mode = clip_intersect);
		/** ����restore(nPush) */
		bool popClipRect(int nPush);
		/** ����restore() */
		bool popClipRect(void);

		/** ����������� */
		bool clear(void);
		/** ��������������� */
		bool clear(const Core::FRect& rect);

		/** �����������ݵ�hDest */
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
		/** ʹ��BackColor������ */
		bool drawRect(const Core::FRect& rect, const FPaint* paint = NULL);
		/** ʹ��BackColor��Բ */
		bool drawCircle(int xLeft, int yTop, int radius, const FPaint* paint = NULL);
		bool drawCircle(const Core::FRect& rect, const FPaint* paint = NULL);
		/** ����Բ */
		bool drawEllipse(Core::FRect& rect, const FPaint* paint = NULL);
		/** ���� */
		bool drawLine(int x0, int y0, int x1, int y1, const FPaint* paint = NULL);
		/** ��Ⱦͼ�� */
		bool drawImage(const FImage& image, int xLeft, int yTop, const FPaint* paint = NULL);
		bool drawImage(const FImage& image, const Core::FRect& rcDest, const FPaint* paint = NULL);
		bool drawImage(const FImage& image, const Core::FRect& rcSource, const Core::FRect& rcDest, const FPaint* paint = NULL);
		/** ��Ⱦ�Ź���ͼ�� */
		bool draw9PatchImage(const FImage& image, const Core::FRect& rcDest, const Core::FEdge& egNinePatch, const FPaint* paint = NULL);
		bool draw9PatchImage(const FImage& image, const Core::FRect& rcSource, const Core::FRect& rcDest, const Core::FEdge& egNinePatch, const FPaint* paint = NULL);
		/** ��Ⱦ���� */
		bool drawText(LPCTSTR pString, int nCount, Core::FRect& rect, UINT uFormat, const FPaint* paint = NULL);
		/** ��ȾHTML��ʽ������ */
		bool drawHtmlText(LPCTSTR pString, int nCount, Core::FRect& rect, UINT uFormat, const FPaint* paint = NULL);
		/** ������� */
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
