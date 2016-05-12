/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FImageStyle.h
* ������		FImageStyle
* �������ڣ�	2013-06-26
*
*/

#ifndef FIMAGESTYLE_H
#define FIMAGESTYLE_H

#include "core/FBase.h"
#include "styles/FStyleDef.h"

namespace FooUI { namespace Styles {

	//////////////////////////////////////////////////////////////////////////
	// FImageStyle
	#define FOOSC_IMAGE _T("image")
	class FOOUI_LIB FImageStyle : public FStyleObject
	{
	public:
		enum eFImageStyleFillStyle
		{
			None = 0, 
			Stretch, 
			Tile, 
			HTile, 
			VTile, 
		};
		FImageStyle(void);
		FImageStyle(const FImageStyle& style);
		FImageStyle& operator=(const FImageStyle& style);
		~FImageStyle(void);
		FOOUI_DECLARE_STYLETYPE(FStyleObject::ImageStyle)

	public:
		bool draw(Widgets::FWidget* pWidget, /** �ؼ����� */
					Painting::FGraphics* pGraphics, /** ���� */
					const Core::FRect* prcUpdate, /** �������� */
					DWORD dwState, /** ״̬ */
					const Core::FRect& rcDest, /** ��ͼ���� */
					LPCTSTR pszType = NULL, 
					LPCTSTR pszPart = NULL, 
					FSTYLESTRUCT* fss = NULL);

		bool drawNone(Widgets::FWidget* pWidget, /** �ؼ����� */
					Painting::FGraphics* pGraphics, /** ���� */
					const Core::FRect* prcUpdate, /** �������� */
					DWORD dwState, /** ״̬ */
					const Core::FRect& rcDest, /** ��ͼ���� */
					LPCTSTR pszType = NULL, 
					LPCTSTR pszPart = NULL, 
					FSTYLESTRUCT* fss = NULL);

		bool drawStretch(Widgets::FWidget* pWidget, /** �ؼ����� */
						Painting::FGraphics* pGraphics, /** ���� */
						const Core::FRect* prcUpdate, /** �������� */
						DWORD dwState, /** ״̬ */
						const Core::FRect& rcDest, /** ��ͼ���� */
						LPCTSTR pszType = NULL, 
						LPCTSTR pszPart = NULL, 
						FSTYLESTRUCT* fss = NULL);


		bool drawTile(Widgets::FWidget* pWidget, /** �ؼ����� */
					Painting::FGraphics* pGraphics, /** ���� */
					const Core::FRect* prcUpdate, /** �������� */
					DWORD dwState, /** ״̬ */
					const Core::FRect& rcDest, /** ��ͼ���� */
					LPCTSTR pszType = NULL, 
					LPCTSTR pszPart = NULL, 
					FSTYLESTRUCT* fss = NULL);


		bool drawHTile(Widgets::FWidget* pWidget, /** �ؼ����� */
						Painting::FGraphics* pGraphics, /** ���� */
						const Core::FRect* prcUpdate, /** �������� */
						DWORD dwState, /** ״̬ */
						const Core::FRect& rcDest, /** ��ͼ���� */
						LPCTSTR pszType = NULL, 
						LPCTSTR pszPart = NULL, 
						FSTYLESTRUCT* fss = NULL);


		bool drawVTile(Widgets::FWidget* pWidget, /** �ؼ����� */
					Painting::FGraphics* pGraphics, /** ���� */
					const Core::FRect* prcUpdate, /** �������� */
					DWORD dwState, /** ״̬ */
					const Core::FRect& rcDest, /** ��ͼ���� */
					LPCTSTR pszType = NULL, 
					LPCTSTR pszPart = NULL, 
					FSTYLESTRUCT* fss = NULL);

	protected:
		bool setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue);

	private:
		class FImageStylePrivate;
		FImageStylePrivate* m_imageStylePrivate;
	};

}} // !namespace

#endif // !FIMAGESTYLE_H