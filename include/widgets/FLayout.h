/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FLayout.h
* 描述：		FLayout，布局（layout）、水平布局（horzlayout）、垂直布局（vertlayout）
* 创建日期：	2013-06-30
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
		/** 设置选中页 */
		void setLayoutStyle(DWORD dwLayoutStyle);
		DWORD getLayoutStyle(void) const;

		/** 获取Tab布局当前显示页的Index（仅对TabLayout有效） */
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
		int m_nCurSelected;	// 仅在TabLayout时有效
	};

}} // !namespace

#endif // !FLAYOUT_H