/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FZipPackage_p.h
* 描述：		FZipPackage_p, ZIP资源包
* 创建日期：	2013-08-19
*
*/

#ifndef FZIPPACKAGE_P_H
#define FZIPPACKAGE_P_H

#include "core/FBase.h"
#include "markup/FMarkup.h" 
 

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// FZipPackage
	class FZipPackage : public CSkinPackage
	{
	public:
		FZipPackage(void);
		~FZipPackage(void);

	public:
		/*bool open(LPCTSTR pszFileName, LPCTSTR pszPassword = NULL, int nPasswordLength = -1);*/
		bool open(LPCTSTR pszFileName);
		bool open(void* pStream, int nStreamLength, LPCTSTR pszPassword = NULL, int nPasswordLength = -1);
		//bool FZipPackage::open(LPCTSTR pszFileName, int len);

		bool isOpened(void) const;
		bool findFile(LPCTSTR pszFileName) const;
		/** 从资源包中获取文件
			返回值：大于0，所需缓冲区大小
					小于0，没有找到这个文件或者发生错误
		*/
		int unpackFile(LPCTSTR pszFileName, void* buffer, int bufferLength) const;
		bool close(void);
 
	};

}} // !namespace

#endif // !FZIPPACKAGE_P_H


////////

