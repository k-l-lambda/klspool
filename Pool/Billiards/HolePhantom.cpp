#include "StableHeaders.h"

#include "HolePhantom.h"
#include "Ball.h"
#include "AudioSocket.h"

namespace Billiards
{
	std::vector<BallPtr>* HolePhantom::m_AllBalls;

	HolePhantom::HolePhantom( const hkpShape *shape, const hkTransform &transform, hkUint32 m_collisionFilterInfo )
	: hkpSimpleShapePhantom( shape, transform, m_collisionFilterInfo )
	{
		m_position = transform.getTranslation();
	}

	void HolePhantom::addOverlappingCollidable( hkpCollidable* c )
	{
		hkpRigidBody* rb = hkGetRigidBody(c);

		// Ignore other phantoms and fixed rigid bodies.
		if ( (rb != HK_NULL) && !rb->isFixed() )
		{
			hkpSimpleShapePhantom::addOverlappingCollidable( c );
			std::cout<<rb->getPosition()(0)<<","<<rb->getPosition()(1)<<","<<rb->getPosition()(2)<<std::endl;
			AudioSocket::instance().playSound("falling", m_position, 100);
			
			if(!m_AllBalls)
				return;

			// Put the ball into the list.
			std::vector<BallPtr>& ballList = *m_AllBalls;
			for(std::vector<BallPtr>::iterator iter = ballList.begin();
				iter != ballList.end();
				++iter)
			{
				if(((*iter)->m_RigidBody).get() == rb)
					m_collidingList.push_back(*iter);
			}
		} 
	}

	void HolePhantom::removeOverlappingCollidable( hkpCollidable* c )
	{
		hkpRigidBody* rb = hkGetRigidBody(c);

		if ( (rb != HK_NULL) && !rb->isFixed() )
		{
			hkpSimpleShapePhantom::removeOverlappingCollidable( c );

			for(std::list<BallPtr>::iterator iter = m_collidingList.begin();
				iter != m_collidingList.end();)
			{
				if(((*iter)->m_RigidBody).get() == rb)
				{
					iter = m_collidingList.erase(iter);
				}
				else
					++iter;
			}
		}
	}

	/*bool HolePhantom::isBallFalling()
	{
		bool result = false;

		for(std::list<BallPtr>::iterator iter = m_collidingList.begin();
			iter != m_collidingList.end();)
		{
			hkVector4 ballPos = ((*iter)->m_RigidBody)->getPosition();
			ballPos.sub4(m_position);
			Real distance = ballPos.length3();
			
			if(distance < (*iter)->m_Radius)
			{
				// Falling into the hole.
				AudioSocket::instance().playSound("collide b-b", m_position, 100);
				(*iter)->m_IsInHole = true;
				// 1 在holephantom中加入hkworld的引用
				// 2 在Game中加入检测球是否进洞的函数，并在poolgame中调用
				// 3 使球静止的同时不可见。 需要在Ball中加入相应的函数

				iter = m_collidingList.erase(iter);
				result = true;
			}
			else
				++iter;
		}

		return result;
	}*/

	void HolePhantom::setBallList(std::vector<BallPtr>* balls)
	{
		m_AllBalls = balls;
	}
}