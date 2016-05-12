/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FApplication.h
* 描述：		FApplication
* 创建日期：	2013-06-06
*
*/

#ifndef FAPPLICATION_H
#define FAPPLICATION_H

#include "core/FBase.h"

namespace FooUI { namespace Core {

	//////////////////////////////////////////////////////////////////////////
	// 
	class FOOUI_LIB FMessageFilter
	{
	public:
		virtual ~FMessageFilter(void) { }

	public:
		virtual bool translateMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	// FApplication
	class FOOUI_LIB FApplication
	{
	public:
		enum eFRenderEngineType
		{
			none = 0, 
			gdi, 
			gdip, 
			skia, 
			agg, 
			cairo, 
		};

		FApplication(void);
		~FApplication(void);

	public:
		/** 开始运行实例 */
		int exce(void);
		virtual bool idle(int nIdleCount) { return false; }

	public:
		/** 结束实例 */
		static void exit(int nExitCode);
		static void doEvents(void);
		static bool isIdleMessage(MSG* pMsg);

	public:
		static int getWindowCount(void);

	public:
		/** 消息循环的每条消息都将经过这个函数 
			当返回true， 消息循环终止将该消息往下投递
			* 如果不使用FApplication内部的消息循环，应当将消息往这里转发
		*/
		virtual bool translateMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		/** 返回Instance句柄 */
		static HINSTANCE getInstance(void);
		operator HINSTANCE();
		/** 返回实例根目录 */
		static LPCTSTR getApplicationDirectory(void);
		/** 返回全局唯一的Application类 */
		static FApplication* getApplication(void);
		/** 返回渲染引擎类型 */
		static eFRenderEngineType getRenderEngineType(void);

		static bool addMessageFilter(FMessageFilter* msgFilter);
		static bool removeMessageFilter(FMessageFilter* msgFilter);
	};

}} // !namespace

#endif // !FAPPLICATION_H