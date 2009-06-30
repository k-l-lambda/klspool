/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include "BallCollisionListener.h"
#include "AudioSocket.h"

#include <iostream>
#include <ctime>


namespace Billiards
{
	BallCollisionListener::BallCollisionListener(const hkpRigidBodyPtr& rigidBody)
	{
		rigidBody->addCollisionListener(this);
		rigidBody->addEntityListener(this);
		rigidBody->addEntityActivationListener(this);
		m_uniqueIdCounter = 0;
	}

	BallCollisionListener::~BallCollisionListener()
	{
	}

	void BallCollisionListener::contactPointAddedCallback(hkpContactPointAddedEvent& event)
	{
		hkpShapeType shapeTypeA = event.m_bodyA->m_shape->getType();
		hkpShapeType shapeTypeB = event.m_bodyB->m_shape->getType();
		if(shapeTypeA == HK_SHAPE_SPHERE && shapeTypeB == HK_SHAPE_SPHERE )
		{
			boost::mutex::scoped_lock lock(AudioSocket::getPlaySoundMutex());

			AudioSocket::instance().playSound("collide b-b", event.m_contactPoint->getPosition(), -event.m_projectedVelocity);
			//std::cout <<  "Velocity : " << event.m_projectedVelocity << std::endl;
		}
		else if(event.m_contactPoint->getPosition()(1) > 8.25f)
		{
			boost::mutex::scoped_lock lock(AudioSocket::getPlaySoundMutex());

			AudioSocket::instance().playSound("bound", event.m_contactPoint->getPosition(), -event.m_projectedVelocity);
		}

		event.m_callbackFiredFrom->setProcessContactCallbackDelay(0);
	}

	void BallCollisionListener::contactPointRemovedCallback(hkpContactPointRemovedEvent& event)
	{
		hkpEntity* entA = event.m_entityA;
		hkpEntity* entB = event.m_entityB;

		if(entA->isFixed() && entB->isFixed())
			return;

		hkVector4 v;
		std::string bname("BAFFLES");

		//std::cout<<"***************Collision Velocity*****************"<<std::endl;
		if(bname == std::string(entB->getName()))
		{
			v = entA->getMotion()->m_linearVelocity;
			hkVector4 newV(v(0), 0, v(2));
			newV.normalize3();
			newV.mul4(v.length3());

			entA->getMotion()->setLinearVelocity(newV);
			//std::cout<<"("<<newV(0)<<",\t"<<newV(1)<<",\t"<<newV(2)<<")"<<std::endl;
		}
		else if(bname == std::string(entA->getName()))
		{
			v = entB->getMotion()->m_linearVelocity;
			hkVector4 newV(v(0), 0, v(2));
			newV.normalize3();
			newV.mul4(v.length3());

			entB->getMotion()->setLinearVelocity(newV);
			//std::cout<<"("<<newV(0)<<",\t"<<newV(1)<<",\t"<<newV(2)<<")"<<std::endl;
		}
		else
			return;

		//std::cout<<"("<<v(0)<<",\t"<<v(1)<<",\t"<<v(2)<<")"<<std::endl;
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


	void BallCollisionListener::entityDeactivatedCallback(hkpEntity* entity)
	{
	}

	void BallCollisionListener::entityActivatedCallback(hkpEntity* entity)
	{
	}
}
