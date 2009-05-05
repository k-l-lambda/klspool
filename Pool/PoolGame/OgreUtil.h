/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef	__OGREUTIL_H__
#define	__OGREUTIL_H__



#include <windows.h>


namespace Ogre
{
	class RenderWindow;
}


::HWND	getRenderWindowHandle(const Ogre::RenderWindow* rw);



#endif	// !defined(__OGREUTIL_H__)
