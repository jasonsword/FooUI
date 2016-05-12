/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FLock.h
* 描述：		FLock
* 创建日期：	2013-06-06
*
*/

#ifndef FLOCK_H
#define FLOCK_H

#include "core/FBase.h"

namespace FooUI { namespace Core { 

	//////////////////////////////////////////////////////////////////////////
	// FLock
	class FOOUI_LIB FLock
	{
	public:
		virtual ~FLock() { }

	public:
		virtual HRESULT lock(void) = 0;
		virtual HRESULT unlock(void) = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	// FCriticalSectionLock
	class FOOUI_LIB FCriticalSectionLock : public FLock
	{
	public:
		FCriticalSectionLock(void);
		~FCriticalSectionLock(void);

	public:
		HRESULT lock(void);
		HRESULT unlock(void);

	private:
		CRITICAL_SECTION m_sec;
	};

	//////////////////////////////////////////////////////////////////////////
	// FAutoLock
	class FOOUI_LIB FAutoLock
	{
	public:
		explicit FAutoLock(FLock* pLock) : m_pLock(pLock)
		{
			FASSERT(NULL != m_pLock);
			m_pLock->lock();
		}

		virtual ~FAutoLock(void)
		{
			FASSERT(NULL != m_pLock);
			m_pLock->unlock();
		}

	private:
		FLock* const m_pLock;
	};
}} // !namespace

#endif // !FLOCK_H