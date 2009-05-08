/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef __GAMEHOST_H__
#define __GAMEHOST_H__



#include "..\Billiards\Billiards.h"


struct GameHost
{
	virtual void	initialize(Billiards::Game* game)	= 0;
};



#endif	// !defined(__GAMEHOST_H__)
