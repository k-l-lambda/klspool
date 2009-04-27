/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef _BALLCOLLISIONLISTENER_H_
#define _BALLCOLLISIONLISTENER_H_


#include "Billiards.h"
#include "HavokRef.h"


// This specific implementation is not referenced by any other class, but adds itself
// as a listener to the entity. It automatically deletes itself, if the entity gets 
// deleted.	

class BallCollisionListener : public hkReferencedObject, public hkpCollisionListener, public hkpEntityListener
{

   	public:

		BallCollisionListener( hkpRigidBody* rigidBody);
        
		~BallCollisionListener();

		
		//Members from base class hkpCollisionListener which must be implemented:
		

		// Called after a contact point was added 
		virtual void contactPointAddedCallback(	hkpContactPointAddedEvent& event );

		virtual void contactPointConfirmedCallback( hkpContactPointConfirmedEvent& event);

		// Called before a contact point gets removed. We do not implement this.
		virtual void contactPointRemovedCallback( hkpContactPointRemovedEvent& event );

		// Called just before the collisionResult is passed to the constraint system (solved).
		virtual void contactProcessCallback( hkpContactProcessEvent& event );


		//Members from base class hkpEntityListener which must be implemented:


		// Called when the entity is deleted. Important to use this event to remove ourselves as a listener.
		void entityDeletedCallback( hkpEntity* entity );

		// Called when the entity is removed from the hkpWorld
		void entityRemovedCallback( hkpEntity* entity );

	protected:
		// a small structure, which gets attached to each contact point
		struct  ContactPointInfo
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO,  ContactPointInfo);
			int m_uniqueId;
		};
		int m_uniqueIdCounter;

	public:

		int m_reportLevel;

};


#endif