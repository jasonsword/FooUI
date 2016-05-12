/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FMarkup.cpp
* 描述：		FMarkup
* 创建日期：	2013-06-19
*
*/

#include "StdAfx.h"

#include "core/FApplication.h"
#include "widgets/FWidget.h"
#include "markup/FMarkup.h"
#include "markup/FAttributeScript.h"

#include "styles/FStyleDef.h"

#include "F3rd_p.h"
#include "markup/ResourcePackage/zip_utils/FZipPackage_p.h"

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#define FMARKUP_INLINE FOOINLINE

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FPathHelper
	FMARKUP_INLINE /*static*/ LPCTSTR FPathHelper::pathHelper(LPCTSTR pszPath)
	{
		if (_tcslen(FMarkup::getResourcePath()) > 0)
		{
			return FPathHelper::pathHelper(FMarkup::getResourcePath(), pszPath);
		}
		
		return FPathHelper::pathHelper(Core::FApplication::getApplicationDirectory(), pszPath);
	}

	FMARKUP_INLINE /*static*/ LPCTSTR FPathHelper::pathHelper(LPCTSTR pszRootPath, LPCTSTR pszPath)
	{
		FASSERT(NULL != pszRootPath);
		FASSERT(NULL != pszPath);

		static TCHAR tchFullPath[MAX_PATH] = {0};

		Core::FString strTmpPath;
		if (_T(':') == pszPath[1]
		|| NULL == pszRootPath 
			|| _tcslen(pszRootPath) == 0)
		{
			strTmpPath = pszPath;
		}
		else if (pszRootPath[_tcslen(pszRootPath) - 1] != _T('\\') 
			&& pszPath[0] != _T('\\'))
		{
			strTmpPath = pszRootPath;
			strTmpPath += _T('\\');
			strTmpPath += pszPath;
		}
		else if (pszRootPath[_tcslen(pszRootPath) - 1] == _T('\\') 
			&& pszPath[0] == _T('\\'))
		{
			strTmpPath = pszRootPath;
			strTmpPath += (LPCTSTR)(pszPath + 1);
		}
		else
		{
			strTmpPath = pszRootPath;
			strTmpPath += pszPath;
		}

		if (::PathCanonicalize(tchFullPath, strTmpPath))
		{
			return tchFullPath;
		}

		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	// FMarkup
	static Core::FString s_markupResourcePath;
	FMARKUP_INLINE /*static*/ void FMarkup::setResourcePath(LPCTSTR pszResourcePath)
	{
		if (NULL == pszResourcePath)
		{
			FASSERT(false);
			s_markupResourcePath = _T("");
			return;
		}

		LPCTSTR pszFullPath = NULL;
		if (_tcslen(Core::FApplication::getApplicationDirectory()) > 0)
		{
			pszFullPath = FPathHelper::pathHelper(Core::FApplication::getApplicationDirectory(), pszResourcePath);
		}
		else
		{
			pszFullPath = pszResourcePath;
		}

		s_markupResourcePath = (NULL != pszFullPath) ? pszFullPath : _T("");
	}

	FMARKUP_INLINE /*static*/ LPCTSTR FMarkup::getResourcePath(void)
	{
		return s_markupResourcePath;
	}

	/*static*/ CSkinPackage* FMarkup::getSkinPackage(void)
	{
		return FModule::getSkinPackage();
	}

	/*static*/ bool FMarkup::isSkinPackageOpened(void)
	{
		if (NULL != getSkinPackage())
		{
			return getSkinPackage()->isOpened();
		}
		else
		{
			return false;
		}
	}

	/*static*/ int FMarkup::unpackFile(LPCTSTR pszFileName, void* buffer, int bufferLength)
	{
		if (!isSkinPackageOpened())
		{
			FASSERT(FALSE);
			return -1;
		}

		return getSkinPackage()->unpackFile(pszFileName, buffer, bufferLength);
	}

	FMARKUP_INLINE /*static*/ bool FMarkup::openZipSkinPackage(LPCTSTR pszFileName, LPCTSTR pszPassword/* = NULL*/)
	{
		FZipPackage* pZipPackage = new FZipPackage;

		LPCTSTR pszFullPath = FPathHelper::pathHelper(pszFileName);
		
		if (pZipPackage->open(pszFullPath))
		{
			FModule::destroyResourcePackage();
			FModule::setResourcePackage(pZipPackage);

			return true;
		}
		else if (pZipPackage->open(pszFileName))
		{
			FModule::destroyResourcePackage();
			FModule::setResourcePackage(pZipPackage);

			return true;
		}
		else
		{
			return false;
		}
	}

	FMARKUP_INLINE /*static*/ bool FMarkup::openZipSkinPackage(void* pStream, int nStreamLength, LPCTSTR pszPassword/* = NULL*/)
	{
		FZipPackage* pZipPackage = new FZipPackage;
		if (pZipPackage->open(pStream, nStreamLength, pszPassword))
		{
			FModule::destroyResourcePackage();
			FModule::setResourcePackage(pZipPackage);

			return true;
		}
		else
		{
			return false;
		}
	}

	FMARKUP_INLINE /*static*/ bool FMarkup::openZipSkinPackageFromResource(LPCTSTR lpResName, LPCTSTR lpResType, 
																LPCTSTR pszPassword/* = NULL*/)
	{
		return FMarkup::openZipSkinPackageFromResource(Core::FApplication::getInstance(), lpResName, lpResType, pszPassword);
	}

	FMARKUP_INLINE /*static*/ bool FMarkup::openZipSkinPackageFromResource(HMODULE hResModule, LPCTSTR lpResName, 
																LPCTSTR lpResType, LPCTSTR pszPassword/* = NULL*/)
	{
		HRSRC hrSrc = ::FindResource(hResModule, lpResName, lpResType);
		if (NULL == hrSrc)
		{
			return false;
		}

		DWORD dwSize = ::SizeofResource(hResModule, hrSrc);
		HGLOBAL hGlobal = ::LoadResource(hResModule, hrSrc);
		if (NULL == hGlobal)
		{
			return false;
		}

		bool bRet = FMarkup::openZipSkinPackage(::LockResource(hGlobal), dwSize, pszPassword);
		//::UnlockResource(hGlobal);
		::FreeResource(hGlobal);

		return bRet;
	}

	FMARKUP_INLINE /*static*/ bool FMarkup::markup(LPCTSTR pszFileName, 
		Widgets::FWidget* pRootWidget, FMARKUP_CREATEWIDGET_CALLBACK callback, LPVOID callback_parameter)
	{
		FMarkupParser parser;
		return parser.markup(pszFileName, pRootWidget, callback, callback_parameter);
	}

	FMARKUP_INLINE /*static*/ bool FMarkup::markup(void* pStream, DWORD dwStreamLength, 
		Widgets::FWidget* pRootWidget, FMARKUP_CREATEWIDGET_CALLBACK callback, LPVOID callback_parameter)
	{
		FMarkupParser parser;
		return parser.markup(pStream, dwStreamLength, pRootWidget, callback, callback_parameter);
	}
	
	//////////////////////////////////////////////////////////////////////////
	// FMarkupObject
	FMARKUP_INLINE FMarkupObject::FMarkupObject(void) : 
		m_dwMarkupObjectFlag(MOF_NONE)
	{
	}

	FMARKUP_INLINE void FMarkupObject::autoDelete(bool bAutoDelete)
	{
		if (bAutoDelete)
			m_dwMarkupObjectFlag |= MOF_AUTODELETE;
		else
			m_dwMarkupObjectFlag &= ~MOF_AUTODELETE;
	}
	FMARKUP_INLINE bool FMarkupObject::isAutoDelete(void) const
	{
		return fooTestAttribute(m_dwMarkupObjectFlag, MOF_AUTODELETE);
	}

	FMARKUP_INLINE bool FMarkupObject::parseAttribute(FMarkup* pMarkup, LPCTSTR pszValue)
	{
		return parseAttributeScript(pMarkup, pszValue);
	}

	FMARKUP_INLINE bool FMarkupObject::parseAttributeScript(FMarkup* pMarkup, LPCTSTR pszAttributeScript)
	{
		FAttributeScript attrScript(pszAttributeScript);
		FATTRIBUTESCRIPT *pAttr = attrScript.firstAttribute();
		//这里进行数据解析记录
		if (NULL != pAttr)
		{
			beginMarkup(pMarkup);
			while (NULL != pAttr)
			{
				setAttribute(pMarkup, pAttr->name, pAttr->value);
				pAttr = pAttr->next;
			}
			endMarkup(pMarkup);

			return true;
		}

		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// Methods
	/** Dock停靠位置 */
#define FOODOCK_NAME_NONE	_T("none")
#define FOODOCK_NAME_LEFT	_T("left")
#define FOODOCK_NAME_TOP	_T("top")
#define FOODOCK_NAME_RIGHT	_T("right")
#define FOODOCK_NAME_BOTTOM	_T("bottom")
#define FOODOCK_NAME_FILL	_T("fill")

	/** 控件状态值 */
#define FOOSTATE_NAME_NONE			_T("")
#define FOOSTATE_NAME_ALL			_T("all")
#define FOOSTATE_NAME_NORMAL		_T("normal")
#define FOOSTATE_NAME_HOVER			_T("hover")
#define FOOSTATE_NAME_PUSHED		_T("pushed")
#define FOOSTATE_NAME_DISABLED		_T("disabled")
#define FOOSTATE_NAME_ENABLED		_T("enabled")
#define FOOSTATE_NAME_INVISIBLE		_T("invisible")
#define FOOSTATE_NAME_VISIBLE		_T("visible")
#define FOOSTATE_NAME_UNFOCUSED		_T("unfocused")
#define FOOSTATE_NAME_FOCUSED		_T("focused")
#define FOOSTATE_NAME_UNSELECTED	_T("unselected")
#define FOOSTATE_NAME_SELECTED		_T("selected")
#define FOOSTATE_NAME_UNCHECKED		_T("unchecked")
#define FOOSTATE_NAME_CHECKED		_T("checked")
#define FOOSTATE_NAME_DEACTIVATED	_T("deactivated")
#define FOOSTATE_NAME_ACTIVATED		_T("activated")
#define FOOSTATE_NAME_NONEMPTY		_T("nonempty")
#define FOOSTATE_NAME_EMPTY			_T("empty")

	/** 对齐方式 */
#define FOOALIGN_NAME_NONE			_T("none")
#define FOOALIGN_NAME_LEFT			_T("left")
#define FOOALIGN_NAME_TOP			_T("top")
#define FOOALIGN_NAME_RIGHT			_T("right")
#define FOOALIGN_NAME_XCENTER		_T("xcenter")
#define FOOALIGN_NAME_YCENTER		_T("ycenter")
#define FOOALIGN_NAME_BOTTOM		_T("bottom")

	/** 控件角色 */
#define FOOROLE_NAME_NONE			_T("none")
#define FOOROLE_NAME_CAPTION		_T("caption")
#define FOOROLE_NAME_SYSMIN			_T("sysmin")
#define FOOROLE_NAME_SYSMAX			_T("sysmax")
#define FOOROLE_NAME_SYSRESTORE		_T("sysrestore")
#define FOOROLE_NAME_SYSCLOSE		_T("sysclose")
#define FOOROLE_NAME_SIZEGRIP		_T("sizegrip")

	bool FOOUI_API fooMarkupCheckNameFlag(LPCTSTR _Str, LPCTSTR _SubStr)
	{
		FASSERT(NULL != _Str);
		FASSERT(NULL != _SubStr);

		LPCTSTR _Result = _tcsstr(_Str, _SubStr);
		if (NULL == _Result) return false;

		/** 如果结果不是字符串起始位置，判断前一个字符是否为空格或|分割符 */
		if ( _Result != _Str
			&& (_Result - 1)[0] != _T(' ') 
			&& (_Result - 1)[0] != _T('|')
			)
		{
			return false;
		}

		/** 如果字符不是字符串结束位置 */
		LPCTSTR _EndResult = _Result + _tcslen(_SubStr);
		if ( _EndResult != (_Str + _tcslen(_Str))
			&& _EndResult[0] != _T(' ') 
			&& _EndResult[0] != _T('|') 
			)
		{
			return false;
		}

		return true;
	}

	bool FOOUI_API fooMarkupParseBoolean(LPCTSTR pszAttrValue)
	{
		FASSERT(NULL != pszAttrValue);

		return (_tcscmp(pszAttrValue, _T("true")) == 0 
			|| _tcscmp(pszAttrValue, _T("yes")) == 0);
	}

	bool FOOUI_API fooMarkupParsePoint(LPPOINT lpPoint, LPCTSTR pszAttrValue)
	{
		FASSERT(NULL != pszAttrValue);

		LPTSTR pstrPoint = NULL;
		lpPoint->x = _tcstol(pszAttrValue, &pstrPoint, 10);
		if (NULL == pstrPoint
			|| _tcslen(pstrPoint) == 0)
		{
			lpPoint->y = lpPoint->x;
		}
		else
		{
			lpPoint->y = _tcstol(pstrPoint + 1, &pstrPoint, 10);
		}

		return true;
	}

	bool FOOUI_API fooMarkupParseSize(LPSIZE lpSize, LPCTSTR pszAttrValue)
	{
		FASSERT(NULL != pszAttrValue);

		LPTSTR pstrSize = NULL;
		lpSize->cx = _tcstol(pszAttrValue, &pstrSize, 10);
		if (NULL == pstrSize
			|| _tcslen(pstrSize) == 0)
		{
			lpSize->cy = lpSize->cx;
		}
		else
		{
			lpSize->cy = _tcstol(pstrSize + 1, &pstrSize, 10);
		}

		return true;
	}

	bool FOOUI_API fooMarkupParseRect(LPRECT lpRect, LPCTSTR pszAttrValue)
	{
		FASSERT(NULL != pszAttrValue);

		LPTSTR pstrRect = NULL;
		lpRect->left = _tcstol(pszAttrValue, &pstrRect, 10);	FASSERT(NULL != pstrRect);
		lpRect->top = _tcstol(pstrRect + 1, &pstrRect, 10);		FASSERT(NULL != pstrRect);
		lpRect->right = _tcstol(pstrRect + 1, &pstrRect, 10);	FASSERT(NULL != pstrRect);
		lpRect->bottom = _tcstol(pstrRect + 1, &pstrRect, 10);

		return true;
	}

	bool FOOUI_API fooMarkupParseEdge(LPRECT lpRect, LPCTSTR pszAttrValue)
	{
		FASSERT(NULL != pszAttrValue);

		LPTSTR pstrRect = NULL;
		lpRect->left = _tcstol(pszAttrValue, &pstrRect, 10);
		if (NULL == pstrRect
			|| _tcslen(pstrRect) == 0)
		{
			lpRect->bottom = lpRect->right = lpRect->top = lpRect->left;
		}
		else
		{
			lpRect->top = _tcstol(pstrRect + 1, &pstrRect, 10); FASSERT(NULL != pstrRect);
			lpRect->right = _tcstol(pstrRect + 1, &pstrRect, 10); FASSERT(NULL != pstrRect);
			lpRect->bottom = _tcstol(pstrRect + 1, &pstrRect, 10);
		}

		return true;
	}

	bool FOOUI_API fooMarkupParseRectWH(LPRECT lpRect, LPCTSTR pszAttrValue)
	{
		FASSERT(NULL != pszAttrValue);

		LPTSTR pstrRect = NULL;
		lpRect->left	= _tcstol(pszAttrValue, &pstrRect, 10);					FASSERT(NULL != pstrRect);
		lpRect->top		= _tcstol(pstrRect + 1, &pstrRect, 10);					FASSERT(NULL != pstrRect);
		lpRect->right	= lpRect->left + _tcstol(pstrRect + 1, &pstrRect, 10);	FASSERT(NULL != pstrRect);
		lpRect->bottom	= lpRect->top + _tcstol(pstrRect + 1, &pstrRect, 10);

		return true;
	}

	DWORD FOOUI_API fooMarkupParseState(LPCTSTR pszAttrValue)
	{
		FASSERT(NULL != pszAttrValue);

		DWORD dwState = FOOSTATE_NONE;

		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOSTATE_NAME_ALL) )
			dwState |= FOOSTATE_ALL;
		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOSTATE_NAME_NORMAL) )
			dwState |= FOOSTATE_NORMAL;
		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOSTATE_NAME_HOVER) )
			dwState |= FOOSTATE_HOVER;
		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOSTATE_NAME_PUSHED) )
			dwState |= FOOSTATE_PUSHED;
		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOSTATE_NAME_DISABLED) )
			dwState |= FOOSTATE_DISABLED;
		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOSTATE_NAME_ENABLED) )
			dwState |= FOOSTATE_ENABLED;
		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOSTATE_NAME_INVISIBLE) )
			dwState |= FOOSTATE_INVISIBLE;
		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOSTATE_NAME_VISIBLE) )
			dwState |= FOOSTATE_VISIBLE;
		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOSTATE_NAME_UNFOCUSED) )
			dwState |= FOOSTATE_UNFOCUSED;
		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOSTATE_NAME_FOCUSED) )
			dwState |= FOOSTATE_FOCUSED;
		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOSTATE_NAME_UNSELECTED) )
			dwState |= FOOSTATE_UNSELECTED;
		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOSTATE_NAME_SELECTED) )
			dwState |= FOOSTATE_SELECTED;
		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOSTATE_NAME_UNCHECKED) )
			dwState |= FOOSTATE_UNCHECKED;
		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOSTATE_NAME_CHECKED) )
			dwState |= FOOSTATE_CHECKED;
		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOSTATE_NAME_DEACTIVATED) )
			dwState |= FOOSTATE_DEACTIVATED;
		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOSTATE_NAME_ACTIVATED) )
			dwState |= FOOSTATE_ACTIVATED;
		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOSTATE_NAME_NONEMPTY) )
			dwState |= FOOSTATE_NONEMPTY;
		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOSTATE_NAME_EMPTY) )
			dwState |= FOOSTATE_EMPTY;

		return dwState;
	}

	DWORD FOOUI_API fooMarkupParseDock(LPCTSTR pszAttrValue)
	{
		FASSERT(NULL != pszAttrValue);

		if (_tcscmp(pszAttrValue, FOODOCK_NAME_LEFT) == 0)
			return FOODOCK_LEFT;
		else if (_tcscmp(pszAttrValue, FOODOCK_NAME_TOP) == 0)
			return FOODOCK_TOP;
		else if (_tcscmp(pszAttrValue, FOODOCK_NAME_RIGHT) == 0)
			return FOODOCK_RIGHT;
		else if (_tcscmp(pszAttrValue, FOODOCK_NAME_BOTTOM) == 0)
			return FOODOCK_BOTTOM;
		else if (_tcscmp(pszAttrValue, FOODOCK_NAME_FILL) == 0)
			return FOODOCK_FILL;

		return FOODOCK_NONE;
	}

	DWORD FOOUI_API fooMarkupParseAlign(LPCTSTR pszAttrValue)
	{
		FASSERT(NULL != pszAttrValue);

		UINT uAlign = FOOALIGN_NONE;

		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOALIGN_NAME_LEFT) )
			uAlign |= FOOALIGN_LEFT;
		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOALIGN_NAME_TOP) )
			uAlign |= FOOALIGN_TOP;
		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOALIGN_NAME_RIGHT) )
			uAlign |= FOOALIGN_RIGHT;
		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOALIGN_NAME_XCENTER) )
			uAlign |= FOOALIGN_XCENTER;
		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOALIGN_NAME_YCENTER) )
			uAlign |= FOOALIGN_YCENTER;
		if ( fooMarkupCheckNameFlag(pszAttrValue, FOOALIGN_NAME_BOTTOM) )
			uAlign |= FOOALIGN_BOTTOM;

		return uAlign;
	}

	DWORD FOOUI_API fooMarkupParseRole(LPCTSTR pszAttrValue)
	{
		FASSERT(NULL != pszAttrValue);

		if (_tcscmp(pszAttrValue, FOOROLE_NAME_NONE) == 0) return FOOROLE_NONE;
		else if (_tcscmp(pszAttrValue, FOOROLE_NAME_CAPTION) == 0) return FOOROLE_CAPTION;
		else if (_tcscmp(pszAttrValue, FOOROLE_NAME_SYSMIN) == 0) return FOOROLE_SYSMIN;
		else if (_tcscmp(pszAttrValue, FOOROLE_NAME_SYSMAX) == 0) return FOOROLE_SYSMAX;
		else if (_tcscmp(pszAttrValue, FOOROLE_NAME_SYSRESTORE) == 0) return FOOROLE_SYSRESTORE;
		else if (_tcscmp(pszAttrValue, FOOROLE_NAME_SYSCLOSE) == 0) return FOOROLE_SYSCLOSE;
		else if (_tcscmp(pszAttrValue, FOOROLE_NAME_SIZEGRIP) == 0) return FOOROLE_SIZEGRIP;

		return FOOROLE_NONE;
	}

}} // !namespace