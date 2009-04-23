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
		Ball(VisualObjectPtr vobj, const Vector& pos, Real mass, Real radius);

		void creatRigidBody();

		void applyForce(const Vector& force, const Vector& pos, Real deltaTime);

		void setPos(const Vector& pos);

		bool isInHole() const;
		bool isStill() const;

		const Vector&		getPos() const;
		const Quaternion&	getRotation() const;

		void update();

		~Ball();

		static void setupStatic(hkpWorld* hw);

	private:
		Real			m_radius;
		Real			m_mass;
		Vector			m_position;
		Quaternion		m_rotation;

		hkpRigidBody*	m_havokRigid;

		VisualObjectPtr	m_VisualObject;

		static hkpWorld* s_hkpWorld;
	};
}



#endif	// !defined(__BALL_H__)
