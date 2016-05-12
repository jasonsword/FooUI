/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FStyleDef.h
* 描述：		FStyleDef
* 创建日期：	2013-06-19
*
*/

#ifndef FSTYLEDEF_H
#define FSTYLEDEF_H

#include "core/FBase.h"
#include "core/FString.h"
#include "core/FTypes.h"

#include "markup/FMarkup.h"

#include "painting/FGraphics.h"

namespace FooUI { namespace Widgets { class FWidget; }}
namespace FooUI { namespace Markup { class FMarkupObject; }}

namespace FooUI { namespace Styles {

	typedef struct tagFSTYLESTRUCT
	{
		UINT uType;
	} FSTYLESTRUCT, *PFSTYLESTRUCT, far *LPFSTYLESTRUCT;

	typedef struct tagFHTMLSTYLESTRUCT
	{
		FSTYLESTRUCT fss;
		RECT rcHref[255];
		int numOfHref;
	} FHTMLSTYLESTRUCT, *PFHTMLSTYLESTRUCT, far *LPFHTMLSTYLESTRUCT;

	//////////////////////////////////////////////////////////////////////////
	// FStyleObject
	#define FOOUI_DECLARE_STYLETYPE(_type) \
	DWORD getStyleType(void) const \
	{ \
		return (_type); \
	}

	class FOOUI_LIB FStyleObject : public Markup::FMarkupObject
	{
	public:
		enum eFStyleType
		{
			UnknownStyle = 0, 
			IncludeStyle, 
			ColorStyle, 
			ImageStyle, 
			TextStyle, 
			StringStyle, 
			FocusRectStyle, 
			WindowRegionStyle, 
		};
		FStyleObject(void);
		virtual ~FStyleObject(void) { }
		virtual DWORD getStyleType(void) const = 0;

	public:
		LPCTSTR getName(void) const;
		LPCTSTR getType(void) const;
		LPCTSTR getPart(void) const;
		DWORD getState(void) const;
		DWORD getDock(void) const;
		DWORD getAlign(void) const;
		const Core::FEdge& getMargin(void) const;
		const Core::FSize& getSize(void) const;
		const Core::FPoint& getOffset(void) const;

	public:
		virtual bool draw(Widgets::FWidget* pWidget, /** 控件对象 */
						Painting::FGraphics* pGraphics, /** 画布 */
						const Core::FRect* prcUpdate, /** 更新区域 */
						DWORD dwState, /** 状态 */
						const Core::FRect& rcDest, /** 绘图区域 */
						LPCTSTR pszType = NULL, 
						LPCTSTR pszPart = NULL, 
						FSTYLESTRUCT* fss = NULL) = 0;
		/** 检查是否包含dwState1或dwState2的样式（重绘前检查，减少无用重绘） */
		virtual bool testStyle(DWORD dwState, LPCTSTR pszType = NULL, LPCTSTR pszPart = NULL) const;
	
	public:
		virtual bool beginMarkup(Markup::FMarkup* pMarkup) { return true; }
		virtual bool setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue);
		virtual bool endMarkup(Markup::FMarkup* pMarkup) { return true; }

	public:
		static FStyleObject* createStyle(LPCTSTR pszName);

	protected:
		/** 计算最佳输出 */
		void calcRect(Core::FRect& rcDraw, Core::FRect& rcClip, const Core::FRect& rcDest) const;

	private:
		Core::FString m_strName;
		Core::FString m_strType;
		Core::FString m_strPart;
		DWORD m_dwState;
		DWORD m_dwAlign;
		Core::FEdge m_egMargin;
		Core::FEdge m_egPadding;
		Core::FSize m_size;
		Core::FSize m_minsize;
		Core::FSize m_maxsize;
		Core::FPoint m_ptOffset;
	};
}} // !namespace

#endif // !FSTYLEDEF_H