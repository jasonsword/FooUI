/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		F3rd_p.h
* 描述：		F3rd_p
* 创建日期：	2013-06-06
*
*/

#ifndef F3RD_P_H
#define F3RD_P_H

#include "config/FConfig.h"

//////////////////////////////////////////////////////////////////////////
// markup
//#define FOOUI_SUPPORT_PUGIXML

#define FOOUI_SUPPORT_PUGIXML

#ifdef FOOUI_SUPPORT_PUGIXML
# include "markup/pugixml/F3rd_Pugixml_p.h"
# include "markup/pugixml/FMarkupParser_Pugixml_p.h"
#endif // !FOOUI_SUPPORT_PUGIXML

//////////////////////////////////////////////////////////////////////////
// patting
//#define FOOUI_SUPPORT_GDI		/** 未完善（除图像外不支持alpha通道） */
//#define FOOUI_SUPPORT_GDIP	/** 未支持 */
//#define FOOUI_SUPPORT_OPENGL	/** 未支持 */
//#define FOOUI_SUPPORT_SKIA	/** 支持 */
//#define FOOUI_SUPPORT_AGG		/** 未支持 */
//#define FOOUI_SUPPORT_CAIRO	/** 未支持 */

#if defined FOOUI_SUPPORT_GDI
# include "painting/gdi/F3rd_gdi_p.h"
# include "painting/gdi/FFont_gdi_p.h"
# include "painting/gdi/FImage_gdi_p.h"
# include "painting/gdi/FGraphics_gdi_p.h"
#elif defined FOOUI_SUPPORT_SKIA
# include "painting/skia/F3rd_Skia_p.h"
# include "painting/skia/FFont_Skia_p.h"
# include "painting/skia/FImage_Skia_p.h"
# include "painting/skia/FGraphics_skia_p.h"
#endif // !

#endif // !F3RD_P_H