/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include "Ball.h"

#include <assert.h>


namespace Billiards
{
	hkpWorld* Ball::m_hkpWorld = 0;

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
		sphereInfo.m_solverDeactivation = sphereInfo.SOLVER_DEACTIVATION_MEDIUM;
		sphereInfo.m_shape = shape;
		sphereInfo.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		sphereInfo.m_position = m_position;
			
		//creat Havok hkpRigidBody
		m_havokRigid = new hkpRigidBody(sphereInfo);
		m_hkpWorld->addEntity(m_havokRigid);

		m_hkpWorld->unmarkForWrite();

		m_havokRigid->removeReference();
		shape->removeReference();
	}

	void Ball::setPos(hkVector4 &pos)
	{
		m_position = pos;

		if(m_havokRigid)
		{
			m_hkpWorld->markForWrite();

			m_havokRigid->setPosition(pos);

			m_hkpWorld->unmarkForWrite();
		}
	}

	hkVector4 Ball::getPos()
	{
		if(m_havokRigid)
		{
			m_hkpWorld->markForRead();

			m_position = m_havokRigid->getPosition();

			m_hkpWorld->unmarkForRead();
		}

		return m_position;
	}

	hkQuaternion Ball::getRotation()
	{
		hkQuaternion q;

		if(m_havokRigid)
		{
			m_hkpWorld->markForRead();

			q = m_havokRigid->getRotation();

			m_hkpWorld->unmarkForRead();
		}

		return q;
	}

	void Ball::setupStatic(hkpWorld *kw)
	{
		assert(!kw);

		m_hkpWorld = kw;	
	}

	Ball::~Ball()
	{
		m_hkpWorld->markForWrite();

		m_hkpWorld->removeEntity(m_havokRigid);

		m_hkpWorld->unmarkForWrite();
	}
}