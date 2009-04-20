/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef _HAVOKSYSTEM_H_
#define _HAVOKSYSTEM_H_

#include "stableHeaders.h"


class HavokSystem
{
public:
	HavokSystem(void);
	

	~HavokSystem(void);
	//创建hkpWorld
	virtual bool createHavokWorld(hkReal worldsize);
	//初始化VDB
	virtual bool InitVDB();
	//创建物理场景
	virtual void createPhysicsScene() = 0;

	//step simulation
	virtual void simulate();

	void setup();

	//Physics
	hkpWorld*					m_World;

protected:
	//成员变量
	hkPoolMemory*				m_MemoryManager;
	hkThreadMemory*				m_ThreadMemory;
	char*						m_StackBuffer;
	int							m_StackSize;
	//多线程相关
	hkJobThreadPool*			m_ThreadPool;
	int							m_TotalNumThreadUsed;
	hkJobQueue*					m_JobQueue;
	
	//VDB相关
	hkArray<hkProcessContext*>	m_Contexts;
	hkpPhysicsContext*			m_Context;
	hkVisualDebugger*			m_Vdb;

	//模拟控制
	bool						m_bPause;
	//是否启用Debugger
	bool						m_bEnableVDB;
};


#endif