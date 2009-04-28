/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef	__BALLCONFIG_H__
#define	__BALLCONFIG_H__



#include "Billiards.h"


namespace Billiards
{
	struct BallConfig
	{
		std::string	Name;
		Real		Redius;
		Real		Mass;
		std::string	Material;
	};
}



#endif	// !defined(__BALLCONFIG_H__)
