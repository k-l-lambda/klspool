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
#include "ThreadAccessLock.h"
#include <iostream>


namespace Billiards
{
	struct TableParams
	{
		float height;
		float width;
		float lenth;

		float baffleHeight;
		float baffleWidth;

		float holeRadius;
	};

	static const TableParams s_TableParams = {5.36f, 15.45f, 29.3f, 0.45f, 0.7f, 0.6f};

	static const Real s_BallRadius = 0.27;
	static const Real s_BallMass = 1;


	Game::Game(const VisualObjectCreationFunctor& fnCreateVisualObject)
		: /*m_table(NULL)
		, m_MainBall(NULL)
		,*/ m_HavokSystem(NULL)
		, m_fnCreateVisualObject(fnCreateVisualObject)
	{
		m_HavokSystem = new HavokSystem();

		// set defult result of m_fnCreateVisualObject to null
		if(!m_fnCreateVisualObject)
		{
			struct LocalScope
			{
				static VisualObjectPtr null(...)	{return VisualObjectPtr();};
			};
			m_fnCreateVisualObject = &LocalScope::null;
		}

		setup();
	}

	void Game::setup()
	{
		if(m_HavokSystem)
		{
			m_HavokSystem->setup();

			createPhysicsScene();
		}
	}

	Game::~Game()
	{
		m_table.reset();
		m_baffles.reset();

		/*for(std::vector<Ball*>::iterator iter = m_ballList.begin();
			iter != m_ballList.end(); ++ iter)
			delete *iter;*/
		m_ballList.clear();
		m_MainBall.reset();

		if(m_HavokSystem)
			delete m_HavokSystem;
	}

	void Game::createPhysicsScene()
	{
		// set the world pointer in ball
		//Ball::setupStatic(m_HavokSystem->getWorld());

		creatTable();

		// add sample balls
		{
			WorldWritingLock wlock(m_HavokSystem->getWorld());

			addBall("ball1", "Pool/Balls/P1", Vector(1e-4, 10, 0));
			addBall("ball2", "Pool/Balls/P2", Vector(0, 11, 1e-4));
			addBall("ball3", "Pool/Balls/P3", Vector(0, 12, 0));
			addBall("ball4", "Pool/Balls/P4", Vector(0, 13, 0));
			addBall("ball5", "Pool/Balls/P5", Vector(0, 14, 0));
			addBall("ball6", "Pool/Balls/P6", Vector(0, 15, 0));
			addBall("ball7", "Pool/Balls/P7", Vector(0, 16, 0));

			m_MainBall = m_ballList.front();
		}
	}

	void Game::creatTable()
	{
		WorldWritingLock wlock(m_HavokSystem->getWorld());

		// create the table shape
		//
		hkArray<hkpShape*> shapeArray;

		// tableBoard

		float boardHeight = 2.0f;
		hkpBoxShape* tableBoard = new hkpBoxShape(Vector(s_TableParams.lenth/2 - s_TableParams.baffleWidth, boardHeight/2, s_TableParams.width/2 - s_TableParams.baffleWidth), 0);

		// creat rigidBody

		{
			hkpRigidBodyCinfo ci;
			ci.m_shape = tableBoard;
			ci.m_motionType = hkpMotion::MOTION_FIXED;
			ci.m_position = Vector(0, 0, 0);
			ci.m_qualityType = HK_COLLIDABLE_QUALITY_FIXED;
			ci.m_friction = 0.2f;
			ci.m_restitution = 0.2f;
			ci.m_allowedPenetrationDepth = 1e-2f;

			m_table.reset(new hkpRigidBody(ci));
			m_HavokSystem->getWorld()->addEntity(m_table.get());
		}

		// vbaffles
		hkTransform t ;
		t = t.getIdentity();
		Vector trans = Vector(0.0f, 0.0f, 0.0f);

		float vbaffleLenth = s_TableParams.lenth / 2 - 3 * s_TableParams.holeRadius;

		hkpBoxShape* vbaffle = new hkpBoxShape(Vector(vbaffleLenth/2,
			s_TableParams.baffleHeight, s_TableParams.baffleWidth/2), 0);

		float vX = vbaffleLenth/2 + s_TableParams.holeRadius;
		float vY = boardHeight/2 + s_TableParams.baffleHeight/2;
		float vZ = s_TableParams.width/2 - s_TableParams.baffleWidth/2;

		trans = Vector(vX, vY, vZ);
		t.setTranslation(trans);
		hkpTransformShape* vbaffleTrans1 = new hkpTransformShape(vbaffle, t);
		shapeArray.pushBack(vbaffleTrans1);

		trans = Vector(vX, vY, -vZ);
		t.setTranslation(trans);
		hkpTransformShape* vbaffleTrans2 = new hkpTransformShape(vbaffle, t);
		shapeArray.pushBack(vbaffleTrans2);

		trans = Vector(-vX, vY, vZ);
		t.setTranslation(trans);
		hkpTransformShape* vbaffleTrans3 = new hkpTransformShape(vbaffle, t);
		shapeArray.pushBack(vbaffleTrans3);

		trans = Vector(-vX, vY, -vZ);
		t.setTranslation(trans);
		hkpTransformShape* vbaffleTrans4 = new hkpTransformShape(vbaffle, t);
		shapeArray.pushBack(vbaffleTrans4);

		// nbaffles

		float nbaffleLenth = s_TableParams.width - 2*2*s_TableParams.holeRadius;
		hkpBoxShape* nbaffle = new hkpBoxShape(Vector(s_TableParams.baffleWidth/2, 
			s_TableParams.baffleHeight, 
			nbaffleLenth/2), 0);

		float nX = s_TableParams.lenth/2 - s_TableParams.baffleWidth/2;
		float nY = vY;
		float nZ = 0;

		trans = Vector(nX, nY, nZ);
		t.setTranslation(trans);
		hkpTransformShape* nbaffleTrans1 = new hkpTransformShape(nbaffle ,t);
		shapeArray.pushBack(nbaffleTrans1);

		trans = Vector(-nX, nY, nZ);
		t.setTranslation(trans);
		hkpTransformShape* nbaffleTrans2 = new hkpTransformShape(nbaffle ,t);
		shapeArray.pushBack(nbaffleTrans2);

		hkpListShape* baffles = new hkpListShape(&shapeArray[0], shapeArray.getSize());

		{
			hkpRigidBodyCinfo ci;
			ci.m_shape = baffles;
			ci.m_motionType = hkpMotion::MOTION_FIXED;
			ci.m_position = Vector(0, 0, 0);
			ci.m_qualityType = HK_COLLIDABLE_QUALITY_FIXED;
			ci.m_friction = 0.01f;
			ci.m_restitution = 0.92f;
			ci.m_allowedPenetrationDepth = 1e-4f;

			m_baffles.reset(new hkpRigidBody(ci));
			m_HavokSystem->getWorld()->addEntity(m_baffles.get());
		}

		// Do some translate according to the real model
		m_baffles->setPosition(Vector(0, 7.2, -0.1));
		m_table->setPosition(Vector(0, 7.2, -0.1));

		tableBoard->removeReference();
		baffles->removeReference();
	}

	void Game::addBall(const std::string& name, const std::string& materialname, const Vector& position)
	{
		VisualObjectParameters param =
		{
			name, "Sphere",
			boost::assign::map_list_of(0, materialname).to_container(VisualObjectParameters::MaterialNameMap_t()),
			boost::make_tuple(s_BallRadius, s_BallRadius, s_BallRadius),
		};
		VisualObjectPtr vobj = m_fnCreateVisualObject(param);
		BallPtr ball(new Ball(m_HavokSystem->getWorld(), vobj));

		ball->resetRigidBody(position , s_BallMass , s_BallRadius);

		m_ballList.push_back(ball);
	}

	void Game::setPosOfBall(Real x, Real y, Real z, int number)
	{
		//assert(number > (int)m_ballList.size());

		m_ballList[number]->setPosition(Vector(x, y, z));
	}

	void Game::applyForceOnBall(const Vector& force, const Vector& pos, Real deltaTime, int number)
	{
		//assert(number > (int)m_ballList.size());

		m_ballList[number]->applyForce(force, pos, deltaTime);
	}

	/*void Game::deleteBall(int number)
	{
	//assert(number > (int)m_ballList.size());

	delete m_ballList[number];

	std::vector<Ball*>::iterator iter = m_ballList.begin() + number;
	m_ballList.erase(iter);
	}*/

	void Game::updateAllBalls()
	{
		for(int i = 0; i < (int)m_ballList.size(); ++i)
			m_ballList[i]->update();
	}

	void Game::simulate(Real elapsedTime)
	{
		m_HavokSystem->simulate(elapsedTime);

		updateAllBalls();

		// recycle fallen balls
		for(size_t i = 0; i < m_ballList.size(); ++ i)
			if(m_ballList[i]->getPosition()(1) < 0)
			{
				m_ballList[i]->setPosition(Vector(0, 10, 0));

				Vector v = m_ballList[i]->getVelocity();
				v.mul4(1e-1f);
				m_ballList[i]->setVelocity(v);
			}
	}

	void Game::shot(const Vector& impulse, const Vector& pos)
	{
		static const Real s_ShotTime = 1e-3;

		Vector force(impulse);
		force.mul4(1 / s_ShotTime);

		Vector p(pos);
		p.normalize3();
		p.mul4(s_BallRadius);
		Vector wp(m_MainBall->getPosition());
		wp.add4(p);

		m_MainBall->applyForce(force, wp, s_ShotTime);
	}
}
