/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include "LuaGameHost.h"

#include "..\Billiards\Game.h"


extern "C"
{
   #include <lua.h>
   #include <lualib.h>
   #include <lauxlib.h>
}
#ifdef	_DEBUG
#	pragma comment(lib, "Lua_Debug.lib")
#else
#	pragma comment(lib, "Lua_Release.lib")
#endif

#pragma warning(push)
#pragma warning(disable: 4996)
#pragma warning(disable: 4819)
#pragma warning(disable: 4267)
#include <luabind\luabind.hpp>
#include <luabind\operator.hpp>
#include <luabind\adopt_policy.hpp>
#pragma warning(pop)
#ifdef	_DEBUG
#	pragma comment(lib, "luabind_Debug.lib")
#else
#	pragma comment(lib, "luabind_Release.lib")
#endif


LuaGameHost::LuaGameHost(const std::string& filename)
	: m_LuaState(NULL)
{
	m_LuaState = lua_open();

	luaopen_base(m_LuaState);
	luaopen_math(m_LuaState);
	luaopen_string(m_LuaState);
	luaopen_table(m_LuaState);

	Billiards::registerLuaSymbols(m_LuaState);

	if(luaL_loadfile(m_LuaState, filename.data()) || lua_pcall(m_LuaState, 0, 0, 0))
	{
		const char* errormessage = lua_tostring(m_LuaState, lua_gettop(m_LuaState));
		if(errormessage)
			std::cerr << "lua error: " << errormessage << std::endl;

		throw std::runtime_error("lua script parsing failed");
	}
}

LuaGameHost::~LuaGameHost()
{
	if(m_LuaState)
		lua_close(m_LuaState);
}

void LuaGameHost::initialize(Billiards::Game* game)
{
	try
	{
		luabind::call_function<void>(m_LuaState, "initialize", game);
	}
	catch(const luabind::error& e)
	{
		const char* errormessage = lua_tostring(e.state(), lua_gettop(e.state()));
		if(errormessage)
			std::cerr << "lua error: " << errormessage << std::endl;

		throw e;
	}
}
