/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FFont_Skia_p.h
* 描述：		FFont_Skia_p
* 创建日期：	2013-06-06
*
*/

#ifndef FFONT_SKIA_P_H
#define FFONT_SKIA_P_H

#include "F3rd_p.h"
#ifdef FOOUI_SUPPORT_SKIA

#include "painting/FFont.h"
#include "painting/IPatting_p.h"

namespace FooUI { namespace Painting {

	class FFont::FFontPrivate : public IFont
	{
	public:
		FFontPrivate(void);
		~FFontPrivate(void);

	public:
		/** 创建为DEFAULT_GUI_FONT */
		bool create(void);
		/** 创建字体 */
		bool create(const LOGFONT* lplf);
		bool create(LPCTSTR pszFaceName, 
					int nSize = 0, 
					bool bBold = false, 
					bool bItalic = false, 
					bool bUnderLine = false, 
					bool bStrickeOut = false);
		/** 从其他字体拷贝 */
		bool copy(HFONT hFont);
		/** 销毁字体 */
		bool destroy(void);
		/** 如果返回true，还没有创建字体 */
		bool isNull(void) const;

		/** 返回字体对象（根据渲染引擎而异） */
		LPVOID font(void) const;
		/** 返回字体句柄 */
		HFONT handle(void) const;

	private:
		HFONT m_hFont;
	};

}} // !namespace

#endif // !FOOUI_SUPPORT_SKIA
#endif // !FFONT_SKIA_P_H