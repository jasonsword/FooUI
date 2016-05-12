/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FMarkup.h
* 描述：		FMarkup
* 创建日期：	2013-06-14
*
*/

#ifndef FMARKUPDEF_H
#define FMARKUPDEF_H

#include "core/FBase.h"

namespace FooUI { namespace Markup {

	class FMarkup;
	//////////////////////////////////////////////////////////////////////////
	// FMarkupObject
	class FOOUI_LIB FMarkupObject
	{
	public:
		enum eFMarkupObjectFlag
		{
			MOF_NONE = 0x00000000, 
			MOF_AUTODELETE = 0x00000001, 
		};

		FMarkupObject(void);
		virtual ~FMarkupObject(void) { }

	public:
		/** 如果设置为true，控件关闭时会自动析构 */
		void autoDelete(bool bAutoDelete);
		bool isAutoDelete(void) const;

	public:
		virtual bool parseAttribute(FMarkup* pMarkup, LPCTSTR pszValue);

	private:
		bool parseAttributeScript(FMarkup* pMarkup, LPCTSTR pszAttributeScript);

	public:
		virtual bool beginMarkup(FMarkup* pMarkup) = 0;
		virtual bool setAttribute(FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue) = 0;
		virtual bool endMarkup(FMarkup* pMarkup) = 0;

	private:
		DWORD m_dwMarkupObjectFlag;
	};

	//////////////////////////////////////////////////////////////////////////
	// Methods

	/** fooMarkupCheckNameFlag:
		判断XML属性中， 是否包含某个特征符， 例如：
		fooMarkupCheckNameFlag(_T("hover | focus"), _T("focus"))将返回true
		但是， 以下情况将返回false
		fooMarkupCheckNameFlag(_T("myhover | focus"), _T("hover"))将返回false
	*/
	bool FOOUI_API fooMarkupCheckNameFlag(LPCTSTR pszAttrValue, LPCTSTR pszCheck);

	/** fooMarkupParseBoolean:
		将字符串解析为布尔值
		如果字符串为true或yes将返回true，否则返回false
	*/
	bool FOOUI_API fooMarkupParseBoolean(LPCTSTR pszAttrValue);

	/** fooMarkupParsePoint：
		将字符串解析为Point
	*/
	bool FOOUI_API fooMarkupParsePoint(LPPOINT lpPoint, LPCTSTR pszAttrValue);

	/** fooMarkupParseSize：
		将字符串解析为Size
	*/
	bool FOOUI_API fooMarkupParseSize(LPSIZE lpSize, LPCTSTR pszAttrValue);

	/** fooMarkupParseRect：
		将字符串解析为RECT，字符串必须足够四个成员"l,t,r,b"

		fooMarkupParseEdge：
		将字符串解析为Edge，字符串必须足够一个成员以上"x"解析为RECT(x, x, x, x)

		fooMarkupParseRectWH：
		将字符串解析为RECT，字符串必须足够四个成员"l,t,w,h"解析为RECT(l, t, l + w, t + h)
	*/
	bool FOOUI_API fooMarkupParseRect(LPRECT lpRect, LPCTSTR pszAttrValue);
	bool FOOUI_API fooMarkupParseEdge(LPRECT lpRect, LPCTSTR pszAttrValue);
	bool FOOUI_API fooMarkupParseRectWH(LPRECT lpRect, LPCTSTR pszAttrValue);

	/** fooMarkupParseState：
		将字符串解析为状态值
	*/
	DWORD FOOUI_API fooMarkupParseState(LPCTSTR pszAttrValue);

	/** fooMarkupParseDock：
		将字符串解析为Dock值
	*/
	DWORD FOOUI_API fooMarkupParseDock(LPCTSTR pszAttrValue);

	/** fooMarkupParseAlign：
		将字符串解析为Align值
	*/
	DWORD FOOUI_API fooMarkupParseAlign(LPCTSTR pszAttrValue);

	/** fooMarkupParseRole：
		将字符串解析为Role值
	*/
	DWORD FOOUI_API fooMarkupParseRole(LPCTSTR pszAttrValue);
}} // !namespace

#endif // !FMARKUPDEF_H