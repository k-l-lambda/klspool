/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

// TODO: refine this reference of AudioSystem and PoolGame
#include "../AudioSystem/PoolAudio.h"
#pragma comment(lib, "AudioSystem.lib")

#include "BallCollisionListener.h"

#include <iostream>
#include <ctime>


namespace Billiards
{
	BallCollisionListener::BallCollisionListener(const hkpRigidBodyPtr& rigidBody)
	{
		rigidBody->addCollisionListener(this);
		rigidBody->addEntityListener(this);
		m_uniqueIdCounter = 0;
	}

	BallCollisionListener::~BallCollisionListener()
	{
	}

	void BallCollisionListener::contactPointAddedCallback(hkpContactPointAddedEvent& event)
	{
		std::cout << "Collision: " << std::clock() << "\t" << event.m_contactPoint->getPosition()<<std::endl;

		hkpShapeType shapeTypeA = event.m_bodyA->m_shape->getType();
		hkpShapeType shapeTypeB = event.m_bodyB->m_shape->getType();
		if(shapeTypeA == HK_SHAPE_SPHERE && shapeTypeB == HK_SHAPE_SPHERE )
		{
			PoolAudio::instance().playSound(0, &(event.m_contactPoint->getPosition()(0)), -event.m_projectedVelocity);
			std::cout<< "Velocity : "<<event.m_projectedVelocity<<std::endl;
		}
		else if(event.m_contactPoint->getPosition()(1) != 8.2f)
		{
			PoolAudio::instance().playSound(2, &(event.m_contactPoint->getPosition()(0)), -event.m_projectedVelocity);
		}

		event.m_callbackFiredFrom->setProcessContactCallbackDelay(0);
	}

	void BallCollisionListener::contactPointRemovedCallback(hkpContactPointRemovedEvent& event)
	{
	}

	void BallCollisionListener::contactPointConfirmedCallback(hkpContactPointConfirmedEvent& event)
	{
	}

	void BallCollisionListener::contactProcessCallback(hkpContactProcessEvent& event)
	{
	}

	//
	// entityDeletedCallback
	//
	void BallCollisionListener::entityDeletedCallback(hkpEntity* entity)
	{
		// Remove the collision event listener
		entity->removeCollisionListener(this); 

		// Now that we're removed from our entity, we can remove ourselves, too.
		delete this;
	}


	void BallCollisionListener::entityRemovedCallback(hkpEntity* entity)
	{
		// Do nothing (we'll remove the collision event listener only when the entity is deleted)
	}
}
