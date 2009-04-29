/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include "Ball.h"
#include "HavokRef.h"
#include "VisualObject.h"
#include "ThreadAccessLock.h"

#include "BallCollisionListener.h"


namespace Billiards
{
	//hkpWorld* Ball::s_hkpWorld = 0;


	Ball::Ball(hkpWorld* world, const VisualObjectPtr& vobj)
		: m_World(world)
		, m_Radius(0)
		/*, m_mass(mass)
		, m_radius(radius)
		, m_position(pos)*/
		, m_VisualObject(vobj)
	{
		m_VisualObject->setVisible(m_RigidBody);
	}

	Ball::~Ball()
	{
		releaseRigidBody();
	}

	void Ball::resetRigidBody(const Vector& position, Real mass, Real radius)
	{
		m_Radius = radius;

		WorldWritingLock wlock(m_World);

		hkpConvexShape* shape = new hkpSphereShape(radius);

		// compute necessary properties
		hkpRigidBodyCinfo sphereInfo;

		sphereInfo.m_mass = mass;

		hkpMassProperties massProperties;
		hkpInertiaTensorComputer::computeSphereVolumeMassProperties(radius, mass, massProperties);

		sphereInfo.m_mass = massProperties.m_mass;
		sphereInfo.m_centerOfMass = massProperties.m_centerOfMass;
		sphereInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
		sphereInfo.m_solverDeactivation = hkpRigidBodyCinfo::SOLVER_DEACTIVATION_LOW;
		sphereInfo.m_qualityType = HK_COLLIDABLE_QUALITY_MOVING;
		sphereInfo.m_shape = shape;
		sphereInfo.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		sphereInfo.m_position = position;
		sphereInfo.m_angularDamping = 0.8f;
		sphereInfo.m_restitution = 0.92f;
		sphereInfo.m_allowedPenetrationDepth = 1e-3f;
		sphereInfo.m_friction = 0.06f;

		//creat Havok hkpRigidBody
		m_RigidBody.reset(new hkpRigidBody(sphereInfo));
		m_World->addEntity(m_RigidBody.get());

		// Add the collision event listener to the rigid body
		/*BallCollisionListener* listener = */new BallCollisionListener(m_RigidBody);

		shape->removeReference();

		m_VisualObject->setVisible(m_RigidBody);
	}

	void Ball::releaseRigidBody()
	{
		if(m_RigidBody)
		{
			WorldWritingLock wlock(m_World);

			m_RigidBody.reset();
		}

		m_VisualObject->setVisible(m_RigidBody);
	}

	void Ball::applyForce(const Vector& force, const Vector& pos, Real deltaTime)
	{
		WorldWritingLock wlock(m_World);

		m_RigidBody->applyForce(deltaTime, force, pos);
	}

	void Ball::setPosition(const Vector &pos)
	{
		//m_position = pos;

		if(m_RigidBody)
		{
			WorldWritingLock wlock(m_World);

			m_RigidBody->setPosition(pos);
		}
	}

	void Ball::setVelocity(const Vector& vel)
	{
		if(m_RigidBody)
		{
			WorldWritingLock wlock(m_World);

			m_RigidBody->setLinearVelocity(vel);
		}
	}

	Real Ball::getRadius() const
	{
		return m_Radius;
	}

	Vector Ball::getPosition() const
	{
		return m_RigidBody->getPosition();
	}

	const Quaternion& Ball::getRotation() const
	{
		return m_RigidBody->getRotation();
	}

	Vector Ball::getVelocity() const
	{
		return m_RigidBody->getLinearVelocity();
	}

	/*void Ball::setupStatic(hkpWorld *hw)
	{
		s_hkpWorld = hw;	
	}*/

	void Ball::update()
	{
		if(m_VisualObject)
		{
			WorldReadingLock rlock(m_World);

			//m_position = m_RigidBody->getPosition();
			//m_rotation = m_RigidBody->getRotation();

			m_VisualObject->updateTransform(m_RigidBody->getPosition(), m_RigidBody->getRotation());
		}
	}
}
