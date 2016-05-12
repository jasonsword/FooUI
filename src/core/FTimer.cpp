/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FTimer.cpp
* 描述：		FTimer
* 创建日期：	2013-06-25
*
*/

#include "StdAfx.h"
#include "core/FObject.h"
#include "core/FLock.h"
#include "core/FTimer.h"

#include "widgets/thunk/atlstdthunk.h"

#define FTIMER_INLINE FOOINLINE

namespace FooUI { namespace Core {

	//////////////////////////////////////////////////////////////////////////
	// FTimerPrivate
	class FTimer::FTimerPrivate
	{
	public:
		explicit FTimerPrivate(FTimer* pTimer);
		~FTimerPrivate(void);

	public:
		bool startTimer (int intervalInMilliseconds);
		bool stopTimer(void);
		bool isTimerRunning(void) const;

	private:
		void timeout(void);

	private:
		FTimer* m_pTimer;
		ATL::CStdCallThunk m_thunk;
		UINT_PTR m_nIDEvent;

	private:
		static VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	};

	//////////////////////////////////////////////////////////////////////////
	// FTimerPrivate
	FTimer::FTimerPrivate::FTimerPrivate(FTimer* pTimer) :
		m_pTimer(pTimer) 
		, m_nIDEvent(NULL)
	{
		FASSERT(NULL != m_pTimer);
		m_thunk.Init((DWORD_PTR)TimerProc, this);
	}

	FTimer::FTimerPrivate::~FTimerPrivate(void)
	{
		if (isTimerRunning())
		{
			stopTimer();
		}
	}

	bool FTimer::FTimerPrivate::startTimer (int intervalInMilliseconds)
	{
		if (isTimerRunning())
		{
			//FASSERT(FALSE);
			return false;
		}

		m_nIDEvent = ::SetTimer(NULL, NULL, intervalInMilliseconds, (TIMERPROC)m_thunk.GetCodeAddress());
		return (m_nIDEvent != 0);
	}

	bool FTimer::FTimerPrivate::stopTimer(void)
	{
		if (!isTimerRunning())
		{
			//FASSERT(FALSE);
			return false;
		}

		if (::KillTimer(NULL, m_nIDEvent))
		{
			m_nIDEvent = 0;
		}

		return (m_nIDEvent == 0);
	}

	bool FTimer::FTimerPrivate::isTimerRunning(void) const
	{
		return (m_nIDEvent != 0);
	}

	void FTimer::FTimerPrivate::timeout(void)
	{
		FASSERT(NULL != m_pTimer);
		m_pTimer->timeout();
	}

	/*static*/ VOID CALLBACK FTimer::FTimerPrivate::TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
	{
		FTimerPrivate *pTimerData = (FTimerPrivate*)hwnd;
		if (NULL == pTimerData)
		{
			FASSERT(FALSE);
		}
		else
		{
			pTimerData->timeout();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// FTimer
	FTIMER_INLINE FTimer::FTimer(void) : 
		m_timerPrivate(new FTimerPrivate(this)) 
		, m_interval(0)
	{
	}

	FTIMER_INLINE FTimer::~FTimer(void)
	{
		FASSERT(NULL != m_timerPrivate);
		fooSafeDelete(m_timerPrivate);
	}

	FTIMER_INLINE bool FTimer::startTimer(int intervalInMilliseconds)
	{
		if (m_timerPrivate->isTimerRunning())
		{
			if (intervalInMilliseconds == m_interval)
			{
				return true;
			}
			else
			{
				stopTimer();
			}
		}

		m_interval = intervalInMilliseconds;
		return m_timerPrivate->startTimer(intervalInMilliseconds);
	}

	FTIMER_INLINE bool FTimer::stopTimer(void)
	{
		if (!m_timerPrivate->isTimerRunning())
		{
			return false;
		}

		return m_timerPrivate->stopTimer();
	}

	FTIMER_INLINE int FTimer::getTimerInterval(void) const
	{
		return m_interval;
	}

	FTIMER_INLINE bool FTimer::isTimerRunning(void) const
	{
		return m_timerPrivate->isTimerRunning();
	}

}} // !namespace