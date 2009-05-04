/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef __THREADACCESSLOCK_H__
#define __THREADACCESSLOCK_H__



#include "Billiards.h"


namespace Billiards
{
	template<typename T, T Mark, T Unmark>
	struct WorldAccessLock
	{
		hkpWorld* const pWorld;

		explicit WorldAccessLock(hkpWorld* world)
			: pWorld(world)
		{
			(pWorld->*Mark)();
		};

		~WorldAccessLock()
		{
			(pWorld->*Unmark)();
		};
	};

	typedef	WorldAccessLock<void (hkpWorld::*)(), &hkpWorld::markForWrite, &hkpWorld::unmarkForWrite>		WorldWritingLock;
	typedef	WorldAccessLock<void (hkpWorld::*)() const, &hkpWorld::markForRead, &hkpWorld::unmarkForRead>	WorldReadingLock;
}



#endif	// !defined(__THREADACCESSLOCK_H__)
