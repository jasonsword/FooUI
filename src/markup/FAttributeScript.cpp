/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FAttributeScript.h
* 描述：		FAttributeScript
* 创建日期：	2013-06-26
*
*/

#include "StdAfx.h"
#include "markup/FAttributeScript.h"

#define FATTRIBUTESCRIPT_INLINE FOOINLINE

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FAttributeScriptPrivate
	class FAttributeScript::FAttributeScriptPrivate
	{
	public:
		FAttributeScriptPrivate(void);
		~FAttributeScriptPrivate(void);

	public:
		bool parseAttributeScript(LPCTSTR pszAttributeScript);
		FATTRIBUTESCRIPT* firstAttribute(void) const;
		bool clear(void);
		bool isEmpty(void) const;

	public:
		static bool insertAttr(FATTRIBUTESCRIPT* parent, FATTRIBUTESCRIPT* attr);
		static bool deleteAttr(FATTRIBUTESCRIPT* pAttr);

	private:
		FATTRIBUTESCRIPT* m_pFirstAttribute;
	};

	//////////////////////////////////////////////////////////////////////////
	// FAttributeScriptStockT
	#define FATTRIBUTESCRIPTSTOCK_LENGTH 255
	template <class T>
	class FAttributeScriptStockT
	{
	public:
		FAttributeScriptStockT(void) : top(0)
		{ }

		bool isEmpty(void) const { return (0 == top); }

		bool push(T element)
		{
			if (FATTRIBUTESCRIPTSTOCK_LENGTH == top)
			{
				FASSERT(false);
				return false;
			}
			elements[top++] = element;
			return true;
		}

		bool pop(void)
		{
			if (isEmpty())
			{
				FASSERT(false);
				return false;
			}

			top--;
			return true;
		}

		T getTop(void) const
		{
			if (isEmpty())
			{
				FASSERT(false);
				return false;
			}

			return elements[top - 1];
		}

	private:
		int top;
		T elements[FATTRIBUTESCRIPTSTOCK_LENGTH];
	};

	// 
	FATTRIBUTESCRIPT_INLINE FAttributeScript::FAttributeScriptPrivate::FAttributeScriptPrivate(void) : 
		m_pFirstAttribute(NULL)
	{
	}

	FATTRIBUTESCRIPT_INLINE FAttributeScript::FAttributeScriptPrivate::~FAttributeScriptPrivate(void)
	{
		clear();
	}

	FATTRIBUTESCRIPT_INLINE bool FAttributeScript::FAttributeScriptPrivate::parseAttributeScript(LPCTSTR pszAttributeScript)
	{
		FAttributeScriptStockT<TCHAR> stockChar;
		FAttributeScriptStockT<FATTRIBUTESCRIPT*> stockAttr;

		clear();

		m_pFirstAttribute = new FATTRIBUTESCRIPT;
		stockAttr.push(m_pFirstAttribute);

		LPCTSTR pszTraversal = pszAttributeScript;
		int nPos = 0, nToken = 0;
		for (TCHAR tch = *pszTraversal; tch; tch = *(++pszTraversal), nPos++)
		{
			switch (tch)
			{
			case _T('\"'):
			case _T('\''):
				break;

			case _T('\t'):
			case _T('\r'):
			case _T('\n'):
			//case _T(' '):
				nToken = (0 == nPos) ? 0 : nPos + 1;
				continue;
				break;

			case _T('{'):
			case _T('['):
			case _T('<'):
			case _T('('):
				{
					FATTRIBUTESCRIPT *pTopAttr = stockAttr.getTop();
					FASSERT(NULL != pTopAttr);

					FATTRIBUTESCRIPT *pAttr = new FATTRIBUTESCRIPT;
					if (!insertAttr(pTopAttr, pAttr))
					{
						FASSERT(false);
						return false;
					}

					_tcsncpy_s(pAttr->name.getBuffer(nPos - nToken + 1), nPos - nToken + 1, 
								pszAttributeScript + nToken, nPos - nToken);
					pAttr->name.releaseBuffer();
					pAttr->name.trim(_T(' '));

					stockChar.push(tch);
					stockAttr.push(pAttr);

					nToken = (0 == nPos) ? 0 : nPos + 1;
				}
				break;

			case _T('}'):
			case _T(']'):
			case _T('>'):
			case _T(')'):
				{
					if (_T('{') != stockChar.getTop() && _T('}') == tch)
					{
						FASSERT(false);
						return false;
					}
					if (_T('[') != stockChar.getTop() && _T(']') == tch)
					{
						FASSERT(false);
						return false;
					}
					if (_T('<') != stockChar.getTop() && _T('>') == tch)
					{
						FASSERT(false);
						return false;
					}
					if (_T('(') != stockChar.getTop() && _T(')') == tch)
					{
						FASSERT(false);
						return false;
					}

					FATTRIBUTESCRIPT *pTopAttr = stockAttr.getTop();
					_tcsncpy_s(pTopAttr->value.getBuffer(nPos - nToken + 1), 
							nPos - nToken + 1, 
							pszAttributeScript + nToken, 
							nPos - nToken);
							pTopAttr->value.releaseBuffer();

					stockChar.pop();
					stockAttr.pop();

					nToken = (0 == nPos) ? 0 : nPos + 1;

				}
				break;

			default:
				break;
			}
		}

		if (!stockChar.isEmpty())
		{
			/** {[<(左右不匹配（堆栈不平衡） */
			FASSERT(false);
			return false;
		}

		return true;
	}

	FATTRIBUTESCRIPT_INLINE FATTRIBUTESCRIPT* FAttributeScript::FAttributeScriptPrivate::firstAttribute(void) const
	{
		if (NULL == m_pFirstAttribute)
		{
			return NULL;
		}
		return m_pFirstAttribute->children;
	}

	FATTRIBUTESCRIPT_INLINE bool FAttributeScript::FAttributeScriptPrivate::clear(void)
	{
		if (NULL == m_pFirstAttribute)
		{
			return false;
		}

		FATTRIBUTESCRIPT *pChild = m_pFirstAttribute;
		while (NULL != pChild)
		{
			FATTRIBUTESCRIPT *pTemp = pChild;
			pChild = pChild->next;

			deleteAttr(pTemp);
		}

		m_pFirstAttribute = NULL;
		return true;
	}

	FATTRIBUTESCRIPT_INLINE bool FAttributeScript::FAttributeScriptPrivate::isEmpty(void) const
	{
		return (NULL == m_pFirstAttribute);
	}

	FATTRIBUTESCRIPT_INLINE /*static*/ bool FAttributeScript::FAttributeScriptPrivate::insertAttr(FATTRIBUTESCRIPT* parent, FATTRIBUTESCRIPT* attr)
	{
		if (NULL == parent || NULL == attr)
		{
			FASSERT(false);
			return false;
		}

		if (NULL == parent->children)
		{
			parent->children = attr;
			return true;
		}
		else
		{
			FATTRIBUTESCRIPT *pNext = parent->children;
			while (NULL != pNext)
			{
				if (NULL == pNext->next)
				{
					 pNext->next = attr;
					 return true;
				}

				pNext = pNext->next;
			}
		}

		return false;
	}

	FATTRIBUTESCRIPT_INLINE /*static*/ bool FAttributeScript::FAttributeScriptPrivate::deleteAttr(FATTRIBUTESCRIPT* pAttr)
	{
		FASSERT(NULL != pAttr);

		FATTRIBUTESCRIPT *pChild = pAttr->children;
		while (NULL != pChild)
		{
			FATTRIBUTESCRIPT *pTemp = pChild;
			pChild = pChild->next;

			deleteAttr(pTemp);
		}

		fooSafeDelete(pAttr);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// FAttributeScript
	FATTRIBUTESCRIPT_INLINE FAttributeScript::FAttributeScript(void)
	{
		m_attributeScriptPrivate = new FAttributeScriptPrivate;
	}

	FATTRIBUTESCRIPT_INLINE FAttributeScript::FAttributeScript(LPCTSTR pszAttributeScript)
	{
		m_attributeScriptPrivate = new FAttributeScriptPrivate;
		if (!m_attributeScriptPrivate->parseAttributeScript(pszAttributeScript))
		{
			FASSERT(false);
		}
	}

	FATTRIBUTESCRIPT_INLINE FAttributeScript::~FAttributeScript(void)
	{
		FASSERT(NULL != m_attributeScriptPrivate);
		fooSafeDelete(m_attributeScriptPrivate);
	}

	FATTRIBUTESCRIPT_INLINE bool FAttributeScript::parseAttributeScript(LPCTSTR pszAttributeScript)
	{
		FASSERT(NULL != m_attributeScriptPrivate);
		return m_attributeScriptPrivate->parseAttributeScript(pszAttributeScript);
	}

	FATTRIBUTESCRIPT_INLINE FATTRIBUTESCRIPT* FAttributeScript::firstAttribute(void) const
	{
		FASSERT(NULL != m_attributeScriptPrivate);
		return m_attributeScriptPrivate->firstAttribute();
	}

}} // !namespace
