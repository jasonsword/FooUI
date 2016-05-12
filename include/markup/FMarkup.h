/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FMarkup.h
* 描述：		FMarkup
* 创建日期：	2013-06-14
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
		/** 从资源包中获取文件
			返回值：大于0，所需缓冲区大小
					小于0，没有找到这个文件或者发生错误
		*/
		virtual int unpackFile(LPCTSTR pszFileName, void* buffer, int bufferLength) const = 0;
		virtual bool close(void) = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	// FPathHelper
	class FOOUI_LIB FPathHelper
	{
	public:
		/** 解析pszPath到完整路径 
			参考根目录优先级：FMarkup::getResourcePath
					实例根目录
		*/
		static LPCTSTR pathHelper(LPCTSTR pszPath);
		/** 参考pszRootPath解析pszPath到完整路径 
			例如：
				pszRootPath = "C:\1\2"
				pszPath = "..\abc"
				结果为："C:\1\abc"
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
		/** 设置/返回资源根目录
			可以相对于实例根目录，例如"..\..\path"
			或绝对路径，例如"C:\"
			（没有设置时默认为实例根目录）
		*/
		static void setResourcePath(LPCTSTR pszResourcePath);
		static LPCTSTR getResourcePath(void);

		/** 皮肤资源包
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