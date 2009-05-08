/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include "Game.h"
#include "GameLayout.h"

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
			class_<Game>("Game")
				.def("deployLayout",	&Game::deployLayout)
		];

		return 0;
	}
}
