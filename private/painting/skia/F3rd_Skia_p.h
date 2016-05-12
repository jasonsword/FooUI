/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		F3rd_p.h
* 描述：		F3rd_p
* 创建日期：	2013-06-06
*
*/

#ifndef F3RD_SKIA_P_H
#define F3RD_SKIA_P_H

#include "F3rd_p.h"

#ifdef FOOUI_SUPPORT_SKIA

# include "SkCanvas.h"
# include "SkBitmap.h"
# include "SkImageDecoder.h"
# include "SkStream.h"

# pragma comment(lib, "skia_core.lib")
# pragma comment(lib, "skia_opts.lib")
# pragma comment(lib, "skia_opts_ssse3.lib")
# pragma comment(lib, "skia_utils.lib")
# pragma comment(lib, "skia_effects.lib")
# pragma comment(lib, "skia_images.lib")
# pragma comment(lib, "skia_ports.lib")
# pragma comment(lib, "skia_gr.lib")
# pragma comment(lib, "skia_skgr.lib")
# pragma comment(lib, "skia_sfnt.lib")

# pragma comment(lib, "opengl32.lib")
# pragma comment(lib, "usp10.lib")
# pragma comment(lib, "windowscodecs.lib")

#endif // !FOOUI_SUPPORT_SKIA


#endif // !F3RD_SKIA_P_H