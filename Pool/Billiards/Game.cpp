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
	const TableParams Game::m_tableParams = {5.36f, 17.2f, 31.3f,
											 0.45f, 1.5f,
											 0.0f};

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

		setup();
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

		creatTable();

		// add sample balls
		{
			WorldWritingLock wlock(m_hkSystem->m_World);

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
			VisualObjectParameters param3 =
			{
				"ball3", "Sphere",
				boost::assign::map_list_of(0, "Pool/Balls/P3").to_container(VisualObjectParameters::MaterialNameMap_t()),
			};
			VisualObjectParameters param4 =
			{
				"ball4", "Sphere",
				boost::assign::map_list_of(0, "Pool/Balls/P4").to_container(VisualObjectParameters::MaterialNameMap_t()),
			};
			VisualObjectParameters param5 =
			{
				"ball5", "Sphere",
				boost::assign::map_list_of(0, "Pool/Balls/P5").to_container(VisualObjectParameters::MaterialNameMap_t()),
			};
			VisualObjectParameters param6 =
			{
				"ball6", "Sphere",
				boost::assign::map_list_of(0, "Pool/Balls/P6").to_container(VisualObjectParameters::MaterialNameMap_t()),
			};
			VisualObjectParameters param7 =
			{
				"ball7", "Sphere",
				boost::assign::map_list_of(0, "Pool/Balls/P7").to_container(VisualObjectParameters::MaterialNameMap_t()),
			};
			addBall(param1, 1e-4, 41, 0, 1, 0.32);
			addBall(param2, 0, 42, 1e-4, 1, 0.32);
			addBall(param3, 0, 43, 0, 1, 0.32);
			addBall(param4, 0, 44, 0, 1, 0.32);
			addBall(param6, 0, 45, 0, 1, 0.32);
			addBall(param5, 0, 46, 0, 1, 0.32);
			addBall(param7, 0, 47, 0, 1, 0.32);
		}
	}

	void Game::creatTable()
	{
		WorldWritingLock wlock(m_hkSystem->m_World);

		// create the table shape
		//
		hkArray<hkpShape*> shapeArray;

		// tableBoard

		float boardHeight = 2.0f;
		hkpBoxShape* tableBoard = new hkpBoxShape(hkVector4(m_tableParams.lenth/2, boardHeight/2, m_tableParams.width/2), 0);

		hkTransform t ;
		t = t.getIdentity();
		hkVector4 trans = hkVector4(0.0f, 0.0f, 0.0f);
		t.setTranslation( trans );
		hkpTransformShape* tableBoardTrans = new hkpTransformShape( tableBoard , t);
		shapeArray.pushBack(tableBoardTrans);

		// vbaffles
		float vbaffleLenth = m_tableParams.lenth - 3*2*m_tableParams.holeRadius;

		hkpBoxShape* vbaffle = new hkpBoxShape( hkVector4(vbaffleLenth/2, 
			                                    m_tableParams.baffleHeight/2, 
											    m_tableParams.baffleWidth/2), 0);

		float vX = vbaffleLenth/2 + m_tableParams.holeRadius/2;
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
		m_hkSystem->m_World->addEntity(m_table);

		m_table->setPosition(hkVector4(0, 7.2, 0));
		table->removeReference();
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

	/*Vector Game::getPosOfBall(int number) const
	{
		//assert(number > (int)m_ballList.size());

		return m_ballList[number]->getPos();
	}

	Quaternion Game::getRotationOfBall(int number) const
	{
		//assert(number > (int)m_ballList.size());

		return m_ballList[number]->getRotation();
	}*/

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
