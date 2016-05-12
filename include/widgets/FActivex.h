#ifndef		FACTIVEX_H
#define		FACTIVEX_H

#include "core/FBase.h"
#include "widgets/FWidget.h" 
#include "core/FApplication.h"
#include <ocidl.h>
 
 
template< class T >
class CSafeRelease
{
public:
	CSafeRelease(T* p) : m_p(p) { };
	~CSafeRelease() { if( m_p != NULL ) m_p->Release(); };
	T* Detach() { T* t = m_p; m_p = NULL; return t; };
	T* m_p;
};


namespace FooUI	{ namespace Widgets{

	class FActivex;
	class FActivexWnd;
	class FActivexCtrl;

	
	//////////////////////////////////////////
	//FActiveX
	#define FOOWC_ACTIVEX	_T("activex") 
	class FOOUI_LIB FActivex : public FWidget
	{
		friend class FActivexCtrl;
	public:
		explicit FActivex(FWidget* parent = NULL);
		~FActivex(void);  
			FOODECLARE_CLASS(FOOWC_ACTIVEX)
			FOODECLARE_CONTAINER(true)
	public:
		bool CreateControl(const CLSID clsid);
		bool CreateControl(LPCTSTR pstrCLSID);
		CLSID GetClsid(); 
		bool setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue);
		HRESULT GetControl(const IID iid, LPVOID* ppRet);
		void ReleaseControl();
	protected:
		void paintEvent(Event::FPaintEvent* e); 
		virtual bool DoCreateControl();
		virtual void layout(const Core::FRect& rcLayout); 
		void SetDelayCreate(bool bDelayCreate);
	
		LRESULT		handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	protected:
		CLSID m_clsid;
		bool  m_bCreate;
		bool  m_bDelayCreate;
		IOleObject* m_pUnk;
		HWND	m_hwndHost;
		LPCTSTR m_sModuleName;
		FActivexCtrl* m_pControl; 
		LPCTSTR pszCLSID;
	
	};


	///////////////////////////////////////////
	//FActivexWnd

	class FActivexWnd : FWidget
	{
	public: 
		HWND Init(FActivexCtrl* pOwner, HWND hWndParent);

		LPCTSTR GetWindowClassName() const; 
	protected:
		void DoVerb(LONG iVerb);

		LRESULT OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
		LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
		LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
		LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
		LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
		LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	protected:
		FActivexCtrl* m_pOwner;
	};

	///////////////////////////////////////////
	//FActivexCtrl

	class FActivexCtrl :
		public IOleClientSite,
		public IOleInPlaceSiteWindowless,
		public IOleControlSite,
		public IObjectWithSite,
		public IOleContainer
	{
		friend class FActivex;
		friend class FActivexWnd;
	public:
		  FActivexCtrl();
		~FActivexCtrl();

		// IUnknown
		STDMETHOD_(ULONG,AddRef)();
		STDMETHOD_(ULONG,Release)();
		STDMETHOD(QueryInterface)(REFIID riid, LPVOID *ppvObject);
  
		// IObjectWithSite
		STDMETHOD(SetSite)(IUnknown *pUnkSite);
		STDMETHOD(GetSite)(REFIID riid, LPVOID* ppvSite);

		// IOleClientSite
		STDMETHOD(SaveObject)(void);       
		STDMETHOD(GetMoniker)(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker** ppmk);
		STDMETHOD(GetContainer)(IOleContainer** ppContainer);        
		STDMETHOD(ShowObject)(void);        
		STDMETHOD(OnShowWindow)(BOOL fShow);        
		STDMETHOD(RequestNewObjectLayout)(void);

		// IOleInPlaceSiteWindowless
		STDMETHOD(CanWindowlessActivate)(void);
		STDMETHOD(GetCapture)(void);
		STDMETHOD(SetCapture)(BOOL fCapture);
		STDMETHOD(GetFocus)(void);
		STDMETHOD(SetFocus)(BOOL fFocus);
		STDMETHOD(GetDC)(LPCRECT pRect, DWORD grfFlags, HDC* phDC);
		STDMETHOD(ReleaseDC)(HDC hDC);
		STDMETHOD(InvalidateRect)(LPCRECT pRect, BOOL fErase);
		STDMETHOD(InvalidateRgn)(HRGN hRGN, BOOL fErase);
		STDMETHOD(ScrollRect)(INT dx, INT dy, LPCRECT pRectScroll, LPCRECT pRectClip);
		STDMETHOD(AdjustRect)(LPRECT prc);
		STDMETHOD(OnDefWindowMessage)(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* plResult);

		// IOleInPlaceSiteEx
		STDMETHOD(OnInPlaceActivateEx)(BOOL *pfNoRedraw, DWORD dwFlags);        
		STDMETHOD(OnInPlaceDeactivateEx)(BOOL fNoRedraw);       
		STDMETHOD(RequestUIActivate)(void);

		// IOleInPlaceSite
		STDMETHOD(CanInPlaceActivate)(void);       
		STDMETHOD(OnInPlaceActivate)(void);        
		STDMETHOD(OnUIActivate)(void);
		STDMETHOD(GetWindowContext)(IOleInPlaceFrame** ppFrame, IOleInPlaceUIWindow** ppDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo);
		STDMETHOD(Scroll)(SIZE scrollExtant);
		STDMETHOD(OnUIDeactivate)(BOOL fUndoable);
		STDMETHOD(OnInPlaceDeactivate)(void);
		STDMETHOD(DiscardUndoState)( void);
		STDMETHOD(DeactivateAndUndo)( void);
		STDMETHOD(OnPosRectChange)(LPCRECT lprcPosRect);

		// IOleWindow
		STDMETHOD(GetWindow)(HWND* phwnd);
		STDMETHOD(ContextSensitiveHelp)(BOOL fEnterMode);

		// IOleControlSite
		STDMETHOD(OnControlInfoChanged)(void);      
		STDMETHOD(LockInPlaceActive)(BOOL fLock);       
		STDMETHOD(GetExtendedControl)(IDispatch** ppDisp);        
		STDMETHOD(TransformCoords)(POINTL* pPtlHimetric, POINTF* pPtfContainer, DWORD dwFlags);       
		STDMETHOD(TranslateAccelerator)(MSG* pMsg, DWORD grfModifiers);
		STDMETHOD(OnFocus)(BOOL fGotFocus);
		STDMETHOD(ShowPropertyFrame)(void);

		// IOleContainer
		STDMETHOD(EnumObjects)(DWORD grfFlags, IEnumUnknown** ppenum);
		STDMETHOD(LockContainer)(BOOL fLock);

		// IParseDisplayName
		STDMETHOD(ParseDisplayName)(IBindCtx* pbc, LPOLESTR pszDisplayName, ULONG* pchEaten, IMoniker** ppmkOut);

	protected:
		HRESULT CreateActiveXWnd(); 
	public:
		FActivex* m_pOwner;
	protected:
		LONG m_dwRef;
		
		FActivexWnd* m_pWindow;
		IUnknown* m_pUnkSite;
		IViewObject* m_pViewObject;
		IOleInPlaceObjectWindowless* m_pInPlaceObject;
		bool m_bLocked;
		bool m_bFocused;
		bool m_bCaptured;
		bool m_bUIActivated;
		bool m_bInPlaceActive;
		bool m_bWindowless;
		bool m_bDelayCreate;
	};
	static void PixelToHiMetric(const SIZEL* lpSizeInPix, LPSIZEL lpSizeInHiMetric);

}}
#endif