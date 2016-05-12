/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FZipPackage_p.h
* ������		FZipPackage_p, ZIP��Դ��
* �������ڣ�	2013-08-19
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
		/** ����Դ���л�ȡ�ļ�
			����ֵ������0�����軺������С
					С��0��û���ҵ�����ļ����߷�������
		*/
		int unpackFile(LPCTSTR pszFileName, void* buffer, int bufferLength) const;
		bool close(void);
 
	};

}} // !namespace

#endif // !FZIPPACKAGE_P_H


////////

