/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FWindowRegionStyle.h
* ������		FWindowRegionStyle
* �������ڣ�	2013-07-03
*
*/

#ifndef FWINDOWREGIONSTYLE_H
#define FWINDOWREGIONSTYLE_H

#include "core/FBase.h"
#include "styles/FStyleDef.h"

namespace FooUI { namespace Styles {

	//////////////////////////////////////////////////////////////////////////
	// FWindowRegionStyle
	#define FOOSC_WINDOWREGION _T("windowregion")
	class FOOUI_LIB FWindowRegionStyle : public FStyleObject
	{
	public:
		enum eFWindowRegionShape
		{
			NoneShape, 
			RoundRectShape, 
			RectAngleShape, 
		};
		FWindowRegionStyle(void);
		FWindowRegionStyle(const FWindowRegionStyle&);
		FWindowRegionStyle& operator=(const FWindowRegionStyle&);
		~FWindowRegionStyle(void);
		FOOUI_DECLARE_STYLETYPE(FStyleObject::WindowRegionStyle)

	public:
		bool draw(Widgets::FWidget* pWidget, /** �ؼ����� */
				Painting::FGraphics* pGraphics, /** ���� */
				const Core::FRect* prcUpdate, /** �������� */
				DWORD dwState, /** ״̬ */
				const Core::FRect& rcDest, /** ��ͼ���� */
				LPCTSTR pszType = NULL, 
				LPCTSTR pszPart = NULL, 
				FSTYLESTRUCT* fss = NULL) { return false; }

	public:
		int shape(void) const { return m_shape; }
		int combine(void) const { return m_combine; }
		const Core::FSize& roundsize(void) const { return m_roundsize; }
		const Core::FRect& deflaterect(void) const { return m_deflaterect; }

	protected:
		bool setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue);

	private:
		int m_shape;
		int m_combine;
		Core::FSize m_roundsize;
		Core::FRect m_deflaterect;
	};

	//////////////////////////////////////////////////////////////////////////
	// 

}} // !namespace

#endif // !FWINDOWREGIONSTYLE_H