/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FComboBox.cpp
* 描述：		FComboBox, 组合框
* 创建日期：	2013-07-24
*
*/

#include "StdAfx.h"
#include "widgets/FComboBox.h"
#include "widgets/FWidgetPrivate_p.h"

#include "widgets/thunk/atlstdthunk.h"

#define FCOMBOBOX_INLINE FOOINLINE

namespace FooUI { namespace Widgets {

	//////////////////////////////////////////////////////////////////////////
	// FComboBoxPrivate
	class FComboBox::FComboBoxPrivate
	{
	public:
		explicit FComboBoxPrivate(FComboBox* pComboBox);
		~FComboBoxPrivate(void);

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

	private:
		FComboBox* m_pComboBox;
	};

	// 
	FCOMBOBOX_INLINE FComboBox::FComboBoxPrivate::FComboBoxPrivate(FComboBox* pEdit) : 
		m_hWndParent(NULL)
		, m_hBkBrush(NULL) 
		, m_parentWndProc(NULL) 
		, m_pComboBox(pEdit) 
	{
		FASSERT(NULL != m_pComboBox);
	}

	FCOMBOBOX_INLINE FComboBox::FComboBoxPrivate::~FComboBoxPrivate(void)
	{
		if (NULL != m_hBkBrush)
		{
			fooSafeDeleteObject(m_hBkBrush);
		}
	}

	FCOMBOBOX_INLINE WNDPROC FComboBox::FComboBoxPrivate::getThunkProc(void)
	{
		return (WNDPROC)m_thunk.GetCodeAddress();
	}

	FCOMBOBOX_INLINE LRESULT FComboBox::FComboBoxPrivate::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		FASSERT(NULL != m_parentWndProc);
		FASSERT(NULL != m_hWndParent);
		FASSERT(NULL != m_pComboBox);

		bool bHandled = false;
		LRESULT lResult = m_pComboBox->handleParentMessage(m_hWndParent, uMsg, wParam, lParam, bHandled);

		if (bHandled)
			return lResult;
		else
			return ::CallWindowProc(m_parentWndProc, m_hWndParent, uMsg, wParam, lParam);
	}

	FCOMBOBOX_INLINE LRESULT FComboBox::FComboBoxPrivate::defWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		FASSERT(NULL != m_parentWndProc);
		FASSERT(NULL != m_hWndParent);
		FASSERT(NULL != m_pComboBox);
		return ::CallWindowProc(m_parentWndProc, m_hWndParent, uMsg, wParam, lParam);
	}

	FCOMBOBOX_INLINE /*static*/ LRESULT CALLBACK FComboBox::FComboBoxPrivate::thunkProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		FComboBoxPrivate* pThis = reinterpret_cast<FComboBoxPrivate*>(hWnd);
		if (NULL == pThis)
		{
			return 0;
		}

		return pThis->handleMessage(uMsg, wParam, lParam);
	}

	//////////////////////////////////////////////////////////////////////////
	// FComboBox
	FCOMBOBOX_INLINE FComboBox::FComboBox(DWORD dwComboBoxStyle/* = DroplistComboBox*/, FWidget* parent/* = NULL*/) : 
		FWidget(parent)
		, m_comboboxPrivate(NULL) 
		, m_passwordChar(_T('\0')) 
		, m_comboboxList(NULL) 
		, m_dwComboBoxStyle(dwComboBoxStyle) 
	{
		m_comboboxList = new FComboBoxList;
		m_comboboxList->setOwner(this);
		m_comboboxList->autoDelete(true);
	}

	FCOMBOBOX_INLINE FComboBox::~FComboBox(void)
	{
		if (NULL != m_comboboxPrivate)
		{
			delete m_comboboxPrivate;
		}

		if (NULL != m_comboboxList 
			&& m_comboboxList->isAutoDelete())
		{
			fooSafeDelete(m_comboboxList);
		}
	}

	FCOMBOBOX_INLINE void FComboBox::setComboBoxStyle(DWORD dwComboBoxStyle)
	{
		if (dwComboBoxStyle == m_dwComboBoxStyle)
		{
			return;
		}

		m_dwComboBoxStyle = dwComboBoxStyle;

		HWND hHostWindowHwnd = getHostWindowHwnd();
		if (DropdownComboBox == m_dwComboBoxStyle)
		{
			if (!isWindow() 
				&& ::IsWindow(hHostWindowHwnd))
			{
				createWindow(hHostWindowHwnd);
			}
		}
		else
		{
			if (isWindow() 
				&& hHostWindowHwnd != getHwnd())
			{
				destroyWindow();
			}
		}
	}

	FCOMBOBOX_INLINE DWORD FComboBox::getComboBoxStyle(void) const
	{
		return m_dwComboBoxStyle;
	}

	FCOMBOBOX_INLINE bool FComboBox::selectElement(int nIndex)
	{
		FASSERT(NULL != m_comboboxList);
		if (NULL != m_comboboxList)
		{
			return m_comboboxList->selectElement(nIndex);
		}

		return false;
	}

	FCOMBOBOX_INLINE bool FComboBox::selectElement(FAbstractListElement* element)
	{
		FASSERT(NULL != m_comboboxList);
		if (NULL != m_comboboxList)
		{
			return m_comboboxList->selectElement(element);
		}

		return false;
	}

	FCOMBOBOX_INLINE FAbstractListElement* FComboBox::getSelectedElement(void) const
	{
		FASSERT(NULL != m_comboboxList);
		if (NULL != m_comboboxList)
		{
			return m_comboboxList->getSelectedElement();
		}

		return NULL;
	}

	FCOMBOBOX_INLINE void FComboBox::mousePressEvent(Event::FMouseEvent* e)
	{
		FASSERT(NULL != m_comboboxList);
		if (LeftButton == e->button)
		{
			m_comboboxList->dropdown();
		}
	}

	FCOMBOBOX_INLINE bool FComboBox::createWindow(HWND hParentWnd)
	{
		FASSERT(!::IsWindow(getHwnd()));
		FASSERT(::IsWindow(hParentWnd));

		DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | ES_AUTOHSCROLL | ES_AUTOVSCROLL;
		
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

		Core::FRect rcLayout = getLayoutRect();
		bool bRet = __super::createWindow(0, WC_EDIT, getText(), dwStyle, 
			rcLayout.left, rcLayout.top, rcLayout.width(), rcLayout.height(), 
			hParentWnd);

		if (bRet)
		{
			::SendMessage(getHwnd(), WM_SETFONT, (WPARAM)m_resFont.font().handle(), (LPARAM)TRUE);
			::SendMessage(getHwnd(), EM_SETPASSWORDCHAR, (WPARAM)m_passwordChar, 0L);

			if (NULL == m_comboboxPrivate) m_comboboxPrivate = new FComboBoxPrivate(this);
		}

		return bRet;
	}

	FCOMBOBOX_INLINE void FComboBox::setPasswordChar(TCHAR tchPassword)
	{
		m_passwordChar = tchPassword;
		if (isWindow())
		{
			::SendMessage(getHwnd(), EM_SETPASSWORDCHAR, (WPARAM)m_passwordChar, 0L);
		}
	}

	FCOMBOBOX_INLINE void FComboBox::emptyPasswordChar(void)
	{
		setPasswordChar(_T('\0'));
	}

	FCOMBOBOX_INLINE bool FComboBox::setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		if (_tcscmp(pszName, _T("comboboxstyle")) == 0)
		{
			if (_tcscmp(pszValue, _T("droplist")) == 0)
			{
				setComboBoxStyle(DroplistComboBox);
			}
			else if (_tcscmp(pszValue, _T("dropdown")) == 0)
			{
				setComboBoxStyle(DropdownComboBox);
			}
			else if (_tcscmp(pszValue, _T("simple")) == 0)
			{
				setComboBoxStyle(SimpleComboBox);
			}
		}
		else if (_tcscmp(pszName, _T("font")) == 0)
		{
			m_resFont.parseAttribute(pMarkup, pszValue);
		}
		else if (_tcscmp(pszName, _T("passwordchar")) == 0)
		{
			setPasswordChar(pszValue[0]);
		}

		return __super::setAttribute(pMarkup, pszName, pszValue);
	}

	FCOMBOBOX_INLINE bool FComboBox::endMarkup(Markup::FMarkup* pMarkup)
	{
		HWND hParentWnd = getSafeHwnd();
		if (DropdownComboBox == m_dwComboBoxStyle 
			&& !::IsWindow(getHwnd()) 
			&& ::IsWindow(hParentWnd))
		{
			createWindow(hParentWnd);
		}

		return __super::endMarkup(pMarkup);
	}

	FCOMBOBOX_INLINE LRESULT FComboBox::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
	{
		if (NULL == m_comboboxPrivate)
		{
			return 0;
		}

		if (NULL == m_comboboxPrivate->m_hWndParent)
		{
			FASSERT(NULL != m_comboboxPrivate);
			m_comboboxPrivate->m_hWndParent = ::GetParent(getHwnd());
			m_comboboxPrivate->m_thunk.Init((DWORD_PTR)FComboBoxPrivate::thunkProc, m_comboboxPrivate);
			WNDPROC	pOldProc = (WNDPROC)::SetWindowLongPtr(m_comboboxPrivate->m_hWndParent, 
				GWLP_WNDPROC, (LONG_PTR)m_comboboxPrivate->getThunkProc());
			m_comboboxPrivate->m_parentWndProc = pOldProc;
		}

		switch (uMsg)
		{
		case WM_CHAR:
			break;

		case WM_ERASEBKGND:
			if (NULL != m_comboboxPrivate->m_hBkBrush)
			{
				HDC hDCEdit = (HDC)wParam;

				Core::FRect rcClient;
				::GetClientRect(getHwnd(), &rcClient);
				::FillRect(hDCEdit, &rcClient, m_comboboxPrivate->m_hBkBrush);
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

		//case WM_LBUTTONDBLCLK:
		//case WM_LBUTTONDOWN:
		//	setStatePushed(true);
		//	break;

		case WM_HSCROLL:
		case WM_VSCROLL:
			{
				::LockWindowUpdate(m_comboboxPrivate->m_hWndParent);
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

	LRESULT FComboBox::handleParentMessage(HWND hParentWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
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
					return (LRESULT)(m_comboboxPrivate->m_hBkBrush);
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

					::LockWindowUpdate(m_comboboxPrivate->m_hWndParent);
					::InvalidateRect(hWndEdit, NULL, TRUE);
					::UpdateWindow(hWndEdit);
					LRESULT lResult = m_comboboxPrivate->defWindowProc(uMsg, wParam, lParam);
					::LockWindowUpdate(NULL);
					bHandled = true;
					return lResult;
				}
			}
			break;
		}
		return 0;
	}

	FCOMBOBOX_INLINE void FComboBox::paintEvent(Event::FPaintEvent* e)
	{
		__super::paintEvent(e);

		if (NULL != m_comboboxPrivate)
		{
			if (NULL != m_comboboxPrivate->m_hBkBrush)
			{
				fooSafeDeleteObject(m_comboboxPrivate->m_hBkBrush);
			}

			Core::FRect rcEdit;
			::GetWindowRect(getHwnd(), &rcEdit);
			::ScreenToClient(m_comboboxPrivate->m_hWndParent, (LPPOINT)&rcEdit);
			::ScreenToClient(m_comboboxPrivate->m_hWndParent, ((LPPOINT)&rcEdit) + 1);

			HDC hDC = ::CreateCompatibleDC(e->graphics()->getSafeHdc());
			HBITMAP hBitmap = ::CreateCompatibleBitmap(e->graphics()->getSafeHdc(), 
				rcEdit.width(), rcEdit.height());
			HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hDC, hBitmap);
			::BitBlt(hDC, 0, 0, 
				rcEdit.width(), rcEdit.height(), 
				e->graphics()->getSafeHdc(), 
				rcEdit.left, rcEdit.top, SRCCOPY);

			m_comboboxPrivate->m_hBkBrush = ::CreatePatternBrush(hBitmap);
			FASSERT(NULL != m_comboboxPrivate->m_hBkBrush);

			::SelectObject(hDC, hOldBitmap);
			::DeleteDC(hDC);
			::DeleteObject(hBitmap);

			::InvalidateRect(getHwnd(), NULL, TRUE);
			::UpdateWindow(getHwnd());
		} // ! if (NULL != m_comboboxPrivate)
	}

	FCOMBOBOX_INLINE void FComboBox::showEvent(Event::FEvent* e)
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

	FCOMBOBOX_INLINE void FComboBox::hideEvent(Event::FEvent* e)
	{
		if (isWindow())
		{
			::ShowWindow(getHwnd(), SW_HIDE);
		}
	}

	FCOMBOBOX_INLINE bool FComboBox::markupAddWidget(Markup::FMarkup* pMarkup, FWidget* pWidget)
	{
		FComboBoxList *pComboBoxList = static_cast<FComboBoxList*>(pWidget->getInterface(FOOWC_COMBOBOXLIST));
		if (NULL != pComboBoxList)
		{
			if (NULL != m_comboboxList 
				&& m_comboboxList->isAutoDelete())
			{
				fooSafeDelete(m_comboboxList);
			}

			m_comboboxList = pComboBoxList;
			m_comboboxList->setOwner(this);

			return true;
		}

		return __super::markupAddWidget(pMarkup, pWidget);
	}

	FCOMBOBOX_INLINE void FComboBox::changedEvent(Event::FChangedEvent* e)
	{
		if (CT_TEXT == e->changedType)
		{
			if (m_strText.isEmpty())
				setStateEmpty(true);
			else
				setStateNonEmpty(true);
		}
	}

	FCOMBOBOX_INLINE void FComboBox::layoutChangedEvent(Event::FLayoutChangedEvent* e)
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

	FCOMBOBOX_INLINE void FComboBox::parentWindowCreatedEvent(Event::FParentWindowEvent* e)
	{
		if (DropdownComboBox == m_dwComboBoxStyle 
			&& !::IsWindow(getHwnd()) 
			&& ::IsWindow(e->hWnd) )
		{
			createWindow(e->hWnd);
		}

		__super::parentWindowCreatedEvent(e);
	}

	//////////////////////////////////////////////////////////////////////////
	// FComboBoxList
	FCOMBOBOX_INLINE FComboBoxList::FComboBoxList(FComboBox* parent/* = NULL*/) : 
		FAbstractList(NULL)
	{
		modifyFlag(FOOFLAG_TRANSPARENT, 0);
		hide();
	}

	FCOMBOBOX_INLINE FComboBoxList::~FComboBoxList(void)
	{
	}

	FCOMBOBOX_INLINE bool FComboBoxList::selectElement(int nIndex)
	{
		FWidget *pWidget = getChild(nIndex);
		if (NULL == pWidget) return false;

		return selectElement((FAbstractListElement*)pWidget->getInterface(FOOWC_ABSTRACTLISTELEMENT));
	}

	FCOMBOBOX_INLINE bool FComboBoxList::selectElement(FAbstractListElement* element)
	{
		FASSERT(NULL != getParent());
		FComboBox *parent = static_cast<FComboBox*>(getParent()->getInterface(FOOWC_COMBOBOX));
		if (NULL == parent)
		{
			FASSERT(FALSE);
			return false;
		}

		FAbstractListElement* pOldElement = m_pSelectedElement;
		m_pSelectedElement = element;

		if (pOldElement != element)
		{
			parent->setText(element->getText());

			if (NULL != pOldElement)
			{
				pOldElement->setSelect(false);
			}

			if (NULL != element)
			{
				element->setSelect(true);
				parent->makeWidgetMessage(FWM_ITEMSELECTED, (WPARAM)element->childIndex(), (LPARAM)element);
			}

			return true;
		}

		return false;
	}

	FCOMBOBOX_INLINE void FComboBoxList::dropdown(bool bDropdown/* = true*/)
	{
		FASSERT(NULL != getParent());

		FComboBox *parent = static_cast<FComboBox*>(getParent()->getInterface(FOOWC_COMBOBOX));
		if (NULL == parent)
		{
			FASSERT(FALSE);
			return;
		}

		HWND hParentWnd = parent->getHostWindowHwnd();
		FASSERT(::IsWindow(hParentWnd));

		/** 需要收起下拉列表 */
		if (!bDropdown)
		{
			hide();
			return;
		}

		/** 父控件的屏幕矩形 */
		Core::FRect rcParent = parent->getLayoutRect();
		::MapWindowPoints(hParentWnd, HWND_DESKTOP, (POINT *)&rcParent, 2);

		/** 屏幕大小(使用工作区域） */
		MONITORINFO mi = {};
		mi.cbSize = sizeof(MONITORINFO);
		::GetMonitorInfo(::MonitorFromWindow(hParentWnd, MONITOR_DEFAULTTOPRIMARY), &mi);
		int cyWorkRect = mi.rcWork.bottom - mi.rcWork.top;

		/** 计算最佳弹出高度 */
		int cyNeedDrop = getHeight();
		if (!isFixedHeight())
		{
			cyNeedDrop = getPadding().edgeHeight();
			for (int i = 0; i < getChildrenCount(); ++i)
			{
				FWidget *pWidget = getChild(i);
				FASSERT(NULL != pWidget);

				cyNeedDrop += pWidget->getHeight() + pWidget->getMargin().edgeHeight();
			}
		}
		if (cyNeedDrop <= 0) cyNeedDrop = rcParent.height();
		/** 计算最大可行高度 */
		if (rcParent.top < mi.rcWork.top + cyWorkRect / 2) 
		{
			/** 如果弹出垂直坐标在工作区域上方，
				最大高度参考，工作区域的下方高度
			*/
			if (rcParent.bottom + cyNeedDrop > mi.rcWork.bottom)
			{
				cyNeedDrop = mi.rcWork.bottom - rcParent.bottom;
			}
		}
		else
		{
			/** 如果弹出垂直坐标在工作下方，
				最大高度参考，工作区域的上方高度
			*/
			if (rcParent.top - cyNeedDrop < mi.rcWork.top)
			{
				cyNeedDrop = rcParent.top - mi.rcWork.top;
			}
		}

		/** 计算最佳弹出坐标 */
		Core::FPoint ptShow(rcParent.left, rcParent.bottom);
		if (ptShow.y + cyNeedDrop > mi.rcWork.bottom)
		{
			ptShow.y = rcParent.top - cyNeedDrop;
		}

		/** 如果需要创建窗口 */
		if (!isWindow())
		{
			createWindow(0, getClass(), _T(""), WS_CHILD | WS_POPUP, 0, 0, 0, 0, hParentWnd);
			::SetClassLong(getHwnd(), GCL_STYLE, ::GetClassLong(getHwnd(), GCL_STYLE) | CS_DROPSHADOW);
		}

		::SetWindowPos(getHwnd(), HWND_TOP, ptShow.x, ptShow.y, rcParent.width(), cyNeedDrop, SWP_NOACTIVATE);
		show();
		::SendMessage(hParentWnd, WM_NCACTIVATE, (WPARAM)TRUE, 0);
	}

	FCOMBOBOX_INLINE bool FComboBoxList::isDropdown(void) const
	{
		if (isWindow())
		{
			return (::IsWindowVisible(getHwnd()) == TRUE);
		}

		return false;
	}

	FCOMBOBOX_INLINE void FComboBoxList::elementClickedEvent(FAbstractListElement* pSender)
	{
		if (pSender == FWidgetPrivate::fPushedWidget)
		{
			selectElement(pSender);
			dropdown(false);
		}
	}

	FCOMBOBOX_INLINE LRESULT FComboBoxList::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
	{
		switch (uMsg)
		{
		case WM_KILLFOCUS:
			dropdown(false);
			break;
		}
		return 0;
	}

}} // !namespace