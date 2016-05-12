#include "StdAfx.h"
#include "widgets/FActivex.h"  
#include <assert.h>
#include "core/FApplication.h"
/////////////////////
 

//2 延迟加载问题

#define FACTIVEX_INLINE  FOOINLINE
HWND hwebbrowser;
namespace FooUI{ namespace Widgets{

	//////////////////////////////////
	//FActivexEnum

	class FActivexEnum : public IEnumUnknown
	{
	public:
		FActivexEnum(IUnknown* pUnk) : m_pUnk(pUnk), m_dwRef(1), m_iPos(0)
		{
			m_pUnk->AddRef();
		}
		~FActivexEnum()
		{
			m_pUnk->Release();
		}

		LONG m_iPos;
		ULONG m_dwRef;
		IUnknown* m_pUnk;

		STDMETHOD_(ULONG,AddRef)()
		{
			return ++m_dwRef;
		}
		STDMETHOD_(ULONG,Release)()
		{
			LONG lRef = --m_dwRef;
			if( lRef == 0 ) delete this;
			return lRef;
		}
		STDMETHOD(QueryInterface)(REFIID riid, LPVOID *ppvObject)
		{
			*ppvObject = NULL;
			if( riid == IID_IUnknown ) *ppvObject = static_cast<IEnumUnknown*>(this);
			else if( riid == IID_IEnumUnknown ) *ppvObject = static_cast<IEnumUnknown*>(this);
			if( *ppvObject != NULL ) AddRef();
			return *ppvObject == NULL ? E_NOINTERFACE : S_OK;
		}
		STDMETHOD(Next)(ULONG celt, IUnknown **rgelt, ULONG *pceltFetched)
		{
			if( pceltFetched != NULL ) *pceltFetched = 0;
			if( ++m_iPos > 1 ) return S_FALSE;
			*rgelt = m_pUnk;
			(*rgelt)->AddRef();
			if( pceltFetched != NULL ) *pceltFetched = 1;
			return S_OK;
		}
		STDMETHOD(Skip)(ULONG celt)
		{
			m_iPos += celt;
			return S_OK;
		}
		STDMETHOD(Reset)(void)
		{
			m_iPos = 0;
			return S_OK;
		}
		STDMETHOD(Clone)(IEnumUnknown **ppenum)
		{
			return E_NOTIMPL;
		}
	};


	//////////////////////////////////////////////
	//FActivexFrameWnd

	class FActivexFrameWnd : public IOleInPlaceFrame
	{
	public:
		FActivexFrameWnd(FActivex* pOwner) : m_dwRef(1), m_pOwner(pOwner), m_pActiveObject(NULL)
		{
		}
		~FActivexFrameWnd()
		{
			if( m_pActiveObject != NULL ) m_pActiveObject->Release();
		}

		ULONG m_dwRef;
		FActivex* m_pOwner;
		IOleInPlaceActiveObject* m_pActiveObject;

		// IUnknown
		STDMETHOD_(ULONG,AddRef)()
		{
			return ++m_dwRef;
		}
		STDMETHOD_(ULONG,Release)()
		{
			ULONG lRef = --m_dwRef;
			if( lRef == 0 ) delete this;
			return lRef;
		}
		STDMETHOD(QueryInterface)(REFIID riid, LPVOID *ppvObject)
		{
			*ppvObject = NULL;
			if( riid == IID_IUnknown ) *ppvObject = static_cast<IOleInPlaceFrame*>(this);
			else if( riid == IID_IOleWindow ) *ppvObject = static_cast<IOleWindow*>(this);
			else if( riid == IID_IOleInPlaceFrame ) *ppvObject = static_cast<IOleInPlaceFrame*>(this);
			else if( riid == IID_IOleInPlaceUIWindow ) *ppvObject = static_cast<IOleInPlaceUIWindow*>(this);
			if( *ppvObject != NULL ) AddRef();
			return *ppvObject == NULL ? E_NOINTERFACE : S_OK;
		}  
		// IOleInPlaceFrameWindow
		STDMETHOD(InsertMenus)(HMENU /*hmenuShared*/, LPOLEMENUGROUPWIDTHS /*lpMenuWidths*/)
		{
			return S_OK;
		}
		STDMETHOD(SetMenu)(HMENU /*hmenuShared*/, HOLEMENU /*holemenu*/, HWND /*hwndActiveObject*/)
		{
			return S_OK;
		}
		STDMETHOD(RemoveMenus)(HMENU /*hmenuShared*/)
		{
			return S_OK;
		}
		STDMETHOD(SetStatusText)(LPCOLESTR /*pszStatusText*/)
		{
			return S_OK;
		}
		STDMETHOD(EnableModeless)(BOOL /*fEnable*/)
		{
			return S_OK;
		}
		STDMETHOD(TranslateAccelerator)(LPMSG /*lpMsg*/, WORD /*wID*/)
		{
			return S_FALSE;
		}
		// IOleWindow
		STDMETHOD(GetWindow)(HWND* phwnd)
		{
			if( m_pOwner == NULL ) return E_UNEXPECTED;
			*phwnd = m_pOwner->getHostWindowHwnd();
			return S_OK;
		}
		STDMETHOD(ContextSensitiveHelp)(BOOL /*fEnterMode*/)
		{
			return S_OK;
		}
		// IOleInPlaceUIWindow
		STDMETHOD(GetBorder)(LPRECT /*lprectBorder*/)
		{
			//::GetClientRect(); 
			return S_OK;
		}
		STDMETHOD(RequestBorderSpace)(LPCBORDERWIDTHS /*pborderwidths*/)
		{
			return INPLACE_E_NOTOOLSPACE;
		}
		STDMETHOD(SetBorderSpace)(LPCBORDERWIDTHS /*pborderwidths*/)
		{
			return S_OK;
		}
		STDMETHOD(SetActiveObject)(IOleInPlaceActiveObject* pActiveObject, LPCOLESTR /*pszObjName*/)
		{
			if( pActiveObject != NULL ) pActiveObject->AddRef();
			if( m_pActiveObject != NULL ) m_pActiveObject->Release();
			m_pActiveObject = pActiveObject;
			return S_OK;
		}
	};



	////////////////////////////////
	// FActivexWnd
	 
	HWND FActivexWnd::Init(FActivexCtrl* pOwner, HWND hWndParent)
	{
		m_pOwner = pOwner;
	  
		FASSERT(!::IsWindow(getHwnd()));
		FASSERT(::IsWindow(hWndParent));

		DWORD dwStyle = WS_CHILD  | WS_VISIBLE;// |  WS_CLIPSIBLINGS | WS_CLIPCHILDREN
		/*createWindow(hWndParent);*/

		Core::FRect rcLayout = getLayoutRect();
		bool bRet = __super::createWindow(0, NULL, _T("activex"), dwStyle , 
			0, 0, 0, 0, 
			hWndParent, NULL, NULL); 
   
		return getHwnd(); 
	}

	

	void FActivexWnd::DoVerb(LONG iVerb)
	{
		if( m_pOwner == NULL ) return;
		if( m_pOwner->m_pOwner == NULL ) return;
		IOleObject* pUnk = NULL;
		m_pOwner->m_pOwner->GetControl(IID_IOleObject, (LPVOID*) &pUnk);
		if( pUnk == NULL ) return;
		CSafeRelease<IOleObject> RefOleObject = pUnk;
		IOleClientSite* pOleClientSite = NULL;
		m_pOwner->QueryInterface(IID_IOleClientSite, (LPVOID*) &pOleClientSite);
		CSafeRelease<IOleClientSite> RefOleClientSite = pOleClientSite;
		pUnk->DoVerb(iVerb, NULL, pOleClientSite, 0, m_pOwner->m_pOwner->getHwnd(), &m_pOwner->m_pOwner->getLayoutRect());
 
	}

	LRESULT FActivexWnd::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
	{
		bHandled = TRUE;
		LRESULT lRes;  
		switch( uMsg ) {
	case WM_PAINT:      
		lRes = OnPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_SETFOCUS:     
		lRes = OnSetFocus(uMsg, wParam, lParam, bHandled); break;
	case WM_KILLFOCUS:   
		lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break;
	case WM_ERASEBKGND:  
		lRes = OnEraseBkgnd(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEACTIVATE: 
		lRes = OnMouseActivate(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
		}
		if( !bHandled ) return __super::handleMessage(uMsg, wParam, lParam, bHandled);
		return lRes;
	}

	LRESULT FActivexWnd::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
	{
		if( m_pOwner->m_pViewObject == NULL ) bHandled = FALSE;
		return 1;
	}

	LRESULT FActivexWnd::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
	{
		IOleObject* pUnk = NULL;
		m_pOwner->m_pOwner->GetControl(IID_IOleObject, (LPVOID*) &pUnk);
		if( pUnk == NULL ) return 0;
		CSafeRelease<IOleObject> RefOleObject = pUnk;
		DWORD dwMiscStatus = 0;
		pUnk->GetMiscStatus(DVASPECT_CONTENT, &dwMiscStatus);
		if( (dwMiscStatus & OLEMISC_NOUIACTIVATE) != 0 ) return 0;
		if( !m_pOwner->m_bInPlaceActive ) DoVerb(OLEIVERB_INPLACEACTIVATE);
		bHandled = FALSE;
		return 0;
	}

	LRESULT FActivexWnd::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
	{
		bHandled = FALSE;
		m_pOwner->m_bFocused = true;
		if( !m_pOwner->m_bUIActivated ) DoVerb(OLEIVERB_UIACTIVATE);
		return 0;
	}

	LRESULT FActivexWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
	{
		bHandled = FALSE;
		m_pOwner->m_bFocused = false;
		return 0;
	}

	LRESULT FActivexWnd::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
	{
		PAINTSTRUCT ps = { 0 };
		//////////////修改 过  
		::BeginPaint(getHwnd(), &ps); 
		::EndPaint(getHwnd(), &ps);

	
		return 1;
	}

	//////////////////////////////////
	//FActivex
	FACTIVEX_INLINE FActivex::FActivex(FWidget* parent):
		FWidget(parent)
		,m_sModuleName(_T(""))
		,m_pUnk(NULL)
		,m_pControl(NULL)
		,m_hwndHost(NULL)
		,m_bCreate(false)
		,m_bDelayCreate(true)
		,pszCLSID(_T(""))
	 
	{
		m_clsid = IID_NULL;
		CoInitialize(NULL);
		modifyFlag(FOOFLAG_TRANSPARENT, 0); 
	}
	FACTIVEX_INLINE FActivex::~FActivex(void) 
	{
		ReleaseControl();
		CoUninitialize();
		
	}
	bool FActivex::CreateControl(const CLSID clsid)
	{
		assert(clsid != IID_NULL);
		if (clsid == IID_NULL) return false;
		m_bCreate = false ;
		m_clsid = clsid;
		if (!m_bDelayCreate)
			DoCreateControl(); 
		return true;
	}


	static void PixelToHiMetric(const SIZEL* lpSizeInPix, LPSIZEL lpSizeInHiMetric)
	{
#define HIMETRIC_PER_INCH   2540
#define MAP_PIX_TO_LOGHIM(x,ppli)   MulDiv(HIMETRIC_PER_INCH, (x), (ppli))
#define MAP_LOGHIM_TO_PIX(x,ppli)   MulDiv((ppli), (x), HIMETRIC_PER_INCH)
		int nPixelsPerInchX;    // Pixels per logical inch along width
		int nPixelsPerInchY;    // Pixels per logical inch along height
		HDC hDCScreen = ::GetDC(NULL);
		nPixelsPerInchX = ::GetDeviceCaps(hDCScreen, LOGPIXELSX);
		nPixelsPerInchY = ::GetDeviceCaps(hDCScreen, LOGPIXELSY);
		::ReleaseDC(NULL, hDCScreen);
		lpSizeInHiMetric->cx = MAP_PIX_TO_LOGHIM(lpSizeInPix->cx, nPixelsPerInchX);
		lpSizeInHiMetric->cy = MAP_PIX_TO_LOGHIM(lpSizeInPix->cy, nPixelsPerInchY);
	}

	FACTIVEX_INLINE void FActivex::layout(const Core::FRect& rcLayout)
	{ 
		FWidget::layout(&rcLayout); 
		if( !m_bCreate ) DoCreateControl();
		if( m_pUnk == NULL )
		{  
			return;
		}
		if( m_pControl == NULL ) 
		{	  
			return;
		}
	 
		SIZEL hmSize = { 0 };
		SIZEL pxSize = { 0 };
		pxSize.cx = rcLayout.right - rcLayout.left;
		pxSize.cy = rcLayout.bottom - rcLayout.top;
		PixelToHiMetric(&pxSize, &hmSize);

		if( m_pUnk != NULL ) {
			m_pUnk->SetExtent(DVASPECT_CONTENT, &hmSize);
		}
		if( m_pControl->m_pInPlaceObject != NULL ) {
			Core::FRect rcItem = rcLayout;
			if( !m_pControl->m_bWindowless ) 
			{   
				 
 				rcItem.offsetX(-rcItem.left);
 				rcItem.offsetY(-rcItem.top);
			}
			m_pControl->m_pInPlaceObject->SetObjectRects(&rcItem, &rcItem); 
		}
		if( !m_pControl->m_bWindowless ) { 
			 
			::MoveWindow(m_pControl->m_pOwner->m_hwndHost, rcLayout.left, rcLayout.top, rcLayout.right - rcLayout.left, rcLayout.bottom - rcLayout.top, TRUE);
		}
		
	}
	void FActivex::paintEvent(Event::FPaintEvent* e)
	{    
		if( m_pControl != NULL /*&& m_pControl->m_bWindowless*/ && m_pControl->m_pViewObject != NULL )
		{
			m_pControl->m_pViewObject->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, e->graphics()->getSafeHdc(), (RECTL*) &e->updateRect(), (RECTL*) &getLayoutRect(), NULL, NULL); 
		}
	}


	LRESULT FActivex::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
	{
		if( m_pControl == NULL ) return 0; 
		if( !m_pControl->m_bInPlaceActive ) return 0;
		if( m_pControl->m_pInPlaceObject == NULL ) return 0;
		if(  uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST ) return 0;
		bool bWasHandled = true;
		if( (uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST) || uMsg == WM_SETCURSOR ) {
			// Mouse message only go when captured or inside rect
			DWORD dwHitResult = m_pControl->m_bCaptured ? HITRESULT_HIT : HITRESULT_OUTSIDE;
			if( dwHitResult == HITRESULT_OUTSIDE && m_pControl->m_pViewObject != NULL ) {
				IViewObjectEx* pViewEx = NULL;
				m_pControl->m_pViewObject->QueryInterface(IID_IViewObjectEx, (LPVOID*) &pViewEx);
				if( pViewEx != NULL ) {
					POINT ptMouse = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
				//	pViewEx->QueryHitPoint(DVASPECT_CONTENT, &m_rcItem, ptMouse, 0, &dwHitResult);
					pViewEx->Release();
				}
			}
			if( dwHitResult != HITRESULT_HIT ) return 0;
			if( uMsg == WM_SETCURSOR ) bWasHandled = false;
		}
		else if( uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST ) {
			// Keyboard messages just go when we have focus
			//if( !IsFocused() ) return 0;
		}
		else {
			switch( uMsg ) {
			case WM_HELP:
			case WM_CONTEXTMENU:
				bWasHandled = false;
				break;
			default:
				return 0;
			}
		}
		LRESULT lResult = 0;
		HRESULT Hr = m_pControl->m_pInPlaceObject->OnWindowMessage(uMsg, wParam, lParam, &lResult);
		if( Hr == S_OK ) bHandled = bWasHandled;
		return lResult;
	}

	HRESULT FActivex::GetControl(const IID iid, LPVOID* ppRet)
	{ 
		if( ppRet == NULL ) return E_POINTER;
		if( m_pUnk == NULL ) return E_PENDING;
		return m_pUnk->QueryInterface(iid, (LPVOID*) ppRet);
	}

	FACTIVEX_INLINE void FActivex::SetDelayCreate(bool bDelayCreate)
	{
		if( m_bDelayCreate == bDelayCreate ) return;
		if( bDelayCreate == false ) {
			if( m_bCreate == false && m_clsid != IID_NULL ) DoCreateControl();
		}
		m_bDelayCreate = bDelayCreate;
	}

//	FEDIT_INLINE bool FEdit::setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	FACTIVEX_INLINE bool FActivex::setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue)
	{
		__super::setAttribute(pMarkup, pszName, pszValue);
		if( _tcscmp(pszName, _T("clsid")) == 0 ) 
		{
			CreateControl(pszValue); 
		}
		 else if( _tcscmp(pszName, _T("delaycreate")) == 0 ) SetDelayCreate(_tcscmp(pszValue, _T("true")) == 0);
		return true;
	}

	bool FActivex::CreateControl(LPCTSTR pstrCLSID)
	{
		CLSID clsid = {0};
		OLECHAR	 szCLSID[100] = {0};
#ifndef _UNICODE
		::MultiByteToWideChar(::GetACP(), 0, pstrCLSID, -1, szCLSID, lengthof(szCLSID), -1);
#else
		wcsncpy_s(szCLSID, pstrCLSID, lengthof(szCLSID) );
#endif
		if(pstrCLSID[0] == '{') ::CLSIDFromString(szCLSID, &clsid);
		else ::CLSIDFromProgID(szCLSID, &clsid);
		return CreateControl(clsid);
	}
	void FActivex::ReleaseControl()
	{
		m_hwndHost = NULL;
		if( m_pUnk != NULL ) {
			IObjectWithSite* pSite = NULL;
			m_pUnk->QueryInterface(IID_IObjectWithSite, (LPVOID*) &pSite);
			if( pSite != NULL ) {
				pSite->SetSite(NULL);
				pSite->Release();
			}
			m_pUnk->Close(OLECLOSE_NOSAVE);
			m_pUnk->SetClientSite(NULL);
			m_pUnk->Release(); 
			m_pUnk = NULL;
		}
		if( m_pControl != NULL ) {
			m_pControl->m_pOwner = NULL;
			m_pControl->Release();
			m_pControl = NULL;
		}
		Core::FApplication::getApplication()->removeMessageFilter((Core::FMessageFilter*)this);
	}
	
	typedef HRESULT (__stdcall *DllGetClassObjectFunc)(REFCLSID rclsid, REFIID riid, LPVOID* ppv); 

	bool FActivex::DoCreateControl()
	{ 
		if (m_bCreate)
			return true;
		ReleaseControl();
		m_bCreate = true;
		IOleControl* pOleControl = NULL;
		HRESULT Hr = -1;
		if (_tcscmp(m_sModuleName, _T("")) != 0)
		{
			HMODULE hModule = ::LoadLibrary(m_sModuleName);
			IClassFactory* pClassFactory = NULL;
			DllGetClassObjectFunc pDllGetClassObjectFunc = (DllGetClassObjectFunc)GetProcAddress(hModule,"DllGetClassObject");
			Hr = pDllGetClassObjectFunc(m_clsid, IID_IClassFactory, (LPVOID*)pClassFactory );
			if (SUCCEEDED(Hr))
			{
				Hr = pClassFactory->CreateInstance(NULL, IID_IOleObject, (LPVOID*)&pOleControl);
			}
			pClassFactory->Release();
		}
		// CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID);
		if (FAILED(Hr))
		{
			Hr = ::CoCreateInstance(m_clsid, NULL, CLSCTX_ALL, IID_IOleObject,(LPVOID*)&pOleControl); 
		}
		assert(SUCCEEDED(Hr));
		if (FAILED(Hr)) return false;
		pOleControl->QueryInterface(IID_IOleObject, (LPVOID*) &m_pUnk);
		pOleControl->Release();
	 
		if(m_pUnk == NULL)
		{  
			return false;
		}
		// Create the host too
		m_pControl = new FActivexCtrl; 
		m_pControl->m_pOwner = this;
		 // More control creation stuff
		DWORD dwMiscStatus = 0;
		m_pUnk->GetMiscStatus(DVASPECT_CONTENT, &dwMiscStatus);
		IOleClientSite* pOleCLientSite = NULL;
		m_pControl->QueryInterface(IID_IOleClientSite, (LPVOID*)&pOleCLientSite);
	//	CSafeRelease<IOleClientSite> RefOleClientSite = pOleCLientSite;
		 // More control creation stuff
		if (dwMiscStatus & OLEMISC_SETCLIENTSITEFIRST != 0) Hr = m_pUnk->SetClientSite(pOleCLientSite);
		IPersistStreamInit* pPersistStreamInit = NULL;
		m_pUnk->QueryInterface(IID_IPersistStreamInit, (LPVOID*) &pPersistStreamInit);
		if (pPersistStreamInit != NULL)
		{
			Hr = pPersistStreamInit->InitNew();
			pPersistStreamInit->Release();
		}
		if (FAILED(Hr)) return false;
		if(dwMiscStatus & OLEMISC_SETCLIENTSITEFIRST == 0) m_pUnk->SetClientSite(pOleCLientSite);
		// Grab the view
		Hr = m_pUnk->QueryInterface(IID_IViewObjectEx,(LPVOID*) &m_pControl->m_pViewObject );
		if(FAILED(Hr)) Hr = m_pUnk->QueryInterface(IID_IViewObject2, (LPVOID*)&m_pControl->m_pViewObject);
		if(FAILED(Hr)) Hr = m_pUnk->QueryInterface(IID_IOleObject, (LPVOID*)&m_pControl->m_pViewObject);
		 
		//Activate and done..
	 	m_pUnk->SetHostNames(OLESTR("activex"), NULL);
		if ((dwMiscStatus & OLEMISC_INVISIBLEATRUNTIME) == 0 )
		{   
			Hr = m_pUnk->DoVerb(OLEIVERB_INPLACEACTIVATE, NULL, pOleCLientSite, 0, getSafeHwnd(),  &getLayoutRect()); 
		}
		 
		///////////
		IObjectWithSite* pSite = NULL;
		m_pUnk->QueryInterface(IID_IObjectWithSite, (LPVOID*) &pSite);
		if (pSite)
		{
			pSite->SetSite(static_cast<IOleClientSite*>(m_pControl));
			pSite->Release();
		}
		return SUCCEEDED(Hr);
	}
	 
	//////////////////////////////////////////////////
	//FActivexCtrl
	FActivexCtrl::FActivexCtrl() : 
	m_dwRef(1), 
	m_pOwner(NULL), 
	m_pWindow(NULL),
	m_pUnkSite(NULL), 
	m_pViewObject(NULL),
	m_pInPlaceObject(NULL),
	m_bLocked(false), 
	m_bFocused(false),
	m_bCaptured(false),
	m_bWindowless(true),
	m_bUIActivated(false),
	m_bInPlaceActive(false)
	{
		
	}

	FActivexCtrl::~FActivexCtrl()
	{
		if( m_pWindow != NULL ) {
			::DestroyWindow(m_pOwner->getHwnd());
			delete m_pWindow;
		}
		if( m_pUnkSite != NULL ) m_pUnkSite->Release();
		if( m_pViewObject != NULL ) m_pViewObject->Release();
		if( m_pInPlaceObject != NULL ) m_pInPlaceObject->Release();
	}

	HRESULT FActivexCtrl::CreateActiveXWnd()
	{
		if (m_pWindow != NULL) return S_OK;
		m_pWindow = new FActivexWnd; 
		if (m_pWindow == NULL) return E_OUTOFMEMORY;
	 
		m_pOwner->m_hwndHost = m_pWindow->Init(this,  m_pOwner->getHostWindowHwnd());

		return 0;
	} 

	STDMETHODIMP FActivexCtrl::QueryInterface(REFIID riid, LPVOID *ppvObject)
	{
		*ppvObject = NULL;
		if( riid == IID_IUnknown )                       *ppvObject = static_cast<IOleWindow*>(this);
		else if( riid == IID_IOleClientSite )            *ppvObject = static_cast<IOleClientSite*>(this);
		else if( riid == IID_IOleInPlaceSiteWindowless ) *ppvObject = static_cast<IOleInPlaceSiteWindowless*>(this);
		else if( riid == IID_IOleInPlaceSiteEx )         *ppvObject = static_cast<IOleInPlaceSiteEx*>(this);
		else if( riid == IID_IOleInPlaceSite )           *ppvObject = static_cast<IOleInPlaceSite*>(this);
		else if( riid == IID_IOleWindow )                *ppvObject = static_cast<IOleWindow*>(this);
		else if( riid == IID_IOleControlSite )           *ppvObject = static_cast<IOleControlSite*>(this);
		else if( riid == IID_IOleContainer )             *ppvObject = static_cast<IOleContainer*>(this);
		else if( riid == IID_IObjectWithSite )           
			*ppvObject = static_cast<IObjectWithSite*>(this);
		if( *ppvObject != NULL ) AddRef();
		return *ppvObject == NULL ? E_NOINTERFACE : S_OK;
	}

	STDMETHODIMP_(ULONG) FActivexCtrl::AddRef()
	{
		return ++m_dwRef;
	}

	STDMETHODIMP_(ULONG) FActivexCtrl::Release()
	{
		LONG lRef = --m_dwRef;
		if( lRef == 0 ) delete this;
		return lRef;
	}
	// IObjectWithSite
	STDMETHODIMP FActivexCtrl::SetSite (IUnknown *pUnkSite)
	{
		if (m_pUnkSite != NULL)
		{
			m_pUnkSite->Release();
			m_pUnkSite = NULL;
		}
		if (pUnkSite != NULL)
		{
			m_pUnkSite = pUnkSite;
			m_pUnkSite->AddRef();
		}
		return S_OK;
	}
	STDMETHODIMP  FActivexCtrl::GetSite (REFIID riid, LPVOID* ppvSite)
	{
		if (ppvSite == NULL) return E_POINTER;
		*ppvSite = NULL;
		if (m_pUnkSite == NULL) return E_FAIL;
		return m_pUnkSite->QueryInterface(riid, ppvSite);
	}

	// IOleClientSite
	STDMETHODIMP FActivexCtrl::SaveObject (void)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP FActivexCtrl::GetMoniker (DWORD dwAssign, DWORD dwWhichMoniker, IMoniker** ppmk)
	{
		if (ppmk != NULL) *ppmk = NULL;
		return	E_NOTIMPL;
	}
	STDMETHODIMP FActivexCtrl::GetContainer(IOleContainer** ppContainer)
	{
		if (ppContainer == NULL) return E_POINTER;
		*ppContainer = NULL;
		HRESULT Hr = E_NOTIMPL;
		if (m_pUnkSite != NULL)
		{
			Hr = m_pUnkSite->QueryInterface(IID_IOleContainer, (LPVOID*) ppContainer);
		}
		if (FAILED(Hr))
		{
			Hr  = QueryInterface(IID_IOleContainer, (LPVOID*) ppContainer);
		}
		return Hr;
	}
	STDMETHODIMP FActivexCtrl::ShowObject(void)
	{
		if (m_pOwner == NULL || hwebbrowser  == NULL)
		{
			return E_UNEXPECTED;
		} 
		HDC hDC = ::GetDC(m_pOwner->m_hwndHost);
		if (hDC == NULL) return E_FAIL;
		if (m_pViewObject != NULL)
		{
			m_pViewObject->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, hDC, (RECTL*)&m_pOwner->getLayoutRect(), (RECTL*)&m_pOwner->getLayoutRect(), NULL, NULL);
		}
		::ReleaseDC(m_pOwner->m_hwndHost, hDC);
		return S_OK;
	}
	STDMETHODIMP FActivexCtrl::OnShowWindow(BOOL fShow)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP FActivexCtrl::RequestNewObjectLayout(void)
	{
		return S_OK;
	}

	// IOleInPlaceSiteWindowless
	STDMETHODIMP FActivexCtrl::CanWindowlessActivate(void)
	{
		return S_OK;
	}
	STDMETHODIMP FActivexCtrl::GetCapture(void)
	{
		if (m_pOwner == NULL) return E_UNEXPECTED;
		
		
		return m_bCaptured ? S_OK : S_FALSE;
	}
	STDMETHODIMP FActivexCtrl::SetCapture(BOOL fCapture)
	{
		if(m_pOwner == NULL) return E_UNEXPECTED;
		m_bCaptured = (fCapture == TRUE);
		if(fCapture) ::SetCapture(m_pOwner->m_hwndHost);
		else	::ReleaseCapture();
		return S_OK;
	}
	STDMETHODIMP FActivexCtrl::GetFocus(void)
	{
		if( m_pOwner == NULL ) return E_UNEXPECTED;
		return m_bFocused ? S_OK : S_FALSE;
		return 0;
	}
	STDMETHODIMP FActivexCtrl::SetFocus(BOOL fFocus)
	{
		if(m_pOwner == NULL) return E_UNEXPECTED;
		if(fFocus) 
			m_pOwner->setFocus();
		m_bFocused = (fFocus == TRUE);
		return S_OK; 
	 
	}
	STDMETHODIMP FActivexCtrl::GetDC(LPCRECT pRect, DWORD grfFlags, HDC* phDC)
	{
		if(phDC == NULL) return E_POINTER;
		if(m_pOwner == NULL) return E_UNEXPECTED;
		*phDC = ::GetDC(m_pOwner->m_hwndHost);
		if ((grfFlags & OLEDC_PAINTBKGND) != 0)
		{
			Core::FRect rcItem = m_pOwner->getLayoutRect();
			if(!m_bWindowless) 
			{
				rcItem.right = rcItem.left;
				rcItem.bottom = rcItem.top;
			}
			::FillRect(*phDC, &rcItem, (HBRUSH) (COLOR_WINDOW + 1));
		}
		return S_OK;
	}
	STDMETHODIMP FActivexCtrl::ReleaseDC(HDC hDC)
	{
		if(m_pOwner == NULL) return E_UNEXPECTED;
		::ReleaseDC(m_pOwner->m_hwndHost, hDC);
		return S_OK;
	}

	STDMETHODIMP FActivexCtrl::InvalidateRect(LPCRECT pRect, BOOL fErase)
	{
		if(m_pOwner == NULL) return E_UNEXPECTED;

		if(m_pOwner->m_hwndHost == NULL) return E_FAIL;
		return ::InvalidateRect(m_pOwner->m_hwndHost, pRect, fErase) ? S_OK : E_FAIL;
	}
	STDMETHODIMP FActivexCtrl::InvalidateRgn(HRGN hRGN, BOOL fErase)
	{
		if(m_pOwner == NULL) return E_UNEXPECTED;
		
		return  ::InvalidateRgn(m_pOwner->m_hwndHost, hRGN, fErase) ? S_OK : E_FAIL;
	}
	STDMETHODIMP FActivexCtrl::ScrollRect(INT dx, INT dy, LPCRECT pRectScroll, LPCRECT pRectClip)
	{
		return S_OK;
	}
	STDMETHODIMP FActivexCtrl::AdjustRect(LPRECT prc)
	{
		return S_OK;
	}
	STDMETHODIMP FActivexCtrl::OnDefWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* plResult)
	{
		if(m_pOwner == NULL) return E_UNEXPECTED;
		*plResult = ::DefWindowProc(m_pOwner->m_hwndHost, msg, wParam, lParam);
		return S_OK;
	}

	// IOleInPlaceSiteEx
	STDMETHODIMP FActivexCtrl::OnInPlaceActivateEx(BOOL *pfNoRedraw, DWORD dwFlags)
	{
		if( m_pOwner == NULL ) return E_UNEXPECTED;
		if( m_pOwner->m_pUnk == NULL ) return E_UNEXPECTED;
		::OleLockRunning(m_pOwner->m_pUnk, TRUE, FALSE);
		HWND hWndFrame = m_pOwner->getHostWindowHwnd();
		HRESULT Hr = E_FAIL;
		if( (dwFlags & ACTIVATE_WINDOWLESS) != 0 ) {
			m_bWindowless = true;
			Hr = m_pOwner->m_pUnk->QueryInterface(IID_IOleInPlaceObjectWindowless, (LPVOID*) &m_pInPlaceObject);
			m_pOwner->m_hwndHost = hWndFrame;			 
			Core::FApplication::getApplication()->addMessageFilter((Core::FMessageFilter *)m_pOwner);
		}
		if( FAILED(Hr) ) {
			m_bWindowless = false;
			Hr = CreateActiveXWnd();
			if( FAILED(Hr) ) return Hr;
			Hr = m_pOwner->m_pUnk->QueryInterface(IID_IOleInPlaceObject, (LPVOID*) &m_pInPlaceObject);
		}
		if( m_pInPlaceObject != NULL ) {
			Core::FRect rcItem = m_pOwner->getLayoutRect();
			if( !m_bWindowless )
			{ 
				rcItem.offsetX(-rcItem.left);
				rcItem.offsetY(-rcItem.top);
			}
			m_pInPlaceObject->SetObjectRects(&rcItem, &rcItem);
		}
		m_bInPlaceActive = SUCCEEDED(Hr);
		return Hr;
		return 0;
	}
	STDMETHODIMP FActivexCtrl::OnInPlaceDeactivateEx(BOOL fNoRedraw)
	{
		m_bInPlaceActive = false;
		if( m_pInPlaceObject != NULL ) {
			m_pInPlaceObject->Release();
			m_pInPlaceObject = NULL;
		}
		if( m_pWindow != NULL ) {
			::DestroyWindow(m_pOwner->getHwnd());
			delete m_pWindow;
			m_pWindow = NULL;
		}
		return S_OK;
	}
	STDMETHODIMP FActivexCtrl::RequestUIActivate(void)
	{
		return S_OK;
	}

	// IOleInPlaceSite
	STDMETHODIMP FActivexCtrl::CanInPlaceActivate(void)
	{
		return S_OK;
	}
	STDMETHODIMP FActivexCtrl::OnInPlaceActivate(void)
	{
		BOOL bDummy = FALSE;
		return OnInPlaceActivateEx(&bDummy, 0);
	}
	STDMETHODIMP FActivexCtrl::OnUIActivate(void)
	{
		m_bUIActivated = true;
		return S_OK;
	}
	STDMETHODIMP FActivexCtrl::GetWindowContext(IOleInPlaceFrame** ppFrame, IOleInPlaceUIWindow** ppDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo)
	{
		if( ppDoc == NULL ) return E_POINTER;
		if( ppFrame == NULL ) return E_POINTER;
		if( lprcPosRect == NULL ) return E_POINTER;
		if( lprcClipRect == NULL ) return E_POINTER;
		*ppFrame = new FActivexFrameWnd(m_pOwner);
		*ppDoc = NULL;
		ACCEL ac = { 0 };
		HACCEL hac = ::CreateAcceleratorTable(&ac, 1);
		lpFrameInfo->cb = sizeof(OLEINPLACEFRAMEINFO);
		lpFrameInfo->fMDIApp = FALSE;
		lpFrameInfo->hwndFrame = m_pOwner->getHostWindowHwnd();

		lpFrameInfo->haccel = hac;
		lpFrameInfo->cAccelEntries = 1;
		return S_OK;
	}
	STDMETHODIMP FActivexCtrl::Scroll(SIZE scrollExtant)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP FActivexCtrl::OnUIDeactivate(BOOL fUndoable)
	{
		return S_OK;
	}
	STDMETHODIMP FActivexCtrl::OnInPlaceDeactivate(void)
	{
		return OnInPlaceDeactivateEx(TRUE);
	}
	STDMETHODIMP FActivexCtrl::DiscardUndoState( void)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP FActivexCtrl::DeactivateAndUndo( void)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP FActivexCtrl::OnPosRectChange(LPCRECT lprcPosRect)
	{
		return E_NOTIMPL;
	}

	// IOleWindow
	STDMETHODIMP FActivexCtrl::GetWindow(HWND* phwnd)
	{
		if( m_pOwner == NULL ) return E_UNEXPECTED;
		if( m_pOwner->m_hwndHost == NULL ) CreateActiveXWnd();
		if( m_pOwner->m_hwndHost == NULL ) return E_FAIL;
		*phwnd = m_pOwner->m_hwndHost;
		return S_OK;
	}

	STDMETHODIMP FActivexCtrl::ContextSensitiveHelp(BOOL fEnterMode)
	{ 
		return S_OK;
	}

 

	// IOleControlSite
	STDMETHODIMP FActivexCtrl::OnControlInfoChanged(void)
	{
		return S_OK;
	}
	STDMETHODIMP FActivexCtrl::LockInPlaceActive(BOOL fLock)
	{
		return S_OK;
	}
	STDMETHODIMP FActivexCtrl::GetExtendedControl(IDispatch** ppDisp)
	{
		if( ppDisp == NULL ) return E_POINTER;   
		if( m_pOwner == NULL ) return E_UNEXPECTED;
		if( m_pOwner->m_pUnk == NULL ) return E_UNEXPECTED;
		return m_pOwner->m_pUnk->QueryInterface(IID_IDispatch, (LPVOID*) ppDisp);
	}
	STDMETHODIMP FActivexCtrl::TransformCoords(POINTL* pPtlHimetric, POINTF* pPtfContainer, DWORD dwFlags)
	{
		return S_OK;
	}
	STDMETHODIMP FActivexCtrl::TranslateAccelerator(MSG* pMsg, DWORD grfModifiers)
	{
		return S_OK;
	}
	STDMETHODIMP FActivexCtrl::OnFocus(BOOL fGotFocus)
	{
		 m_bFocused = (fGotFocus == TRUE);
		return S_OK;
	}
	STDMETHODIMP FActivexCtrl::ShowPropertyFrame(void)
	{
		return E_NOTIMPL;
	}

	// IOleContainer
	STDMETHODIMP FActivexCtrl::EnumObjects(DWORD grfFlags, IEnumUnknown** ppenum)
	{
		if( ppenum == NULL ) return E_POINTER;
		if( m_pOwner == NULL ) return E_UNEXPECTED;
		*ppenum = new FActivexEnum(m_pOwner->m_pUnk);
		return S_OK;
	}
	STDMETHODIMP FActivexCtrl::LockContainer(BOOL fLock)
	{
		m_bLocked = fLock != FALSE;
		return S_OK;
	}

	// IParseDisplayName
	STDMETHODIMP FActivexCtrl::ParseDisplayName(IBindCtx* pbc, LPOLESTR pszDisplayName, ULONG* pchEaten, IMoniker** ppmkOut)
	{
		return E_NOTIMPL;
	}
}}