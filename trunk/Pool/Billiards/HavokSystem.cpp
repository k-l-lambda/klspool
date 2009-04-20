/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include "HavokSystem.h"

//错误处理函数
static void HK_CALL errorReport(const char* msg,void*)
{
	printf("%s",msg);
}

HavokSystem::HavokSystem(void)
{
	m_MemoryManager = new hkPoolMemory();
	m_ThreadMemory = new hkThreadMemory(m_MemoryManager);
	hkBaseSystem::init(m_MemoryManager,m_ThreadMemory,errorReport);
	
	m_MemoryManager->removeReference();
	//初始化堆栈
	{
		m_StackSize = 0x100000;
		m_StackBuffer = hkAllocate<char>(m_StackSize,HK_MEMORY_CLASS_BASE);
		hkThreadMemory::getInstance().setStackArea(m_StackBuffer,m_StackSize);
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


HavokSystem::~HavokSystem(void)
{
	m_World->removeReference();
	m_Vdb->removeReference();
	m_Context->removeReference();
	delete m_JobQueue;
	m_ThreadPool->removeReference();
	m_ThreadMemory->setStackArea(0,0);
	hkDeallocate(m_StackBuffer);
	m_ThreadMemory->removeReference();

	hkBaseSystem::quit();
}

bool HavokSystem::createHavokWorld(hkReal worldsize)
{
	hkpWorldCinfo worldInfo;
	worldInfo.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED;
	worldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_REMOVE_ENTITY;
	//设置world尺寸
	worldInfo.setBroadPhaseWorldSize(worldsize);
	//worldInfo.m_gravity = hkVector4(0.0f,-16.0f,0.0f);

	m_World = new hkpWorld(worldInfo);
	m_World->m_wantDeactivation = false;
	m_World->markForWrite();
	//注册碰撞代理
	hkpAgentRegisterUtil::registerAllAgents(m_World->getCollisionDispatcher());
	m_World->registerWithJobQueue(m_JobQueue);

	m_World->unmarkForWrite();
	return true;
}

bool HavokSystem::InitVDB()
{

	m_World->markForWrite();
	m_Context = new hkpPhysicsContext();
	hkpPhysicsContext::registerAllPhysicsProcesses();
	m_Context->addWorld(m_World);
	m_Contexts.pushBack(m_Context);

	m_World->unmarkForWrite();

	m_Vdb = new hkVisualDebugger(m_Contexts);
	m_Vdb->serve();

	return true;
}

void HavokSystem::simulate()
{
	hkStopwatch stopWatch;
	stopWatch.start();
	hkReal lastTime = stopWatch.getElapsedSeconds();

	hkReal timestep = 1.0f/60.0f;

	m_World->stepMultithreaded(m_JobQueue,m_ThreadPool,timestep);

		m_Context->syncTimers(m_ThreadPool);
		m_Vdb->step();
	

	hkMonitorStream::getInstance().reset();
	m_ThreadPool->clearTimerData();

	while(stopWatch.getElapsedSeconds()<lastTime+timestep);
	lastTime += timestep;
	//step the graphics display here
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