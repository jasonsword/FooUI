/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FProgressBar.cpp
* 描述：		FProgressBar
* 创建日期：	2013-07-25
*
*/

#include "StdAfx.h"
#include "widgets/FProgressBar.h"

#define FPROGRESSBARORIENTATION_NAME_HORZ _T("horz")
#define FPROGRESSBARORIENTATION_NAME_VERT _T("vert")

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FProgressBar
	FProgressBar::FProgressBar(DWORD dwOrientation/* = HorzProgressBar*/, FWidget* parent/* = NULL*/) : 
		FAbstractSlider(parent) 
		, m_dwOrientation(dwOrientation) 
	{
		modifyFlag(0, FOOFLAG_SKIPMOUSE | FOOFLAG_SKIPFOCUS | FOOFLAG_SKIPTAB);
	}

	FProgressBar::~FProgressBar(void)
	{
	}

	void FProgressBar::setProgressBarOrientation(DWORD dwOrientation)
	{
		if (m_dwOrientation == dwOrientation)
		{
			return;
		}

		m_dwOrientation = dwOrientation;
		repaint();
	}

	DWORD FProgressBar::getProgressBarOrientation(void) const
	{
		return m_dwOrientation;
	}

	void FProgressBar::sliderPropertiesChanged(void)
	{
		repaint();
	}

	void FProgressBar::paintEvent(Event::FPaintEvent* e)
	{
		__super::paintEvent(e);

		Core::FRect rcClient;
		Core::FRect rcThumb;
		getValidLayoutRect(&rcClient);

		switch (getProgressBarOrientation())
		{
		case HorzProgressBar:
			{
				rcThumb.setXYWH(rcClient.left, 
								rcClient.top, 
								(int)(rcClient.width() * getSliderPercent()), 
								rcClient.height());

				m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
					getState(), getLayoutRect(), _T(""), _T(""));
				m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
					getState(), getLayoutRect(), FPROGRESSBARORIENTATION_NAME_HORZ, _T(""));

				m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
							getState(), rcThumb, _T(""), _T("fill"));
				m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
							getState(), rcThumb, FPROGRESSBARORIENTATION_NAME_HORZ, _T("fill"));
				
				m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
							getState(), getLayoutRect(), _T(""), _T("text"));
				m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
							getState(), getLayoutRect(), FPROGRESSBARORIENTATION_NAME_HORZ, _T("text"));
			}
			break;

		case VertProgressBar:
			{
				rcThumb.setRect(rcClient.left, 
							rcClient.bottom - (int)(rcClient.height() * getSliderPercent()), 
							rcClient.right, 
							rcClient.bottom);

				m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
					getState(), getLayoutRect(), _T(""), _T(""));
				m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
					getState(), getLayoutRect(), FPROGRESSBARORIENTATION_NAME_VERT, _T(""));

				m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
						getState(), rcThumb, _T(""), _T("fill"));
				m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
						getState(), rcThumb, FPROGRESSBARORIENTATION_NAME_VERT, _T("fill"));

				m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
						getState(), getLayoutRect(), _T(""), _T("text"));
				m_stylesheet.draw(this, e->graphics(), &e->updateRect(), 
						getState(), getLayoutRect(), FPROGRESSBARORIENTATION_NAME_VERT, _T("text"));
			}
			break;

		default:
			break;
		}
	}

	bool FProgressBar::setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		if (_tcscmp(pszName, _T("orientation")) == 0)
		{
			if (_tcscmp(pszValue, FPROGRESSBARORIENTATION_NAME_HORZ) == 0)
				setProgressBarOrientation(HorzProgressBar);
			else if (_tcscmp(pszValue, FPROGRESSBARORIENTATION_NAME_VERT) == 0)
				setProgressBarOrientation(VertProgressBar);
		}
		return __super::setAttribute(pMarkup, pszName, pszValue);
	}

}} // !namespace