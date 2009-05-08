/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef __LUAGAMEHOST_H__
#define __LUAGAMEHOST_H__



#include "GameHost.h"


struct lua_State;


class LuaGameHost
	: public GameHost
{
public:
	explicit LuaGameHost(const std::string& filename);
	~LuaGameHost();

private:
	virtual void	initialize(Billiards::Game* game);

private:
	lua_State*	m_LuaState;
};



#endif	// !defined(__LUAGAMEHOST_H__)
