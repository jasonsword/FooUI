/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FMarkup.h
* ������		FMarkup
* �������ڣ�	2013-06-14
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
		/** �������Ϊtrue���ؼ��ر�ʱ���Զ����� */
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
		�ж�XML�����У� �Ƿ����ĳ���������� ���磺
		fooMarkupCheckNameFlag(_T("hover | focus"), _T("focus"))������true
		���ǣ� �������������false
		fooMarkupCheckNameFlag(_T("myhover | focus"), _T("hover"))������false
	*/
	bool FOOUI_API fooMarkupCheckNameFlag(LPCTSTR pszAttrValue, LPCTSTR pszCheck);

	/** fooMarkupParseBoolean:
		���ַ�������Ϊ����ֵ
		����ַ���Ϊtrue��yes������true�����򷵻�false
	*/
	bool FOOUI_API fooMarkupParseBoolean(LPCTSTR pszAttrValue);

	/** fooMarkupParsePoint��
		���ַ�������ΪPoint
	*/
	bool FOOUI_API fooMarkupParsePoint(LPPOINT lpPoint, LPCTSTR pszAttrValue);

	/** fooMarkupParseSize��
		���ַ�������ΪSize
	*/
	bool FOOUI_API fooMarkupParseSize(LPSIZE lpSize, LPCTSTR pszAttrValue);

	/** fooMarkupParseRect��
		���ַ�������ΪRECT���ַ��������㹻�ĸ���Ա"l,t,r,b"

		fooMarkupParseEdge��
		���ַ�������ΪEdge���ַ��������㹻һ����Ա����"x"����ΪRECT(x, x, x, x)

		fooMarkupParseRectWH��
		���ַ�������ΪRECT���ַ��������㹻�ĸ���Ա"l,t,w,h"����ΪRECT(l, t, l + w, t + h)
	*/
	bool FOOUI_API fooMarkupParseRect(LPRECT lpRect, LPCTSTR pszAttrValue);
	bool FOOUI_API fooMarkupParseEdge(LPRECT lpRect, LPCTSTR pszAttrValue);
	bool FOOUI_API fooMarkupParseRectWH(LPRECT lpRect, LPCTSTR pszAttrValue);

	/** fooMarkupParseState��
		���ַ�������Ϊ״ֵ̬
	*/
	DWORD FOOUI_API fooMarkupParseState(LPCTSTR pszAttrValue);

	/** fooMarkupParseDock��
		���ַ�������ΪDockֵ
	*/
	DWORD FOOUI_API fooMarkupParseDock(LPCTSTR pszAttrValue);

	/** fooMarkupParseAlign��
		���ַ�������ΪAlignֵ
	*/
	DWORD FOOUI_API fooMarkupParseAlign(LPCTSTR pszAttrValue);

	/** fooMarkupParseRole��
		���ַ�������ΪRoleֵ
	*/
	DWORD FOOUI_API fooMarkupParseRole(LPCTSTR pszAttrValue);
}} // !namespace

#endif // !FMARKUPDEF_H