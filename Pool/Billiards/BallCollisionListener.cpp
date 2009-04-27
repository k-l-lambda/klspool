/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"
#include "BallCollisionListener.h"

#include <iostream>


BallCollisionListener::BallCollisionListener( hkpRigidBody* rigidBody)
{
	rigidBody->addCollisionListener( this );
	rigidBody->addEntityListener( this );
	m_uniqueIdCounter = 0;
}


BallCollisionListener::~BallCollisionListener()
{
}


void BallCollisionListener::contactPointAddedCallback(	hkpContactPointAddedEvent& event )
{
	const hkVector4& pos = event.m_contactPoint->getPosition();
	std::cout<<"*************************************************************"<<std::endl;
	std::cout<<"**Collide point position:"<<'('<<pos(0)<<','<<pos(1)<<','<<pos(2)<<','<<pos(3)<<')'<<std::endl;
	std::cout<<"*************************************************************"<<std::endl;


	// By setting the  ProcessContactCallbackDelay to 0 we will receive callbacks for 
	// any collisions processed for this body every frame (simulation step), i.e. the delay between
	// any such callbacks is 0 frames.

	// If you wish to only be notified every N frames simply set the delay to be N-1.
	// The default is 65536, i.e. (for practical purpose) once for the first collision only, until
	// the bodies separate to outside the collision tolerance. 
	event.m_callbackFiredFrom->setProcessContactCallbackDelay(0);
}

void BallCollisionListener::contactPointRemovedCallback( hkpContactPointRemovedEvent& event )
{
}

void BallCollisionListener::contactPointConfirmedCallback( hkpContactPointConfirmedEvent& event)
{
}

void BallCollisionListener::contactProcessCallback( hkpContactProcessEvent& event )
{
}

//
// entityDeletedCallback
//
void BallCollisionListener::entityDeletedCallback( hkpEntity* entity )
{
	// Remove the collision event listener
	entity->removeCollisionListener( this ); 

	// Now that we're removed from our entity, we can remove ourselves, too.
	delete this;
}


void BallCollisionListener::entityRemovedCallback( hkpEntity* entity )
{
	// Do nothing (we'll remove the collision event listener only when the entity is deleted)
}
