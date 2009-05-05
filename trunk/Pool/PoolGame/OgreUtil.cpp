/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include "OgreUtil.h"

#include <Direct3D9\include\OgreD3D9RenderWindow.h>
#define	protected	public
#include <Direct3D10\include\OgreD3D10RenderWindow.h>
#undef	protected
#include <GL\include\OgreWin32Window.h>


::HWND	getRenderWindowHandle(const Ogre::RenderWindow* rw)
{
	if(const Ogre::D3D9RenderWindow* d3d9w = dynamic_cast<const Ogre::D3D9RenderWindow*>(rw))
		return d3d9w->getWindowHandle();
	if(const Ogre::D3D10RenderWindow* d3d10w = dynamic_cast<const Ogre::D3D10RenderWindow*>(rw))
		return d3d10w->mHWnd;
	else if(const Ogre::Win32Window* glw = dynamic_cast<const Ogre::Win32Window*>(rw))
		return glw->getWindowHandle();

	return NULL;
}
