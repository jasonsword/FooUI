/**
* Copyright (C) 2015 FooUI
*
* 文件名：		FWebBrowser.cpp
* 描述：		Web 浏览器控件
* 创建日期：	2015-04-22
* author:		huangyihui
*/


//https://support2.microsoft.com/default.aspx?scid=kb;en-us;261003

#include "StdAfx.h"
#include "widgets/FWebBrowser.h"
#include <atlconv.h>
#include <atlcomcli.h>
#include <mshtml.h>
#include <exdispid.h>
#include "../util/downloadmgr.h"

namespace FooUI {namespace Widgets {
	
	FWebBrowser::FWebBrowser(FWidget* parent /*= NULL*/)
		:m_pWebBrowser2(NULL)
		, _pHtmlWnd2(NULL)
		, m_dwCookie(0)
		, m_pWebBrowserEventHandler(NULL)
		, m_dwRef(0)
		, m_bAutoNavi(false)
	{
		m_clsid = CLSID_WebBrowser;
		m_sHomePage.empty();
		//CreateControl(L"{8856F961-340A-11D0-A96B-00C04FD705A2}");
	}
	FWebBrowser::~FWebBrowser(void)
	{

	}

	bool FWebBrowser::DoCreateControl()
	{
		if (!FActivex::DoCreateControl())
			return false;
		GetControl(IID_IWebBrowser2, (LPVOID*)&m_pWebBrowser2);
		this->Navigate2(L"about:blank"); //这行代码，如果注释掉，就不会去掉边框，IE有bug，第二次加载网页才会让事件处理器有效
		if (m_bAutoNavi && !m_sHomePage.isEmpty())
		{
			this->Navigate2(m_sHomePage);
		}
		RegisterEventHandler(TRUE);


		return true;

	}
	void FWebBrowser::ReleaseControl()
	{
		RegisterEventHandler(FALSE);
	}
	void FWebBrowser::SetHomePage(LPCTSTR lpszUrl)
	{
		m_sHomePage.format(_T("%s"), lpszUrl);
	}

	LPCTSTR FWebBrowser::GetHomePage()
	{
		return m_sHomePage;
	}

	void FWebBrowser::SetAutoNavigation(bool bAuto /*= TRUE*/)
	{
		if (m_bAutoNavi == bAuto)	return;

		m_bAutoNavi = bAuto;
	}

	bool FWebBrowser::IsAutoNavigation()
	{
		return m_bAutoNavi;
	}

	void FWebBrowser::SetWebBrowserEventHandler(FWebBrowserEventHandler* pEventHandler)
	{
		if (pEventHandler != NULL && m_pWebBrowserEventHandler != pEventHandler)
			m_pWebBrowserEventHandler = pEventHandler;
		if (!m_bDelayCreate)
		{
			DoCreateControl();
		}
	}


	void FWebBrowser::Navigate2(LPCTSTR lpszUrl)
	{
		if (lpszUrl == NULL)
			return;
		if (m_pWebBrowser2)
		{
			VARIANT url;
			url.vt = VT_BSTR;
			url.bstrVal = T2BSTR(lpszUrl);
			HRESULT hr = m_pWebBrowser2->Navigate2(&url, NULL, NULL, NULL, NULL);
		}
	}

	void FWebBrowser::Refresh()
	{
		if (m_pWebBrowser2)
		{
			m_pWebBrowser2->Refresh();
		}
	}
	void FWebBrowser::GoBack()
	{
		if (m_pWebBrowser2)
		{
			m_pWebBrowser2->GoBack();
		}
	}
	void FWebBrowser::GoForward()
	{
		if (m_pWebBrowser2)
		{
			m_pWebBrowser2->GoForward();
		}
	}

	DISPID FWebBrowser::FindId(IDispatch *pObj, LPOLESTR pName)
	{
		DISPID id = 0;
		if (FAILED(pObj->GetIDsOfNames(IID_NULL, &pName, 1, LOCALE_SYSTEM_DEFAULT, &id))) id = -1;
		return id;
	}

	HRESULT FWebBrowser::InvokeMethod(IDispatch *pObj, LPOLESTR pMehtod, VARIANT *pVarResult, VARIANT *ps, int cArgs)
	{
		DISPID dispid = FindId(pObj, pMehtod);
		if (dispid == -1) return E_FAIL;

		DISPPARAMS dispparams;
		dispparams.cArgs = cArgs;
		dispparams.rgvarg = ps;
		dispparams.cNamedArgs = 0;
		dispparams.rgdispidNamedArgs = NULL;

		return pObj->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispparams, pVarResult, NULL, NULL);
	}
	HRESULT FWebBrowser::GetProperty(IDispatch *pObj, LPOLESTR pName, VARIANT *pValue)
	{
		DISPID dispid = FindId(pObj, pName);
		if (dispid == -1) return E_FAIL;

		DISPPARAMS ps;
		ps.cArgs = 0;
		ps.rgvarg = NULL;
		ps.cNamedArgs = 0;
		ps.rgdispidNamedArgs = NULL;

		return pObj->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &ps, pValue, NULL, NULL);
	}

	HRESULT FWebBrowser::SetProperty(IDispatch *pObj, LPOLESTR pName, VARIANT *pValue)
	{
		DISPID dispid = FindId(pObj, pName);
		if (dispid == -1) return E_FAIL;

		DISPPARAMS ps;
		ps.cArgs = 1;
		ps.rgvarg = pValue;
		ps.cNamedArgs = 0;
		ps.rgdispidNamedArgs = NULL;

		return pObj->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUT, &ps, NULL, NULL, NULL);
	}

	void FWebBrowser::NavigateHomePage()
	{
		if (!m_sHomePage.isEmpty())
			this->NavigateUrl(m_sHomePage);
	}

	void FWebBrowser::NavigateUrl(LPCTSTR lpszUrl)
	{
		if (m_pWebBrowser2 && lpszUrl)
		{
			m_pWebBrowser2->Navigate((BSTR)SysAllocString(T2BSTR(lpszUrl)), NULL, NULL, NULL, NULL);
		}
	}


	HRESULT FWebBrowser::RegisterEventHandler(BOOL inAdvise)
	{
		CComPtr<IWebBrowser2> pWebBrowser;
		CComPtr<IConnectionPointContainer>  pCPC;
		CComPtr<IConnectionPoint> pCP;
		CComPtr<IDocHostUIHandler> pDHUH;
		HRESULT hr = GetControl(IID_IWebBrowser2, (void**)&pWebBrowser);
		if (FAILED(hr))
			return hr;
		hr = pWebBrowser->QueryInterface(IID_IConnectionPointContainer, (void **)&pCPC);
		if (FAILED(hr))
			return hr;
		hr = pCPC->FindConnectionPoint(DIID_DWebBrowserEvents2, &pCP);
		if (FAILED(hr))
			return hr;
	
		if (inAdvise)
		{
			hr = pCP->Advise((IDispatch*)this, &m_dwCookie);
		}
		else
		{
			hr = pCP->Unadvise(m_dwCookie);
		}
		if (FAILED(hr))
			return hr;
		return hr;
	}





#pragma region DWebBrowserEvents2
	/// DWebBrowserEvents2
	void FWebBrowser::BeforeNavigate2(IDispatch *pDisp, VARIANT *&url, VARIANT *&Flags, VARIANT *&TargetFrameName, VARIANT *&PostData, VARIANT *&Headers, VARIANT_BOOL *&Cancel)
	{
		CComPtr<IDispatch> spDoc;
		m_pWebBrowser2->get_Document(&spDoc);

		if (spDoc)
		{
			CComQIPtr<ICustomDoc, &IID_ICustomDoc> spCustomDoc(spDoc);
			if (spCustomDoc)
				spCustomDoc->SetUIHandler(this);
		}

		if (m_pWebBrowserEventHandler)
		{
			m_pWebBrowserEventHandler->BeforeNavigate2(pDisp, url, Flags, TargetFrameName, PostData, Headers, Cancel);
		}
	}

	void FWebBrowser::NavigateError(IDispatch *pDisp, VARIANT * &url, VARIANT *&TargetFrameName, VARIANT *&StatusCode, VARIANT_BOOL *&Cancel)
	{
		if (m_pWebBrowserEventHandler)
		{
			m_pWebBrowserEventHandler->NavigateError(pDisp, url, TargetFrameName, StatusCode, Cancel);
		}
	}

	void FWebBrowser::NavigateComplete2(IDispatch *pDisp, VARIANT *&url)
	{
	
		CComPtr<IDispatch> spDoc;
		m_pWebBrowser2->get_Document(&spDoc);

		if (spDoc)
		{
			CComQIPtr<ICustomDoc, &IID_ICustomDoc> spCustomDoc(spDoc);
			if (spCustomDoc)
				spCustomDoc->SetUIHandler(this);
		}

		if (m_pWebBrowserEventHandler)
		{
			m_pWebBrowserEventHandler->NavigateComplete2(pDisp, url);
		}
		
	}

	void FWebBrowser::ProgressChange(LONG nProgress, LONG nProgressMax)
	{
		if (m_pWebBrowserEventHandler)
		{
			m_pWebBrowserEventHandler->ProgressChange(nProgress, nProgressMax);
		}
	}

	void FWebBrowser::NewWindow3(IDispatch **pDisp, VARIANT_BOOL *&Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl)
	{
		if (m_pWebBrowserEventHandler)
		{
			m_pWebBrowserEventHandler->NewWindow3(pDisp, Cancel, dwFlags, bstrUrlContext, bstrUrl);
		}
	}
	void FWebBrowser::CommandStateChange(long Command, VARIANT_BOOL Enable)
	{
		if (m_pWebBrowserEventHandler)
		{
			m_pWebBrowserEventHandler->CommandStateChange(Command, Enable);
		}
	}

#pragma endregion

#pragma region IUnknown
	// IUnknown
	STDMETHODIMP FWebBrowser::QueryInterface(REFIID riid, LPVOID *ppvObject)
	{
		*ppvObject = NULL;

		if (riid == IID_IDocHostUIHandler)
			*ppvObject = static_cast<IDocHostUIHandler*>(this);
		else if (riid == IID_IDispatch)
			*ppvObject = static_cast<IDispatch*>(this);
		else if (riid == IID_IServiceProvider)
			*ppvObject = static_cast<IServiceProvider*>(this);
		else if (riid == IID_IOleCommandTarget)
			*ppvObject = static_cast<IOleCommandTarget*>(this);
		if (*ppvObject != NULL)
			AddRef();
		return *ppvObject == NULL ? E_NOINTERFACE : S_OK;
	}

	STDMETHODIMP_(ULONG) FWebBrowser::AddRef()
	{
		InterlockedIncrement(&m_dwRef);
		return m_dwRef;
	}

	STDMETHODIMP_(ULONG) FWebBrowser::Release()
	{
		ULONG ulRefCount = InterlockedDecrement(&m_dwRef);
		return ulRefCount;
	}
#pragma endregion

#pragma region IDispatch
	// IDispatch
	STDMETHODIMP FWebBrowser::GetTypeInfoCount(UINT *iTInfo)
	{
		*iTInfo = 0;
		return E_NOTIMPL;
	}

	STDMETHODIMP FWebBrowser::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP FWebBrowser::GetIDsOfNames(REFIID riid, OLECHAR **rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP FWebBrowser::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr)
	{
		if ((riid != IID_NULL))
			return E_INVALIDARG;

		switch (dispIdMember)
		{
		case DISPID_BEFORENAVIGATE2:
			BeforeNavigate2(
				pDispParams->rgvarg[6].pdispVal,
				pDispParams->rgvarg[5].pvarVal,
				pDispParams->rgvarg[4].pvarVal,
				pDispParams->rgvarg[3].pvarVal,
				pDispParams->rgvarg[2].pvarVal,
				pDispParams->rgvarg[1].pvarVal,
				pDispParams->rgvarg[0].pboolVal);
			break;
		case DISPID_COMMANDSTATECHANGE:
			CommandStateChange(
				pDispParams->rgvarg[1].lVal,
				pDispParams->rgvarg[0].boolVal);
			break;
		case DISPID_NAVIGATECOMPLETE2:
			NavigateComplete2(
				pDispParams->rgvarg[1].pdispVal,
				pDispParams->rgvarg[0].pvarVal);
			break;
		case DISPID_NAVIGATEERROR:
			NavigateError(
				pDispParams->rgvarg[4].pdispVal,
				pDispParams->rgvarg[3].pvarVal,
				pDispParams->rgvarg[2].pvarVal,
				pDispParams->rgvarg[1].pvarVal,
				pDispParams->rgvarg[0].pboolVal);
			break;
		case DISPID_STATUSTEXTCHANGE:
			break;
			//  	case DISPID_NEWWINDOW2:
			//  		break;
		case DISPID_NEWWINDOW3:
			NewWindow3(
				pDispParams->rgvarg[4].ppdispVal,
				pDispParams->rgvarg[3].pboolVal,
				pDispParams->rgvarg[2].uintVal,
				pDispParams->rgvarg[1].bstrVal,
				pDispParams->rgvarg[0].bstrVal);
			break;
			// 	case DISPID_PROPERTYCHANGE:
			// 		if (pDispParams->cArgs>0 && pDispParams->rgvarg[0].vt == VT_BSTR) {
			// 			TRACE(_T("PropertyChange(%s)\n"), pDispParams->rgvarg[0].bstrVal);
			// 		}
			// 		break;
		default:
			return DISP_E_MEMBERNOTFOUND;
		}
		return S_OK;
	}
#pragma endregion

#pragma region IDownloadManager
	// IDownloadManager
	STDMETHODIMP FWebBrowser::Download( /* [in] */ IMoniker *pmk, /* [in] */ IBindCtx *pbc, /* [in] */ DWORD dwBindVerb, /* [in] */ LONG grfBINDF, /* [in] */ BINDINFO *pBindInfo, /* [in] */ LPCOLESTR pszHeaders, /* [in] */ LPCOLESTR pszRedir, /* [in] */ UINT uiCP)
	{
		if (m_pWebBrowserEventHandler)
		{
			return m_pWebBrowserEventHandler->Download(pmk, pbc, dwBindVerb, grfBINDF, pBindInfo, pszHeaders, pszRedir, uiCP);
		}
		return S_OK;
	}
#pragma endregion

#pragma region IDocHostUIHandler
	// IDocHostUIHandler
	STDMETHODIMP FWebBrowser::ShowContextMenu(DWORD dwID, POINT* pptPosition, IUnknown* pCommandTarget, IDispatch* pDispatchObjectHit)
	{
		if (m_pWebBrowserEventHandler)
		{
			return m_pWebBrowserEventHandler->ShowContextMenu(dwID, pptPosition, pCommandTarget, pDispatchObjectHit);
		}
		return S_FALSE;
	}

	STDMETHODIMP FWebBrowser::GetHostInfo(DOCHOSTUIINFO* pInfo)
	{
		if (m_pWebBrowserEventHandler != NULL)
		{
			return m_pWebBrowserEventHandler->GetHostInfo(pInfo);
		}
		return E_NOTIMPL;
	}

	STDMETHODIMP FWebBrowser::ShowUI(DWORD dwID, IOleInPlaceActiveObject* pActiveObject, IOleCommandTarget* pCommandTarget, IOleInPlaceFrame* pFrame, IOleInPlaceUIWindow* pDoc)
	{
		if (m_pWebBrowserEventHandler)
		{
			return m_pWebBrowserEventHandler->ShowUI(dwID, pActiveObject, pCommandTarget, pFrame, pDoc);
		}
		return S_OK;
	}

	STDMETHODIMP FWebBrowser::HideUI()
	{
		if (m_pWebBrowserEventHandler)
		{
			return m_pWebBrowserEventHandler->HideUI();
		}
		return S_OK;
	}

	STDMETHODIMP FWebBrowser::UpdateUI()
	{
		if (m_pWebBrowserEventHandler)
		{
			return m_pWebBrowserEventHandler->UpdateUI();
		}
		return S_OK;
	}

	STDMETHODIMP FWebBrowser::EnableModeless(BOOL fEnable)
	{
		if (m_pWebBrowserEventHandler)
		{
			return m_pWebBrowserEventHandler->EnableModeless(fEnable);
		}
		return E_NOTIMPL;
	}

	STDMETHODIMP FWebBrowser::OnDocWindowActivate(BOOL fActivate)
	{
		if (m_pWebBrowserEventHandler)
		{
			return m_pWebBrowserEventHandler->OnDocWindowActivate(fActivate);
		}
		return E_NOTIMPL;
	}

	STDMETHODIMP FWebBrowser::OnFrameWindowActivate(BOOL fActivate)
	{
		if (m_pWebBrowserEventHandler)
		{
			return m_pWebBrowserEventHandler->OnFrameWindowActivate(fActivate);
		}
		return E_NOTIMPL;
	}

	STDMETHODIMP FWebBrowser::ResizeBorder(LPCRECT prcBorder, IOleInPlaceUIWindow* pUIWindow, BOOL fFrameWindow)
	{
		if (m_pWebBrowserEventHandler)
		{
			return m_pWebBrowserEventHandler->ResizeBorder(prcBorder, pUIWindow, fFrameWindow);
		}
		return E_NOTIMPL;
	}

	STDMETHODIMP FWebBrowser::TranslateAccelerator(LPMSG lpMsg, const GUID* pguidCmdGroup, DWORD nCmdID)
	{
		if (m_pWebBrowserEventHandler)
		{
			return m_pWebBrowserEventHandler->TranslateAccelerator(lpMsg, pguidCmdGroup, nCmdID);
		}
		return S_FALSE;
	}

	STDMETHODIMP FWebBrowser::GetOptionKeyPath(LPOLESTR* pchKey, DWORD dwReserved)
	{
		if (m_pWebBrowserEventHandler)
		{
			return m_pWebBrowserEventHandler->GetOptionKeyPath(pchKey, dwReserved);
		}
		return E_NOTIMPL;
	}

	STDMETHODIMP FWebBrowser::GetDropTarget(IDropTarget* pDropTarget, IDropTarget** ppDropTarget)
	{
		if (m_pWebBrowserEventHandler)
		{
			return m_pWebBrowserEventHandler->GetDropTarget(pDropTarget, ppDropTarget);
		}
		return S_FALSE;	// 使用系统拖拽
	}

	STDMETHODIMP FWebBrowser::GetExternal(IDispatch** ppDispatch)
	{
		if (m_pWebBrowserEventHandler)
		{
			return m_pWebBrowserEventHandler->GetExternal(ppDispatch);
		}
		return S_FALSE;
	}

	STDMETHODIMP FWebBrowser::TranslateUrl(DWORD dwTranslate, OLECHAR* pchURLIn, OLECHAR** ppchURLOut)
	{
		if (m_pWebBrowserEventHandler)
		{
			return m_pWebBrowserEventHandler->TranslateUrl(dwTranslate, pchURLIn, ppchURLOut);
		}
		else
		{
			*ppchURLOut = 0;
			return E_NOTIMPL;
		}
	}

	STDMETHODIMP FWebBrowser::FilterDataObject(IDataObject* pDO, IDataObject** ppDORet)
	{
		if (m_pWebBrowserEventHandler)
		{
			return m_pWebBrowserEventHandler->FilterDataObject(pDO, ppDORet);
		}
		else
		{
			*ppDORet = 0;
			return E_NOTIMPL;
		}
	}
#pragma endregion

#pragma region IOleCommandTarget
	// IOleCommandTarget
	HRESULT STDMETHODCALLTYPE FWebBrowser::QueryStatus(__RPC__in_opt const GUID *pguidCmdGroup, ULONG cCmds, __RPC__inout_ecount_full(cCmds) OLECMD prgCmds[], __RPC__inout_opt OLECMDTEXT *pCmdText)
	{
		HRESULT hr = OLECMDERR_E_NOTSUPPORTED;
		return hr;
	}

	HRESULT STDMETHODCALLTYPE FWebBrowser::Exec(__RPC__in_opt const GUID *pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, __RPC__in_opt VARIANT *pvaIn, __RPC__inout_opt VARIANT *pvaOut)
	{
		HRESULT hr = S_OK;
		if (pguidCmdGroup && IsEqualGUID(*pguidCmdGroup, CGID_DocHostCommandHandler))
		{

			switch (nCmdID)
			{

			case OLECMDID_SHOWSCRIPTERROR:
			{
					IHTMLDocument2*             pDoc = NULL;
					IHTMLWindow2*               pWindow = NULL;
					IHTMLEventObj*              pEventObj = NULL;
					BSTR                        rgwszNames[5] =
					{
						SysAllocString(L"errorLine"),
						SysAllocString(L"errorCharacter"),
						SysAllocString(L"errorCode"),
						SysAllocString(L"errorMessage"),
						SysAllocString(L"errorUrl")
					};
					DISPID                      rgDispIDs[5];
					VARIANT                     rgvaEventInfo[5];
					DISPPARAMS                  params;
					BOOL                        fContinueRunningScripts = false;
					int                         i;

					params.cArgs = 0;
					params.cNamedArgs = 0;

					// Get the document that is currently being viewed.
					hr = pvaIn->punkVal->QueryInterface(IID_IHTMLDocument2, (void **)&pDoc);
					// Get document.parentWindow.
					hr = pDoc->get_parentWindow(&pWindow);
					pDoc->Release();
					// Get the window.event object.
					hr = pWindow->get_event(&pEventObj);
					// Get the error info from the window.event object.
					for (i = 0; i < 5; i++)
					{
						// Get the property's dispID.
						hr = pEventObj->GetIDsOfNames(IID_NULL, &rgwszNames[i], 1,
							LOCALE_SYSTEM_DEFAULT, &rgDispIDs[i]);
						// Get the value of the property.
						hr = pEventObj->Invoke(rgDispIDs[i], IID_NULL,
							LOCALE_SYSTEM_DEFAULT,
							DISPATCH_PROPERTYGET, &params, &rgvaEventInfo[i],
							NULL, NULL);
						SysFreeString(rgwszNames[i]);
					}

					// At this point, you would normally alert the user with 
					// the information about the error, which is now contained
					// in rgvaEventInfo[]. Or, you could just exit silently.

					(*pvaOut).vt = VT_BOOL;
					if (fContinueRunningScripts)
					{
						// Continue running scripts on the page.
						(*pvaOut).boolVal = VARIANT_TRUE;
					}
					else
					{
						// Stop running scripts on the page.
						(*pvaOut).boolVal = VARIANT_FALSE;
					}
					break;
			}
			default:
				hr = OLECMDERR_E_NOTSUPPORTED;
				break;
			}
		}
		else
		{
			hr = OLECMDERR_E_UNKNOWNGROUP;
		}
		return (hr);
	}
#pragma endregion


#pragma region IServiceProvider

	// IServiceProvider
	/* [local] */ HRESULT STDMETHODCALLTYPE FWebBrowser::QueryService(
		/* [in] */ __in REFGUID guidService,
		/* [in] */ __in REFIID riid,
		/* [out] */ __deref_out void __RPC_FAR *__RPC_FAR *ppvObject)
	{

#if 0
		*ppvObject = NULL;
		HRESULT hr = E_NOINTERFACE;
		if (IsEqualGUID(guidService, SID_SInternetSecurityManager) && IsEqualGUID(riid, IID_IInternetSecurityManager))
		{
			hr = QueryInterface(riid, ppvObject);
		}
		else if (guidService == IID_INewWindowManager && riid == IID_INewWindowManager)
		{
			hr = QueryInterface(riid, ppvObject);
		}
		else if (guidService == IID_IDownloadManager && riid == IID_IDownloadManager)
		{
			hr = QueryInterface(riid, ppvObject);
		}
		else
		{
			*ppvObject = NULL;
		}
		return hr;
#else
		HRESULT hr = E_NOINTERFACE;
		*ppvObject = NULL;

		if (guidService == SID_SDownloadManager && riid == IID_IDownloadManager)
		{
			*ppvObject = this;
			return S_OK;
		}

		return hr;
#endif
	
	}
#pragma endregion

	
}}