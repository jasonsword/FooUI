/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FTimer.h
* ������		FTimer
* �������ڣ�	2013-06-25
*
* ���棺		::SetTimer(NULL, NULL, interval, callback)
*				��Timer���߳�����Ҫ��Ϣѭ��ά��
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