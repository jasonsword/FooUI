/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FColorStyle.h
* ������		FColorStyle
* �������ڣ�	2013-06-27
*
*/

#ifndef FCOLORSTYLE_H
#define FCOLORSTYLE_H

#include "core/FBase.h"
#include "painting/FColor.h"
#include "styles/FStyleDef.h"

namespace FooUI { namespace Styles {

	//////////////////////////////////////////////////////////////////////////
	// FColorStyle
	#define FOOSC_COLOR _T("color")
	class FOOUI_LIB FColorStyle : public FStyleObject
	{
	public:
		FColorStyle(void);
		FColorStyle(const FColorStyle&);
		FColorStyle& operator=(const FColorStyle&);
		~FColorStyle(void);
		FOOUI_DECLARE_STYLETYPE(FStyleObject::ColorStyle)

	public:
		const Painting::FColor& getColor(void) const;

	public:
		bool draw(Widgets::FWidget* pWidget, /** �ؼ����� */
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
		class FColorStylePrivate;
		FColorStylePrivate* m_colorResPrivate;
	};

}} // !namespace

#endif // !FCOLORSTYLE_H