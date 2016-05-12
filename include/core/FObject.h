/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FObject.h
* 描述：		FObject
* 创建日期：	2013-06-06
*
*/

#ifndef FOBJECT_H
#define FOBJECT_H

#include "core/FBase.h"

namespace FooUI { namespace Core {

	//////////////////////////////////////////////////////////////////////////
	// FObject
	class FOOUI_LIB FObject
	{
	};

	//////////////////////////////////////////////////////////////////////////
	// FNoncopyableObject
	class FOOUI_LIB FNoncopyableObject
	{
		FNoncopyableObject(const FNoncopyableObject&);
		void operator=(const FNoncopyableObject&);
	public:
		FNoncopyableObject(void) { }
	};


	//////////////////////////////////////////////////////////////////////////
	// FReferenceObject
	class FOOUI_LIB FReferenceObject
	{
	public:
		FReferenceObject(void) : m_uRefCount(0) { }
		virtual ~FReferenceObject(void) { }

	public:
		unsigned int addRef(void)
		{
			return (++m_uRefCount);
		}

		unsigned int release(void)
		{
			FASSERT(m_uRefCount > 0);

			unsigned int uRefCount = (--m_uRefCount);
			if (0 == uRefCount)
			{
				delete this;
			}

			return uRefCount;
		}

		unsigned int referenceCount(void) const 
		{
			return m_uRefCount;
		}

	private:
		unsigned int m_uRefCount;
	};

}} // !namespace

#define FOBJECT_CAST(T, obj) static_cast<T>(obj)

#endif !FOBJECT_H