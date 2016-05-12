/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FAttributeScript.h
* 描述：		FAttributeScript
* 创建日期：	2013-06-26
*
*/

#ifndef FATTRIBUTESCRIPT_H
#define FATTRIBUTESCRIPT_H

#include "core/FBase.h"
#include "core/FObject.h"
#include "core/FString.h"

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FAttributeScript
	typedef struct tagFATTRIBUTESCRIPT
	{
		tagFATTRIBUTESCRIPT(void) : children(NULL) , next(NULL) { }
		Core::FString name;
		Core::FString value;
		tagFATTRIBUTESCRIPT* children;
		tagFATTRIBUTESCRIPT* next;
	} FATTRIBUTESCRIPT;

	class FOOUI_LIB FAttributeScript : public Core::FNoncopyableObject
	{
	public:
		FAttributeScript(void);
		explicit FAttributeScript(LPCTSTR pszAttributeScript);
		~FAttributeScript(void);
		bool parseAttributeScript(LPCTSTR pszAttributeScript);
		FATTRIBUTESCRIPT* firstAttribute(void) const;

	private:
		FAttributeScript(const FAttributeScript&);
		void operator=(const FAttributeScript&);

	private:
		class FAttributeScriptPrivate;
		FAttributeScriptPrivate* m_attributeScriptPrivate;
	};

}} // !namespace

#endif // !FATTRIBUTESCRIPT_H