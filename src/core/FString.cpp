/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FString.cpp
* 描述：		FString
* 创建日期：	2013-06-06
*
*/

#include "StdAfx.h"

#include "core/FString.h"

#define FSTRING_INLINE FOOINLINE

namespace FooUI { namespace Core {

	//////////////////////////////////////////////////////////////////////////
	// FStringA
	FSTRING_INLINE FStringA::FStringA(void)
	{
	}

	FSTRING_INLINE FStringA::FStringA(LPCSTR pszSrc)
	{
		m_StringData = pszSrc;
	}

	FSTRING_INLINE FStringA::FStringA(const FStringA& strSrc)
	{
		m_StringData = strSrc.m_StringData;
	}

	FSTRING_INLINE FStringA::~FStringA(void)
	{
	}

	FSTRING_INLINE void FStringA::append(PCSTR pszSrc)
	{
		m_StringData.Append(pszSrc);
	}

	FSTRING_INLINE void FStringA::append(PCSTR pszSrc, int nLength)
	{
		m_StringData.Append(pszSrc, nLength);
	}

	FSTRING_INLINE void FStringA::appendChar(char ch)
	{
		m_StringData.AppendChar(ch);
	}

	FSTRING_INLINE void FStringA::append(const FStringA& strSrc)
	{
		m_StringData.Append(strSrc.m_StringData);
	}

	FSTRING_INLINE void FStringA::empty(void)
	{
		m_StringData.Empty();
	}

	FSTRING_INLINE void FStringA::freeExtra(void)
	{
		m_StringData.FreeExtra();
	}

	FSTRING_INLINE int FStringA::getAllocLength() const
	{
		return m_StringData.GetAllocLength();
	}

	FSTRING_INLINE char FStringA::getAt(int iChar) const
	{
		return m_StringData.GetAt(iChar);
	}

	FSTRING_INLINE PSTR FStringA::getBuffer(void)
	{
		return m_StringData.GetBuffer();
	}

	FSTRING_INLINE PSTR FStringA::getBuffer(int nMinBufferLength)
	{
		return m_StringData.GetBuffer(nMinBufferLength);
	}

	FSTRING_INLINE PSTR FStringA::getBufferSetLength(int nLength)
	{
		return m_StringData.GetBufferSetLength(nLength);
	}

	FSTRING_INLINE int FStringA::getLength(void) const
	{
		return m_StringData.GetLength();
	}

	FSTRING_INLINE PCSTR FStringA::getString(void) const
	{
		return m_StringData.GetString();
	}

	FSTRING_INLINE bool FStringA::isEmpty(void) const
	{
		return m_StringData.IsEmpty();
	}

	FSTRING_INLINE PSTR FStringA::lockBuffer(void)
	{
		return m_StringData.LockBuffer();
	}

	FSTRING_INLINE void FStringA::unlockBuffer(void)
	{
		m_StringData.UnlockBuffer();
	}

	FSTRING_INLINE void FStringA::releaseBuffer(int nNewLength)
	{
		m_StringData.ReleaseBuffer(nNewLength);
	}

	FSTRING_INLINE void FStringA::releaseBufferSetLength(int nNewLength)
	{
		m_StringData.ReleaseBufferSetLength(nNewLength);
	}

	FSTRING_INLINE void FStringA::truncate(int nNewLength)
	{
		m_StringData.Truncate(nNewLength);
	}

	FSTRING_INLINE void FStringA::setAt(int iChar, char ch)
	{
		m_StringData.SetAt(iChar, ch);
	}

	FSTRING_INLINE void FStringA::setString(PCSTR pszSrc)
	{
		m_StringData.SetString(pszSrc);
	}

	FSTRING_INLINE void FStringA::setString(PCSTR pszSrc, int nLength)
	{
		m_StringData.SetString(pszSrc, nLength);
	}

	FSTRING_INLINE int FStringA::compare(PCSTR psz) const
	{
		return m_StringData.Compare(psz);
	}

	FSTRING_INLINE int FStringA::compareNoCase(PCSTR psz) const
	{
		return m_StringData.CompareNoCase(psz);
	}

	FSTRING_INLINE int FStringA::collate(PCSTR psz) const
	{
		return m_StringData.Collate(psz);
	}

	FSTRING_INLINE int FStringA::collateNoCase(PCSTR psz) const
	{
		return m_StringData.CollateNoCase(psz);
	}

	FSTRING_INLINE int FStringA::deleteAt(int iIndex, int nCount)
	{
		return m_StringData.Delete(iIndex, nCount);
	}

	FSTRING_INLINE int FStringA::insertAt(int iIndex, char ch)
	{
		return m_StringData.Insert(iIndex, ch);
	}

	FSTRING_INLINE int FStringA::insertAt(int iIndex, PCSTR psz)
	{
		return m_StringData.Insert(iIndex, psz);
	}

	FSTRING_INLINE int FStringA::replace(char chOld, char chNew)
	{
		return m_StringData.Replace(chOld, chNew);
	}

	FSTRING_INLINE int FStringA::replace(PCSTR pszOld, PCSTR pszNew)
	{
		return m_StringData.Replace(pszOld, pszNew);
	}

	FSTRING_INLINE int FStringA::remove(char chRemove)
	{
		return m_StringData.Remove(chRemove);
	}

	FSTRING_INLINE FStringA FStringA::tokenize(PCSTR pszTokens, int& iStart) const
	{
		return FStringA(m_StringData.Tokenize(pszTokens, iStart));
	}

	FSTRING_INLINE int FStringA::find(char ch, int iStart) const
	{
		return m_StringData.Find(ch, iStart);
	}

	FSTRING_INLINE int FStringA::find(PCSTR pszSub, int iStart) const
	{
		return m_StringData.Find(pszSub, iStart);
	}

	FSTRING_INLINE int FStringA::findOneOf(PCSTR pszCharSet) const
	{
		return m_StringData.FindOneOf(pszCharSet);
	}

	FSTRING_INLINE int FStringA::reverseFind(char ch) const
	{
		return m_StringData.ReverseFind(ch);
	}

	FSTRING_INLINE FStringA& FStringA::makeUpper(void)
	{
		m_StringData.MakeUpper();
		return (*this);
	}

	FSTRING_INLINE FStringA& FStringA::makeLower(void)
	{
		m_StringData.MakeLower();
		return (*this);
	}

	FSTRING_INLINE FStringA& FStringA::makeReverse(void)
	{
		m_StringData.MakeReverse();
		return (*this);
	}

	FSTRING_INLINE FStringA& FStringA::trimRight(void)
	{
		m_StringData.TrimRight();
		return (*this);
	}

	FSTRING_INLINE FStringA& FStringA::trimLeft(void)
	{
		m_StringData.TrimLeft();
		return (*this);
	}

	FSTRING_INLINE FStringA& FStringA::trim(void)
	{
		m_StringData.Trim();
		return (*this);
	}

	FSTRING_INLINE FStringA& FStringA::trim(char chTarget)
	{
		m_StringData.Trim(chTarget);
		return (*this);
	}

	FSTRING_INLINE FStringA& FStringA::trim(PCSTR pszTargets)
	{
		m_StringData.Trim(pszTargets);
		return (*this);
	}

	FSTRING_INLINE FStringA& FStringA::trimRight(char chTarget)
	{
		m_StringData.TrimRight(chTarget);
		return (*this);
	}

	FSTRING_INLINE FStringA& FStringA::trimRight(PCSTR pszTargets)
	{
		m_StringData.TrimRight(pszTargets);
		return (*this);
	}

	FSTRING_INLINE FStringA& FStringA::trimLeft(char chTarget)
	{
		m_StringData.TrimLeft(chTarget);
		return (*this);
	}

	FSTRING_INLINE FStringA& FStringA::trimLeft(PCSTR pszTargets)
	{
		m_StringData.TrimLeft(pszTargets);
		return (*this);
	}

	FSTRING_INLINE FStringA FStringA::mid(int iFirst) const
	{
		return( mid( iFirst, getLength()-iFirst ) );
	}

	FSTRING_INLINE FStringA FStringA::mid(int iFirst, int nCount) const
	{
		return FStringA(m_StringData.Mid(iFirst, nCount));
	}

	FSTRING_INLINE FStringA FStringA::right(int nCount) const
	{
		return FStringA(m_StringData.Right(nCount));
	}

	FSTRING_INLINE FStringA FStringA::left(int nCount) const
	{
		return FStringA(m_StringData.Left(nCount));
	}

	FSTRING_INLINE FStringA FStringA::spanIncluding(PCSTR pszCharSet) const
	{
		return FStringA(m_StringData.SpanIncluding(pszCharSet));
	}

	FSTRING_INLINE FStringA FStringA::spanExcluding(PSTR pszCharSet) const
	{
		return FStringA(m_StringData.SpanExcluding(pszCharSet));
	}

	FSTRING_INLINE void __cdecl FStringA::format(PCSTR pszFormat, ...)
	{
		va_list argList;
		va_start( argList, pszFormat );
		m_StringData.FormatV(pszFormat, argList);
		va_end( argList );
	}

	FSTRING_INLINE void __cdecl FStringA::appendFormat(PCSTR pszFormat, ...)
	{
		va_list argList;
		va_start( argList, pszFormat );
		m_StringData.AppendFormat(pszFormat, argList);
		va_end( argList );
	}

	FSTRING_INLINE void FStringA::formatV(PCSTR pszFormat, va_list args)
	{
		m_StringData.FormatV(pszFormat, args);
	}

	FSTRING_INLINE void FStringA::appendFormatV(PCSTR pszFormat, va_list args)
	{
		m_StringData.AppendFormatV(pszFormat, args);
	}

	FSTRING_INLINE /*static*/ void __cdecl FStringA::concatenate(
		FStringA& strResult, 
		PCSTR psz1, int nLength1,
		PCSTR psz2, int nLength2)
	{
		int nNewLength = nLength1 + nLength2;
		PSTR pszBuffer = strResult.getBuffer( nNewLength );
		copyChars( pszBuffer, nLength1, psz1, nLength1 );
		copyChars( pszBuffer+nLength1, nLength2, psz2, nLength2 );
		strResult.releaseBufferSetLength( nNewLength );
	}

	FSTRING_INLINE /*static*/ void __cdecl FStringA::copyChars(
		char* pchDest,
		const char* pchSrc, 
		int nChars)
	{
		if (NULL != pchSrc)
		{
#pragma warning (push)
#pragma warning(disable : 4996)
			memcpy( pchDest, pchSrc, nChars * sizeof( char ) );
#pragma warning (pop)
		}
	}

	FSTRING_INLINE /*static*/ void __cdecl FStringA::copyChars(
		char* pchDest,
		size_t nDestLen,
		const char* pchSrc,
		int nChars)
	{
		memcpy_s( pchDest, nDestLen * sizeof( char ),
			pchSrc, nChars * sizeof( char ) );
	}

	FSTRING_INLINE /*static*/ int __cdecl FStringA::stringLength(const char* psz)
	{
		if( psz == NULL )
		{
			return( 0 );
		}
		return( int( strlen( psz ) ) );
	}

	FSTRING_INLINE char FStringA::operator[](int iChar) const
	{
		return m_StringData[iChar];
	}

	FSTRING_INLINE FStringA::operator PCSTR() const
	{
		return m_StringData;
	}

	FSTRING_INLINE FStringA& FStringA::operator=(const FStringA& strSrc)
	{
		m_StringData = strSrc.m_StringData;
		return (*this);
	}

	FSTRING_INLINE FStringA& FStringA::operator=(LPCSTR pszSrc)
	{
		m_StringData = pszSrc;
		return (*this);
	}

	FSTRING_INLINE FStringA& FStringA::operator+=(const FStringA& strSrc)
	{
		m_StringData += strSrc.m_StringData;
		return (*this);
	}

	FSTRING_INLINE FStringA& FStringA::operator+=(PCSTR pszSrc)
	{
		m_StringData += pszSrc;
		return (*this);
	}

	FSTRING_INLINE FStringA& FStringA::operator+=(char ch)
	{
		m_StringData += ch;
		return (*this);
	}

	FSTRING_INLINE FStringA& FStringA::operator+=(unsigned char ch)
	{
		m_StringData += ch;
		return (*this);
	}

	/////////////////////////////////////////////////////////////////////////////
	// FStringW
	FSTRING_INLINE FStringW::FStringW(void)
	{
	}

	FSTRING_INLINE FStringW::FStringW(PCWSTR pszSrc)
	{
		m_StringData = pszSrc;
	}

	FSTRING_INLINE FStringW::FStringW(const FStringW& strSrc)
	{
		m_StringData = strSrc.m_StringData;
	}

	FSTRING_INLINE FStringW::~FStringW(void)
	{
	}

	FSTRING_INLINE void FStringW::append(PCWSTR pszSrc)
	{
		m_StringData.Append(pszSrc);
	}

	FSTRING_INLINE void FStringW::append(PCWSTR pszSrc, int nLength)
	{
		m_StringData.Append(pszSrc, nLength);
	}

	FSTRING_INLINE void FStringW::appendChar(wchar_t ch)
	{
		m_StringData.AppendChar(ch);
	}

	FSTRING_INLINE void FStringW::append(const FStringW& strSrc)
	{
		m_StringData.Append(strSrc.m_StringData);
	}

	FSTRING_INLINE void FStringW::empty(void)
	{
		m_StringData.Empty();
	}

	FSTRING_INLINE void FStringW::freeExtra(void)
	{
		m_StringData.FreeExtra();
	}

	FSTRING_INLINE int FStringW::getAllocLength() const
	{
		return m_StringData.GetAllocLength();
	}

	FSTRING_INLINE wchar_t FStringW::getAt(int iChar) const
	{
		return m_StringData.GetAt(iChar);
	}

	FSTRING_INLINE PWSTR FStringW::getBuffer(void)
	{
		return m_StringData.GetBuffer();
	}

	FSTRING_INLINE PWSTR FStringW::getBuffer(int nMinBufferLength)
	{
		return m_StringData.GetBuffer(nMinBufferLength);
	}

	FSTRING_INLINE PWSTR FStringW::getBufferSetLength(int nLength)
	{
		return m_StringData.GetBufferSetLength(nLength);
	}

	FSTRING_INLINE int FStringW::getLength(void) const
	{
		return m_StringData.GetLength();
	}

	FSTRING_INLINE PCWSTR FStringW::getString(void) const
	{
		return m_StringData.GetString();
	}

	FSTRING_INLINE bool FStringW::isEmpty(void) const
	{
		return m_StringData.IsEmpty();
	}

	FSTRING_INLINE PWSTR FStringW::lockBuffer(void)
	{
		return m_StringData.LockBuffer();
	}

	FSTRING_INLINE void FStringW::unlockBuffer(void)
	{
		m_StringData.UnlockBuffer();
	}

	FSTRING_INLINE void FStringW::releaseBuffer(int nNewLength)
	{
		m_StringData.ReleaseBuffer(nNewLength);
	}

	FSTRING_INLINE void FStringW::releaseBufferSetLength(int nNewLength)
	{
		m_StringData.ReleaseBufferSetLength(nNewLength);
	}

	FSTRING_INLINE void FStringW::truncate(int nNewLength)
	{
		m_StringData.Truncate(nNewLength);
	}

	FSTRING_INLINE void FStringW::setAt(int iChar, wchar_t ch)
	{
		m_StringData.SetAt(iChar, ch);
	}

	FSTRING_INLINE void FStringW::setString(PCWSTR pszSrc)
	{
		m_StringData.SetString(pszSrc);
	}

	FSTRING_INLINE void FStringW::setString(PCWSTR pszSrc, int nLength)
	{
		m_StringData.SetString(pszSrc, nLength);
	}

	FSTRING_INLINE int FStringW::compare(PCWSTR psz) const
	{
		return m_StringData.Compare(psz);
	}

	FSTRING_INLINE int FStringW::compareNoCase(PCWSTR psz) const
	{
		return m_StringData.CompareNoCase(psz);
	}

	FSTRING_INLINE int FStringW::collate(PCWSTR psz) const
	{
		return m_StringData.Collate(psz);
	}

	FSTRING_INLINE int FStringW::collateNoCase(PCWSTR psz) const
	{
		return m_StringData.CollateNoCase(psz);
	}

	FSTRING_INLINE int FStringW::deleteAt(int iIndex, int nCount)
	{
		return m_StringData.Delete(iIndex, nCount);
	}

	FSTRING_INLINE int FStringW::insertAt(int iIndex, wchar_t ch)
	{
		return m_StringData.Insert(iIndex, ch);
	}

	FSTRING_INLINE int FStringW::insertAt(int iIndex, PCWSTR psz)
	{
		return m_StringData.Insert(iIndex, psz);
	}

	FSTRING_INLINE int FStringW::replace(wchar_t chOld, wchar_t chNew)
	{
		return m_StringData.Replace(chOld, chNew);
	}

	FSTRING_INLINE int FStringW::replace(PCWSTR pszOld, PCWSTR pszNew)
	{
		return m_StringData.Replace(pszOld, pszNew);
	}

	FSTRING_INLINE int FStringW::remove(wchar_t chRemove)
	{
		return m_StringData.Remove(chRemove);
	}

	FSTRING_INLINE FStringW FStringW::tokenize(PCWSTR pszTokens, int& iStart) const
	{
		return FStringW(m_StringData.Tokenize(pszTokens, iStart));
	}

	FSTRING_INLINE int FStringW::find(wchar_t ch, int iStart) const
	{
		return m_StringData.Find(ch, iStart);
	}

	FSTRING_INLINE int FStringW::find(PCWSTR pszSub, int iStart) const
	{
		return m_StringData.Find(pszSub, iStart);
	}

	FSTRING_INLINE int FStringW::findOneOf(PCWSTR pszCharSet) const
	{
		return m_StringData.FindOneOf(pszCharSet);
	}

	FSTRING_INLINE int FStringW::reverseFind(wchar_t ch) const
	{
		return m_StringData.ReverseFind(ch);
	}

	FSTRING_INLINE FStringW& FStringW::makeUpper(void)
	{
		m_StringData.MakeUpper();
		return (*this);
	}

	FSTRING_INLINE FStringW& FStringW::makeLower(void)
	{
		m_StringData.MakeLower();
		return (*this);
	}

	FSTRING_INLINE FStringW& FStringW::makeReverse(void)
	{
		m_StringData.MakeReverse();
		return (*this);
	}

	FSTRING_INLINE FStringW& FStringW::trimRight(void)
	{
		m_StringData.TrimRight();
		return (*this);
	}

	FSTRING_INLINE FStringW& FStringW::trimLeft(void)
	{
		m_StringData.TrimLeft();
		return (*this);
	}

	FSTRING_INLINE FStringW& FStringW::trim(void)
	{
		m_StringData.Trim();
		return (*this);
	}

	FSTRING_INLINE FStringW& FStringW::trim(wchar_t chTarget)
	{
		m_StringData.Trim(chTarget);
		return (*this);
	}

	FSTRING_INLINE FStringW& FStringW::trim(PCWSTR pszTargets)
	{
		m_StringData.Trim(pszTargets);
		return (*this);
	}

	FSTRING_INLINE FStringW& FStringW::trimRight(wchar_t chTarget)
	{
		m_StringData.Trim(chTarget);
		return (*this);
	}

	FSTRING_INLINE FStringW& FStringW::trimRight(PCWSTR pszTargets)
	{
		m_StringData.TrimRight(pszTargets);
		return (*this);
	}

	FSTRING_INLINE FStringW& FStringW::trimLeft(wchar_t chTarget)
	{
		m_StringData.TrimRight(chTarget);
		return (*this);
	}

	FSTRING_INLINE FStringW& FStringW::trimLeft(PCWSTR pszTargets)
	{
		m_StringData.TrimLeft(pszTargets);
		return (*this);
	}

	FSTRING_INLINE FStringW FStringW::mid(int iFirst) const
	{
		return FStringW(m_StringData.Mid(iFirst));
	}

	FSTRING_INLINE FStringW FStringW::mid(int iFirst, int nCount) const
	{
		return FStringW(m_StringData.Mid(iFirst, nCount));
	}

	FSTRING_INLINE FStringW FStringW::right(int nCount) const
	{
		return FStringW(m_StringData.Right(nCount));
	}

	FSTRING_INLINE FStringW FStringW::left(int nCount) const
	{
		return FStringW(m_StringData.Left(nCount));
	}

	FSTRING_INLINE FStringW FStringW::spanIncluding(PCWSTR pszCharSet) const
	{
		return FStringW(m_StringData.SpanIncluding(pszCharSet));
	}

	FSTRING_INLINE FStringW FStringW::spanExcluding(PWSTR pszCharSet) const
	{
		return FStringW(m_StringData.SpanExcluding(pszCharSet));
	}

	FSTRING_INLINE void __cdecl FStringW::format(PCWSTR pszFormat, ...)
	{
		va_list argList;
		va_start( argList, pszFormat );
		formatV( pszFormat, argList );
		va_end( argList );
	}

	FSTRING_INLINE void __cdecl FStringW::appendFormat(PCWSTR pszFormat, ...)
	{
		va_list argList;
		va_start( argList, pszFormat );
		appendFormatV( pszFormat, argList );
		va_end( argList );
	}

	FSTRING_INLINE void FStringW::formatV(PCWSTR pszFormat, va_list args)
	{
		m_StringData.FormatV(pszFormat, args);
	}

	FSTRING_INLINE void FStringW::appendFormatV(PCWSTR pszFormat, va_list args)
	{
		m_StringData.AppendFormatV(pszFormat, args);
	}

	FSTRING_INLINE /*static*/ void __cdecl FStringW::concatenate(
		FStringW& strResult,
		PCWSTR psz1,
		int nLength1,
		PCWSTR psz2,
		int nLength2)
	{
		int nNewLength = nLength1+nLength2;
		PWSTR pszBuffer = strResult.getBuffer( nNewLength );
		copyChars( pszBuffer, nLength1, psz1, nLength1 );
		copyChars( pszBuffer+nLength1, nLength2, psz2, nLength2 );
		strResult.releaseBufferSetLength( nNewLength );
	}

	FSTRING_INLINE /*static*/ void __cdecl FStringW::copyChars(
		wchar_t* pchDest,
		const wchar_t* pchSrc,
		int nChars)
	{
		if (pchSrc != NULL)
		{
#pragma warning (push)
#pragma warning(disable : 4996)
			memcpy( pchDest, pchSrc, nChars * sizeof( wchar_t ) );
#pragma warning (pop)
		}
	}

	FSTRING_INLINE /*static*/ void __cdecl FStringW::copyChars(
		wchar_t* pchDest,
		size_t nDestLen,
		const wchar_t* pchSrc,
		int nChars)
	{
		memcpy_s( pchDest, nDestLen*sizeof( wchar_t ),
			pchSrc, nChars*sizeof( wchar_t ) );
	}

	FSTRING_INLINE /*static*/ int __cdecl FStringW::stringLength(const wchar_t* psz)
	{
		if( psz == NULL )
		{
			return( 0 );
		}
		return( int( wcslen( psz ) ) );
	}

	FSTRING_INLINE wchar_t FStringW::operator[](int iChar) const
	{
		return m_StringData[iChar];
	}

	FSTRING_INLINE FStringW::operator PCWSTR() const
	{
		return m_StringData;
	}

	FSTRING_INLINE FStringW& FStringW::operator=(const FStringW& strSrc)
	{
		m_StringData = strSrc.m_StringData;
		return (*this);
	}

	FSTRING_INLINE FStringW& FStringW::operator=(PCWSTR pszSrc)
	{
		m_StringData = pszSrc;
		return (*this);
	}

	FSTRING_INLINE FStringW& FStringW::operator+=(const FStringW& strSrc)
	{
		m_StringData += strSrc.m_StringData;
		return (*this);
	}

	FSTRING_INLINE FStringW& FStringW::operator+=(PCWSTR pszSrc)
	{
		m_StringData += pszSrc;
		return (*this);
	}

	FSTRING_INLINE FStringW& FStringW::operator+=(wchar_t ch)
	{
		m_StringData += ch;
		return (*this);
	}

	//////////////////////////////////////////////////////////////////////////
	// FW2A
	FSTRING_INLINE FW2A::FW2A(LPCWSTR lpWideCharStr)
	{
		FASSERT(NULL != lpWideCharStr);

		int numOfChar = ::WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, -1, NULL, 0, NULL, NULL);
		::WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, -1, m_StringData.GetBuffer(numOfChar + 1), numOfChar + 1, NULL, NULL);
		m_StringData.ReleaseBuffer();
	}

	FSTRING_INLINE FW2A::FW2A(LPCSTR lpMultiByteStr)
	{
		FASSERT(NULL != lpMultiByteStr);
		m_StringData = lpMultiByteStr;
	}

	FSTRING_INLINE FW2A::operator PCSTR() const
	{
		return (PCSTR)m_StringData;
	}

	//////////////////////////////////////////////////////////////////////////
	// FA2W
	FSTRING_INLINE FA2W::FA2W(LPCSTR lpMultiByteStr)
	{
		FASSERT(NULL != lpMultiByteStr);

		int numOfWChar = ::MultiByteToWideChar(CP_ACP, 0, lpMultiByteStr, -1, NULL, 0);
		FASSERT(numOfWChar > 0);
		::MultiByteToWideChar(CP_ACP, 0, lpMultiByteStr, -1, m_StringData.GetBuffer(numOfWChar + 1), numOfWChar + 1);
		m_StringData.ReleaseBuffer();
	}

	FSTRING_INLINE FA2W::FA2W(LPCWSTR lpWideCharStr)
	{
		FASSERT(NULL != lpWideCharStr);
		m_StringData = lpWideCharStr;
	}

	FSTRING_INLINE FA2W::operator PCWSTR() const
	{
		return (PCWSTR)m_StringData;
	}

}} // !namespace