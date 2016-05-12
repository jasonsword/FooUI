/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FTimer.h
* 描述：		FTimer
* 创建日期：	2013-06-25
*
* 警告：		::SetTimer(NULL, NULL, interval, callback)
*				此Timer在线程中需要消息循环维持
*/

#ifndef FTIMER_H
#define FTIMER_H

#include "core/FBase.h"
#include "core/FObject.h"

namespace FooUI { namespace Core {

	//////////////////////////////////////////////////////////////////////////
	// FTimer
	class FOOUI_LIB FTimer : public FNoncopyableObject
	{
	public:
		FTimer(void);
		~FTimer(void);

	public:
		bool startTimer(int intervalInMilliseconds);
		bool stopTimer(void);
		int getTimerInterval(void) const;
		bool isTimerRunning(void) const;

	private:
		virtual void timeout(void) { }

	private:
		class FTimerPrivate;
		int m_interval;
		FTimerPrivate* m_timerPrivate;
	};

}} // !namespace

#endif // !FTIMER_H