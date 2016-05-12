/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FStyle.h
* 描述：		FStyle
* 创建日期：	2013-06-19
*
*/

#ifndef FSTYLE_H
#define FSTYLE_H

#include "core/FBase.h"
#include "styles/FStyleDef.h"

namespace FooUI { namespace Styles {

	//////////////////////////////////////////////////////////////////////////
	// FStyleSheet
	class FOOUI_LIB FStyleSheet
	{
	public:
		FStyleSheet(void);
		FStyleSheet(const FStyleSheet&);
		FStyleSheet& operator=(const FStyleSheet&);
		virtual ~FStyleSheet(void);

	public:
		bool addStyle(FStyleObject* pStyle);
		FStyleObject* getStyle(int nIndex) const;
		FStyleObject* getStyle(LPCTSTR pszName) const;
		bool remove(int nIndex);
		bool remove(FStyleObject* pStyle);
		bool clear(void);
		int getStyleCount(void) const;
		bool isNull(void) const;

	public:
		bool draw(Widgets::FWidget* pWidget, /** 控件对象 */
					Painting::FGraphics* pGraphics, /** 画布 */
					const Core::FRect* prcUpdate, /** 更新区域 */
					DWORD dwState, /** 状态 */
					const Core::FRect& rcDest, /** 绘图区域 */
					LPCTSTR pszType = NULL, 
					LPCTSTR pszPart = NULL, 
					FSTYLESTRUCT* fss = NULL) const;

		/** 创建窗口区域 */
		bool createWindowRegion(HWND hWnd, DWORD dwState, int xLeft, int yTop, int xRight, int yBottom, 
								LPCTSTR pszType = NULL, LPCTSTR pszPart = NULL) const;

		/** 检查是否包含dwState1或dwState2的样式（重绘前检查，减少无用重绘） */
		bool testStyle(DWORD dwState, LPCTSTR pszType = NULL, LPCTSTR pszPart = NULL) const;

	private:
		class FStyleSheetPrivate;
		FStyleSheetPrivate* m_stylesheetPrivate;
	};

}} // !namespace

#endif // !FSTYLE_H