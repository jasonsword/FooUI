/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FIncludeStyle.h
* 描述：		FIncludeStyle
* 创建日期：	2013-07-29
*
*/

#ifndef FINCLUDESTYLE_H
#define FINCLUDESTYLE_H

#include "core/FBase.h"
#include "styles/FStyleDef.h"

namespace FooUI { namespace Styles {

	//////////////////////////////////////////////////////////////////////////
	// FIncludeStyle
	#define FOOSC_INCLUDE _T("include")
	class FOOUI_LIB FIncludeStyle : public FStyleObject
	{
	public:
		FIncludeStyle(void);
		FIncludeStyle(const FIncludeStyle& style);
		FIncludeStyle& operator=(const FIncludeStyle& style);
		~FIncludeStyle(void);
		FOOUI_DECLARE_STYLETYPE(FStyleObject::IncludeStyle)

	public:
		bool draw(Widgets::FWidget* pWidget, /** 控件对象 */
					Painting::FGraphics* pGraphics, /** 画布 */
					const Core::FRect* prcUpdate, /** 更新区域 */
					DWORD dwState, /** 状态 */
					const Core::FRect& rcDest, /** 绘图区域 */
					LPCTSTR pszType = NULL, 
					LPCTSTR pszPart = NULL, 
					FSTYLESTRUCT* fss = NULL);
		bool testStyle(DWORD dwState, LPCTSTR pszType = NULL, LPCTSTR pszPart = NULL) const;

	protected:
		bool setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue);

	private:
		class FIncludeStylePrivate;
		FIncludeStylePrivate* m_includeStylePrivate;
	};

}} // !namespace

#endif // !FINCLUDESTYLE_H