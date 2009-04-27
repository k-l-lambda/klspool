/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef __HAVOKSYSTEM_H__
#define __HAVOKSYSTEM_H__



#include "Billiards.h"
#include "HavokRef.h"


namespace Billiards
{
	class HavokSystem
	{
	public:
		HavokSystem();
		~HavokSystem();

		void setup();

		hkpWorld*	getWorld() const;

		//step simulation
		void simulate(Real elapsedTime);

	private:
		//创建hkpWorld
		bool createHavokWorld(Real worldsize);
		//初始化VDB
		bool InitVDB();
		//创建物理场景
		void createPhysicsScene();

	private:
		//Physics
		hkpWorld*					m_World;

		hkPoolMemory*				m_MemoryManager;
		hkThreadMemory*				m_ThreadMemory;
		char*						m_StackBuffer;
		int							m_StackSize;
		//多线程相关
		hkJobThreadPool*			m_ThreadPool;
		int							m_TotalNumThreadUsed;
		hkJobQueue*					m_JobQueue;

		//VDB相关
#pragma warning(suppress: 4251)	// 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
		hkArray<hkProcessContext*>	m_Contexts;
		hkpPhysicsContext*			m_Context;
		hkVisualDebugger*			m_Vdb;

		//模拟控制
		bool						m_bPause;
		//是否启用Debugger
		bool						m_bEnableVDB;
	};
}



#endif	// !defined(__HAVOKSYSTEM_H__)
