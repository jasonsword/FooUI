#ifndef _FWEBBROWSER_H_
#define _FWEBBROWSER_H_

#include <Mshtml.h>
#include <MsHtmdid.h>
#include <MsHtmHst.h>
#include <ShObjIdl.h>
#include <ExDisp.h>

#include "core/FBase.h"
#include "core/FString.h"

#include "widgets/FWidget.h"
#include "widgets/FActivex.h"

namespace FooUI {
	namespace Widgets {
		class FWebBrowserEventHandler
		{
		public:
			FWebBrowserEventHandler() {}
			~FWebBrowserEventHandler() {}

			virtual void BeforeNavigate2(IDispatch *pDisp, VARIANT *&url, VARIANT *&Flags, VARIANT *&TargetFrameName, VARIANT *&PostData, VARIANT *&Headers, VARIANT_BOOL *&Cancel) {}
			virtual void NavigateError(IDispatch *pDisp, VARIANT * &url, VARIANT *&TargetFrameName, VARIANT *&StatusCode, VARIANT_BOOL *&Cancel) {}
			virtual void NavigateComplete2(IDispatch *pDisp, VARIANT *&url){}
			virtual void ProgressChange(LONG nProgress, LONG nProgressMax){}
			virtual void NewWindow3(IDispatch **pDisp, VARIANT_BOOL *&Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl){}
			virtual void CommandStateChange(long Command, VARIANT_BOOL Enable){};

			// interface IDocHostUIHandler
			virtual HRESULT STDMETHODCALLTYPE ShowContextMenu(
				/* [in] */ DWORD dwID,
				/* [in] */ POINT __RPC_FAR *ppt,
				/* [in] */ IUnknown __RPC_FAR *pcmdtReserved,
				/* [in] */ IDispatch __RPC_FAR *pdispReserved)
			{
				//return E_NOTIMPL;
				//返回 E_NOTIMPL 正常弹出系统右键菜单
				return S_OK;
				//返回S_OK 则可屏蔽系统右键菜单
			}

			virtual HRESULT STDMETHODCALLTYPE GetHostInfo(
				/* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo)
			{
				if (pInfo != NULL)
				{
					//网页无边框：DOCHOSTUIFLAG_NO3DBORDER
//					pInfo->dwFlags |= DOCHOSTUIFLAG_NO3DBORDER;
					try{
						//网页无边框：DOCHOSTUIFLAG_NO3DBORDER
						// DOCHOSTUIFLAG_SCROLL_NO ---- hidden the scroll
						pInfo->dwFlags = DOCHOSTUIFLAG_SCROLL_NO | DOCHOSTUIFLAG_ENABLE_FORMS_AUTOCOMPLETE | DOCHOSTUIFLAG_FLAT_SCROLLBAR | DOCHOSTUIFLAG_NO3DBORDER;// | DOCHOSTUIFLAG_CODEPAGELINKEDFONTS|DOCHOSTUIFLAG_NO3DBORDER;
						//pInfo->dwFlags = DOCHOSTUIFLAG_NO3DBORDER |DOCHOSTUIFLAG_SCROLL_NO;
						pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
						
					}
					catch (...)
					{
					}
				}
				return S_OK;
			}

			virtual HRESULT STDMETHODCALLTYPE ShowUI(
				/* [in] */ DWORD dwID,
				/* [in] */ IOleInPlaceActiveObject __RPC_FAR *pActiveObject,
				/* [in] */ IOleCommandTarget __RPC_FAR *pCommandTarget,
				/* [in] */ IOleInPlaceFrame __RPC_FAR *pFrame,
				/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pDoc)
			{
				return S_FALSE;
			}

			virtual HRESULT STDMETHODCALLTYPE HideUI(void)
			{
				return S_OK;
			}

			virtual HRESULT STDMETHODCALLTYPE UpdateUI(void)
			{
				return S_OK;
			}

			virtual HRESULT STDMETHODCALLTYPE EnableModeless(
				/* [in] */ BOOL fEnable)
			{
				return S_OK;
			}

			virtual HRESULT STDMETHODCALLTYPE OnDocWindowActivate(
				/* [in] */ BOOL fActivate)
			{
				return S_OK;
			}

			virtual HRESULT STDMETHODCALLTYPE OnFrameWindowActivate(
				/* [in] */ BOOL fActivate)
			{
				return S_OK;
			}

			virtual HRESULT STDMETHODCALLTYPE ResizeBorder(
				/* [in] */ LPCRECT prcBorder,
				/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pUIWindow,
				/* [in] */ BOOL fRameWindow)
			{
				return S_OK;
			}

			virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator(
				/* [in] */ LPMSG lpMsg,
				/* [in] */ const GUID __RPC_FAR *pguidCmdGroup,
				/* [in] */ DWORD nCmdID)
			{
				return S_FALSE;
			}

			virtual HRESULT STDMETHODCALLTYPE GetOptionKeyPath(
				/* [out] */ LPOLESTR __RPC_FAR *pchKey,
				/* [in] */ DWORD dw)
			{
				return S_OK;
			}

			virtual HRESULT STDMETHODCALLTYPE GetDropTarget(
				/* [in] */ IDropTarget __RPC_FAR *pDropTarget,
				/* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget)
			{
				return E_NOTIMPL;
			}

			virtual HRESULT STDMETHODCALLTYPE GetExternal(
				/* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch)
			{
				return E_NOTIMPL;
			}

			virtual HRESULT STDMETHODCALLTYPE TranslateUrl(
				/* [in] */ DWORD dwTranslate,
				/* [in] */ OLECHAR __RPC_FAR *pchURLIn,
				/* [out] */ OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut)
			{
				return S_OK;
			}

			virtual HRESULT STDMETHODCALLTYPE FilterDataObject(
				/* [in] */ IDataObject __RPC_FAR *pDO,
				/* [out] */ IDataObject __RPC_FAR *__RPC_FAR *ppDORet)
			{
				return S_OK;
			}

			virtual HRESULT STDMETHODCALLTYPE GetOverrideKeyPath( 
				/* [annotation][out] */ 
				__deref_out  LPOLESTR *pchKey,
				/* [in] */ DWORD dw)
			{
				return E_NOTIMPL;
			}

			// IDownloadManager
			virtual HRESULT STDMETHODCALLTYPE Download(
				/* [in] */ IMoniker *pmk,
				/* [in] */ IBindCtx *pbc,
				/* [in] */ DWORD dwBindVerb,
				/* [in] */ LONG grfBINDF,
				/* [in] */ BINDINFO *pBindInfo,
				/* [in] */ LPCOLESTR pszHeaders,
				/* [in] */ LPCOLESTR pszRedir,
				/* [in] */ UINT uiCP)
			{
				return S_OK;
			}
		};
		

		//////////////////////////////////////////////////////////////////////////
		// FWebBrowser
		#define FOOWC_WEBBROWSER _T("webbrowser")
		class FOOUI_LIB FWebBrowser 
			: public FActivex
			, public IDocHostUIHandler
			, public IOleCommandTarget
			, public IServiceProvider
			, public IDispatch
		{
		public:
			explicit FWebBrowser(FWidget* parent = NULL);
			~FWebBrowser(void);

			void SetHomePage(LPCTSTR lpszUrl);
			LPCTSTR GetHomePage();

			void SetAutoNavigation(bool bAuto = TRUE);
			bool IsAutoNavigation();

			void SetWebBrowserEventHandler(FWebBrowserEventHandler* pEventHandler);
			void Navigate2(LPCTSTR lpszUrl);
			void Refresh();
			void Refresh2(int Level);
			void GoBack();
			void GoForward();
			void NavigateHomePage();
			void NavigateUrl(LPCTSTR lpszUrl);

			virtual bool DoCreateControl();
			HRESULT RegisterEventHandler(BOOL inAdvise);

			IWebBrowser2* GetWebBrowser2(void);
			IDispatch*		   GetHtmlWindow();

			static DISPID FindId(IDispatch *pObj, LPOLESTR pName);
			static HRESULT InvokeMethod(IDispatch *pObj, LPOLESTR pMehtod, VARIANT *pVarResult, VARIANT *ps, int cArgs);
			static HRESULT GetProperty(IDispatch *pObj, LPOLESTR pName, VARIANT *pValue);
			static HRESULT SetProperty(IDispatch *pObj, LPOLESTR pName, VARIANT *pValue);
			FOODECLARE_CLASS(FOOWC_WEBBROWSER)
			FOODECLARE_CONTAINER(true)

		protected:
			IWebBrowser2*			m_pWebBrowser2; //浏览器指针
			IHTMLWindow2*			_pHtmlWnd2;
			LONG m_dwRef;
			DWORD m_dwCookie;

			FWebBrowserEventHandler* m_pWebBrowserEventHandler;	//浏览器事件处理

			virtual void ReleaseControl();
			// DWebBrowserEvents2
			void BeforeNavigate2(IDispatch *pDisp, VARIANT *&url, VARIANT *&Flags, VARIANT *&TargetFrameName, VARIANT *&PostData, VARIANT *&Headers, VARIANT_BOOL *&Cancel);
			void NavigateError(IDispatch *pDisp, VARIANT * &url, VARIANT *&TargetFrameName, VARIANT *&StatusCode, VARIANT_BOOL *&Cancel);
			void NavigateComplete2(IDispatch *pDisp, VARIANT *&url);
			void ProgressChange(LONG nProgress, LONG nProgressMax);
			void NewWindow3(IDispatch **pDisp, VARIANT_BOOL *&Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl);
			void CommandStateChange(long Command, VARIANT_BOOL Enable);
		public:
			// IUnknown
			STDMETHOD_(ULONG, AddRef)();
			STDMETHOD_(ULONG, Release)();
			STDMETHOD(QueryInterface)(REFIID riid, LPVOID *ppvObject);


			// IDispatch
			virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(__RPC__out UINT *pctinfo);
			virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID lcid, __RPC__deref_out_opt ITypeInfo **ppTInfo);
			virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(__RPC__in REFIID riid, __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames, UINT cNames, LCID lcid, __RPC__out_ecount_full(cNames) DISPID *rgDispId);
			virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr);

			// IDocHostUIHandler
			STDMETHOD(ShowContextMenu)(DWORD dwID, POINT* pptPosition, IUnknown* pCommandTarget, IDispatch* pDispatchObjectHit);
			STDMETHOD(GetHostInfo)(DOCHOSTUIINFO* pInfo);
			STDMETHOD(ShowUI)(DWORD dwID, IOleInPlaceActiveObject* pActiveObject, IOleCommandTarget* pCommandTarget, IOleInPlaceFrame* pFrame, IOleInPlaceUIWindow* pDoc);
			STDMETHOD(HideUI)();
			STDMETHOD(UpdateUI)();
			STDMETHOD(EnableModeless)(BOOL fEnable);
			STDMETHOD(OnDocWindowActivate)(BOOL fActivate);
			STDMETHOD(OnFrameWindowActivate)(BOOL fActivate);
			STDMETHOD(ResizeBorder)(LPCRECT prcBorder, IOleInPlaceUIWindow* pUIWindow, BOOL fFrameWindow);
			STDMETHOD(TranslateAccelerator)(LPMSG lpMsg, const GUID* pguidCmdGroup, DWORD nCmdID);	//浏览器消息过滤
			STDMETHOD(GetOptionKeyPath)(LPOLESTR* pchKey, DWORD dwReserved);
			STDMETHOD(GetDropTarget)(IDropTarget* pDropTarget, IDropTarget** ppDropTarget);
			STDMETHOD(GetExternal)(IDispatch** ppDispatch);
			STDMETHOD(TranslateUrl)(DWORD dwTranslate, OLECHAR* pchURLIn, OLECHAR** ppchURLOut);
			STDMETHOD(FilterDataObject)(IDataObject* pDO, IDataObject** ppDORet);

			// IOleCommandTarget
			virtual HRESULT STDMETHODCALLTYPE QueryStatus(__RPC__in_opt const GUID *pguidCmdGroup, ULONG cCmds, __RPC__inout_ecount_full(cCmds) OLECMD prgCmds[], __RPC__inout_opt OLECMDTEXT *pCmdText);
			virtual HRESULT STDMETHODCALLTYPE Exec(__RPC__in_opt const GUID *pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, __RPC__in_opt VARIANT *pvaIn, __RPC__inout_opt VARIANT *pvaOut);

			// IDownloadManager
			STDMETHOD(Download)(
				/* [in] */ IMoniker *pmk,
				/* [in] */ IBindCtx *pbc,
				/* [in] */ DWORD dwBindVerb,
				/* [in] */ LONG grfBINDF,
				/* [in] */ BINDINFO *pBindInfo,
				/* [in] */ LPCOLESTR pszHeaders,
				/* [in] */ LPCOLESTR pszRedir,
				/* [in] */ UINT uiCP);

			//IServiceProvider
			virtual HRESULT STDMETHODCALLTYPE QueryService(
				/* [in] */ __in REFGUID guidService,
				/* [in] */ __in REFIID riid,
				/* [out] */ __deref_out void __RPC_FAR *__RPC_FAR *ppvObject);
	
		private:
			bool m_bAutoNavi;
			Core::FString m_sHomePage;
		};


	}
}


#endif // _FWEBBROWSER_H_
