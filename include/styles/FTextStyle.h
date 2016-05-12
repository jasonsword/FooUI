/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FTextStyle.h
* ������		FTextStyle
* �������ڣ�	2013-06-27
*
*/

#ifndef FTEXTSTYLE_H
#define FTEXTSTYLE_H

#include "core/FBase.h"
#include "styles/FStyleDef.h"

#define FOOTEXTALIGN_NAME_NONE			_T("none")
#define FOOTEXTALIGN_NAME_LEFT			_T("left")
#define FOOTEXTALIGN_NAME_TOP			_T("top")
#define FOOTEXTALIGN_NAME_RIGHT			_T("right")
#define FOOTEXTALIGN_NAME_XCENTER		_T("xcenter")
#define FOOTEXTALIGN_NAME_YCENTER		_T("ycenter")
#define FOOTEXTALIGN_NAME_BOTTOM		_T("bottom")
#define FOOTEXTALIGN_NAME_END_ELLIPSIS	_T("endellipsis")

namespace FooUI { namespace Styles {

	//////////////////////////////////////////////////////////////////////////
	// FTextStyle
	#define FOOSC_TEXT _T("text")
	class FOOUI_LIB FTextStyle : public FStyleObject
	{
	public:
		FTextStyle(void);
		FTextStyle(const FTextStyle&);
		FTextStyle& operator=(const FTextStyle&);
		~FTextStyle(void);
		FOOUI_DECLARE_STYLETYPE(FStyleObject::TextStyle)

	public:
		bool draw(Widgets::FWidget* pWidget, /** �ؼ����� */
				Painting::FGraphics* pGraphics, /** ���� */
				const Core::FRect* prcUpdate, /** �������� */
				DWORD dwState, /** ״̬ */
				const Core::FRect& rcDest, /** ��ͼ���� */
				LPCTSTR pszType = NULL, 
				LPCTSTR pszPart = NULL, 
				FSTYLESTRUCT* fss = NULL);

		bool drawText(LPCTSTR pszText, /** �ؼ����� */
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
		class FTextStylePrivate;
		FTextStylePrivate* m_textStylePrivate;
	};

	//////////////////////////////////////////////////////////////////////////
	// FStringStyle
	#define FOOSC_STRING _T("string")
	class FOOUI_LIB FStringStyle : public FTextStyle
	{
	public:
		FStringStyle(void);
		FStringStyle(const FStringStyle&);
		FStringStyle& operator=(const FStringStyle&);
		~FStringStyle(void);
		FOOUI_DECLARE_STYLETYPE(FStyleObject::StringStyle)

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
		class FStringStylePrivate;
		FStringStylePrivate* m_stringStylePrivate;
	};

}} // !namespace

#endif // !FTEXTSTYLE_H