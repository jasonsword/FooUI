/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FFont.h
* ������		FFont
* �������ڣ�	2013-06-06
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
		class FFontPrivate;
		friend FFontPrivate;
		FFontPrivate* m_fontPrivate;
	};

}} // !namespace


#endif // !FFONT_H