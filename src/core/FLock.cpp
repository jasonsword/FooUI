/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FLock.cpp
* 描述：		FLock
* 创建日期：	2013-06-06
*
*/

#include "StdAfx.h"
#include "core/FLock.h"

namespace FooUI { namespace Core { 

	//////////////////////////////////////////////////////////////////////////
	// FCriticalSectionLock
	FCriticalSectionLock::FCriticalSectionLock(void)
	{
		memset(&m_sec, 0, sizeof(CRITICAL_SECTION));
		::InitializeCriticalSection(&m_sec);
	}

	FCriticalSectionLock::~FCriticalSectionLock(void)
	{
		::DeleteCriticalSection(&m_sec);
	}

	HRESULT FCriticalSectionLock::lock(void)
	{
		::EnterCriticalSection(&m_sec);
		return S_OK;
	}

	HRESULT FCriticalSectionLock::unlock(void)
	{
		::LeaveCriticalSection(&m_sec);
		return S_OK;
	}

}} // !namespace