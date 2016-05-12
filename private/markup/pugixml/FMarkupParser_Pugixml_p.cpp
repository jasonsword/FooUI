/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FMarkupParser_Pugixml_p.cpp
* 描述：		FMarkupParser_Pugixml_p
* 创建日期：	2013-06-19
*
*/

#include "StdAfx.h"
#include "markup/pugixml/FMarkupParser_Pugixml_p.h"
#ifdef FOOUI_SUPPORT_PUGIXML

#include "core/FApplication.h"

#include "markup/pugixml/pugixml.cpp"

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FMarkupParser
	FMarkup::FMarkupParser::FMarkupParser(void) : 
		m_pRootWidget(NULL) 
		, m_callback(NULL) 
		, m_callback_parameter(NULL) 
		, m_include(INCLUDE_NONE)
	{
	}

	bool FMarkup::FMarkupParser::addResource(LPCTSTR pszName, DWORD dwType, FResourceObject* obj)
	{
		return m_resourceMgr.addResource(pszName, dwType, obj);
	}

	bool FMarkup::FMarkupParser::testResource(LPCTSTR pszName, DWORD dwType) const
	{
		return m_resourceMgr.testResource(pszName, dwType);
	}

	FResourceObject* FMarkup::FMarkupParser::getResource(LPCTSTR pszName, DWORD dwType) const
	{
		FASSERT(NULL != m_pRootWidget);

		FResourceObject *pObj = m_resourceMgr.getResource(pszName, dwType);
		if (NULL != pObj) return pObj;

		return m_pRootWidget->resourceManager().getResource(pszName, dwType);
	}

	Styles::FStyleObject* FMarkup::FMarkupParser::createStyle(LPCTSTR pszName) const
	{
		return Styles::FStyleObject::createStyle(pszName);
	}

	Widgets::FWidget* FMarkup::FMarkupParser::createWidget(LPCTSTR pszName) const
	{
		Widgets::FWidget* pWidgetObject = NULL;
		if (NULL != m_callback)
		{
			/** 优先调用创建控件回调函数 */
			pWidgetObject = m_callback(pszName, m_callback_parameter);
			if (NULL != pWidgetObject)
			{
				return pWidgetObject;
			}
		}

		FASSERT(NULL == pWidgetObject);
		pWidgetObject = Widgets::FWidget::createWidget(pszName);
		FASSERT(NULL != pWidgetObject);

		pWidgetObject->autoDelete(true);

		return pWidgetObject;
	}


	bool FMarkup::FMarkupParser::markup(LPCTSTR pszFileName, 
			Widgets::FWidget* pRootWidget, 
			FMARKUP_CREATEWIDGET_CALLBACK callback/* = NULL*/, LPVOID callback_parameter/* = NULL*/)
	{
		FASSERT(NULL != pszFileName);
		FASSERT(NULL != pRootWidget);

		m_pRootWidget = pRootWidget;
		m_callback = callback;
		m_callback_parameter = callback_parameter;

		bool bRet = false;
		m_strXmlPath = FMarkup::getResourcePath();

		if (FMarkup::isSkinPackageOpened())
		{
			/** 优先从zip打开 */
			int nNeedBuffer = FMarkup::unpackFile(pszFileName, NULL, 0);
			if (nNeedBuffer > 0)
			{
				char *pBuffer = new char[nNeedBuffer];
				FMarkup::unpackFile(pszFileName, pBuffer, nNeedBuffer);
				bRet = m_xmlDoc.load_buffer(pBuffer, nNeedBuffer);
				if (bRet)
				{
					m_resourceMgr = FResourceManager::pump(pszFileName);
				}
				fooSafeDeleteArray(pBuffer);
			}
		}

		if (!bRet)
		{
			/** 优先从XML根目录打开 */
			LPCTSTR pszFullPath = FPathHelper::pathHelper(FMarkup::getResourcePath(), pszFileName);
			if (NULL != pszFullPath)
			{
				bRet = m_xmlDoc.load_file(pszFullPath);
				if (bRet)
				{
					m_resourceMgr = FResourceManager::pump(pszFullPath);
					m_strXmlPath = pszFullPath;
					m_strXmlPath = m_strXmlPath.left(m_strXmlPath.reverseFind(_T('\\')));
				}
			}
		}

		if (!bRet)
		{
			/** 优先从实例根目录打开 */
			LPCTSTR pszFullPath = FPathHelper::pathHelper(Core::FApplication::getApplicationDirectory(), pszFileName);
			if (NULL != pszFullPath)
			{
				bRet = m_xmlDoc.load_file(pszFullPath);
				if (bRet)
				{
					m_resourceMgr = FResourceManager::pump(pszFullPath);

					m_strXmlPath = pszFullPath;
					m_strXmlPath = m_strXmlPath.left(m_strXmlPath.reverseFind(_T('\\')));
				}
			}
		}

		if (!bRet)
		{
			/** 尝试直接打开 */
			bRet = m_xmlDoc.load_file(pszFileName);
			if (bRet)
			{
				m_resourceMgr = FResourceManager::pump(pszFileName);

				m_strXmlPath = pszFileName;
				m_strXmlPath = m_strXmlPath.left(m_strXmlPath.reverseFind(_T('\\')));
			}
		}

		if (bRet)
		{
			m_pRootWidget->resourceManager().include(m_resourceMgr);
			return beginParse();
		}
		
		throw("解析XML失败，请检查是否语法错误！");
		return false;
	}

	bool FMarkup::FMarkupParser::markup(void* pStream, DWORD dwStreamLength, 
			Widgets::FWidget* pRootWidget, 
			FMARKUP_CREATEWIDGET_CALLBACK callback/* = NULL*/, LPVOID callback_parameter/* = NULL*/)
	{
		FASSERT(NULL != pStream);
		FASSERT(NULL != pRootWidget);

		m_pRootWidget = pRootWidget;
		m_callback = callback;
		m_callback_parameter = callback_parameter;

		bool bRet = false;
		m_strXmlPath = FMarkup::getResourcePath();

		bRet = m_xmlDoc.load_buffer((LPCTSTR)pStream, dwStreamLength);

		if (bRet)
		{
			m_pRootWidget->resourceManager().include(m_resourceMgr);
			return beginParse();
		}

		throw("解析XML失败，请检查是否语法错误！");
		return false;
	}

	bool FMarkup::FMarkupParser::beginParse(void)
	{
		switch (m_include)
		{
		case INCLUDE_RESOURCE:
			parseIncludes();
			parseStrings();
			parseFonts();
			parseColors();
			parseImages();
			parseCursors();
			parseStyles();
			break;

		case INCLUDE_WIDGETS:
		default:
			parseIncludes();
			parseStrings();
			parseFonts();
			parseColors();
			parseImages();
			parseCursors();
			parseStyles();
			parseWidgets();
			break;
		}
		
		return true;
	}

	bool FMarkup::FMarkupParser::parseIncludes(void)
	{
		/** <theme>
				<include>
					<include src="path" />
					<include>path</include>
				</include>
			</theme>
		*/
		bool bRet = false;
		pugi::xml_node nodeRoot = m_xmlDoc.child(MARKUP_NODE_THEMEROOT).child(MARKUP_NODE_INCLUDEROOT);
		if (nodeRoot.empty())
		{
			return false;
		}

		for (pugi::xml_node nodeChild = nodeRoot.first_child(); nodeChild; nodeChild = nodeChild.next_sibling())
		{
			if (_tcscmp(nodeChild.name(), MARKUP_NODE_INCLUDE) == 0)
			{
				Core::FString strSrc = nodeChild.attribute(_T("src")).value();
				if (strSrc.isEmpty()) strSrc = nodeChild.text().get();
				if (!strSrc.isEmpty())
				{
					FMarkup::FMarkupParser parser;
					parser.m_include = INCLUDE_RESOURCE;
					parser.markup(strSrc, m_pRootWidget, m_callback, m_callback_parameter);
				}
			}
		}

		return bRet;
	}

	bool FMarkup::FMarkupParser::parseStrings(void)
	{
		/** <theme>
				<string>
					<string attr="value" />
					<string>text</string>
				</string>
			</theme>
		*/
		bool bRet = false;
		pugi::xml_node nodeRoot = m_xmlDoc.child(MARKUP_NODE_THEMEROOT).child(MARKUP_NODE_STRINGROOT);
		if (nodeRoot.empty())
		{
			return false;
		}

		for (pugi::xml_node nodeChild = nodeRoot.child(MARKUP_NODE_STRING); 
			nodeChild; 
			nodeChild = nodeChild.next_sibling(MARKUP_NODE_STRING))
		{
			FStringResource *pStringRes = new FStringResource;

			FOBJECT_CAST(FMarkupObject*, pStringRes)->beginMarkup(this);

			for (pugi::xml_attribute attr = nodeChild.first_attribute(); attr; attr = attr.next_attribute())
			{
				FOBJECT_CAST(FMarkupObject*, pStringRes)->setAttribute(this, attr.name(), attr.value());
			}
			Core::FString strText = nodeChild.text().get();
			if (!strText.isEmpty()) FOBJECT_CAST(FMarkupObject*, pStringRes)->setAttribute(this, _T(""), strText);

			FOBJECT_CAST(FMarkupObject*, pStringRes)->endMarkup(this);

			if (!m_resourceMgr.addResource(pStringRes->getName(), pStringRes->getType(), pStringRes))
			{
				fooSafeDelete(pStringRes);
			}
		}

		return bRet;
	}

	bool FMarkup::FMarkupParser::parseFonts(void)
	{
		/** <theme>
				<font>
					<font attr="value" />
				</font>
			</theme>
		*/
		bool bRet = false;
		pugi::xml_node nodeRoot = m_xmlDoc.child(MARKUP_NODE_THEMEROOT).child(MARKUP_NODE_FONTROOT);
		if (nodeRoot.empty())
		{
			return false;
		}

		for (pugi::xml_node nodeChild = nodeRoot.child(MARKUP_NODE_FONT); 
			nodeChild; 
			nodeChild = nodeChild.next_sibling(MARKUP_NODE_FONT))
		{
			FFontResource *pFontRes = new FFontResource;

			FOBJECT_CAST(FMarkupObject*, pFontRes)->beginMarkup(this);

			for (pugi::xml_attribute attr = nodeChild.first_attribute(); attr; attr = attr.next_attribute())
			{
				FOBJECT_CAST(FMarkupObject*, pFontRes)->setAttribute(this, attr.name(), attr.value());
			}

			FOBJECT_CAST(FMarkupObject*, pFontRes)->endMarkup(this);

			if (!m_resourceMgr.addResource(pFontRes->getName(), pFontRes->getType(), pFontRes))
			{
				fooSafeDelete(pFontRes);
			}
		}

		return bRet;
	}

	bool FMarkup::FMarkupParser::parseColors(void)
	{
		/** <theme>
				<color>
					<color attr="value" />
				</color>
			</theme>
		*/
		bool bRet = false;
		pugi::xml_node nodeRoot = m_xmlDoc.child(MARKUP_NODE_THEMEROOT).child(MARKUP_NODE_COLORROOT);
		if (nodeRoot.empty())
		{
			return false;
		}

		for (pugi::xml_node nodeChild = nodeRoot.child(MARKUP_NODE_COLOR); 
			nodeChild; 
			nodeChild = nodeChild.next_sibling(MARKUP_NODE_COLOR))
		{
			FColorResource *pColorRes = new FColorResource;

			FOBJECT_CAST(FMarkupObject*, pColorRes)->beginMarkup(this);

			for (pugi::xml_attribute attr = nodeChild.first_attribute(); attr; attr = attr.next_attribute())
			{
				FOBJECT_CAST(FMarkupObject*, pColorRes)->setAttribute(this, attr.name(), attr.value());
			}

			FOBJECT_CAST(FMarkupObject*, pColorRes)->endMarkup(this);

			if (!m_resourceMgr.addResource(pColorRes->getName(), pColorRes->getType(), pColorRes))
			{
				fooSafeDelete(pColorRes);
			}
		}

		return bRet;
	}

	bool FMarkup::FMarkupParser::parseImages(void)
	{
		/** <theme>
				<image>
					<image attr="value" />
				</image>
			</theme>
		*/
		bool bRet = false;
		pugi::xml_node nodeRoot = m_xmlDoc.child(MARKUP_NODE_THEMEROOT).child(MARKUP_NODE_IMAGEROOT);
		if (nodeRoot.empty())
		{
			return false;
		}

		for (pugi::xml_node nodeChild = nodeRoot.child(MARKUP_NODE_IMAGE); 
			nodeChild; 
			nodeChild = nodeChild.next_sibling(MARKUP_NODE_IMAGE))
		{
			FImageResource *pImageRes = new FImageResource;

			FOBJECT_CAST(FMarkupObject*, pImageRes)->beginMarkup(this);

			for (pugi::xml_attribute attr = nodeChild.first_attribute(); attr; attr = attr.next_attribute())
			{
				FOBJECT_CAST(FMarkupObject*, pImageRes)->setAttribute(this, attr.name(), attr.value());
			}

			FOBJECT_CAST(FMarkupObject*, pImageRes)->endMarkup(this);

			if (!m_resourceMgr.addResource(pImageRes->getName(), pImageRes->getType(), pImageRes))
			{
				fooSafeDelete(pImageRes);
			}
		}

		return bRet;
	}

	bool FMarkup::FMarkupParser::parseCursors(void)
	{
		/** <theme>
				<cursor>
					<cursor attr="value" />
				</cursor>
			</theme>
		*/
		bool bRet = false;
		pugi::xml_node nodeRoot = m_xmlDoc.child(MARKUP_NODE_THEMEROOT).child(MARKUP_NODE_CURSORROOT);
		if (nodeRoot.empty())
		{
			return false;
		}

		for (pugi::xml_node nodeChild = nodeRoot.child(MARKUP_NODE_CURSOR); 
			nodeChild; 
			nodeChild = nodeChild.next_sibling(MARKUP_NODE_CURSOR))
		{
			FCursorResource *pCursorRes = new FCursorResource;

			FOBJECT_CAST(FMarkupObject*, pCursorRes)->beginMarkup(this);

			for (pugi::xml_attribute attr = nodeChild.first_attribute(); attr; attr = attr.next_attribute())
			{
				FOBJECT_CAST(FMarkupObject*, pCursorRes)->setAttribute(this, attr.name(), attr.value());
			}

			FOBJECT_CAST(FMarkupObject*, pCursorRes)->endMarkup(this);

			if (!m_resourceMgr.addResource(pCursorRes->getName(), pCursorRes->getType(), pCursorRes))
			{
				fooSafeDelete(pCursorRes);
			}
		}

		return bRet;
	}

	bool FMarkup::FMarkupParser::parseStyles(void)
	{
		/** <theme>
				<style>
					<style attr="value" />
				</style>
			</theme>
		*/
		bool bRet = false;
		pugi::xml_node nodeRoot = m_xmlDoc.child(MARKUP_NODE_THEMEROOT).child(MARKUP_NODE_STYLEROOT);
		if (nodeRoot.empty())
		{
			return false;
		}

		for (pugi::xml_node nodeChild = nodeRoot.child(MARKUP_NODE_STYLE); 
			nodeChild; 
			nodeChild = nodeChild.next_sibling(MARKUP_NODE_STYLE))
		{
			FStyleSheetResource *pStyleSheetRes = new FStyleSheetResource;

			FOBJECT_CAST(FMarkupObject*, pStyleSheetRes)->beginMarkup(this);

			for (pugi::xml_attribute attr = nodeChild.first_attribute(); attr; attr = attr.next_attribute())
			{
				FOBJECT_CAST(FMarkupObject*, pStyleSheetRes)->setAttribute(this, attr.name(), attr.value());
			}

			for (pugi::xml_node node = nodeChild.first_child(); node; node = node.next_sibling())
			{
				parseStyleChildren(node, pStyleSheetRes);
			}

			FOBJECT_CAST(FMarkupObject*, pStyleSheetRes)->endMarkup(this);

			if (!m_resourceMgr.addResource(pStyleSheetRes->getName(), pStyleSheetRes->getType(), pStyleSheetRes))
			{
				fooSafeDelete(pStyleSheetRes);
			}
		}

		return bRet;
	}

	bool FMarkup::FMarkupParser::parseStyleChildren(pugi::xml_node& node, Markup::FStyleSheetResource* parent)
	{
		/** 创建风格对象 */
		Styles::FStyleObject *pStyleObject = Styles::FStyleObject::createStyle(node.name());
		if (NULL == pStyleObject)
		{
			FASSERT(false);
			return false;
		}
		
		FOBJECT_CAST(FMarkupObject*, pStyleObject)->beginMarkup(this);

		/** 设置属性 */
		for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute())
		{
			FOBJECT_CAST(FMarkupObject*, pStyleObject)->setAttribute(this, attr.name(), attr.value());
		}
		Core::FString strText = node.text().get();
		if (!strText.isEmpty()) FOBJECT_CAST(FMarkupObject*, pStyleObject)->setAttribute(this, _T(""), strText);

		FOBJECT_CAST(FMarkupObject*, pStyleObject)->endMarkup(this);

		/** 插入到风格容器 */
		if (!parent->addStyle(pStyleObject))
		{
			fooSafeDelete(pStyleObject);
			return false;
		}

		return true;
	}

	bool FMarkup::FMarkupParser::parseWidgets(void)
	{
		/** <theme>
				<rootwidget attr="value">
					
				</rootwidget>
			</theme>
		*/
		pugi::xml_node nodeRoot = m_xmlDoc.child(MARKUP_NODE_THEMEROOT).child(MARKUP_NODE_WIDGETROOT);
		if (nodeRoot.empty())
		{
			return false;
		}

		FOBJECT_CAST(FMarkupObject*, m_pRootWidget)->beginMarkup(this);

		/** 设置风格 */
		for (pugi::xml_attribute attr = nodeRoot.first_attribute(); attr; attr = attr.next_attribute())
		{
			FOBJECT_CAST(FMarkupObject*, m_pRootWidget)->setAttribute(this, attr.name(), attr.value());
		}
		Core::FString strText = nodeRoot.text().get();
		if (!strText.isEmpty()) FOBJECT_CAST(FMarkupObject*, m_pRootWidget)->setAttribute(this, _T(""), strText);

		/** 解析子控件 */
		for (pugi::xml_node node = nodeRoot.first_child(); node; node = node.next_sibling())
		{
			parseWidgetChildren(node, m_pRootWidget);
		}

		FOBJECT_CAST(FMarkupObject*, m_pRootWidget)->endMarkup(this);

		return true;
	}

	bool FMarkup::FMarkupParser::parseWidgetChildren(pugi::xml_node& node, Widgets::FWidget* parent)
	{
		FASSERT(NULL != parent);

		if (_tcscmp(node.name(), MARKUP_NODE_INCLUDE) == 0)
		{
			Core::FString strSrc = node.attribute(_T("src")).value();
			if (strSrc.isEmpty()) strSrc = node.text().get();
			if (!strSrc.isEmpty())
			{
				FMarkup::FMarkupParser parser;
				parser.m_include = INCLUDE_WIDGETS;
				parser.markup(strSrc, parent, m_callback, m_callback_parameter);
			}
		}
		else if (_tcscmp(node.name(), MARKUP_NODE_ATTRIBUTE) == 0)
		{
			/** 分析<attribute />节点 */
			/** 设置属性 */

			for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute())
			{
				FOBJECT_CAST(FMarkupObject*, parent)->setAttribute(this, attr.name(), attr.value());
			}
		}
		else
		{
			/** 创建控件对象 */
			Widgets::FWidget *pWidget = NULL;
			/** 优先调用父类创建控件对象 */
			pWidget = parent->createChildWidget(node.name());
			/** 调用xml解析类的控件创建函数 */
			if (NULL == pWidget) pWidget = createWidget(node.name());

			if (NULL == pWidget)
			{
				FASSERT(false);
				return false;
			}

			if (!parent->markupAddWidget(this, pWidget))
			{
				fooSafeDelete(pWidget);
				return false;
			}

			FOBJECT_CAST(FMarkupObject*, pWidget)->beginMarkup(this);

			for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute())
			{
				FOBJECT_CAST(FMarkupObject*, pWidget)->setAttribute(this, attr.name(), attr.value());
			}
			Core::FString strText = node.text().get();
			if (!strText.isEmpty()) FOBJECT_CAST(FMarkupObject*, pWidget)->setAttribute(this, _T(""), strText);

			for (pugi::xml_node nodeChild = node.first_child(); nodeChild; nodeChild = nodeChild.next_sibling())
			{
				parseWidgetChildren(nodeChild, pWidget);
			}

			FOBJECT_CAST(FMarkupObject*, pWidget)->endMarkup(this);
		}

		return true;
	}

}} // !namespace
#endif // !FOOUI_SUPPORT_PUGIXML