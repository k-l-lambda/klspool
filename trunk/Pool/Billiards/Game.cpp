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

		float boardThickness;
	};

	static const TableParams s_TableParams = {7.2f, 15.45f, 29.3f, 0.45f, 0.7f, 0.6f, 2.0f};


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
		m_table->removeReference();
		m_table.reset();
		m_baffles->removeReference();
		m_baffles.reset();

		m_Balls.clear();
		m_HolePhantoms.clear();
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

		hkpSimpleMeshShape* table = new hkpSimpleMeshShape();

		loadMesh(table, "table.hkm");
	
		{
			hkpRigidBodyCinfo ci;
			ci.m_shape = table;
			ci.m_motionType = hkpMotion::MOTION_FIXED;
			ci.m_position = Vector(0, 8, -0.15f);
			ci.m_qualityType = HK_COLLIDABLE_QUALITY_FIXED;
			ci.m_friction = 0.01f;
			ci.m_restitution = 0.92f;
			ci.m_allowedPenetrationDepth = 1e-2f;

			m_baffles.reset(new hkpRigidBody(ci));
			m_HavokSystem->getWorld()->addEntity(m_baffles.get());
		
			table->removeReference();
		}

		// Create Table board
		int numVertices = 16;

		// 16 = 4 (size of "each float group", 3 for x,y,z, 1 for padding) * 4 (size of float)
		int stride = sizeof(float) * 4;

		float vertices[] = {
			13.92f, -0.255f, 6.554f, 0.0f,
			13.54f, -0.255f, 6.928f, 0.0f,
			13.54f, -0.655f, 6.928f, 0.0f,
			13.92f, -0.655f, 6.554f, 0.0f,
				   
			-13.92f, -0.255f, 6.554f, 0.0f,
			-13.54f, -0.255f, 6.928f, 0.0f,
			-13.54f, -0.655f, 6.928f, 0.0f,
			-13.92f, -0.655f, 6.554f, 0.0f,
				   
			-13.92f, -0.255f, -6.554f, 0.0f,
			-13.54f, -0.255f, -6.928f, 0.0f,
			-13.54f, -0.655f, -6.928f, 0.0f,
			-13.92f, -0.655f, -6.554f, 0.0f,
				   
			13.92f, -0.255f, -6.554f, 0.0f,
			13.54f, -0.255f, -6.928f, 0.0f,
			13.54f, -0.655f, -6.928f, 0.0f,
			13.92f, -0.655f, -6.554f, 0.0f
		};

		// Shape construction
		hkArray<hkVector4> planeEquations;
		hkGeometry geom;

		hkStridedVertices stridedVerts;
		{
			stridedVerts.m_numVertices = numVertices;
			stridedVerts.m_striding = stride;
			stridedVerts.m_vertices = vertices;
		}

		hkGeometryUtility::createConvexGeometry( stridedVerts, geom, planeEquations );

		{
			stridedVerts.m_numVertices = geom.m_vertices.getSize();
			stridedVerts.m_striding = sizeof(hkVector4);
			stridedVerts.m_vertices = &(geom.m_vertices[0](0));
		}

		hkpConvexVerticesShape* tableBoardS = new hkpConvexVerticesShape(stridedVerts, planeEquations);

		// creat rigidBody

		{
			hkpRigidBodyCinfo ci;
			ci.m_shape = tableBoardS;
			ci.m_motionType = hkpMotion::MOTION_FIXED;
			ci.m_position = Vector(0, s_TableParams.height + 1.2f, -0.03f);
			ci.m_qualityType = HK_COLLIDABLE_QUALITY_FIXED;
			ci.m_friction = 0.2f;
			ci.m_restitution = 0.2f;
			ci.m_allowedPenetrationDepth = 1e-2f;

			m_table.reset(new hkpRigidBody(ci));
			m_HavokSystem->getWorld()->addEntity(m_table.get());

			tableBoardS->removeReference();
		}

		//creatPhantoms();
	}

	void Game::loadMesh(hkpSimpleMeshShape* shap, std::string fileName) const
	{
		std::ifstream inf;
		inf.open(fileName.c_str());
		if(inf.fail())
			return;

		std::string tmp;
		inf>>tmp;

		int vetexCount = 0;
		inf>>vetexCount;

		for(int i=0 ; i<vetexCount; ++i)
		{
			float x, y, z;
			inf>>x>>y>>z;
			hkVector4 tmp(x,y,z);
			shap->m_vertices.pushBack(tmp);
		}

		inf>>tmp;
		int triangleCount = 0;
		inf>>triangleCount;
		for(int i=0 ; i<triangleCount; ++i)
		{
			hkpSimpleMeshShape::Triangle tmp;
			inf>>tmp.m_a>>tmp.m_b>>tmp.m_c;

			shap->m_triangles.pushBack(tmp);
		}

		inf.close();
	}

	void Game::creatPhantoms()
	{
		// Create hole phatoms

		// Lock the world for write.
		WorldWritingLock wlock(m_HavokSystem->getWorld());

		HolePhantom::setBallList(&m_Balls);
		
		Vector pointA(0.0f,  - 0.7f, 0.0f);
		Vector pointB(0.0f,  + 0.7f, 0.0f);
		hkpCylinderShape* holeShape = new hkpCylinderShape(pointA, pointB, 0.3f);

		{
			hkTransform t ;
			t = t.getIdentity();
			Vector trans = Vector(0.0f, 
				                  s_TableParams.height + s_TableParams.boardThickness/2, 
								  s_TableParams.width / 2 + 0.2f);
			t.setTranslation(trans);

			HolePhantomPtr p(new HolePhantom(holeShape, t));
			m_HolePhantoms.push_back(p);

			m_HavokSystem->getWorld()->addPhantom(p.get());
		}

		{
			hkTransform t ;
			t = t.getIdentity();
			Vector trans = Vector(0.0f, 
				                  s_TableParams.height + s_TableParams.boardThickness/2, 
								  -s_TableParams.width / 2 - 0.4f);
			t.setTranslation(trans);

			HolePhantomPtr p(new HolePhantom(holeShape, t));
			m_HolePhantoms.push_back(p);

			m_HavokSystem->getWorld()->addPhantom(p.get());
		}

		{
			hkTransform t ;
			t = t.getIdentity();
			Vector trans = Vector(s_TableParams.lenth/2 /*- s_TableParams.baffleWidth*/, 
				                  s_TableParams.height + s_TableParams.boardThickness/2, 
								  -(s_TableParams.width / 2 /*- s_TableParams.baffleWidth*/ + 0.1f));
			t.setTranslation(trans);

			HolePhantomPtr p(new HolePhantom(holeShape, t));
			m_HolePhantoms.push_back(p);

			m_HavokSystem->getWorld()->addPhantom(p.get());
		}

		{
			hkTransform t ;
			t = t.getIdentity();
			Vector trans = Vector(s_TableParams.lenth/2 /*- s_TableParams.baffleWidth*/, 
				                  s_TableParams.height + s_TableParams.boardThickness/2, 
								  s_TableParams.width / 2 /*- s_TableParams.baffleWidth*/);
			t.setTranslation(trans);

			HolePhantomPtr p(new HolePhantom(holeShape, t));
			m_HolePhantoms.push_back(p);

			m_HavokSystem->getWorld()->addPhantom(p.get());
		}

		{
			hkTransform t ;
			t = t.getIdentity();
			Vector trans = Vector(-(s_TableParams.lenth/2 /*- s_TableParams.baffleWidth*/), 
				                  s_TableParams.height + s_TableParams.boardThickness/2, 
								  -(s_TableParams.width / 2 /*- s_TableParams.baffleWidth*/ + 0.1f));
			t.setTranslation(trans);

			HolePhantomPtr p(new HolePhantom(holeShape, t));
			m_HolePhantoms.push_back(p);

			m_HavokSystem->getWorld()->addPhantom(p.get());
		}

		{
			hkTransform t ;
			t = t.getIdentity();
			Vector trans = Vector(-(s_TableParams.lenth/2 /*- s_TableParams.baffleWidth*/), 
				                  s_TableParams.height + s_TableParams.boardThickness/2, 
								  s_TableParams.width / 2 /*- s_TableParams.baffleWidth*/);
			t.setTranslation(trans);

			HolePhantomPtr p(new HolePhantom(holeShape, t));
			m_HolePhantoms.push_back(p);

			m_HavokSystem->getWorld()->addPhantom(p.get());
		}

		holeShape->removeReference();
	}

	void Game::updateAllBalls()
	{
		for(size_t i = 0; i < m_Balls.size(); ++ i)
			m_Balls[i]->update();
	}

	bool Game::isStill() const
	{
		for(size_t i = 0; i < m_Balls.size(); ++ i)
			if(!m_Balls[i]->isStill())
				return false;

		return true;
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
		static const Real s_ShotTime = 1e-3f;

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
				boost::assign::map_list_of(0, "Pool/Balls/" + config.Material),
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
