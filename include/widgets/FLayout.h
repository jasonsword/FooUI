/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FLayout.h
* ������		FLayout�����֣�layout����ˮƽ���֣�horzlayout������ֱ���֣�vertlayout��
* �������ڣ�	2013-06-30
*
*/

#ifndef FLAYOUT_H
#define FLAYOUT_H

#include "core/FBase.h"
#include "widgets/FWidget.h"

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FLayout
	#define FOOWC_LAYOUT _T("layout")
	#define FOOWC_TABLAYOUT _T("tablayout")
	#define FOOWC_HORZLAYOUT _T("horzlayout")
	#define FOOWC_VERTLAYOUT _T("vertlayout")
	#define FOOWC_FLOWLAYOUT _T("flowlayout")
	class FOOUI_LIB FLayout : public FWidget
	{
	public:
		enum eFLayoutStyle
		{
			NoneLayout = 0, 
			TabLayout, 
			HorzLayout, 
			VertLayout, 
			FlowLayout, 
		};
		explicit FLayout(DWORD dwLayoutStyle = NoneLayout, FWidget* parent = NULL);
		~FLayout(void);
		FOODECLARE_CLASS(FOOWC_LAYOUT)
		FOODECLARE_CONTAINER(true)

	public:
		/** ����ѡ��ҳ */
		void setLayoutStyle(DWORD dwLayoutStyle);
		DWORD getLayoutStyle(void) const;

		/** ��ȡTab���ֵ�ǰ��ʾҳ��Index������TabLayout��Ч�� */
		void setCurSelected(int nCurSelected);
		int getCurSelected(void) const;

	protected:
		void layout(const Core::FRect& rcLayout);
		void layoutHorz(const Core::FRect& rcLayout);
		void layoutVert(const Core::FRect& rcLayout);
		void layoutFlow(const Core::FRect& rcLayout);

	protected:
		bool setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue);
		bool endMarkup(Markup::FMarkup* pMarkup);

	private:
		DWORD m_dwLayoutStyle;
		int m_nCurSelected;	// ����TabLayoutʱ��Ч
	};

}} // !namespace

#endif // !FLAYOUT_H