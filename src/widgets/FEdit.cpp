/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FEdit.cpp
* 描述：		FEdit, 编辑框
* 创建日期：	2013-07-24
*
*/

#include "StdAfx.h"
#include "widgets/FEdit.h"

#include "widgets/thunk/atlstdthunk.h"

#define FEDIT_INLINE FOOINLINE

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FEditPrivate
	class FEdit::FEditPrivate
	{
	public:
		explicit FEditPrivate(FEdit* pEdit);
		~FEditPrivate(void);

	public:
		WNDPROC getThunkProc(void);
		LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT defWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK thunkProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		HWND m_hWndParent;
		HBRUSH m_hBkBrush;
		ATL::CStdCallThunk m_thunk;	// parent
		WNDPROC m_parentWndProc;

	public:
		TCHAR m_passwordChar;
		UINT m_limitMaxChars;
		bool m_bNumberOnly;
		bool m_bReadOnly;

	private:
		FEdit* m_pEdit;
	};

	// 
	FEdit::FEditPrivate::FEditPrivate(FEdit* pEdit) : 
		m_hWndParent(NULL)
		, m_hBkBrush(NULL) 
		, m_parentWndProc(NULL) 
		, m_passwordChar(_T('\0')) 
		, m_limitMaxChars(0) 
		, m_bNumberOnly(false) 
		, m_bReadOnly(false) 
		, m_pEdit(pEdit) 
	{
		FASSERT(NULL != m_pEdit);
	}

	FEdit::FEditPrivate::~FEditPrivate(void)
	{
		if (NULL != m_hBkBrush)
		{
			fooSafeDeleteObject(m_hBkBrush);
		}
	}

	WNDPROC FEdit::FEditPrivate::getThunkProc(void)
	{
		return (WNDPROC)m_thunk.GetCodeAddress();
	}

	LRESULT FEdit::FEditPrivate::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		FASSERT(NULL != m_parentWndProc);
		FASSERT(NULL != m_hWndParent);
		FASSERT(NULL != m_pEdit);

		bool bHandled = false;
		LRESULT lResult = m_pEdit->handleParentMessage(m_hWndParent, uMsg, wParam, lParam, bHandled);

		if (bHandled)
			return lResult;
		else
			return ::CallWindowProc(m_parentWndProc, m_hWndParent, uMsg, wParam, lParam);
	}

	LRESULT FEdit::FEditPrivate::defWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		FASSERT(NULL != m_parentWndProc);
		FASSERT(NULL != m_hWndParent);
		FASSERT(NULL != m_pEdit);
		return ::CallWindowProc(m_parentWndProc, m_hWndParent, uMsg, wParam, lParam);
	}

	/*static*/ LRESULT CALLBACK FEdit::FEditPrivate::thunkProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		FEditPrivate* pThis = reinterpret_cast<FEditPrivate*>(hWnd);
		if (NULL == pThis)
		{
			return 0;
		}

		return pThis->handleMessage(uMsg, wParam, lParam);
	}

	//////////////////////////////////////////////////////////////////////////
	// FEdit
	FEDIT_INLINE FEdit::FEdit(FWidget* parent/* = NULL*/) : 
		FWidget(parent) 
		, m_editPrivate(new FEditPrivate(this))
	{
		if (m_strText.isEmpty())
			setStateEmpty(true);
		else
			setStateNonEmpty(true);
	}

	FEDIT_INLINE FEdit::~FEdit(void)
	{
		FASSERT(NULL != m_editPrivate);
		delete m_editPrivate;
	}

	FEDIT_INLINE bool FEdit::createWindow(HWND hParentWnd)
	{
		FASSERT(!::IsWindow(getHwnd()));
		FASSERT(::IsWindow(hParentWnd));

		DWORD dwStyle = WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | ES_AUTOHSCROLL | ES_AUTOVSCROLL;
		if (isVisible()) dwStyle |= WS_VISIBLE;

		FWidget *parent = this;
		while (NULL != parent)
		{
			if (!parent->isVisible())
			{
				dwStyle &= ~WS_VISIBLE;
				break;
			}

			parent = parent->getParent();
		}

		if (NULL != m_editPrivate)
		{
			if (m_editPrivate->m_bNumberOnly) dwStyle |= ES_NUMBER;
			if (m_editPrivate->m_bReadOnly) dwStyle |= ES_READONLY;
		}

		Core::FRect rcLayout = getLayoutRect();
		bool bRet = __super::createWindow(0, WC_EDIT, getText(), dwStyle, 
					rcLayout.left, rcLayout.top, rcLayout.width(), rcLayout.height(), 
					hParentWnd);
		if (bRet)
		{
			::SendMessage(getHwnd(), WM_SETFONT, (WPARAM)m_resFont.font().handle(), (LPARAM)TRUE);

			if (NULL != m_editPrivate)
			{
				::SendMessage(getHwnd(), EM_SETPASSWORDCHAR, (WPARAM)m_editPrivate->m_passwordChar, 0L);
				::SendMessage(getHwnd(), EM_SETLIMITTEXT, (WPARAM)m_editPrivate->m_limitMaxChars, 0L);
			}
		}

		return bRet;
	}

	FEDIT_INLINE void FEdit::setPasswordChar(TCHAR tchPassword)
	{
		FASSERT(NULL != m_editPrivate);
		if (NULL != m_editPrivate)
		{
			m_editPrivate->m_passwordChar = tchPassword;
		}

		if (isWindow())
		{
			::SendMessage(getHwnd(), EM_SETPASSWORDCHAR, (WPARAM)tchPassword, 0L);
		}
	}

	FEDIT_INLINE void FEdit::clearPasswordChar(void)
	{
		setPasswordChar(_T('\0'));
	}

	FEDIT_INLINE TCHAR FEdit::getPasswordChar(void) const
	{
		if (isWindow())
		{
			return (TCHAR)::SendMessage(getHwnd(), EM_GETPASSWORDCHAR, 0L, 0L);
		}
		else if (NULL != m_editPrivate)
		{
			return m_editPrivate->m_passwordChar;
		}

		return _T('\0');
	}

	FEDIT_INLINE void FEdit::setLimitText(UINT nMax)
	{
		FASSERT(NULL != m_editPrivate);
		if (NULL != m_editPrivate)
		{
			m_editPrivate->m_limitMaxChars = nMax;
		}

		if (isWindow())
		{
			::SendMessage(getHwnd(), EM_SETLIMITTEXT, (WPARAM)nMax, 0L);
		}
	}

	FEDIT_INLINE void FEdit::clearLimitText(UINT nMax)
	{
		setLimitText(0);
	}

	FEDIT_INLINE UINT FEdit::getLimitText(void) const
	{
		if (isWindow())
		{
			return (UINT)::SendMessage(getHwnd(), EM_GETLIMITTEXT, 0L, 0L);
		}
		else if (NULL != m_editPrivate)
		{
			return m_editPrivate->m_limitMaxChars;
		}

		return 0;
	}

	FEDIT_INLINE void FEdit::setNumberOnly(bool bNumberOnly/* = true*/)
	{
		FASSERT(NULL != m_editPrivate);
		if (NULL != m_editPrivate)
		{
			m_editPrivate->m_bNumberOnly = bNumberOnly;
		}

		if (isWindow())
		{
			DWORD dwStyle = ::GetWindowLong(getHwnd(), GWL_STYLE);
			if (bNumberOnly)
				dwStyle |= ES_NUMBER;
			else
				dwStyle &= ~ES_NUMBER;

			::SetWindowLong(getHwnd(), GWL_STYLE, dwStyle);
		}
	}

	FEDIT_INLINE bool FEdit::isNumberOnly(void) const
	{
		if (isWindow())
		{
			fooTestAttribute(::GetWindowLong(getHwnd(), GWL_STYLE), ES_NUMBER);
		}
		else if (NULL != m_editPrivate)
		{
			return m_editPrivate->m_bNumberOnly;
		}

		return false;
	}

	FEDIT_INLINE void FEdit::setReadOnly(bool bReadOnly/* = true*/)
	{
		FASSERT(NULL != m_editPrivate);
		if (NULL != m_editPrivate)
		{
			m_editPrivate->m_bReadOnly = bReadOnly;
		}

		if (isWindow())
		{
			::SendMessage(getHwnd(), EM_SETREADONLY, bReadOnly, 0L);
		}
	}

	FEDIT_INLINE bool FEdit::isReadOnly(void) const
	{
		if (isWindow())
		{
			fooTestAttribute(::GetWindowLong(getHwnd(), GWL_STYLE), ES_READONLY);
		}
		else if (NULL != m_editPrivate)
		{
			return m_editPrivate->m_bReadOnly;
		}

		return false;
	}

	FEDIT_INLINE void FEdit::setSel(int nStartChar, int nEndChar)
	{
		FASSERT(isWindow());
		if (isWindow())
		{
			::SendMessage(getHwnd(), EM_SETSEL, nStartChar, nEndChar);
		}
	}

	FEDIT_INLINE void FEdit::selectAll(void)
	{
		setSel(0, -1);
	}

	FEDIT_INLINE int FEdit::getSelStart(void) const
	{
		return 0;
	}

	FEDIT_INLINE int FEdit::getSelLength(void) const
	{
		return 0;
	}

	FEDIT_INLINE bool FEdit::setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		if (_tcscmp(pszName, _T("font")) == 0)
		{
			m_resFont.parseAttribute(pMarkup, pszValue);
		}
		else if (_tcscmp(pszName, _T("passwordchar")) == 0)
		{
			setPasswordChar(pszValue[0]);
		}
		else if (_tcscmp(pszName, _T("maxchars")) == 0 
			|| _tcscmp(pszName, _T("limittext")) == 0)
		{
			setLimitText(_tcstoul(pszValue, NULL, 10));
		}
		else if (_tcscmp(pszName, _T("numberonly")) == 0)
		{
			setNumberOnly(Markup::fooMarkupParseBoolean(pszValue));
		}
		else if (_tcscmp(pszName, _T("readonly")) == 0)
		{
			setReadOnly(Markup::fooMarkupParseBoolean(pszValue));
		}

		return __super::setAttribute(pMarkup, pszName, pszValue);
	}

	FEDIT_INLINE bool FEdit::endMarkup(Markup::FMarkup* pMarkup)
	{
		HWND hParentWnd = getSafeHwnd();
		if (!::IsWindow(getHwnd()) && ::IsWindow(hParentWnd))
		{
			createWindow(hParentWnd);
		}

		return __super::endMarkup(pMarkup);
	}

	FEDIT_INLINE LRESULT FEdit::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
	{
		FASSERT(NULL != m_editPrivate);
		if (NULL == m_editPrivate->m_hWndParent)
		{
			FASSERT(NULL != m_editPrivate);
			m_editPrivate->m_hWndParent = ::GetParent(getHwnd());
			m_editPrivate->m_thunk.Init((DWORD_PTR)FEditPrivate::thunkProc, m_editPrivate);
			WNDPROC	pOldProc = (WNDPROC)::SetWindowLongPtr(m_editPrivate->m_hWndParent, 
				GWLP_WNDPROC, (LONG_PTR)m_editPrivate->getThunkProc());
			m_editPrivate->m_parentWndProc = pOldProc;
		}

		switch (uMsg)
		{
		case WM_CHAR:
			if (1 == wParam)
			{
				selectAll();
				bHandled = true;
				return 0;
			}
			break;

		case WM_ERASEBKGND:
			if (NULL != m_editPrivate->m_hBkBrush)
			{
				HDC hDCEdit = (HDC)wParam;

				Core::FRect rcClient;
				::GetClientRect(getHwnd(), &rcClient);
				::FillRect(hDCEdit, &rcClient, m_editPrivate->m_hBkBrush);
			}
			bHandled = true;
			return TRUE;
			break;

		case WM_PAINT:
			bHandled = true;
			return defWindowProc(uMsg, wParam, lParam);
			break;

		case WM_SETFOCUS:
			setFocus();
			setStateFocused(true);
			break;

		case WM_KILLFOCUS:
			setStateUnFocused(true);
			break;

		case WM_MOUSEMOVE:
			setStateHover(true);
			break;

		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONDOWN:
			setStatePushed(true);
			break;

		case WM_HSCROLL:
		case WM_VSCROLL:
			{
				::LockWindowUpdate(m_editPrivate->m_hWndParent);
				LRESULT lResult = defWindowProc(uMsg, wParam, lParam);
				::InvalidateRect(getHwnd(), NULL, TRUE);
				::UpdateWindow(getHwnd());
				::LockWindowUpdate(NULL);
				bHandled = true;
				return lResult;
			}
			break;

		default:
			break;
		}

		return 0;
	}

	LRESULT FEdit::handleParentMessage(HWND hParentWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
	{
		switch (uMsg)
		{
		case WM_CTLCOLOREDIT:
			{
				HWND hWndEdit = (HWND)lParam;
				if (getHwnd() == hWndEdit)
				{
					HDC hDCEdit = (HDC)wParam;
					::SetBkMode(hDCEdit, TRANSPARENT);
					::SetTextColor(hDCEdit, RGB(0, 0, 0));

					bHandled = true;
					return (LRESULT)(m_editPrivate->m_hBkBrush);
				}
			}
			break;

		case WM_COMMAND:
			{
				HWND hWndEdit = (HWND)lParam;
				if (getHwnd() == hWndEdit)
				{
					int nCode = HIWORD(wParam);
					if (EN_CHANGE == nCode)
					{
						int nTextLength = ::GetWindowTextLength(getHwnd());
						if (nTextLength <= 0)
						{
							m_strText = _T("");
							setStateEmpty(true);
						}
						else
						{
							::GetWindowText(getHwnd(), m_strText.getBuffer(nTextLength + 1), nTextLength + 1);
							m_strText.releaseBuffer();
							setStateNonEmpty(true);
						}

						makeWidgetMessage(FWM_CHANGED, (WPARAM)CT_TEXT, 0L);
					}

					::LockWindowUpdate(m_editPrivate->m_hWndParent);
					::InvalidateRect(hWndEdit, NULL, TRUE);
					::UpdateWindow(hWndEdit);
					LRESULT lResult = m_editPrivate->defWindowProc(uMsg, wParam, lParam);
					::LockWindowUpdate(NULL);
					bHandled = true;
					return lResult;
				}
			}
			break;
		}
		return 0;
	}

	FEDIT_INLINE void FEdit::paintEvent(Event::FPaintEvent* e)
	{
		FASSERT(NULL != m_editPrivate);

		__super::paintEvent(e);

		if (NULL != m_editPrivate->m_hBkBrush)
		{
			fooSafeDeleteObject(m_editPrivate->m_hBkBrush);
		}

		Core::FRect rcEdit;
		::GetWindowRect(getHwnd(), &rcEdit);
		::ScreenToClient(m_editPrivate->m_hWndParent, (LPPOINT)&rcEdit);
		::ScreenToClient(m_editPrivate->m_hWndParent, ((LPPOINT)&rcEdit) + 1);

		HDC hDC = ::CreateCompatibleDC(e->graphics()->getSafeHdc());
		HBITMAP hBitmap = ::CreateCompatibleBitmap(e->graphics()->getSafeHdc(), 
							rcEdit.width(), rcEdit.height());
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hDC, hBitmap);
		::BitBlt(hDC, 0, 0, 
			rcEdit.width(), rcEdit.height(), 
			e->graphics()->getSafeHdc(), 
			rcEdit.left, rcEdit.top, SRCCOPY);

		m_editPrivate->m_hBkBrush = ::CreatePatternBrush(hBitmap);
		FASSERT(NULL != m_editPrivate->m_hBkBrush);

		::SelectObject(hDC, hOldBitmap);
		::DeleteDC(hDC);
		::DeleteObject(hBitmap);

		::InvalidateRect(getHwnd(), NULL, TRUE);
		::UpdateWindow(getHwnd());
	}

	FEDIT_INLINE void FEdit::showEvent(Event::FEvent* e)
	{
		FWidget* parent = this;
		while (NULL != parent)
		{
			if (!parent->isVisible())
			{
				if (isWindow()) 
				{
					::ShowWindow(getHwnd(), SW_HIDE);
				}
				return;
			}

			parent = parent->getParent();
		}

		if (isWindow()) 
		{
			::ShowWindow(getHwnd(), SW_SHOWNOACTIVATE);
		}
	}

	FEDIT_INLINE void FEdit::hideEvent(Event::FEvent* e)
	{
		if (isWindow())
		{
			::ShowWindow(getHwnd(), SW_HIDE);
		}
	}

	FEDIT_INLINE void FEdit::changedEvent(Event::FChangedEvent* e)
	{
		if (CT_TEXT == e->changedType)
		{
			if (m_strText.isEmpty())
				setStateEmpty(true);
			else
				setStateNonEmpty(true);
		}
	}

	FEDIT_INLINE void FEdit::layoutChangedEvent(Event::FLayoutChangedEvent* e)
	{
		if (isWindow())
		{
			Core::FRect rcLayout = e->newLayoutRect;
			rcLayout.deflateRect(getPadding());

			::SetWindowPos(getHwnd(), NULL, 
						rcLayout.left, rcLayout.top, 
						rcLayout.width(), rcLayout.height(), 
						SWP_NOZORDER | SWP_NOACTIVATE);
		}

		__super::layoutChangedEvent(e);
	}

	FEDIT_INLINE void FEdit::parentWindowCreatedEvent(Event::FParentWindowEvent* e)
	{
		if (!::IsWindow(getHwnd()) && ::IsWindow(e->hWnd))
		{
			createWindow(e->hWnd);
		}

		__super::parentWindowCreatedEvent(e);
	}

}} // !namespace