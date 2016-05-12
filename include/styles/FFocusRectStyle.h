/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FFocusRectStyle.h
* 描述：		FFocusRectStyle
* 创建日期：	2013-06-27
*
*/

#ifndef FFOCUSRECTSTYLE_H
#define FFOCUSRECTSTYLE_H

#include "core/FBase.h"
#include "styles/FStyleDef.h"

namespace FooUI { namespace Styles {

	//////////////////////////////////////////////////////////////////////////
	// FFocusRectStyle
	#define FOOSC_FOCUSRECT _T("focusrect")
	class FOOUI_LIB FFocusRectStyle : public FStyleObject
	{
	public:
		FFocusRectStyle(void) { }
		FFocusRectStyle(const FFocusRectStyle&) { }
		FFocusRectStyle& operator=(const FFocusRectStyle&) { return (*this); }
		~FFocusRectStyle(void) { }
		FOOUI_DECLARE_STYLETYPE(FStyleObject::FocusRectStyle)

	public:
		bool draw(Widgets::FWidget* pWidget, /** 控件对象 */
					Painting::FGraphics* pGraphics, /** 画布 */
					const Core::FRect* prcUpdate, /** 更新区域 */
					DWORD dwState, /** 状态 */
					const Core::FRect& rcDest, /** 绘图区域 */
					LPCTSTR pszType = NULL, 
					LPCTSTR pszPart = NULL, 
					FSTYLESTRUCT* fss = NULL);
	};

}} // !namespace

#endif // !FFOCUSRECTSTYLE_H