/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef	__BLDGAME_H__
#define	__BLDGAME_H__



#include "HavokSystem.h"
#include "Ball.h"

#include <boost\noncopyable.hpp>


namespace Billiards
{
	class BILLIARDS_API	Game
		: boost::noncopyable
#pragma warning(suppress: 4275)	// non ¨C DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
	{
	public:
		explicit Game(const VisualObjectCreationFunctor& fnCreateVisualObject);
		~Game();

		// test if all the balls is still
		//bool isStill();

		//Vector getPosOfBall(int number) const;
		//Quaternion getRotationOfBall(int number) const;

		// set the designated ball stilled
		//void disableBall(int number);

		void simulate(Real elapsedTime);

	private:
		void setup();

		void createPhysicsScene();

		void updateAllBalls();

		void addBall(const VisualObjectParameters& param, Real x, Real y, Real z, Real mass, Real radius);
		void deleteBall(int number);

		// give some impuse to the designated ball
		void applyForceOnBall(const Vector& force, const Vector& pos, Real deltaTime, int number);

		void setPosOfBall(Real x, Real y, Real z, int number);

	private:
#pragma warning(suppress: 4251)	// 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
		std::vector<Ball*> m_ballList;
		hkpRigidBody* m_table;

		HavokSystem* m_hkSystem;

#pragma warning(suppress: 4251)	// 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
		VisualObjectCreationFunctor		m_fnCreateVisualObject;
	};
}



#endif	// !defined(__BLDGAME_H__)
