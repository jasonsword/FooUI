/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FFocusRectStyle.h
* ������		FFocusRectStyle
* �������ڣ�	2013-06-27
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
		bool draw(Widgets::FWidget* pWidget, /** �ؼ����� */
					Painting::FGraphics* pGraphics, /** ���� */
					const Core::FRect* prcUpdate, /** �������� */
					DWORD dwState, /** ״̬ */
					const Core::FRect& rcDest, /** ��ͼ���� */
					LPCTSTR pszType = NULL, 
					LPCTSTR pszPart = NULL, 
					FSTYLESTRUCT* fss = NULL);
	};

}} // !namespace

#endif // !FFOCUSRECTSTYLE_H