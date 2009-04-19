/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef _HAVOKSYSTEM_H_
#define _HAVOKSYSTEM_H_

// Math and base include
#include <Common/Base/hkBase.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/Memory/hkThreadMemory.h>
#include <Common/Base/Memory/Memory/Pool/hkPoolMemory.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/Monitor/hkMonitorStream.h>

// Dynamics includes
#include <Physics/Collide/hkpCollide.h>										
#include <Physics/Collide/Agent/ConvexAgent/SphereBox/hkpSphereBoxAgent.h>	
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>					
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>				
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>					

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>			
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>			

#include <Physics/Dynamics/World/hkpWorld.h>								
#include <Physics/Dynamics/Entity/hkpRigidBody.h>							
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>	

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/Thread/Job/ThreadPool/Spu/hkSpuJobThreadPool.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>

// Visual Debugger includes
#include <Common/Visualize/hkVisualDebugger.h>
#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>				

// Classlists
#define INCLUDE_HAVOK_PHYSICS_CLASSES
#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkClasses.h>
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.cxx>

// Generate a custom list to trim memory requirements
#define HK_COMPAT_FILE <Common/Compat/hkCompatVersions.h>
#include <Common/Compat/hkCompat_None.cxx>

//Keycode
//#include <Common/Base/keycode.cxx>

#include <stdio.h>



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