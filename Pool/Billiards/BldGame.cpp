/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include "BldGame.h"


namespace Billiards
{
	void BldGame::createPhysicsScene()
	{
		// set the world pointer in ball
		Ball::setupStatic(m_World);

		m_World->markForWrite();

		// create the ground
		hkVector4 groundSize;
		groundSize.set(200.0f,2.0f,200.0f);
		hkpConvexShape* shape = new hkpBoxShape(groundSize, 0.05f);

		hkpRigidBodyCinfo ci;

		ci.m_shape = shape;
		ci.m_motionType = hkpMotion::MOTION_FIXED;
		ci.m_position = hkVector4(0, 0, 0);
		ci.m_qualityType = HK_COLLIDABLE_QUALITY_FIXED;

		m_table = new hkpRigidBody(ci);
		m_World->addEntity(m_table);
		shape->removeReference();

		m_World->unmarkForWrite();
		
		// add balls
		addBall(0, 50, 2, 100, 3);
		addBall(0, 60, 0, 100, 3);
	}

	void BldGame::addBall(hkReal x, hkReal y, hkReal z, hkReal mass, hkReal radius)
	{
		Ball* ball = new Ball(hkVector4(x, y, z) , mass , radius);
	
		ball->creatRigidBody();

		m_ballList.push_back(ball);
	}

	void BldGame::setPosOfBall(hkReal x, hkReal y, hkReal z, int number)
	{
		//assert(number > (int)m_ballList.size());

		m_ballList[number]->setPos(hkVector4(x, y, z));
	}

	void BldGame::applyForceOnBall(const hkVector4& force, const hkVector4& pos, hkReal deltaTime, int number)
	{
		//assert(number > (int)m_ballList.size());

		m_ballList[number]->applyForce(force, pos, deltaTime);
	}

	hkVector4 BldGame::getPosOfBall(int number) const
	{
		//assert(number > (int)m_ballList.size());

		return m_ballList[number]->getPos();
	}

	hkQuaternion BldGame::getgetRotationOfBall(int number) const
	{
		//assert(number > (int)m_ballList.size());

		return m_ballList[number]->getRotation();
	}

	void BldGame::deleteBall(int number)
	{
		//assert(number > (int)m_ballList.size());

		delete m_ballList[number];

		std::vector<Ball*>::iterator iter = m_ballList.begin() + number;
		m_ballList.erase(iter);
	}

	void BldGame::updateAllBalls()
	{
		for(int i = 0; i < (int)m_ballList.size(); ++i)
			m_ballList[i]->update();
	}

	void BldGame::simulate()
	{
		HavokSystem::simulate();

		updateAllBalls();
	}

	BldGame::~BldGame()
	{
		for(std::vector<Ball*>::iterator iter = m_ballList.begin();
			iter != m_ballList.end();
			++iter)
			delete *iter;
	}
}
