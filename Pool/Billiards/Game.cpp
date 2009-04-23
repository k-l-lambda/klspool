/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include <boost\assign.hpp>

#include "Game.h"
#include "VisualObject.h"


namespace Billiards
{
	Game::Game(const VisualObjectCreationFunctor& fnCreateVisualObject)
		: m_table(NULL)
		, m_hkSystem(NULL)
		, m_fnCreateVisualObject(fnCreateVisualObject)
	{
		m_hkSystem = new HavokSystem();

		// set defult result of m_fnCreateVisualObject to null
		if(!m_fnCreateVisualObject)
		{
			struct LocalScope
			{
				static VisualObjectPtr null(...)	{return VisualObjectPtr();};
			};
			m_fnCreateVisualObject = &LocalScope::null;
		}
	}

	void Game::setup()
	{
		if(m_hkSystem)
		{
			m_hkSystem->setup();

			createPhysicsScene();
		}
	}

	Game::~Game()
	{
		for(std::vector<Ball*>::iterator iter = m_ballList.begin();
			iter != m_ballList.end(); ++ iter)
			delete *iter;

		if(m_hkSystem)
			delete m_hkSystem;
	}

	void Game::createPhysicsScene()
	{
		// set the world pointer in ball
		Ball::setupStatic(m_hkSystem->m_World);

		m_hkSystem->m_World->markForWrite();

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
		m_hkSystem->m_World->addEntity(m_table);
		shape->removeReference();

		m_hkSystem->m_World->unmarkForWrite();

		// add 2 sample balls
		{
			VisualObjectParameters param1 =
			{
				"ball1", "Sphere",
				boost::assign::map_list_of(0, "Pool/Balls/P1").to_container(VisualObjectParameters::MaterialNameMap_t()),
			};
			VisualObjectParameters param2 =
			{
				"ball2", "Sphere",
				boost::assign::map_list_of(0, "Pool/Balls/P2").to_container(VisualObjectParameters::MaterialNameMap_t()),
			};
			addBall(param1, 0, 50, 2, 100, 3);
			addBall(param2, 0, 60, 0, 100, 3);
		}
	}

	void Game::addBall(const VisualObjectParameters& param, Real x, Real y, Real z, Real mass, Real radius)
	{
		VisualObjectPtr vobj = m_fnCreateVisualObject(param);
		Ball* ball = new Ball(vobj, Vector(x, y, z) , mass , radius);

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
		m_hkSystem->simulate(elapsedTime);

		updateAllBalls();
	}
}