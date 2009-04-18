/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

// StableHeaders.h : included by for 'PrecompiledHeaders.cpp'
//

#pragma once



// TODO: 在此处引用程序要求的附加头文件

#include <fstream>

#ifdef _MSC_VER
#	pragma warning(push, 1)
#endif
#pragma warning(disable: 4819)
#include <boost\lexical_cast.hpp>
#include <boost\shared_ptr.hpp>
#include <boost\shared_array.hpp>
#include <boost\bind.hpp>
#include <boost\filesystem.hpp>
#include <boost\function.hpp>
#include <boost\assign.hpp>
#include <boost\format.hpp>
#pragma warning(default: 4819)
#ifdef _MSC_VER
#	pragma warning(pop)
#endif

#include <Ogre.h>

#include <CEGUI\CEGUISystem.h>
#include <CEGUI\CEGUILogger.h>
#include <CEGUI\CEGUISchemeManager.h>
#include <CEGUI\CEGUIWindowManager.h>
#include <CEGUI\CEGUIWindow.h>
#include <CEGUI\elements\CEGUIPushButton.h>
#include <CEGUI\elements\CEGUICheckbox.h>


#include <tchar.h>

#pragma warning(disable:4996)
#include <wx\datetime.h>
#pragma warning(default:4996)
#include <wx\wxprec.h>

#pragma comment(lib, "comctl32.lib")
#if defined(DEBUG) || defined(_DEBUG)
#	if	wxMAJOR_VERSION	== 2 && wxMINOR_VERSION == 6
#		pragma comment(lib, "wxmsw26d_core.lib")
#		pragma comment(lib, "wxmsw26d_adv.lib")
#		pragma comment(lib, "wxbase26d.lib")
#		pragma comment(lib, "wxbase26d_xml.lib")
#	elif	wxMAJOR_VERSION	== 2 && wxMINOR_VERSION == 8
#		pragma comment(lib, "wxmsw28d_core.lib")
#		pragma comment(lib, "wxmsw28d_adv.lib")
#		pragma comment(lib, "wxbase28d.lib")
#		pragma comment(lib, "wxbase28d_xml.lib")
#	else
#		error	unknown wxWidgets version
#	endif
#	pragma comment(lib, "wxtiffd.lib")
#	pragma comment(lib, "wxjpegd.lib")
#	pragma comment(lib, "wxpngd.lib")
#	pragma comment(lib, "wxzlibd.lib")
#	pragma comment(lib, "wxregexd.lib")
#	pragma comment(lib, "wxexpatd.lib")
#	pragma comment(lib, "rpcrt4.lib")
#else
#	if	wxMAJOR_VERSION	== 2 && wxMINOR_VERSION == 6
#		pragma comment(lib, "wxmsw26_core.lib")
#		pragma comment(lib, "wxmsw26_adv.lib")
#		pragma comment(lib, "wxbase26.lib")
#		pragma comment(lib, "wxbase26_xml.lib")
#	elif	wxMAJOR_VERSION	== 2 && wxMINOR_VERSION == 8
#		pragma comment(lib, "wxmsw28_core.lib")
#		pragma comment(lib, "wxmsw28_adv.lib")
#		pragma comment(lib, "wxbase28.lib")
#		pragma comment(lib, "wxbase28_xml.lib")
#	else
#		error	unknown wxWidgets version
#	endif
#	pragma comment(lib, "wxtiff.lib")
#	pragma comment(lib, "wxjpeg.lib")
#	pragma comment(lib, "wxpng.lib")
#	pragma comment(lib, "wxzlib.lib")
#	pragma comment(lib, "wxregex.lib")
#	pragma comment(lib, "wxexpat.lib")
#endif
