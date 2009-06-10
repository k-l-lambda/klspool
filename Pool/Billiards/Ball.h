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
		friend class HolePhantom;

	public:
		Ball(hkpWorld* world, const VisualObjectPtr& vobj);
		~Ball();

		void	resetRigidBody(const Vector& position, Real mass, Real radius);
		void	releaseRigidBody();

		void	applyForce(const Vector& force, const Vector& pos, Real deltaTime);

		void	setPosition(const Vector& pos);
		void	setVelocity(const Vector& vel);

		//bool	isInHole() const;
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

		bool			m_IsInHole;

		hkpWorld*		m_World;
#pragma warning(suppress: 4251)	// 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
		hkpRigidBodyPtr	m_RigidBody;

#pragma warning(suppress: 4251)	// 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
		VisualObjectPtr	m_VisualObject;
	};
}



#endif	// !defined(__BALL_H__)
