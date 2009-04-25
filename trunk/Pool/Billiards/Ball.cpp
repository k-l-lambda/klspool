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

#include <assert.h>
#include <iostream>


namespace Billiards
{
	hkpWorld* Ball::s_hkpWorld = 0;


	Ball::Ball(VisualObjectPtr vobj, const Vector& pos, Real mass, Real radius)
		: m_mass(mass)
		, m_radius(radius)
		, m_position(pos)
		, m_havokRigid(0)
		, m_VisualObject(vobj)
	{
	}

	void Ball::creatRigidBody()
	{
		WorldWritingLock wlock(s_hkpWorld);

		hkpConvexShape* shape = new hkpSphereShape(m_radius);

		// compute necessary properties
		hkpRigidBodyCinfo sphereInfo;

		sphereInfo.m_mass = m_mass;

		hkpMassProperties massProperties;
		hkpInertiaTensorComputer::computeSphereVolumeMassProperties(m_radius,m_mass,massProperties);

		sphereInfo.m_mass = massProperties.m_mass;
		sphereInfo.m_centerOfMass = massProperties.m_centerOfMass;
		sphereInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
		sphereInfo.m_solverDeactivation = sphereInfo.SOLVER_DEACTIVATION_LOW;
		sphereInfo.m_shape = shape;
		sphereInfo.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		sphereInfo.m_position = m_position;

		//creat Havok hkpRigidBody
		m_havokRigid = new hkpRigidBody(sphereInfo);
		s_hkpWorld->addEntity(m_havokRigid);

		shape->removeReference();
	}

	void Ball::applyForce(const Vector& force, const Vector& pos, Real deltaTime)
	{
		WorldWritingLock wlock(s_hkpWorld);

		m_havokRigid->applyForce(deltaTime, force, pos);
	}

	void Ball::setPosition(const Vector &pos)
	{
		m_position = pos;

		if(m_havokRigid)
		{
			WorldWritingLock wlock(s_hkpWorld);

			m_havokRigid->setPosition(pos);
		}
	}

	const Vector& Ball::getPosition() const
	{
		return m_position;
	}

	const Quaternion& Ball::getRotation() const
	{
		return m_rotation;
	}

	void Ball::setupStatic(hkpWorld *hw)
	{
		s_hkpWorld = hw;	
	}

	void Ball::update()
	{
		{
			WorldReadingLock rlock(s_hkpWorld);

			m_position = m_havokRigid->getPosition();

			m_rotation = m_havokRigid->getRotation();
		}

		if(m_VisualObject)
			m_VisualObject->updateTransform(m_position, m_rotation);
	}

	Ball::~Ball()
	{
		WorldWritingLock wlock(s_hkpWorld);

		s_hkpWorld->removeEntity(m_havokRigid);
	}
}
