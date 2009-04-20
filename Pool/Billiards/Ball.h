/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef __BALL_H__
#define __BALL_H__



#include "Billiards.h"


namespace Billiards
{
	// This class synchronizes the graphic and the phsix of the ball,
	// and supports some tests and sets funtions

	class Ball
	{
	public:

		void creatRigidBody();

		void setPos(hkVector4& pos);

		bool isInHole();
		bool isStill();

		hkVector4 getPos();
		hkQuaternion getRotation();

		~Ball();

		static void setupStatic(hkpWorld* kw);

	private:
			
		hkReal m_radius;
		hkReal m_mass;
		hkVector4 m_position;

		hkpRigidBody* m_havokRigid;

		static hkpWorld* m_hkpWorld;
	};
}



#endif	// !defined(__BALL_H__)
