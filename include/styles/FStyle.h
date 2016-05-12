/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FStyle.h
* ������		FStyle
* �������ڣ�	2013-06-19
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
		bool draw(Widgets::FWidget* pWidget, /** �ؼ����� */
					Painting::FGraphics* pGraphics, /** ���� */
					const Core::FRect* prcUpdate, /** �������� */
					DWORD dwState, /** ״̬ */
					const Core::FRect& rcDest, /** ��ͼ���� */
					LPCTSTR pszType = NULL, 
					LPCTSTR pszPart = NULL, 
					FSTYLESTRUCT* fss = NULL) const;

		/** ������������ */
		bool createWindowRegion(HWND hWnd, DWORD dwState, int xLeft, int yTop, int xRight, int yBottom, 
								LPCTSTR pszType = NULL, LPCTSTR pszPart = NULL) const;

		/** ����Ƿ����dwState1��dwState2����ʽ���ػ�ǰ��飬���������ػ棩 */
		bool testStyle(DWORD dwState, LPCTSTR pszType = NULL, LPCTSTR pszPart = NULL) const;

	private:
		class FStyleSheetPrivate;
		FStyleSheetPrivate* m_stylesheetPrivate;
	};

}} // !namespace

#endif // !FSTYLE_H