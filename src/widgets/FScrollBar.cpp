/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FScrollBar.h
* 描述：		FScrollBar
* 创建日期：	2013-06-25
*
*/

#include "StdAfx.h"
#include "core/FTimer.h"
#include "widgets/FWidget.h"
#include "widgets/FScrollBar.h"

#define FSCROLLBAR_TYPE_NAME_HORZ	_T("horz")
#define FSCROLLBAR_TYPE_NAME_VERT	_T("vert")
#define FSCROLLBAR_TYPE_NAME_CTL	_T("ctl")

/** 如果鼠标偏离超出此值，还原到旧的position */
#define FSCROLLBAR_ESCAPE 50

#define FSCROLLBAR_INLINE FOOINLINE

#define FSCROLLBAR_TIMER_INTERVAL 200

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FScrollBarPrivate
	class FScrollBar::FScrollBarPrivate
	{
	public:
		enum eScrollBarButtonRole
		{
			None = 0, 
			Sub, 
			Add, 
			Thumb, 
			Track, 
			Min, 
			Max, 
		};
		friend FScrollBar;
		explicit FScrollBarPrivate(DWORD dwOrientation, FScrollBar* pScrollBar);
		~FScrollBarPrivate(void);

	protected:
		void mousePressEvent(Event::FMouseEvent* e);
		void mouseReleaseEvent(Event::FMouseEvent* e);
		void mouseDoubleClickEvent(Event::FMouseEvent* e);
		void mouseMoveEvent(Event::FMouseEvent* e);
		void mouseEnterEvent(Event::FEvent* e);
		void mouseLeaveEvent(Event::FEvent* e);

	public:
		void setOrientation(DWORD dwOrientation);
		DWORD getOrientation(void) const;
		bool isHorzOrientation(void) const;
		bool isVertOrientation(void) const;

		void trackPosition(int nTrackPosition);
		int trackPosition(void) const;
		double trackPercent(void) const;

	protected:
		FScrollBar* scrollbar(void) { return m_pScrollBar; }
		void calcScrollButtonLayout(void);
		void calcScrollButtonLayout(const Core::FRect& rcLayout);

	private:
		FScrollBar* const m_pScrollBar;
		DWORD m_dwOrientation;
		int m_nTrackPosition;

	protected:
		class FScrollBarButton : public Core::FTimer
		{
			friend FScrollBarPrivate;
		public:
			explicit FScrollBarButton(FScrollBarPrivate* pScrollBarData, DWORD dwRole);
			~FScrollBarButton(void);

		protected:
			void mousePressEvent(Event::FMouseEvent* e);
			void mouseReleaseEvent(Event::FMouseEvent* e);
			void mouseDoubleClickEvent(Event::FMouseEvent* e);
			void mouseMoveEvent(Event::FMouseEvent* e);
			void mouseEnterEvent(Event::FEvent* e);
			void mouseLeaveEvent(Event::FEvent* e);

		protected:
			FScrollBar* scrollbar(void) { return m_scrollbarPrivate->scrollbar(); }
			void timeout(void);

		public:
			void setLayoutRect(const Core::FRect& rcLayout);
			void setLayoutRect(int xLeft, int yTop, int xWidth, int yHeight);
			void setLayoutXYWH(int xLeft, int yTop, int xRight, int yBottom);
			const Core::FRect& getLayoutRect(void) const;
			void setState(DWORD dwState, bool bRepaint);
			void modifyState(DWORD dwRemove, DWORD dwAdd, bool bRepaint);
			DWORD getState(void) const;
			DWORD getRole(void) const;

		public:
			void setVisible(bool bVisible);
			bool isVisible(void) const;
			void setEnabled(bool bEnabled);
			bool isEnabled(void) const;

		private:
			FScrollBarPrivate* m_scrollbarPrivate;
			Core::FRect m_rcLayout;
			DWORD m_dwState;
			DWORD m_dwRole;

			static int old_trackpos;
			static int old_buttonpos;
			static Core::FPoint mouse_point;
		};

		FScrollBarButton* buttonFromPoint(int xPos, int yPos);
		FScrollBarButton m_btnSub;
		FScrollBarButton m_btnAdd;
		FScrollBarButton m_btnThumb;
		FScrollBarButton m_btnTrack;
		FScrollBarButton m_btnMin;
		FScrollBarButton m_btnMax;
		FScrollBarButton* m_pBtnBusy;
	};

	// 
	FSCROLLBAR_INLINE FScrollBar::FScrollBarPrivate::FScrollBarPrivate(DWORD dwOrientation, FScrollBar* pScrollBar) : 
		m_dwOrientation(dwOrientation) 
		, m_pScrollBar(pScrollBar) 
		, m_nTrackPosition(100) 
		, m_btnSub(this, FScrollBar::FScrollBarPrivate::Sub) 
		, m_btnAdd(this, FScrollBar::FScrollBarPrivate::Add) 
		, m_btnThumb(this, FScrollBar::FScrollBarPrivate::Thumb) 
		, m_btnTrack(this, FScrollBar::FScrollBarPrivate::Track) 
		, m_btnMin(this, FScrollBar::FScrollBarPrivate::Min) 
		, m_btnMax(this, FScrollBar::FScrollBarPrivate::Max) 
		, m_pBtnBusy(NULL) 
	{
		FASSERT(NULL != m_pScrollBar);

		m_btnMin.setVisible(false);
		m_btnMax.setVisible(false);
	}

	FSCROLLBAR_INLINE FScrollBar::FScrollBarPrivate::~FScrollBarPrivate(void)
	{
	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::mousePressEvent(Event::FMouseEvent* e)
	{
		m_pBtnBusy = buttonFromPoint(e->mouse.x, e->mouse.y);
		if (NULL != m_pBtnBusy)
		{
			m_pBtnBusy->mousePressEvent(e);
		}
	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::mouseReleaseEvent(Event::FMouseEvent* e)
	{
		if (NULL != m_pBtnBusy)
		{
			m_pBtnBusy->mouseReleaseEvent(e);
		}
	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::mouseDoubleClickEvent(Event::FMouseEvent* e)
	{
		m_pBtnBusy = buttonFromPoint(e->mouse.x, e->mouse.y);
		if (NULL != m_pBtnBusy)
		{
			m_pBtnBusy->mouseDoubleClickEvent(e);
		}
	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::mouseMoveEvent(Event::FMouseEvent* e)
	{
		if (NoButton == e->button)
		{
			FScrollBarButton *pMouseButton = buttonFromPoint(e->mouse.x, e->mouse.y);
			if (pMouseButton != m_pBtnBusy)
			{
				if (NULL != m_pBtnBusy)
				{
					m_pBtnBusy->mouseLeaveEvent(Event::FEvent(Event::FEvent::MouseLeave));
				}

				if (NULL != pMouseButton)
				{
					pMouseButton->mouseEnterEvent(Event::FEvent(Event::FEvent::MouseEnter));
				}

				m_pBtnBusy = pMouseButton;
			}
			else if (NULL != m_pBtnBusy)
			{
				m_pBtnBusy->mouseMoveEvent(e);
			}
		}
		else if (NULL != m_pBtnBusy)
		{
			m_pBtnBusy->mouseMoveEvent(e);
		}
	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::mouseEnterEvent(Event::FEvent* e)
	{

	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::mouseLeaveEvent(Event::FEvent* e)
	{
		if (NULL != m_pBtnBusy)
		{
			m_pBtnBusy->mouseLeaveEvent(e);
			m_pBtnBusy = NULL;
		}
	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::setOrientation(DWORD dwOrientation)
	{
		m_dwOrientation = dwOrientation;
	}

	FSCROLLBAR_INLINE DWORD FScrollBar::FScrollBarPrivate::getOrientation(void) const
	{
		return m_dwOrientation;
	}

	FSCROLLBAR_INLINE bool FScrollBar::FScrollBarPrivate::isHorzOrientation(void) const
	{
		return (FScrollBar::Horz == m_dwOrientation);
	}

	FSCROLLBAR_INLINE bool FScrollBar::FScrollBarPrivate::isVertOrientation(void) const
	{
		return (FScrollBar::Vert == m_dwOrientation);
	}

	void FScrollBar::FScrollBarPrivate::trackPosition(int nTrackPosition)
	{
		m_nTrackPosition = nTrackPosition;
	}

	int FScrollBar::FScrollBarPrivate::trackPosition(void) const
	{
		return m_nTrackPosition;
	}

	double FScrollBar::FScrollBarPrivate::trackPercent(void) const
	{
		if (m_pScrollBar->maximum() - m_pScrollBar->minimum() <= 0) return 0.0f;
		return (double)m_nTrackPosition / (double)(m_pScrollBar->maximum() - m_pScrollBar->minimum());
	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::calcScrollButtonLayout(void)
	{
		calcScrollButtonLayout(m_pScrollBar->getLayoutRect());
	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::calcScrollButtonLayout(const Core::FRect& rcLayout)
	{
		Core::FRect rcUsable = rcLayout;
		rcUsable.deflateRect(m_pScrollBar->getPadding());

		if (isHorzOrientation())
		{
			int nCxHScroll = ::GetSystemMetrics(SM_CXHSCROLL);
			int nCyHScroll = ::GetSystemMetrics(SM_CYHSCROLL);

			if (m_btnMin.isVisible())
			{
				m_btnMin.setLayoutXYWH(rcUsable.left, rcUsable.top, nCyHScroll, rcUsable.height());
				rcUsable.left += m_btnMin.getLayoutRect().width();
			}

			if (m_btnSub.isVisible())
			{
				m_btnSub.setLayoutXYWH(rcUsable.left, rcUsable.top, nCyHScroll, rcUsable.height());
				rcUsable.left += m_btnSub.getLayoutRect().width();
			}

			if (m_btnMax.isVisible())
			{
				m_btnMax.setLayoutXYWH(rcUsable.right - nCyHScroll, rcUsable.top, nCyHScroll, rcUsable.height());
				rcUsable.right -= m_btnMax.getLayoutRect().width();
			}

			if (m_btnAdd.isVisible())
			{
				m_btnAdd.setLayoutXYWH(rcUsable.right - nCyHScroll, rcUsable.top, nCyHScroll, rcUsable.height());
				rcUsable.right -= m_btnAdd.getLayoutRect().width();
			}

			m_btnTrack.setLayoutRect(rcUsable);

			int nThumbWidth = 0;
			if (m_pScrollBar->maximum() - m_pScrollBar->minimum() <= 0)
			{
				nThumbWidth = nCyHScroll;
			}
			else
			{
				nThumbWidth = (int)((double)m_pScrollBar->page() / (double)(m_pScrollBar->maximum() - m_pScrollBar->minimum()) 
								* m_btnTrack.getLayoutRect().width());
				nThumbWidth = max(nThumbWidth, nCyHScroll);
				nThumbWidth = min(nThumbWidth, m_btnTrack.getLayoutRect().width() - nCyHScroll);
			}

			int nThumbLeft = m_btnTrack.getLayoutRect().left + (int)((m_btnTrack.getLayoutRect().width() - nThumbWidth) * trackPercent());
			m_btnThumb.setLayoutXYWH(nThumbLeft, m_btnTrack.getLayoutRect().top, nThumbWidth, m_btnTrack.getLayoutRect().height());
		}
		else if (isVertOrientation())
		{
			int nCxVScroll = ::GetSystemMetrics(SM_CXVSCROLL);
			int nCyVScroll = ::GetSystemMetrics(SM_CYVSCROLL);

			if (m_btnMin.isVisible())
			{
				m_btnMin.setLayoutXYWH(rcUsable.left, rcUsable.top, rcUsable.width(), nCxVScroll);
				rcUsable.top += m_btnMin.getLayoutRect().height();
			}

			if (m_btnSub.isVisible())
			{
				m_btnSub.setLayoutXYWH(rcUsable.left, rcUsable.top, rcUsable.width(), nCxVScroll);
				rcUsable.top += m_btnSub.getLayoutRect().height();
			}

			if (m_btnMax.isVisible())
			{
				m_btnMax.setLayoutXYWH(rcUsable.left, rcUsable.bottom - nCxVScroll, rcUsable.width(), nCxVScroll);
				rcUsable.bottom -= m_btnMax.getLayoutRect().height();
			}

			if (m_btnAdd.isVisible())
			{
				m_btnAdd.setLayoutXYWH(rcUsable.left, rcUsable.bottom - nCxVScroll, rcUsable.width(), nCxVScroll);
				rcUsable.bottom -= m_btnAdd.getLayoutRect().height();
			}

			m_btnTrack.setLayoutRect(rcUsable);

			int nThumbHeight = 0;
			if (m_pScrollBar->maximum() - m_pScrollBar->minimum() <= 0)
			{
				nThumbHeight = nCxVScroll;
			}
			else
			{
				nThumbHeight = (int)((double)m_pScrollBar->page() / (double)(m_pScrollBar->maximum() - m_pScrollBar->minimum()) 
					* m_btnTrack.getLayoutRect().height());
				nThumbHeight = max(nThumbHeight, nCxVScroll);
				nThumbHeight = min(nThumbHeight, m_btnTrack.getLayoutRect().height() - nCxVScroll);
			}

			int nThumbTop = m_btnTrack.getLayoutRect().top + (int)((m_btnTrack.getLayoutRect().height() - nThumbHeight) * trackPercent());
			m_btnThumb.setLayoutXYWH(m_btnTrack.getLayoutRect().left, nThumbTop, m_btnTrack.getLayoutRect().width(), nThumbHeight);
		}
	}

	FScrollBar::FScrollBarPrivate::FScrollBarButton* FScrollBar::FScrollBarPrivate::buttonFromPoint(int xPos, int yPos)
	{
		if (m_btnMin.isVisible() 
			&& m_btnMin.getLayoutRect().isIntersects(xPos, yPos))
		{
			return &m_btnMin;
		}

		if (m_btnSub.isVisible() 
			&& m_btnSub.getLayoutRect().isIntersects(xPos, yPos))
		{
			return &m_btnSub;
		}

		if (m_btnMax.isVisible() 
			&& m_btnMax.getLayoutRect().isIntersects(xPos, yPos))
		{
			return &m_btnMax;
		}

		if (m_btnAdd.isVisible() 
			&& m_btnAdd.getLayoutRect().isIntersects(xPos, yPos))
		{
			return &m_btnAdd;
		}

		if (m_btnThumb.isVisible() 
			&& m_btnThumb.getLayoutRect().isIntersects(xPos, yPos))
		{
			return &m_btnThumb;
		}

		if (m_btnTrack.isVisible() 
			&& m_btnTrack.getLayoutRect().isIntersects(xPos, yPos))
		{
			return &m_btnTrack;
		}

		return NULL;
	}

	// 
	/*static*/ int FScrollBar::FScrollBarPrivate::FScrollBarButton::old_trackpos = 0;
	/*static*/ int FScrollBar::FScrollBarPrivate::FScrollBarButton::old_buttonpos = 0;
	/*static*/ Core::FPoint FScrollBar::FScrollBarPrivate::FScrollBarButton::mouse_point(0, 0);
	FSCROLLBAR_INLINE FScrollBar::FScrollBarPrivate::FScrollBarButton::FScrollBarButton(FScrollBarPrivate* pScrollBarData, DWORD dwRole) : 
		m_scrollbarPrivate(pScrollBarData) 
		, m_dwState(FOOSTATE_NONE | FOOSTATE_NORMAL | FOOSTATE_ENABLED | FOOSTATE_VISIBLE) 
		, m_dwRole(dwRole) 
	{
		FASSERT(NULL != m_scrollbarPrivate);
	}

	FSCROLLBAR_INLINE FScrollBar::FScrollBarPrivate::FScrollBarButton::~FScrollBarButton(void)
	{
	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::FScrollBarButton::mousePressEvent(Event::FMouseEvent* e)
	{
		if (LeftButton == e->button)
		{
			if (fooModifyAttribute<DWORD>(m_dwState, FOOSTATE_NORMAL | FOOSTATE_HOVER, FOOSTATE_PUSHED))
			{
				m_scrollbarPrivate->m_pScrollBar->repaint(getLayoutRect());
			}

			mouse_point = e->mouse;
			old_trackpos = m_scrollbarPrivate->trackPosition();
			if (m_scrollbarPrivate->isHorzOrientation())
			{
				old_buttonpos = getLayoutRect().left;
			}
			else if (m_scrollbarPrivate->isVertOrientation())
			{
				old_buttonpos = getLayoutRect().top;
			}

			switch (getRole())
			{
			case FScrollBarPrivate::Min:
				scrollbar()->makeWidgetMessage(FWM_HSCROLL, FAbstractSlider::Minimum, m_scrollbarPrivate->trackPosition());
				break;

			case FScrollBarPrivate::Sub:
				scrollbar()->makeWidgetMessage(FWM_HSCROLL, FAbstractSlider::Sub, m_scrollbarPrivate->trackPosition());
				startTimer(FSCROLLBAR_TIMER_INTERVAL);
				break;

			case FScrollBarPrivate::Max:
				scrollbar()->makeWidgetMessage(FWM_HSCROLL, FAbstractSlider::Maximum, m_scrollbarPrivate->trackPosition());
				break;

			case FScrollBarPrivate::Add:
				scrollbar()->makeWidgetMessage(FWM_HSCROLL, FAbstractSlider::Add, m_scrollbarPrivate->trackPosition());
				startTimer(FSCROLLBAR_TIMER_INTERVAL);
				break;

			case FScrollBarPrivate::Track:
				if (m_scrollbarPrivate->isHorzOrientation() )
				{
					if (e->mouse.x < m_scrollbarPrivate->m_btnThumb.getLayoutRect().left)
					{
						scrollbar()->makeWidgetMessage(FWM_HSCROLL, FAbstractSlider::PageSub, m_scrollbarPrivate->trackPosition());
					}
					else if (e->mouse.x > m_scrollbarPrivate->m_btnThumb.getLayoutRect().right)
					{
						scrollbar()->makeWidgetMessage(FWM_HSCROLL, FAbstractSlider::PageAdd, m_scrollbarPrivate->trackPosition());
					}
				}
				else if (m_scrollbarPrivate->isVertOrientation())
				{
					if (e->mouse.y < m_scrollbarPrivate->m_btnThumb.getLayoutRect().top)
					{
						scrollbar()->makeWidgetMessage(FWM_VSCROLL, FAbstractSlider::PageSub, m_scrollbarPrivate->trackPosition());
					}
					else if (e->mouse.y > m_scrollbarPrivate->m_btnThumb.getLayoutRect().bottom)
					{
						scrollbar()->makeWidgetMessage(FWM_VSCROLL, FAbstractSlider::PageAdd, m_scrollbarPrivate->trackPosition());
					}
				}
				startTimer(FSCROLLBAR_TIMER_INTERVAL);
				break;

			default:
				break;
			}
		} // if (LeftButton == e->button)
	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::FScrollBarButton::mouseReleaseEvent(Event::FMouseEvent* e)
	{
		if (fooModifyAttribute<DWORD>(m_dwState, FOOSTATE_NORMAL | FOOSTATE_PUSHED, FOOSTATE_HOVER))
		{
			m_scrollbarPrivate->m_pScrollBar->repaint(getLayoutRect());
		}

		if (isTimerRunning())
			stopTimer();

		if (FScrollBarPrivate::Thumb == getRole())
		{
			m_scrollbarPrivate->trackPosition(scrollbar()->sliderPosition());
			m_scrollbarPrivate->calcScrollButtonLayout();
				scrollbar()->repaint(m_scrollbarPrivate->m_btnTrack.getLayoutRect());
		}
	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::FScrollBarButton::mouseDoubleClickEvent(Event::FMouseEvent* e)
	{
		if (LeftButton == e->button)
		{
			if (fooModifyAttribute<DWORD>(m_dwState, FOOSTATE_NORMAL | FOOSTATE_HOVER, FOOSTATE_PUSHED))
			{
				m_scrollbarPrivate->m_pScrollBar->repaint(getLayoutRect());
			}

			mouse_point = e->mouse;
			old_trackpos = m_scrollbarPrivate->trackPosition();
			if (m_scrollbarPrivate->isHorzOrientation())
			{
				old_buttonpos = getLayoutRect().left;
			}
			else if (m_scrollbarPrivate->isVertOrientation())
			{
				old_buttonpos = getLayoutRect().top;
			}

			switch (getRole())
			{
			case FScrollBarPrivate::Sub:
			case FScrollBarPrivate::Add:
			case FScrollBarPrivate::Track:
			case FScrollBarPrivate::Thumb:
				startTimer(FSCROLLBAR_TIMER_INTERVAL);
				break;

			default:
				break;
			}
		}
	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::FScrollBarButton::mouseMoveEvent(Event::FMouseEvent* e)
	{
		FASSERT(NULL != m_scrollbarPrivate);
		if (LeftButton == e->button)
		{
			if (FScrollBarPrivate::Thumb == getRole())
			{
				Core::FRect rcScrollBarEscape = scrollbar()->getLayoutRect();
				rcScrollBarEscape.inflateRect(FSCROLLBAR_ESCAPE, FSCROLLBAR_ESCAPE);
				if (!rcScrollBarEscape.isIntersects(e->mouse))
				{
					m_scrollbarPrivate->trackPosition(old_trackpos);
				}
				else if (m_scrollbarPrivate->isHorzOrientation())
				{
					int nMaxValue = scrollbar()->maximum() - scrollbar()->minimum();
					int nTrackWidth = m_scrollbarPrivate->m_btnTrack.getLayoutRect().width() - getLayoutRect().width();
					int nSliderWidth = e->mouse.x - m_scrollbarPrivate->m_btnTrack.getLayoutRect().left + (old_buttonpos - mouse_point.x);
					nSliderWidth = max(nSliderWidth, 0);
					nSliderWidth = min(nSliderWidth, nTrackWidth);
					if (nTrackWidth == 0 || nMaxValue == 0)
					{
						m_scrollbarPrivate->trackPosition(0);
					}
					else
					{
						m_scrollbarPrivate->trackPosition((int)(((double)nSliderWidth / (double)nTrackWidth) * nMaxValue));
					}
				}
				else if (m_scrollbarPrivate->isVertOrientation())
				{
					int nMaxValue = scrollbar()->maximum() - scrollbar()->minimum();
					int nTrackHeight = m_scrollbarPrivate->m_btnTrack.getLayoutRect().height() - getLayoutRect().height();
					int nSliderHeight = e->mouse.y - m_scrollbarPrivate->m_btnTrack.getLayoutRect().top + (old_buttonpos - mouse_point.y);
					nSliderHeight = max(nSliderHeight, 0);
					nSliderHeight = min(nSliderHeight, nTrackHeight);
					if (nTrackHeight == 0 || nMaxValue == 0)
					{
						m_scrollbarPrivate->trackPosition(0);
					}
					else
					{
						m_scrollbarPrivate->trackPosition((int)(((double)nSliderHeight / (double)nTrackHeight) * nMaxValue));
					}
				}
				scrollbar()->makeWidgetMessage(m_scrollbarPrivate->isHorzOrientation() ? FWM_HSCROLL : FWM_VSCROLL, 
										FAbstractSlider::Track, m_scrollbarPrivate->trackPosition());
				m_scrollbarPrivate->calcScrollButtonLayout();
				scrollbar()->repaint(m_scrollbarPrivate->m_btnTrack.getLayoutRect());
			}
		}
	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::FScrollBarButton::mouseEnterEvent(Event::FEvent* e)
	{
		if (fooModifyAttribute<DWORD>(m_dwState, FOOSTATE_NORMAL | FOOSTATE_PUSHED, FOOSTATE_HOVER))
		{
			m_scrollbarPrivate->m_pScrollBar->repaint(getLayoutRect());
		}
	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::FScrollBarButton::mouseLeaveEvent(Event::FEvent* e)
	{
		if (fooModifyAttribute<DWORD>(m_dwState, FOOSTATE_HOVER | FOOSTATE_PUSHED, FOOSTATE_NORMAL))
		{
			m_scrollbarPrivate->m_pScrollBar->repaint(getLayoutRect());
		}
	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::FScrollBarButton::timeout(void)
	{
		Core::FPoint ptCursor;
		::GetCursorPos(&ptCursor);
		::ScreenToClient(scrollbar()->getSafeHwnd(), &ptCursor);
		if (!getLayoutRect().isIntersects(ptCursor))
		{
			return;
		}

		if (m_scrollbarPrivate->isHorzOrientation() 
			|| m_scrollbarPrivate->isVertOrientation())
		{
			UINT uMsg = m_scrollbarPrivate->isHorzOrientation() ? FWM_HSCROLL : FWM_VSCROLL;
			switch (getRole())
			{
			case FScrollBarPrivate::Sub:
				scrollbar()->makeWidgetMessage(uMsg, FAbstractSlider::Sub, m_scrollbarPrivate->trackPosition());
				break;

			case FScrollBarPrivate::Add:
				scrollbar()->makeWidgetMessage(uMsg, FAbstractSlider::Add, m_scrollbarPrivate->trackPosition());
				break;

			case FScrollBarPrivate::Track:
				if (m_scrollbarPrivate->isHorzOrientation())
				{
					if (ptCursor.x < m_scrollbarPrivate->m_btnThumb.getLayoutRect().left)
					{
						scrollbar()->makeWidgetMessage(uMsg, FAbstractSlider::PageSub, m_scrollbarPrivate->trackPosition());
					}
					else if (ptCursor.x > m_scrollbarPrivate->m_btnThumb.getLayoutRect().right)
					{
						scrollbar()->makeWidgetMessage(uMsg, FAbstractSlider::PageAdd, m_scrollbarPrivate->trackPosition());
					}
				}
				else if (m_scrollbarPrivate->isVertOrientation())
				{
					if (ptCursor.y < m_scrollbarPrivate->m_btnThumb.getLayoutRect().top)
					{
						scrollbar()->makeWidgetMessage(FWM_VSCROLL, FAbstractSlider::PageSub, m_scrollbarPrivate->trackPosition());
					}
					else if (ptCursor.y > m_scrollbarPrivate->m_btnThumb.getLayoutRect().bottom)
					{
						scrollbar()->makeWidgetMessage(FWM_VSCROLL, FAbstractSlider::PageAdd, m_scrollbarPrivate->trackPosition());
					}
				}
				break;

			default:
				break;
			}
		} // if (m_scrollbarPrivate->isHorzOrientation())
	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::FScrollBarButton::setLayoutRect(
													const Core::FRect& rcLayout)
	{
		m_rcLayout = rcLayout;
	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::FScrollBarButton::setLayoutRect(
													int xLeft, int yTop, int xWidth, int yHeight)
	{
		m_rcLayout.setRect(xLeft, yTop, xWidth, yHeight);
	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::FScrollBarButton::setLayoutXYWH(
													int xLeft, int yTop, int xRight, int yBottom)
	{
		m_rcLayout.setXYWH(xLeft, yTop, xRight, yBottom);
	}

	FSCROLLBAR_INLINE const Core::FRect& FScrollBar::FScrollBarPrivate::FScrollBarButton::getLayoutRect(void) const
	{
		return m_rcLayout;
	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::FScrollBarButton::setState(DWORD dwState, bool bRepaint)
	{
		if (dwState == m_dwState)
		{
			return;
		}

		m_dwState = dwState;
		if (bRepaint)
		{
			m_scrollbarPrivate->m_pScrollBar->repaint(m_rcLayout);
		}
	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::FScrollBarButton::modifyState(DWORD dwRemove, DWORD dwAdd, bool bRepaint)
	{
		if (fooModifyAttribute<DWORD>(m_dwState, dwRemove, dwAdd))
		{
			if (bRepaint) m_scrollbarPrivate->m_pScrollBar->repaint(m_rcLayout);
		}
	}

	FSCROLLBAR_INLINE DWORD FScrollBar::FScrollBarPrivate::FScrollBarButton::getState(void) const
	{
		return m_dwState;
	}

	FSCROLLBAR_INLINE DWORD FScrollBar::FScrollBarPrivate::FScrollBarButton::getRole(void) const
	{
		return m_dwRole;
	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::FScrollBarButton::setVisible(bool bVisible)
	{
		if (bVisible)
		{
			fooModifyAttribute<DWORD>(m_dwState, FOOSTATE_INVISIBLE, FOOSTATE_VISIBLE);
		}
		else
		{
			fooModifyAttribute<DWORD>(m_dwState, FOOSTATE_VISIBLE, FOOSTATE_INVISIBLE);
		}
	}

	FSCROLLBAR_INLINE bool FScrollBar::FScrollBarPrivate::FScrollBarButton::isVisible(void) const
	{
		return fooTestAttribute(m_dwState, FOOSTATE_VISIBLE);
	}

	FSCROLLBAR_INLINE void FScrollBar::FScrollBarPrivate::FScrollBarButton::setEnabled(bool bEnabled)
	{
		if (bEnabled)
		{
			fooModifyAttribute<DWORD>(m_dwState, FOOSTATE_DISABLED, FOOSTATE_ENABLED);
		}
		else
		{
			fooModifyAttribute<DWORD>(m_dwState, FOOSTATE_ENABLED, FOOSTATE_DISABLED);
		}
	}

	FSCROLLBAR_INLINE bool FScrollBar::FScrollBarPrivate::FScrollBarButton::isEnabled(void) const
	{
		return fooTestAttribute(m_dwState, FOOSTATE_ENABLED);
	}

	//////////////////////////////////////////////////////////////////////////
	// FScrollBar
	FSCROLLBAR_INLINE FScrollBar::FScrollBar(DWORD dwOrientation, FWidget* parent) : 
		FAbstractSlider(parent) 
		, m_scrollbarPrivate(new FScrollBarPrivate(dwOrientation, this))
	{
		modifyFlag(0, FOOFLAG_SKIPFOCUS | FOOFLAG_SKIPTAB);
	}

	FSCROLLBAR_INLINE FScrollBar::~FScrollBar(void)
	{
		FASSERT(NULL != m_scrollbarPrivate);
		delete m_scrollbarPrivate;
	}

	FSCROLLBAR_INLINE void FScrollBar::paintEvent(Event::FPaintEvent* e)
	{
		FASSERT(NULL != m_scrollbarPrivate);

		LPCTSTR pszType = NULL;
		static const TCHAR pszEmptyName[] = _T("");
		if (m_scrollbarPrivate->isHorzOrientation()) pszType = FSCROLLBAR_TYPE_NAME_HORZ;
		else if (m_scrollbarPrivate->isVertOrientation()) pszType = FSCROLLBAR_TYPE_NAME_VERT;
		
		__super::paintEvent(e);

		m_stylesheet.draw(this, e->graphics(), &e->updateRect(), getState(), getLayoutRect(), pszType, _T(""));

		if (m_scrollbarPrivate->m_btnTrack.isVisible())
		{
			m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
					m_scrollbarPrivate->m_btnTrack.getState(), 
					m_scrollbarPrivate->m_btnTrack.getLayoutRect(), 
					pszEmptyName, _T("track"));
			m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
					m_scrollbarPrivate->m_btnTrack.getState(), 
					m_scrollbarPrivate->m_btnTrack.getLayoutRect(), 
					pszType, _T("track"));
		}

		if (m_scrollbarPrivate->m_btnMin.isVisible())
		{
			m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
				m_scrollbarPrivate->m_btnMin.getState(), 
				m_scrollbarPrivate->m_btnMin.getLayoutRect(), 
				pszEmptyName, _T("min"));
			m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
				m_scrollbarPrivate->m_btnMin.getState(), 
				m_scrollbarPrivate->m_btnMin.getLayoutRect(), 
				pszType, _T("min"));
		}

		if (m_scrollbarPrivate->m_btnSub.isVisible())
		{
			m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
					m_scrollbarPrivate->m_btnSub.getState(), 
					m_scrollbarPrivate->m_btnSub.getLayoutRect(), 
					pszEmptyName, _T("sub"));
			m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
					m_scrollbarPrivate->m_btnSub.getState(), 
					m_scrollbarPrivate->m_btnSub.getLayoutRect(), 
					pszType, _T("sub"));
		}

		if (m_scrollbarPrivate->m_btnMax.isVisible())
		{
			m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
				m_scrollbarPrivate->m_btnMax.getState(), 
				m_scrollbarPrivate->m_btnMax.getLayoutRect(), 
				pszEmptyName, _T("max"));
			m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
				m_scrollbarPrivate->m_btnMax.getState(), 
				m_scrollbarPrivate->m_btnMax.getLayoutRect(), 
				pszType, _T("max"));
		}

		if (m_scrollbarPrivate->m_btnAdd.isVisible())
		{
			m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
					m_scrollbarPrivate->m_btnAdd.getState(), 
					m_scrollbarPrivate->m_btnAdd.getLayoutRect(), 
					pszEmptyName, _T("add"));
			m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
					m_scrollbarPrivate->m_btnAdd.getState(), 
					m_scrollbarPrivate->m_btnAdd.getLayoutRect(), 
					pszType, _T("add"));
		}

		if (m_scrollbarPrivate->m_btnThumb.isVisible())
		{
			m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
				m_scrollbarPrivate->m_btnThumb.getState(), 
				m_scrollbarPrivate->m_btnThumb.getLayoutRect(), 
				pszEmptyName, _T("thumb"));
			m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
				m_scrollbarPrivate->m_btnThumb.getState(), 
				m_scrollbarPrivate->m_btnThumb.getLayoutRect(), 
				pszType, _T("thumb"));
		}
	}

	FSCROLLBAR_INLINE void FScrollBar::mousePressEvent(Event::FMouseEvent* e)
	{
		FASSERT(NULL != m_scrollbarPrivate);
		::SetCapture(getSafeHwnd());
		m_scrollbarPrivate->mousePressEvent(e);
	}

	FSCROLLBAR_INLINE void FScrollBar::mouseReleaseEvent(Event::FMouseEvent* e)
	{
		FASSERT(NULL != m_scrollbarPrivate);
		::ReleaseCapture();
		m_scrollbarPrivate->mouseReleaseEvent(e);
	}

	FSCROLLBAR_INLINE void FScrollBar::mouseDoubleClickEvent(Event::FMouseEvent* e)
	{
		FASSERT(NULL != m_scrollbarPrivate);
		m_scrollbarPrivate->mouseDoubleClickEvent(e);
	}

	FSCROLLBAR_INLINE void FScrollBar::mouseMoveEvent(Event::FMouseEvent* e)
	{
		FASSERT(NULL != m_scrollbarPrivate);
		m_scrollbarPrivate->mouseMoveEvent(e);
	}

	FSCROLLBAR_INLINE void FScrollBar::mouseEnterEvent(Event::FEvent* e)
	{
		FASSERT(NULL != m_scrollbarPrivate);
		m_scrollbarPrivate->mouseEnterEvent(e);
	}

	FSCROLLBAR_INLINE void FScrollBar::mouseLeaveEvent(Event::FEvent* e)
	{
		FASSERT(NULL != m_scrollbarPrivate);
		m_scrollbarPrivate->mouseLeaveEvent(e);
	}

	FSCROLLBAR_INLINE void FScrollBar::layoutChangedEvent(Event::FLayoutChangedEvent* e)
	{
		FASSERT(NULL != m_scrollbarPrivate);
		m_scrollbarPrivate->calcScrollButtonLayout(e->newLayoutRect);
	}

	FSCROLLBAR_INLINE void FScrollBar::sliderPropertiesChanged(void)
	{
		FASSERT(NULL != m_scrollbarPrivate);
		m_scrollbarPrivate->trackPosition(sliderPosition());
		m_scrollbarPrivate->calcScrollButtonLayout();
	}

	FSCROLLBAR_INLINE bool FScrollBar::setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		FASSERT(NULL != m_scrollbarPrivate);
		if (_tcscmp(pszName, _T("showadd")) == 0)
		{
			m_scrollbarPrivate->m_btnAdd.setVisible(Markup::fooMarkupParseBoolean(pszValue));
		}
		if (_tcscmp(pszName, _T("showsub")) == 0)
		{
			m_scrollbarPrivate->m_btnSub.setVisible(Markup::fooMarkupParseBoolean(pszValue));
		}
		if (_tcscmp(pszName, _T("showmin")) == 0)
		{
			m_scrollbarPrivate->m_btnMin.setVisible(Markup::fooMarkupParseBoolean(pszValue));
		}
		if (_tcscmp(pszName, _T("showmax")) == 0)
		{
			m_scrollbarPrivate->m_btnMax.setVisible(Markup::fooMarkupParseBoolean(pszValue));
		}

		return __super::setAttribute(pMarkup, pszName, pszValue);
	}

}} // !namespace