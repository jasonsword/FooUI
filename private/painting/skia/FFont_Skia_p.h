/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FFont_Skia_p.h
* ������		FFont_Skia_p
* �������ڣ�	2013-06-06
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
		/** ����ΪDEFAULT_GUI_FONT */
		bool create(void);
		/** �������� */
		bool create(const LOGFONT* lplf);
		bool create(LPCTSTR pszFaceName, 
					int nSize = 0, 
					bool bBold = false, 
					bool bItalic = false, 
					bool bUnderLine = false, 
					bool bStrickeOut = false);
		/** ���������忽�� */
		bool copy(HFONT hFont);
		/** �������� */
		bool destroy(void);
		/** �������true����û�д������� */
		bool isNull(void) const;

		/** ����������󣨸�����Ⱦ������죩 */
		LPVOID font(void) const;
		/** ���������� */
		HFONT handle(void) const;

	private:
		HFONT m_hFont;
	};

}} // !namespace

#endif // !FOOUI_SUPPORT_SKIA
#endif // !FFONT_SKIA_P_H