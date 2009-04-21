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


namespace Billiards
{
	class BILLIARDS_API	BldGame
	{
	public:
		BldGame();

		~BldGame();

		void setup();

		virtual void createPhysicsScene();

		// test if all the balls is still
		bool isStill();

		// give some impuse to the designated ball
		void applyForceOnBall(const hkVector4& force, const hkVector4& pos, hkReal deltaTime, int number);

		void addBall(hkReal x, hkReal y, hkReal z, hkReal mass, hkReal radius);
		void deleteBall(int number);

		void setPosOfBall(hkReal x, hkReal y, hkReal z, int number);

		hkVector4 getPosOfBall(int number) const;
		hkQuaternion getRotationOfBall(int number) const;

		// set the designated ball stilled
		void disableBall(int number);

		void updateAllBalls();

		void simulate();

	private:
		// forbid copy
		BldGame(const BldGame& game);

#pragma warning(suppress: 4251)	// 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
		std::vector<Ball*> m_ballList;
		hkpRigidBody* m_table;

		HavokSystem* hkSystem;
	};
}



#endif	// !defined(__BLDGAME_H__)
