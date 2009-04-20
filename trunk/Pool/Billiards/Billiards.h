/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef	__BILLIARDS_H__
#define	__BILLIARDS_H__

#include "HavokSystem.h"
#include "Ball.h"

#include <vector>
#include <assert.h>

class Billiards : public HavokSystem
{
public:
	virtual void createPhysicsScene();

	// test if all the balls is still
	bool isStill();

	// give some impuse to the designated ball
	void applyForceOnBall(hkVector4& dir, hkVector4& pos, hkReal val, int number);

	void addBall(hkReal x, hkReal y, hkReal z);
	void deleteBall(int number);

	void setPosOfBall(hkReal x, hkReal y, hkReal z, int number);

	hkVector4 getPosOfBall(int number);
	hkQuaternion getgetRotationOfBall(int number);

	// set the designated ball stilled
	void disableBall(int number);

	~Billiards();

private:

	std::vector<Ball*> m_ballList;
	hkpRigidBody* m_table;
};


#endif	// !defined(__BILLIARDS_H__)
