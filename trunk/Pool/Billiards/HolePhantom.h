/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef _HOLEPHANTOM_H_
#define _HOLEPHANTOM_H_

#include "Billiards.h"
#include "HavokRef.h"

// This class implementation a hole phantom to dectetion falling balls.

namespace Billiards
{

	class HolePhantom : public hkpSimpleShapePhantom
   {
   public:

	   HolePhantom( const hkpShape *shape, const hkTransform &transform, hkUint32 m_collisionFilterInfo=0 );

	   bool isBallFalling();
	   // Callback implementation
	   virtual void addOverlappingCollidable( hkpCollidable* c );

	   virtual void removeOverlappingCollidable( hkpCollidable* c );

	   static void setBallList(std::vector<BallPtr>* balls);
	   
   private:

	   static std::vector<BallPtr>*			m_AllBalls;
	   std::list<BallPtr>					m_collidingList;
	   hkVector4							m_position;	   
   };

}

#endif