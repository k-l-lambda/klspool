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

	class BILLIARDS_API	Ball
	{
	public:
		Ball(hkpWorld* world, const VisualObjectPtr& vobj);
		~Ball();

		void	resetRigidBody(const Vector& position, Real mass, Real radius);
		void	releaseRigidBody();

		void	applyForce(const Vector& force, const Vector& pos, Real deltaTime);

		void	setPosition(const Vector& pos);
		void	setVelocity(const Vector& vel);

		bool	isInHole() const;
		bool	isStill() const;

		Real	getRadius() const;

		Vector				getPosition() const;
		const Quaternion&	getRotation() const;
		Vector				getVelocity() const;

		void update();

	private:
		Real			m_Radius;
		/*Real			m_mass;
		Vector			m_position;
		Quaternion		m_rotation;*/

		hkpWorld*		m_World;
		hkpRigidBodyPtr	m_RigidBody;

		VisualObjectPtr	m_VisualObject;
	};
}



#endif	// !defined(__BALL_H__)
