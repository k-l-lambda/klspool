/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include <iostream>

#pragma warning(push)
#pragma warning(disable: 4819)	// 'bytes' bytes padding added after construct 'member_name'
#include <boost\assign.hpp>
#include <boost\format.hpp>
#pragma warning(pop)

#include "HavokSystem.h"
#include "Game.h"
#include "VisualObject.h"
#include "ThreadAccessLock.h"
#include "GameLayout.h"


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

	static const TableParams s_TableParams = {7.2f, 15.45f, 29.3f, 0.45f, 0.7f, 0.6f};


	Game::Game(const VisualObjectCreationFunctor& fnCreateVisualObject)
		: m_HavokSystem(NULL)
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

		m_Balls.clear();
		m_MainBall.reset();

		if(m_HavokSystem)
			delete m_HavokSystem;
	}

	void Game::createPhysicsScene()
	{
		creatTable();
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
		m_baffles->setPosition(Vector(0, s_TableParams.height, -0.1));
		m_table->setPosition(Vector(0, s_TableParams.height, -0.1));

		tableBoard->removeReference();
		baffles->removeReference();
	}

	void Game::updateAllBalls()
	{
		for(int i = 0; i < (int)m_Balls.size(); ++i)
			m_Balls[i]->update();
	}

	void Game::simulate(Real elapsedTime)
	{
		m_HavokSystem->simulate(elapsedTime);

		updateAllBalls();

		// recycle fallen balls
		for(size_t i = 0; i < m_Balls.size(); ++ i)
			if(m_Balls[i]->getPosition().y < 0)
			{
				m_Balls[i]->setPosition(Vector(0, 10, 0));
				m_Balls[i]->setVelocity(m_Balls[i]->getVelocity() / 10);
			}
	}

	void Game::shot(const Vector& impulse, const Vector& pos)
	{
		static const Real s_ShotTime = 1e-3;

		m_MainBall->applyForce(impulse / s_ShotTime, m_MainBall->getPosition() + pos.normalisedCopy() * m_MainBall->getRadius(), s_ShotTime);
	}

	void Game::loadBallConfigSet(const std::string& setname)
	{
		if(m_LoadedBallConfigSets.insert(setname).second)
		{
			static const std::string s_ArchivePath = "./BallConfigSets/%s.xml";

			BallConfigSet bcset = BallConfigSet::loadFile((boost::format(s_ArchivePath) % setname).str());

			for(BallConfigSet::const_iterator it = bcset.begin(); it != bcset.end(); ++ it)
			{
				m_BallConfigMap[(boost::format("%s/%s") % bcset.NameSpace % it->Name).str()] = *it;
			}
		}
	}

	void Game::deployLayout(const GameLayout& layout)
	{
		m_Balls.clear();
		m_MainBall.reset();

		for(size_t i = 0; i < layout.BallsLayout.size(); ++ i)
		{
			const GameLayout::BallInfo& ballinfo = layout.BallsLayout[i];

			if(!m_BallConfigMap.count(ballinfo.Config))
				throw std::logic_error("cannot find ball config \"" + ballinfo.Config + "\"");

			const BallConfig& config = m_BallConfigMap[ballinfo.Config];

			VisualObjectParameters param =
			{
				(boost::format("ball%d") % i).str(), "Sphere",
				boost::assign::map_list_of(0, "Pool/Balls/" + config.Material).to_container(VisualObjectParameters::MaterialNameMap_t()),
				Vector(config.Redius, config.Redius, config.Redius),
			};
			VisualObjectPtr vobj = m_fnCreateVisualObject(param);
			BallPtr ball(new Ball(m_HavokSystem->getWorld(), vobj));

			ball->resetRigidBody(ballinfo.Position + Vector(0, s_TableParams.height + 1.f, 0), config.Mass ,config.Redius);

			m_Balls.push_back(ball);
		}

		if(!m_Balls.empty())
			m_MainBall = m_Balls.front();

		// resolve unstable status
		// TODO: change the end condition to all balls still
		for(size_t i = 0; i < 100; ++ i)
			m_HavokSystem->simulate(0.01f);
	}

	ConstBallPtr Game::getMainBall() const
	{
		return m_MainBall;
	}
}
