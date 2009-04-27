/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__



#include "..\Billiards\VisualObject.h"


namespace Ogre
{
	class SceneNode;
}


class GameObject
	: public Billiards::IVisualObject
{
public:
	GameObject(Ogre::SceneNode* parent, const Billiards::VisualObjectParameters& param);

private:
	virtual void	updateTransform(const Billiards::Vector& position, const Billiards::Quaternion& orientation);
	virtual void	setVisible(bool visible);

private:
	Ogre::SceneNode*	m_nodeObject;
};



#endif	// !defined(__GAMEOBJECT_H__)
