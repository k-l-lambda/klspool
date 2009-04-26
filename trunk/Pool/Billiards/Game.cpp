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


namespace Billiards
{
	const TableParams Game::m_tableParams = {5.36f, 15.45f, 29.3f,
											 0.45f, 0.7f,
											 0.6f};

	static const Real s_BallRadius = 0.27;

	Game::Game(const VisualObjectCreationFunctor& fnCreateVisualObject)
		: m_table(NULL)
		, m_MainBall(NULL)
		, m_HavokSystem(NULL)
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
		for(std::vector<Ball*>::iterator iter = m_ballList.begin();
			iter != m_ballList.end(); ++ iter)
			delete *iter;

		if(m_HavokSystem)
			delete m_HavokSystem;
	}

	void Game::createPhysicsScene()
	{
		// set the world pointer in ball
		Ball::setupStatic(m_HavokSystem->m_World);
		m_HavokSystem->m_World->setGravity(Vector(0, -98, 0));

		creatTable();

		// add sample balls
		{
			WorldWritingLock wlock(m_HavokSystem->m_World);

			addBall("ball1", "Pool/Balls/P1", 1e-5, 11, 0, 1, s_BallRadius);
			addBall("ball2", "Pool/Balls/P2", 0, 12, 1e-5, 1, s_BallRadius);
			addBall("ball3", "Pool/Balls/P3", 0, 13, 0, 1, s_BallRadius);
			addBall("ball4", "Pool/Balls/P4", 0, 14, 0, 1, s_BallRadius);
			addBall("ball5", "Pool/Balls/P5", 0, 15, 0, 1, s_BallRadius);
			addBall("ball6", "Pool/Balls/P6", 0, 16, 0, 1, s_BallRadius);
			addBall("ball7", "Pool/Balls/P7", 0, 17, 0, 1, s_BallRadius);

			m_MainBall = m_ballList.front();
		}
	}

	void Game::creatTable()
	{
		WorldWritingLock wlock(m_HavokSystem->m_World);

		// create the table shape
		//
		hkArray<hkpShape*> shapeArray;

		// tableBoard

		float boardHeight = 2.0f;
		hkpBoxShape* tableBoard = new hkpBoxShape(hkVector4(m_tableParams.lenth/2 - m_tableParams.baffleWidth, boardHeight/2, m_tableParams.width/2 - m_tableParams.baffleWidth), 0);

		hkTransform t ;
		t = t.getIdentity();
		hkVector4 trans = hkVector4(0.0f, 0.0f, 0.0f);
		t.setTranslation( trans );
		hkpTransformShape* tableBoardTrans = new hkpTransformShape( tableBoard , t);
		shapeArray.pushBack(tableBoardTrans);

		// vbaffles
		float vbaffleLenth = m_tableParams.lenth/2 - 3*m_tableParams.holeRadius;

		hkpBoxShape* vbaffle = new hkpBoxShape( hkVector4(vbaffleLenth/2, 
			                                    m_tableParams.baffleHeight/2, 
											    m_tableParams.baffleWidth/2), 0);

		float vX = vbaffleLenth/2 + m_tableParams.holeRadius;
		float vY = boardHeight/2 + m_tableParams.baffleHeight/2;
		float vZ = m_tableParams.width/2 - m_tableParams.baffleWidth/2;

		trans = hkVector4(vX, vY, vZ);
		t.setTranslation( trans );
		hkpTransformShape* vbaffleTrans1 = new hkpTransformShape( vbaffle, t );
		shapeArray.pushBack(vbaffleTrans1);

		trans = hkVector4(vX, vY, -vZ);
		t.setTranslation( trans );
		hkpTransformShape* vbaffleTrans2 = new hkpTransformShape( vbaffle,  t );
		shapeArray.pushBack(vbaffleTrans2);

		trans = hkVector4(-vX, vY, vZ);
		t.setTranslation( trans );
		hkpTransformShape* vbaffleTrans3 = new hkpTransformShape( vbaffle,  t );
		shapeArray.pushBack(vbaffleTrans3);

		trans = hkVector4(-vX, vY, -vZ);
		t.setTranslation( trans );
		hkpTransformShape* vbaffleTrans4 = new hkpTransformShape( vbaffle,  t );
		shapeArray.pushBack(vbaffleTrans4);

		// nbaffles

		float nbaffleLenth = m_tableParams.width - 2*2*m_tableParams.holeRadius;
		hkpBoxShape* nbaffle = new hkpBoxShape(hkVector4(m_tableParams.baffleWidth/2, 
			                                   m_tableParams.baffleHeight/2, 
											   nbaffleLenth/2), 0);

		float nX = m_tableParams.lenth/2 - m_tableParams.baffleWidth/2;
		float nY = vY;
		float nZ = 0;

		trans = hkVector4(nX, nY, nZ);
		t.setTranslation( trans );
		hkpTransformShape* nbaffleTrans1 = new hkpTransformShape( nbaffle ,t);
		shapeArray.pushBack(nbaffleTrans1);

		trans = hkVector4(-nX, nY, nZ);
		t.setTranslation( trans );
		hkpTransformShape* nbaffleTrans2 = new hkpTransformShape( nbaffle ,t);
		shapeArray.pushBack(nbaffleTrans2);

		hkpListShape* table = new hkpListShape(&shapeArray[0], shapeArray.getSize());

		// creat rigidBody

		hkpRigidBodyCinfo ci;

		ci.m_shape = table;
		ci.m_motionType = hkpMotion::MOTION_FIXED;
		ci.m_position = Vector(0, 0, 0);
		ci.m_qualityType = HK_COLLIDABLE_QUALITY_FIXED;

		m_table = new hkpRigidBody(ci);
		m_HavokSystem->m_World->addEntity(m_table);

		m_table->setPosition(hkVector4(0, 7.2, -0.1));
		table->removeReference();
	}

	void Game::addBall(const std::string& name, const std::string& materialname, Real x, Real y, Real z, Real mass, Real radius)
	{
		VisualObjectParameters param =
		{
			name, "Sphere",
			boost::assign::map_list_of(0, materialname).to_container(VisualObjectParameters::MaterialNameMap_t()),
		};
		VisualObjectPtr vobj = m_fnCreateVisualObject(param);
		Ball* ball = new Ball(vobj, Vector(x, y, z) , mass , radius);

		ball->creatRigidBody();

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
				m_ballList[i]->setPosition(Vector(0, 10, 0));
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
