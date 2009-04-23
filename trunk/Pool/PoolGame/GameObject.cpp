/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include "GameObject.h"
#include "mathTypeConvert.h"


GameObject::GameObject(Ogre::SceneNode* parent, const Billiards::VisualObjectParameters& param)
	: m_nodeObject(parent->createChildSceneNode())
{
	Ogre::Entity* ent = m_nodeObject->getCreator()->createEntity(param.Name, param.MeshName);
	for(Billiards::VisualObjectParameters::MaterialNameMap_t::const_iterator it = param.MaterialNameMap.begin();
		it != param.MaterialNameMap.end(); ++ it)
	{
		if(it->first >= ent->getNumSubEntities())
			throw std::out_of_range("material map index out of range");
		ent->getSubEntity(it->first)->setMaterialName(it->second);
	}
	m_nodeObject->attachObject(ent);
}

void GameObject::updateTransform(const Billiards::Vector& position, const Billiards::Quaternion& orientation)
{
	m_nodeObject->setPosition(bld2Ogre(position));
	m_nodeObject->setOrientation(bld2Ogre(orientation));
}
