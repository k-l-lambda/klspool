/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include "Game.h"


namespace Billiards
{
	Game::Game()
	{
		hkSystem = new HavokSystem();
	}

	void Game::setup()
	{
		if(hkSystem)
		{
			hkSystem->setup();

			createPhysicsScene();
		}
	}

	Game::~Game()
	{
		if(hkSystem)
			delete hkSystem;

		for(std::vector<Ball*>::iterator iter = m_ballList.begin();
			iter != m_ballList.end();
			++iter)
			delete *iter;
	}

	void Game::createPhysicsScene()
	{
		// set the world pointer in ball
		Ball::setupStatic(hkSystem->m_World);

		hkSystem->m_World->markForWrite();

		// create the ground
		Vector groundSize;
		groundSize.set(200.0f,2.0f,200.0f);
		hkpConvexShape* shape = new hkpBoxShape(groundSize, 0.05f);

		hkpRigidBodyCinfo ci;

		ci.m_shape = shape;
		ci.m_motionType = hkpMotion::MOTION_FIXED;
		ci.m_position = Vector(0, 0, 0);
		ci.m_qualityType = HK_COLLIDABLE_QUALITY_FIXED;

		m_table = new hkpRigidBody(ci);
		hkSystem->m_World->addEntity(m_table);
		shape->removeReference();

		hkSystem->m_World->unmarkForWrite();
		
		// add balls
		addBall(0, 50, 2, 100, 3);
		addBall(0, 60, 0, 100, 3);
	}

	void Game::addBall(Real x, Real y, Real z, Real mass, Real radius)
	{
		Ball* ball = new Ball(Vector(x, y, z) , mass , radius);
	
		ball->creatRigidBody();

		m_ballList.push_back(ball);
	}

	void Game::setPosOfBall(Real x, Real y, Real z, int number)
	{
		//assert(number > (int)m_ballList.size());

		m_ballList[number]->setPos(Vector(x, y, z));
	}

	void Game::applyForceOnBall(const Vector& force, const Vector& pos, Real deltaTime, int number)
	{
		//assert(number > (int)m_ballList.size());

		m_ballList[number]->applyForce(force, pos, deltaTime);
	}

	Vector Game::getPosOfBall(int number) const
	{
		//assert(number > (int)m_ballList.size());

		return m_ballList[number]->getPos();
	}

	Quaternion Game::getRotationOfBall(int number) const
	{
		//assert(number > (int)m_ballList.size());

		return m_ballList[number]->getRotation();
	}

	void Game::deleteBall(int number)
	{
		//assert(number > (int)m_ballList.size());

		delete m_ballList[number];

		std::vector<Ball*>::iterator iter = m_ballList.begin() + number;
		m_ballList.erase(iter);
	}

	void Game::updateAllBalls()
	{
		for(int i = 0; i < (int)m_ballList.size(); ++i)
			m_ballList[i]->update();
	}

	void Game::simulate(Real elapsedTime)
	{
		hkSystem->simulate(elapsedTime);

		updateAllBalls();
	}
}
