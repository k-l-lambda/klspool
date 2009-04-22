/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include "Ball.h"
#include "HavokRef.h"

#include <assert.h>
#include <iostream>


namespace Billiards
{
	hkpWorld* Ball::m_hkpWorld = 0;

	Ball::Ball(const Vector& pos, Real mass, Real radius)
		: m_mass(mass)
		, m_radius(radius)
		, m_position(pos)
		, m_havokRigid(0)
	{
	}

	void Ball::creatRigidBody()
	{
		m_hkpWorld->markForWrite();

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
		m_hkpWorld->addEntity(m_havokRigid);

		m_hkpWorld->unmarkForWrite();

		shape->removeReference();
	}

	void Ball::applyForce(const Vector& force, const Vector& pos, Real deltaTime)
	{
		m_hkpWorld->markForWrite();

		m_havokRigid->applyForce(deltaTime, force, pos);

		m_hkpWorld->unmarkForWrite();
	}

	void Ball::setPos(const Vector &pos)
	{
		m_position = pos;

		if(m_havokRigid)
		{
			m_hkpWorld->markForWrite();

			m_havokRigid->setPosition(pos);

			m_hkpWorld->unmarkForWrite();
		}
	}

	Vector Ball::getPos() const
	{
		return m_position;
	}

	Quaternion Ball::getRotation() const
	{
		return m_rotation;
	}

	void Ball::setupStatic(hkpWorld *hw)
	{
		m_hkpWorld = hw;	
	}

	void Ball::update()
	{
		m_hkpWorld->markForRead();

		m_position = m_havokRigid->getPosition();

		m_rotation = m_havokRigid->getRotation();

		m_hkpWorld->unmarkForRead();
	}

	Ball::~Ball()
	{
		m_hkpWorld->markForWrite();

		m_hkpWorld->removeEntity(m_havokRigid);

		m_hkpWorld->unmarkForWrite();
	}
}