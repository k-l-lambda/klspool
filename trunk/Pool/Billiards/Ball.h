/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef _BALL_H_
#define _BALL_H_

#include "StableHeaders.h"

#include <Ogre.h>

// This class synchronizes the graphic and the phsix of the ball,
// and supports some tests and sets funtions

class Ball
{
public:
	bool isInHole();
	bool isStill();
	void update();

	static setupStatics(Ogre::SceneNode* sn, hkpWorld* kw);

private:
	Ogre::SceneNode* m_OgreNode;

	hkpRigidBody* m_HavokRigid;

	static hkpWorld* m_hkpWorld;
	static Ogre::SceneManager* m_SceneMgr;
};

#endif