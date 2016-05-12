/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		F3rd_Pugixml_p.h
* 描述：		F3rd_Pugixml_p
* 创建日期：	2013-06-19
*
*/

#ifndef F3RD_PUGIXML_P_H
#define F3RD_PUGIXML_P_H

#include "F3rd_p.h"

#ifdef FOOUI_SUPPORT_PUGIXML
# ifdef _UNICODE
#	define PUGIXML_WCHAR_MODE
# endif
# include "markup/pugixml/pugiconfig.hpp"
# include "markup/pugixml/pugixml.hpp"
#endif // !FOOUI_SUPPORT_PUGIXML

#endif // !F3RD_PUGIXML_P_H