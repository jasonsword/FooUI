/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FApplication.h
* ������		FApplication
* �������ڣ�	2013-06-06
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
		/** ��ʼ����ʵ�� */
		int exce(void);
		virtual bool idle(int nIdleCount) { return false; }

	public:
		/** ����ʵ�� */
		static void exit(int nExitCode);
		static void doEvents(void);
		static bool isIdleMessage(MSG* pMsg);

	public:
		static int getWindowCount(void);

	public:
		/** ��Ϣѭ����ÿ����Ϣ��������������� 
			������true�� ��Ϣѭ����ֹ������Ϣ����Ͷ��
			* �����ʹ��FApplication�ڲ�����Ϣѭ����Ӧ������Ϣ������ת��
		*/
		virtual bool translateMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		/** ����Instance��� */
		static HINSTANCE getInstance(void);
		operator HINSTANCE();
		/** ����ʵ����Ŀ¼ */
		static LPCTSTR getApplicationDirectory(void);
		/** ����ȫ��Ψһ��Application�� */
		static FApplication* getApplication(void);
		/** ������Ⱦ�������� */
		static eFRenderEngineType getRenderEngineType(void);

		static bool addMessageFilter(FMessageFilter* msgFilter);
		static bool removeMessageFilter(FMessageFilter* msgFilter);
	};

}} // !namespace

#endif // !FAPPLICATION_H