/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FMarkup.h
* ������		FMarkup
* �������ڣ�	2013-06-14
*
*/

#ifndef FMARKUP_H
#define FMARKUP_H

#include "core/FBase.h"
#include "markup/FMarkupDef.h"
#include "markup/FResourceDef.h"

namespace FooUI { namespace Widgets { class FWidget; }}
namespace FooUI { namespace Styles { class FStyleObject; }}

namespace FooUI { namespace Markup {

	//////////////////////////////////////////////////////////////////////////
	// CSkinPackage
	class FOOUI_LIB CSkinPackage
	{
	public:
		virtual ~CSkinPackage(void) { }

	public:
		virtual bool open(LPCTSTR pszFileName) = 0;
		virtual bool open(void* pStream, int nStreamLength, LPCTSTR pszPassword = NULL, int nPasswordLength = -1) = 0;
		virtual bool isOpened(void) const = 0;
		virtual bool findFile(LPCTSTR pszFileName) const = 0;
		/** ����Դ���л�ȡ�ļ�
			����ֵ������0�����軺������С
					С��0��û���ҵ�����ļ����߷�������
		*/
		virtual int unpackFile(LPCTSTR pszFileName, void* buffer, int bufferLength) const = 0;
		virtual bool close(void) = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	// FPathHelper
	class FOOUI_LIB FPathHelper
	{
	public:
		/** ����pszPath������·�� 
			�ο���Ŀ¼���ȼ���FMarkup::getResourcePath
					ʵ����Ŀ¼
		*/
		static LPCTSTR pathHelper(LPCTSTR pszPath);
		/** �ο�pszRootPath����pszPath������·�� 
			���磺
				pszRootPath = "C:\1\2"
				pszPath = "..\abc"
				���Ϊ��"C:\1\abc"
		*/
		static LPCTSTR pathHelper(LPCTSTR pszRootPath, LPCTSTR pszPath);
	};

	//////////////////////////////////////////////////////////////////////////
	// FMARKUP_CREATEWIDGET_CALLBACK
	typedef Widgets::FWidget* (CALLBACK *FMARKUP_CREATEWIDGET_CALLBACK)(LPCTSTR pszName, LPVOID lpParameter);

	//////////////////////////////////////////////////////////////////////////
	// FMarkup
	class FOOUI_LIB FMarkup
	{
	public:
		enum
		{
			kUnknownResourcePackage = 0, 
			kZipResourcePackage = 1, 
			kFooUIResourcePackage = 2, 
		};

	public:
		virtual bool addResource(LPCTSTR pszName, DWORD dwType, FResourceObject* obj) = 0;
		virtual FResourceObject* getResource(LPCTSTR pszName, DWORD dwType) const = 0;
		virtual Styles::FStyleObject* createStyle(LPCTSTR pszName) const = 0;
		virtual Widgets::FWidget* createWidget(LPCTSTR pszName) const = 0;

	public:
		/** ����/������Դ��Ŀ¼
			���������ʵ����Ŀ¼������"..\..\path"
			�����·��������"C:\"
			��û������ʱĬ��Ϊʵ����Ŀ¼��
		*/
		static void setResourcePath(LPCTSTR pszResourcePath);
		static LPCTSTR getResourcePath(void);

		/** Ƥ����Դ��
		*/
		static CSkinPackage* getSkinPackage(void);
		static bool isSkinPackageOpened(void);
		static int unpackFile(LPCTSTR pszFileName, void* buffer, int bufferLength);
		static bool openZipSkinPackage(LPCTSTR pszFileName, LPCTSTR pszPassword = NULL);
		static bool openZipSkinPackage(void* pStream, int nStreamLength, LPCTSTR pszPassword = NULL);
		static bool openZipSkinPackageFromResource(LPCTSTR lpResName, LPCTSTR lpResType, LPCTSTR pszPassword = NULL);
		static bool openZipSkinPackageFromResource(HMODULE hResModule, LPCTSTR lpResName, LPCTSTR lpResType, LPCTSTR pszPassword = NULL);

	public:
		static bool markup(LPCTSTR pszFileName, 
						Widgets::FWidget* pRootWidget, 
						FMARKUP_CREATEWIDGET_CALLBACK callback = NULL, 
						LPVOID callback_parameter = NULL);

		static bool markup(void* pStream, DWORD dwStreamLength, 
						Widgets::FWidget* pRootWidget, 
						FMARKUP_CREATEWIDGET_CALLBACK callback = NULL, 
						LPVOID callback_parameter = NULL);

	private:
		class FMarkupParser;
		class FMarkupZipUtil;
	};

}} // !namespace

#endif // !FMARKUP_H