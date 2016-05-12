/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FMarkupParser_Pugixml_p.h
* 描述：		FMarkupParser_Pugixml_p
* 创建日期：	2013-06-19
*
*/

#ifndef FMARKUPPARSER_PUGIXMI_P_H
#define FMARKUPPARSER_PUGIXMI_P_H

#include "F3rd_p.h"

#ifdef FOOUI_SUPPORT_PUGIXML

#include "widgets/FWidget.h"
#include "markup/FMarkup.h"

#include "markup/FResourceManager.h"
#include "markup/FStringResource.h"
#include "markup/FFontResource.h"
#include "markup/FColorResource.h"
#include "markup/FImageResource.h"
#include "markup/FCursorResource.h"
#include "markup/FStyleSheetResource.h"

#define	MARKUP_NODE_THEMEROOT	_T("theme")
#define	MARKUP_NODE_INCLUDEROOT	_T("include")
#define	MARKUP_NODE_STRINGROOT	_T("string")
#define	MARKUP_NODE_FONTROOT	_T("font")
#define	MARKUP_NODE_COLORROOT	_T("color")
#define	MARKUP_NODE_IMAGEROOT	_T("image")
#define	MARKUP_NODE_CURSORROOT	_T("cursor")
#define	MARKUP_NODE_STYLEROOT	_T("style")
#define	MARKUP_NODE_WIDGETROOT	_T("rootwidget")

#define	MARKUP_NODE_INCLUDE		_T("include")
#define	MARKUP_NODE_STRING		_T("string")
#define	MARKUP_NODE_FONT		_T("font")
#define	MARKUP_NODE_COLOR		_T("color")
#define	MARKUP_NODE_IMAGE		_T("image")
#define	MARKUP_NODE_CURSOR		_T("cursor")
#define	MARKUP_NODE_STYLE		_T("stylesheet")
#define	MARKUP_NODE_WIDGET		_T("rootwidget")
#define	MARKUP_NODE_ATTRIBUTE	_T("attribute")

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FMarkupParser
	class FMarkup::FMarkupParser : public FMarkup
	{
	public:
		enum eFMarkupInclude
		{
			INCLUDE_NONE = 0, 
			INCLUDE_RESOURCE = 1, 
			INCLUDE_WIDGETS = 2,
		};
		FMarkupParser(void);

	public:
		bool addResource(LPCTSTR pszName, DWORD dwType, FResourceObject* obj);
		bool testResource(LPCTSTR pszName, DWORD dwType) const;
		FResourceObject* getResource(LPCTSTR pszName, DWORD dwType) const;
		Styles::FStyleObject* createStyle(LPCTSTR pszName) const;
		Widgets::FWidget* createWidget(LPCTSTR pszName) const;

	public:
		bool markup(LPCTSTR pszFileName, 
			Widgets::FWidget* pRootWidget, 
			FMARKUP_CREATEWIDGET_CALLBACK callback = NULL, 
			LPVOID callback_parameter = NULL);

		bool markup(void* pStream, DWORD dwStreamLength, 
			Widgets::FWidget* pRootWidget, 
			FMARKUP_CREATEWIDGET_CALLBACK callback = NULL, 
			LPVOID callback_parameter = NULL);

	private:
		bool beginParse(void);
		bool parseIncludes(void);
		bool parseStrings(void);
		bool parseFonts(void);
		bool parseColors(void);
		bool parseImages(void);
		bool parseCursors(void);
		bool parseStyles(void);
		bool parseStyleChildren(pugi::xml_node& node, Markup::FStyleSheetResource* parent);
		bool parseWidgets(void);
		bool parseWidgetChildren(pugi::xml_node& node, Widgets::FWidget* parent);

	private:
		pugi::xml_document m_xmlDoc;

		Widgets::FWidget* m_pRootWidget;
		/** 回调 */
		FMARKUP_CREATEWIDGET_CALLBACK m_callback;
		LPVOID m_callback_parameter;

		/** XML根目录 */
		Core::FString m_strXmlPath;

		FResourceManager m_resourceMgr;

		eFMarkupInclude m_include;
	};

}} // !namespace
#endif // !FOOUI_SUPPORT_PUGIXML

#endif // !FMARKUPPARSER_PUGIXMI_P_H