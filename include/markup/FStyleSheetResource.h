/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FStyleSheetResource.h
* 描述：		FStyleSheetResource
* 创建日期：	2013-06-26
*
*/

#ifndef FSTYLESHEETRESOURCE_H
#define FSTYLESHEETRESOURCE_H

#include "core/FBase.h"
#include "markup/FResourceDef.h"
#include "styles/FStyleDef.h"

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FStyleSheetResource
	class FOOUI_LIB FStyleSheetResource : public FResourceObject
	{
	public:
		FStyleSheetResource(void);
		explicit FStyleSheetResource(const FStyleSheetResource&);
		FStyleSheetResource& operator=(const FStyleSheetResource&);
		virtual ~FStyleSheetResource(void);
		FOOUI_DECLARE_RESTYPE(FResourceObject::FRESTYPE_STYLESHEET)

	public:
		bool addStyle(Styles::FStyleObject* pStyle);
		Styles::FStyleObject* getStyle(int nIndex) const;
		bool remove(int nIndex);
		bool remove(Styles::FStyleObject* pStyle);
		bool clear(void);
		int getStyleCount(void) const;
		bool isNull(void) const;

	public:
		bool draw(Widgets::FWidget* pWidget, /** 控件对象 */
					Painting::FGraphics* pGraphics, /** 画布 */
					const Core::FRect* prcUpdate, /** 更新区域 */
					DWORD dwState, /** 状态 */
					const Core::FRect& rcDest, /** 绘图区域 */
					LPCTSTR pszType = NULL, 
					LPCTSTR pszPart = NULL, 
					Styles::FSTYLESTRUCT* fss = NULL) const;

		/** 创建窗口区域 */
		bool createWindowRegion(HWND hWnd, DWORD dwState, int xLeft, int yTop, int xRight, int yBottom, 
						LPCTSTR pszType = NULL, LPCTSTR pszPart = NULL) const;
		/** 检查是否包含dwState样式（重绘前检查，减少无用重绘） */
		bool testStyle(DWORD dwState, LPCTSTR pszType = NULL, LPCTSTR pszPart = NULL) const;

	public:
		bool parseAttribute(FMarkup* pMarkup, LPCTSTR pszValue);

	protected:
		bool parseAttributeScript(FMarkup* pMarkup, LPCTSTR pszAttributeScript);

	protected:
		bool setAttribute(FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue);

	private:
		class FStyleSheetResourcePrivate;
		FStyleSheetResourcePrivate* m_stylesheetResPrivate;
	};

}} // !namespace

#endif // !FSTYLESHEETRESOURCE_H