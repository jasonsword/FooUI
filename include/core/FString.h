/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FString.h
* 描述：		FString
* 创建日期：	2013-06-06
*
*/

#ifndef FSTRING_H
#define FSTRING_H

#include "core/FBase.h"

#ifndef FSTRINGA_DATA
#define FSTRINGA_DATA PSTR
#endif // !FSTRINGA_DATA

#ifndef FSTRINGW_DATA
#define FSTRINGW_DATA PWSTR
#endif // !FSTRINGW_DATA

namespace FooUI { namespace Core {

	//////////////////////////////////////////////////////////////////////////
	// FStringA
	class FOOUI_LIB FStringA
	{
	public:
		FStringA(void);
		FStringA(LPCSTR pszSrc);
		FStringA(const FStringA& strSrc);
		~FStringA(void);

	public:
		void append(PCSTR pszSrc);
		void append(PCSTR pszSrc, int nLength);
		void appendChar(char ch);
		void append(const FStringA& strSrc);
		void empty(void);
		void freeExtra(void);
		int getAllocLength() const;
		char getAt(int iChar) const;
		PSTR getBuffer(void);
		PSTR getBuffer(int nMinBufferLength);
		PSTR getBufferSetLength(int nLength);
		int getLength(void) const;
		PCSTR getString(void) const;
		bool isEmpty(void) const;
		PSTR lockBuffer(void);
		void unlockBuffer(void);
		void releaseBuffer(int nNewLength = -1);
		void releaseBufferSetLength(int nNewLength);
		void truncate(int nNewLength);
		void setAt(int iChar, char ch);
		void setString(PCSTR pszSrc);
		void setString(PCSTR pszSrc, int nLength);
		int compare(PCSTR psz) const;
		int compareNoCase(PCSTR psz) const;
		int collate(PCSTR psz) const;
		int collateNoCase(PCSTR psz) const;
		int deleteAt(int iIndex, int nCount = 1);
		int insertAt(int iIndex, char ch);
		int insertAt(int iIndex, PCSTR psz);
		int replace(char chOld, char chNew);
		int replace(PCSTR pszOld, PCSTR pszNew);
		int remove(char chRemove);
		FStringA tokenize(PCSTR pszTokens, int& istart) const;
		int find(char ch, int istart = 0) const;
		int find(PCSTR pszSub, int istart = 0) const;
		int findOneOf(PCSTR pszCharSet) const;
		int reverseFind(char ch) const;
		FStringA& makeUpper(void);
		FStringA& makeLower(void);
		FStringA& makeReverse(void);
		FStringA& trimRight(void);
		FStringA& trimLeft(void);
		FStringA& trim(void);
		FStringA& trim(char chTarget);
		FStringA& trim(PCSTR pszTargets);
		FStringA& trimRight(char chTarget);
		FStringA& trimRight(PCSTR pszTargets);
		FStringA& trimLeft(char chTarget);
		FStringA& trimLeft(PCSTR pszTargets);
		FStringA mid(int iFirst) const;
		FStringA mid(int iFirst, int nCount) const;
		FStringA right(int nCount) const;
		FStringA left(int nCount) const;
		FStringA spanIncluding(PCSTR pszCharSet) const;
		FStringA spanExcluding(PSTR pszCharSet) const;
		void __cdecl format(PCSTR pszFormat, ...);
		void __cdecl appendFormat(PCSTR pszFormat, ...);
		void formatV(PCSTR pszFormat, va_list args);
		void appendFormatV(PCSTR pszFormat, va_list args);

	protected:
		static void __cdecl concatenate(
			FStringA& strResult,
			PCSTR psz1, int nLength1,
			PCSTR psz2, int nLength2);
		static void __cdecl copyChars(
			char* pchDest,
			const char* pchSrc, 
			int nChars);
		static void __cdecl copyChars(
			char* pchDest,
			size_t nDestLen,
			const char* pchSrc,
			int nChars);
		static int __cdecl stringLength(const char* psz);

	public:
		char operator[](int iChar) const;
		operator PCSTR() const;

	public:
		FStringA& operator=(const FStringA& strSrc);
		FStringA& operator=(PCSTR pszSrc);
		FStringA& operator+=(const FStringA& strSrc);
		FStringA& operator+=(PCSTR pszSrc);
		FStringA& operator+=(char ch);
		FStringA& operator+=(unsigned char ch);

	public:
		friend FStringA operator+(
			const FStringA& str1,
			const FStringA& str2)
		{
			FStringA strResult;
			concatenate( strResult, str1, str1.getLength(), str2, str2.getLength() );
			return( strResult );
		}

		friend FStringA operator+(
			const FStringA& str1,
			PCSTR psz2)
		{
			FStringA strResult;
			concatenate( strResult, str1, str1.getLength(), psz2, stringLength( psz2 ) );
			return( strResult );
		}

		friend FStringA operator+(
			PCSTR psz1,
			const FStringA& str2)
		{
			FStringA strResult;
			concatenate( strResult, psz1, stringLength( psz1 ), str2, str2.getLength() );
			return( strResult );
		}

		friend bool operator==(
			const FStringA& str1,
			const FStringA& str2)
		{
			return( str1.compare( str2 ) == 0 );
		}

		friend bool operator==(
			const FStringA& str1,
			PCSTR psz2)
		{
			return( str1.compare( psz2 ) == 0 );
		}

		friend bool operator==(
			PCSTR psz1,
			const FStringA& str2)
		{
			return( str2.compare( psz1 ) == 0 );
		}

		friend bool operator!=(
			const FStringA& str1,
			const FStringA& str2 )
		{
			return( str1.compare( str2 ) != 0 );
		}

		friend bool operator!=(
			const FStringA& str1,
			PCSTR psz2)
		{
			return( str1.compare( psz2 ) != 0 );
		}

		friend bool operator!=(
			PCSTR psz1,
			const FStringA& str2)
		{
			return( str2.compare( psz1 ) != 0 );
		}

		friend bool operator<(
			const FStringA& str1,
			const FStringA& str2) throw()
		{
			return( str1.compare( str2 ) < 0 );
		}

		friend bool operator<(
			const FStringA& str1,
			PCSTR psz2 ) throw()
		{
			return( str1.compare( psz2 ) < 0 );
		}

		friend bool operator<(
			PCSTR psz1,
			const FStringA& str2) throw()
		{
			return( str2.compare( psz1 ) > 0 );
		}

		friend bool operator>(
			const FStringA& str1,
			const FStringA& str2) throw()
		{
			return( str1.compare( str2 ) > 0 );
		}

		friend bool operator>(
			const FStringA& str1,
			PCSTR psz2) throw()
		{
			return( str1.compare( psz2 ) > 0 );
		}

		friend bool operator>(
			PCSTR psz1,
			const FStringA& str2) throw()
		{
			return( str2.compare( psz1 ) < 0 );
		}

		friend bool operator<=(
			const FStringA& str1,
			const FStringA& str2) throw()
		{
			return( str1.compare( str2 ) <= 0 );
		}

		friend bool operator<=(
			const FStringA& str1,
			PCSTR psz2) throw()
		{
			return( str1.compare( psz2 ) <= 0 );
		}

		friend bool operator<=(
			PCSTR psz1,
			const FStringA& str2) throw()
		{
			return( str2.compare( psz1 ) >= 0 );
		}

		friend bool operator>=(
			const FStringA& str1,
			const FStringA& str2) throw()
		{
			return( str1.compare( str2 ) >= 0 );
		}

		friend bool operator>=(
			const FStringA& str1,
			PCSTR psz2) throw()
		{
			return( str1.compare( psz2 ) >= 0 );
		}

		friend bool operator>=(
			PCSTR psz1,
			const FStringA& str2) throw()
		{
			return( str2.compare( psz1 ) <= 0 );
		}

		friend bool operator==(
			char ch1,
			const FStringA& str2) throw()
		{
			return( (str2.getLength() == 1) && (str2[0] == ch1) );
		}

		friend bool operator==(
			_In_ const FStringA& str1,
			_In_ char ch2) throw()
		{
			return( (str1.getLength() == 1) && (str1[0] == ch2) );
		}

		friend bool operator!=(
			WCHAR ch1,
			const FStringA& str2) throw()
		{
			return( (str2.getLength() != 1) || (str2[0] != ch1) );
		}

		friend bool operator!=(
			const FStringA& str1,
			char ch2) throw()
		{
			return( (str1.getLength() != 1) || (str1[0] != ch2) );
		}

	private:
		FSTRINGA_DATA m_StringData;
	};


	//////////////////////////////////////////////////////////////////////////
	// FStringW
	class FOOUI_LIB FStringW
	{
	public:
		FStringW(void);
		FStringW(PCWSTR pszSrc);
		FStringW(const FStringW& strSrc);
		~FStringW(void);

	public:
		void append(PCWSTR pszSrc);
		void append(PCWSTR pszSrc, int nLength);
		void appendChar(wchar_t ch);
		void append(const FStringW& strSrc);
		void empty(void);
		void freeExtra(void);
		int getAllocLength() const;
		wchar_t getAt(int iChar) const;
		PWSTR getBuffer(void);
		PWSTR getBuffer(int nMinBufferLength);
		PWSTR getBufferSetLength(int nLength);
		int getLength(void) const;
		PCWSTR getString(void) const;
		bool isEmpty(void) const;
		PWSTR lockBuffer(void);
		void unlockBuffer(void);
		void releaseBuffer(int nNewLength = -1);
		void releaseBufferSetLength(int nNewLength);
		void truncate(int nNewLength);
		void setAt(int iChar, wchar_t ch);
		void setString(PCWSTR pszSrc);
		void setString(PCWSTR pszSrc, int nLength);
		int compare(PCWSTR psz) const;
		int compareNoCase(PCWSTR psz) const;
		int collate(PCWSTR psz) const;
		int collateNoCase(PCWSTR psz) const;
		int deleteAt(int iIndex, int nCount = 1);
		int insertAt(int iIndex, wchar_t ch);
		int insertAt(int iIndex, PCWSTR psz);
		int replace(wchar_t chOld, wchar_t chNew);
		int replace(PCWSTR pszOld, PCWSTR pszNew);
		int remove(wchar_t chRemove);
		FStringW tokenize(PCWSTR pszTokens, int& istart) const;
		int find(wchar_t ch, int istart = 0) const;
		int find(PCWSTR pszSub, int istart = 0) const;
		int findOneOf(PCWSTR pszCharSet) const;
		int reverseFind(wchar_t ch) const;
		FStringW& makeUpper(void);
		FStringW& makeLower(void);
		FStringW& makeReverse(void);
		FStringW& trimRight(void);
		FStringW& trimLeft(void);
		FStringW& trim(void);
		FStringW& trim(wchar_t chTarget);
		FStringW& trim(PCWSTR pszTargets);
		FStringW& trimRight(wchar_t chTarget);
		FStringW& trimRight(PCWSTR pszTargets);
		FStringW& trimLeft(wchar_t chTarget);
		FStringW& trimLeft(PCWSTR pszTargets);
		FStringW mid(int iFirst) const;
		FStringW mid(int iFirst, int nCount) const;
		FStringW right(int nCount) const;
		FStringW left(int nCount) const;
		FStringW spanIncluding(PCWSTR pszCharSet) const;
		FStringW spanExcluding(PWSTR pszCharSet) const;
		void __cdecl format(PCWSTR pszFormat, ...);
		void __cdecl appendFormat(PCWSTR pszFormat, ...);
		void formatV(PCWSTR pszFormat, va_list args);
		void appendFormatV(PCWSTR pszFormat, va_list args);

	protected:
		static void __cdecl concatenate(
			FStringW& strResult,
			PCWSTR psz1,
			int nLength1,
			PCWSTR psz2,
			int nLength2);
		static void __cdecl copyChars(
			wchar_t* pchDest,
			const wchar_t* pchSrc,
			int nChars);
		static void __cdecl copyChars(
			wchar_t* pchDest,
			size_t nDestLen,
			const wchar_t* pchSrc,
			int nChars);
		static int __cdecl stringLength(const wchar_t* psz);

	public:
		wchar_t operator[](int iChar) const;
		operator PCWSTR() const;

	public:
		FStringW& operator=(const FStringW& strSrc);
		FStringW& operator=(PCWSTR pszSrc);
		FStringW& operator+=(const FStringW& strSrc);
		FStringW& operator+=(PCWSTR pszSrc);
		FStringW& operator+=(wchar_t ch);

	public:
		friend FStringW operator+(
			const FStringW& str1,
			const FStringW& str2)
		{
			FStringW strResult;
			concatenate( strResult, str1, str1.getLength(), str2, str2.getLength() );
			return( strResult );
		}

		friend FStringW operator+(
			const FStringW& str1,
			PCWSTR psz2)
		{
			FStringW strResult;
			concatenate( strResult, str1, str1.getLength(), psz2, stringLength( psz2 ) );
			return( strResult );
		}

		friend FStringW operator+(
			PCWSTR psz1,
			const FStringW& str2)
		{
			FStringW strResult;
			concatenate( strResult, psz1, stringLength( psz1 ), str2, str2.getLength() );
			return( strResult );
		}

		friend bool operator==(
			const FStringW& str1,
			const FStringW& str2)
		{
			return( str1.compare( str2 ) == 0 );
		}

		friend bool operator==(
			const FStringW& str1,
			PCWSTR psz2)
		{
			return( str1.compare( psz2 ) == 0 );
		}

		friend bool operator==(
			PCWSTR psz1,
			const FStringW& str2)
		{
			return( str2.compare( psz1 ) == 0 );
		}

		friend bool operator!=(
			const FStringW& str1,
			const FStringW& str2 )
		{
			return( str1.compare( str2 ) != 0 );
		}

		friend bool operator!=(
			const FStringW& str1,
			PCWSTR psz2)
		{
			return( str1.compare( psz2 ) != 0 );
		}

		friend bool operator!=(
			PCWSTR psz1,
			const FStringW& str2)
		{
			return( str2.compare( psz1 ) != 0 );
		}

		friend bool operator<(
			const FStringW& str1,
			const FStringW& str2) throw()
		{
			return( str1.compare( str2 ) < 0 );
		}

		friend bool operator<(
			const FStringW& str1,
			PCWSTR psz2 ) throw()
		{
			return( str1.compare( psz2 ) < 0 );
		}

		friend bool operator<(
			PCWSTR psz1,
			const FStringW& str2) throw()
		{
			return( str2.compare( psz1 ) > 0 );
		}

		friend bool operator>(
			const FStringW& str1,
			const FStringW& str2) throw()
		{
			return( str1.compare( str2 ) > 0 );
		}

		friend bool operator>(
			const FStringW& str1,
			PCWSTR psz2) throw()
		{
			return( str1.compare( psz2 ) > 0 );
		}

		friend bool operator>(
			PCWSTR psz1,
			const FStringW& str2) throw()
		{
			return( str2.compare( psz1 ) < 0 );
		}

		friend bool operator<=(
			const FStringW& str1,
			const FStringW& str2) throw()
		{
			return( str1.compare( str2 ) <= 0 );
		}

		friend bool operator<=(
			const FStringW& str1,
			PCWSTR psz2) throw()
		{
			return( str1.compare( psz2 ) <= 0 );
		}

		friend bool operator<=(
			PCWSTR psz1,
			const FStringW& str2) throw()
		{
			return( str2.compare( psz1 ) >= 0 );
		}

		friend bool operator>=(
			const FStringW& str1,
			const FStringW& str2) throw()
		{
			return( str1.compare( str2 ) >= 0 );
		}

		friend bool operator>=(
			const FStringW& str1,
			PCWSTR psz2) throw()
		{
			return( str1.compare( psz2 ) >= 0 );
		}

		friend bool operator>=(
			PCWSTR psz1,
			const FStringW& str2) throw()
		{
			return( str2.compare( psz1 ) <= 0 );
		}

		friend bool operator==(
			wchar_t ch1,
			const FStringW& str2) throw()
		{
			return( (str2.getLength() == 1) && (str2[0] == ch1) );
		}

		friend bool operator==(
			_In_ const FStringW& str1,
			_In_ wchar_t ch2) throw()
		{
			return( (str1.getLength() == 1) && (str1[0] == ch2) );
		}

		friend bool operator!=(
			WCHAR ch1,
			const FStringW& str2) throw()
		{
			return( (str2.getLength() != 1) || (str2[0] != ch1) );
		}

		friend bool operator!=(
			const FStringW& str1,
			wchar_t ch2) throw()
		{
			return( (str1.getLength() != 1) || (str1[0] != ch2) );
		}

	private:
		FSTRINGW_DATA m_StringData;
	};

	//////////////////////////////////////////////////////////////////////////
	// FW2A
	class FOOUI_LIB FW2A
	{
	public:
		explicit FW2A(LPCWSTR lpWideCharStr);
		explicit FW2A(LPCSTR lpMultiByteStr);

	public:
		operator PCSTR() const;

	private:
		FSTRINGA_DATA m_StringData;
	};

	//////////////////////////////////////////////////////////////////////////
	// FA2W
	class FOOUI_LIB FA2W
	{
	public:
		explicit FA2W(LPCSTR lpMultiByteStr);
		explicit FA2W(LPCWSTR lpWideCharStr);

	public:
		operator PCWSTR() const;

	private:
		FSTRINGW_DATA m_StringData;
	};

#ifdef _UNICODE
	typedef	FStringW FStringT, FString;
	typedef FA2W FA2T, FT2W, FW2T;
	typedef FW2A FT2A;
#else
	typedef	FStringA FStringT, FString;
	typedef FW2A FW2T, FT2A, FA2T;
	typedef FA2W FT2W;
#endif // _UNICODE

}} // !namespace

#endif // !FSTRING_H