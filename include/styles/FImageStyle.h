/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FImageStyle.h
* 描述：		FImageStyle
* 创建日期：	2013-06-26
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
		bool draw(Widgets::FWidget* pWidget, /** 控件对象 */
					Painting::FGraphics* pGraphics, /** 画布 */
					const Core::FRect* prcUpdate, /** 更新区域 */
					DWORD dwState, /** 状态 */
					const Core::FRect& rcDest, /** 绘图区域 */
					LPCTSTR pszType = NULL, 
					LPCTSTR pszPart = NULL, 
					FSTYLESTRUCT* fss = NULL);

		bool drawNone(Widgets::FWidget* pWidget, /** 控件对象 */
					Painting::FGraphics* pGraphics, /** 画布 */
					const Core::FRect* prcUpdate, /** 更新区域 */
					DWORD dwState, /** 状态 */
					const Core::FRect& rcDest, /** 绘图区域 */
					LPCTSTR pszType = NULL, 
					LPCTSTR pszPart = NULL, 
					FSTYLESTRUCT* fss = NULL);

		bool drawStretch(Widgets::FWidget* pWidget, /** 控件对象 */
						Painting::FGraphics* pGraphics, /** 画布 */
						const Core::FRect* prcUpdate, /** 更新区域 */
						DWORD dwState, /** 状态 */
						const Core::FRect& rcDest, /** 绘图区域 */
						LPCTSTR pszType = NULL, 
						LPCTSTR pszPart = NULL, 
						FSTYLESTRUCT* fss = NULL);


		bool drawTile(Widgets::FWidget* pWidget, /** 控件对象 */
					Painting::FGraphics* pGraphics, /** 画布 */
					const Core::FRect* prcUpdate, /** 更新区域 */
					DWORD dwState, /** 状态 */
					const Core::FRect& rcDest, /** 绘图区域 */
					LPCTSTR pszType = NULL, 
					LPCTSTR pszPart = NULL, 
					FSTYLESTRUCT* fss = NULL);


		bool drawHTile(Widgets::FWidget* pWidget, /** 控件对象 */
						Painting::FGraphics* pGraphics, /** 画布 */
						const Core::FRect* prcUpdate, /** 更新区域 */
						DWORD dwState, /** 状态 */
						const Core::FRect& rcDest, /** 绘图区域 */
						LPCTSTR pszType = NULL, 
						LPCTSTR pszPart = NULL, 
						FSTYLESTRUCT* fss = NULL);


		bool drawVTile(Widgets::FWidget* pWidget, /** 控件对象 */
					Painting::FGraphics* pGraphics, /** 画布 */
					const Core::FRect* prcUpdate, /** 更新区域 */
					DWORD dwState, /** 状态 */
					const Core::FRect& rcDest, /** 绘图区域 */
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