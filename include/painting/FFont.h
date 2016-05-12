/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FFont.h
* 描述：		FFont
* 创建日期：	2013-06-06
*
*/

#ifndef FFONT_H
#define FFONT_H

#include "core/FBase.h"

namespace FooUI { namespace Painting {

	//////////////////////////////////////////////////////////////////////////
	// FFont
	class FOOUI_LIB FFont
	{
	public:
		FFont(void);
		FFont(const FFont&);
		FFont& operator=(const FFont&);
		~FFont(void);

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
		class FFontPrivate;
		friend FFontPrivate;
		FFontPrivate* m_fontPrivate;
	};

}} // !namespace


#endif // !FFONT_H