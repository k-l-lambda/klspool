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
		Ball();
		Ball(const hkVector4& pos, hkReal mass, hkReal radius);

		void creatRigidBody();

		void applyForce(const hkVector4& force, const hkVector4& pos, hkReal deltaTime);

		void setPos(const hkVector4& pos);

		bool isInHole() const;
		bool isStill() const;

		hkVector4 getPos() const;
		hkQuaternion getRotation() const;

		void update();

		~Ball();

		static void setupStatic(hkpWorld* hw);

	private:
			
		hkReal m_radius;
		hkReal m_mass;
		hkVector4 m_position;
		hkQuaternion m_rotation;

		hkpRigidBody* m_havokRigid;

		static hkpWorld* m_hkpWorld;
	};
}



#endif	// !defined(__BALL_H__)
