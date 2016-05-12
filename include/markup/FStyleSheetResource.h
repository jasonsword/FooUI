/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FStyleSheetResource.h
* ������		FStyleSheetResource
* �������ڣ�	2013-06-26
*
*/

#ifndef FSTYLESHEETRESOURCE_H
#define FSTYLESHEETRESOURCE_H

#include "core/FBase.h"
#include "markup/FResourceDef.h"
#include "styles/FStyleDef.h"

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FStyleSheetResource
	class FOOUI_LIB FStyleSheetResource : public FResourceObject
	{
	public:
		FStyleSheetResource(void);
		explicit FStyleSheetResource(const FStyleSheetResource&);
		FStyleSheetResource& operator=(const FStyleSheetResource&);
		virtual ~FStyleSheetResource(void);
		FOOUI_DECLARE_RESTYPE(FResourceObject::FRESTYPE_STYLESHEET)

	public:
		bool addStyle(Styles::FStyleObject* pStyle);
		Styles::FStyleObject* getStyle(int nIndex) const;
		bool remove(int nIndex);
		bool remove(Styles::FStyleObject* pStyle);
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
					Styles::FSTYLESTRUCT* fss = NULL) const;

		/** ������������ */
		bool createWindowRegion(HWND hWnd, DWORD dwState, int xLeft, int yTop, int xRight, int yBottom, 
						LPCTSTR pszType = NULL, LPCTSTR pszPart = NULL) const;
		/** ����Ƿ����dwState��ʽ���ػ�ǰ��飬���������ػ棩 */
		bool testStyle(DWORD dwState, LPCTSTR pszType = NULL, LPCTSTR pszPart = NULL) const;

	public:
		bool parseAttribute(FMarkup* pMarkup, LPCTSTR pszValue);

	protected:
		bool parseAttributeScript(FMarkup* pMarkup, LPCTSTR pszAttributeScript);

	protected:
		bool setAttribute(FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue);

	private:
		class FStyleSheetResourcePrivate;
		FStyleSheetResourcePrivate* m_stylesheetResPrivate;
	};

}} // !namespace

#endif // !FSTYLESHEETRESOURCE_H