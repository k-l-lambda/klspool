/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef	__GAME_H__
#define	__GAME_H__



#include "Billiards.h"
#include "Ball.h"
#include "BallConfig.h"

#include <boost\noncopyable.hpp>


namespace Billiards
{
	class BILLIARDS_API	Game
		: boost::noncopyable
#pragma warning(suppress: 4275)	// non ¨C DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
	{
		typedef	boost::shared_ptr<Ball>	BallPtr;

		typedef	std::map<std::string, BallConfig>	BallConfigMap;

	public:
		explicit Game(const VisualObjectCreationFunctor& fnCreateVisualObject);
		~Game();

		// test if all the balls is still
		//bool isStill() const;

		void	simulate(Real elapsedTime);

		void	shot(const Vector& impulse, const Vector& pos);

		void	loadBallConfigSet(const std::string& setname);

		void	deployLayout(const GameLayout& layout);

		ConstBallPtr	getMainBall() const;

	private:
		void	setup();

		void	createPhysicsScene();

		void	creatTable();

		void	updateAllBalls();

		// set the designated ball stilled
		//void	freezeBall(int number);

	private:
#pragma warning(suppress: 4251)	// 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
		std::vector<BallPtr>	m_Balls;
		hkpRigidBodyPtr			m_table;
		hkpRigidBodyPtr			m_baffles;

		BallPtr					m_MainBall;

		HavokSystem*			m_HavokSystem;

#pragma warning(suppress: 4251)	// 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
		VisualObjectCreationFunctor		m_fnCreateVisualObject;

		std::set<std::string>	m_LoadedBallConfigSets;
		BallConfigMap			m_BallConfigMap;
	};
}



#endif	// !defined(__GAME_H__)
