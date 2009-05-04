/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include "HavokSystem.h"
#include "ThreadAccessLock.h"

#include <iostream>

// Classlists
#define INCLUDE_HAVOK_PHYSICS_CLASSES
#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkClasses.h>
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.cxx>

// Generate a custom list to trim memory requirements
#define HK_COMPAT_FILE <Common/Compat/hkCompatVersions.h>
#include <Common/Compat/hkCompat_None.cxx>


namespace Billiards
{
	//错误处理函数
	static void HK_CALL errorReport(const char* msg, void*)
	{
		//printf("%s",msg);
		std::cout << "Havok error report: " << msg << std::endl;
	}


	HavokSystem::HavokSystem()
	{
		m_MemoryManager = new hkPoolMemory();
		m_ThreadMemory = new hkThreadMemory(m_MemoryManager);
		hkBaseSystem::init(m_MemoryManager, m_ThreadMemory, errorReport);

		m_MemoryManager->removeReference();
		//初始化堆栈
		{
			m_StackSize = 0x100000;
			m_StackBuffer = hkAllocate<char>(m_StackSize,HK_MEMORY_CLASS_BASE);
			hkThreadMemory::getInstance().setStackArea(m_StackBuffer, m_StackSize);
		}
		//初始化多线程
		int m_TotalNumThreadsUsed;

		hkHardwareInfo hwInfo;
		hkGetHardwareInfo(hwInfo);
		m_TotalNumThreadsUsed	= hwInfo.m_numThreads;

		hkCpuJobThreadPoolCinfo gThreadPoolCinfo;
		gThreadPoolCinfo.m_numThreads = m_TotalNumThreadsUsed-1;
		gThreadPoolCinfo.m_timerBufferPerThreadAllocation = 200000;
		m_ThreadPool	= new hkCpuJobThreadPool(gThreadPoolCinfo);

		hkJobQueueCinfo info;
		info.m_jobQueueHwSetup.m_numCpuThreads = m_TotalNumThreadsUsed;
		m_JobQueue = new hkJobQueue(info);

		hkMonitorStream::getInstance().resize(200000);

		//默认不使用ＶＤＢ
		//m_bEnableVDB = false;
	}

	HavokSystem::~HavokSystem()
	{
		m_World->markForWrite();

		m_World->removeReference();
		m_Vdb->removeReference();
		m_Context->removeReference();
		m_Contexts.clearAndDeallocate();
		delete m_JobQueue;
		m_ThreadPool->removeReference();
		m_ThreadMemory->setStackArea(0,0);
		hkDeallocate(m_StackBuffer);
		m_ThreadMemory->removeReference();

		hkBaseSystem::quit();
	}

	bool HavokSystem::createHavokWorld(Real worldsize)
	{
		hkpWorldCinfo worldInfo;
		worldInfo.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED;
		worldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_FIX_ENTITY;
		//设置world尺寸
		worldInfo.setBroadPhaseWorldSize(worldsize);
		worldInfo.m_gravity = Vector(0, -98, 0);

		m_World = new hkpWorld(worldInfo);
		m_World->m_wantDeactivation = false;
		{
			WorldWritingLock wlock(m_World);

			//注册碰撞代理
			hkpAgentRegisterUtil::registerAllAgents(m_World->getCollisionDispatcher());
			m_World->registerWithJobQueue(m_JobQueue);
		}
		return true;
	}

	void HavokSystem::createPhysicsScene()
	{
	}

	bool HavokSystem::InitVDB()
	{
		{
			WorldWritingLock wlock(m_World);

			m_Context = new hkpPhysicsContext();
			hkpPhysicsContext::registerAllPhysicsProcesses();
			m_Context->addWorld(m_World);
			m_Contexts.pushBack(m_Context);
		}

		m_Vdb = new hkVisualDebugger(m_Contexts);
		m_Vdb->serve();

		return true;
	}

	void HavokSystem::simulate(Real elapsedTime)
	{
		//hkStopwatch stopWatch;
		//stopWatch.start();
		//Real lastTime = stopWatch.getElapsedSeconds();

		//Real timestep = 1.0f/60.0f;

		if(elapsedTime != 0)
			m_World->stepMultithreaded(m_JobQueue, m_ThreadPool, elapsedTime);

		m_Context->syncTimers(m_ThreadPool);
		m_Vdb->step();


		//hkMonitorStream::getInstance().reset();
		//m_ThreadPool->clearTimerData();

		//while(stopWatch.getElapsedSeconds()<lastTime+timestep);
	}

	void HavokSystem::setup()
	{
		//创建hkpWorld
		createHavokWorld(2000);
		//初始化远程调试
		InitVDB();
		//创建物理场景
		createPhysicsScene();
	}

	hkpWorld* HavokSystem::getWorld() const
	{
		return m_World;
	}
}
