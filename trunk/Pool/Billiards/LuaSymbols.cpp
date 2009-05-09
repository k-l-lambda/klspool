/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include "Game.h"
#include "GameLayout.h"

#define	LUABIND_DYNAMIC_LINK	// luabind 0.8 dll linking require this macro
#include <LuabindUtil\utility.hpp>
#include <LuabindUtil\vector.hpp>
#include <LuabindUtil\set.hpp>
#include <LuabindUtil\map.hpp>
#include <LuabindUtil\shared_ptr.hpp>
#include <LuabindUtil\bind.hpp>


namespace Billiards
{
	namespace AuxSymbols
	{
	}


	int registerLuaSymbols(lua_State* L)
	{
		using namespace luabind;


		if(!luabind::detail::class_registry::get_registry(L))
			luabind::open(L);

		module(L, "Billiards")
		[
			class_<Vector>("Vector")
				.def(constructor<>())
				.def(constructor<const Vector&>())
				.def(constructor<Real, Real, Real>())
				.def(const_self == other<const Vector&>())
				.def(const_self + other<const Vector&>())
				.def(const_self - other<const Vector&>())
				.def(const_self * other<Real>())
				.def(const_self / other<Real>())
				.def(tostring(const_self))
				.def(- const_self)
				.def_readwrite("x",						&Vector::x)
				.def_readwrite("y",						&Vector::y)
				.def_readwrite("z",						&Vector::z)
			,
			class_<Game>("Game")
				.def("loadBallConfigSet",				&Game::loadBallConfigSet)
				.def("deployLayout",					&Game::deployLayout)
			,
			class_<GameLayout>("GameLayout")
				.scope
				[
					class_<GameLayout::BallInfo>("BallInfo")
						.scope
						[
							def("make",					&GameLayout::BallInfo::make)
						]
						.def(constructor<>())
						.def_readwrite("Config",		&GameLayout::BallInfo::Config)
						.def_readwrite("Position",		&GameLayout::BallInfo::Position)
				]
				.def(constructor<>())
				.def_readwrite("BallsLayout",			&GameLayout::BallsLayout)
			,
			LuabindUtil::bind<GameLayout::BallsLayout_t>()
		];

		return 0;
	}
}
