/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef	__GAMELAYOUT_H__
#define	__GAMELAYOUT_H__



#include "Billiards.h"


namespace Billiards
{
	struct GameLayout
	{
		struct BallInfo
		{
			std::string		Config;
			Vector			Position;

			static BallInfo	make(const std::string& config, const Vector& position)
			{
				BallInfo info = {config, position};
				return info;
			};
		};
		typedef	std::vector<BallInfo>	BallsLayout_t;

		BallsLayout_t		BallsLayout;
	};
}



#endif	// !defined(__GAMELAYOUT_H__)
