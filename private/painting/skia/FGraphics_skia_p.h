/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FGraphics_skia_p.h
* ������		FGraphics_skia_p
* �������ڣ�	2013-06-06
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
		/** �����豸�����ľ�� */
		HDC getSafeHdc(void) const;
		/** ��ȡ��������ȡ������Ⱦ���棩(canvas) */
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
		bool clipRect(const Core::FRect& rect, eClipMode mode);
		/** ������вü������������޷��ָ��� */
		bool clearClip(void);

		/** ����������� */
		bool clear(void);
		/** ��������������� */
		bool clear(const Core::FRect& rect);

		/** �����������ݵ�hDest */
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
		/** ������� */
		bool drawFocusRect(const Core::FRect& rect);

	public:
		/** ����/�����Ƿ�֧�ֲַ㣬���֧�ֽ���һЩ�Ż� */
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
