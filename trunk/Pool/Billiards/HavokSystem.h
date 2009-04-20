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
	//����hkpWorld
	virtual bool createHavokWorld(hkReal worldsize);
	//��ʼ��VDB
	virtual bool InitVDB();
	//����������
	virtual void createPhysicsScene() = 0;

	//step simulation
	virtual void simulate();

	void setup();

	//Physics
	hkpWorld*					m_World;

protected:
	//��Ա����
	hkPoolMemory*				m_MemoryManager;
	hkThreadMemory*				m_ThreadMemory;
	char*						m_StackBuffer;
	int							m_StackSize;
	//���߳����
	hkJobThreadPool*			m_ThreadPool;
	int							m_TotalNumThreadUsed;
	hkJobQueue*					m_JobQueue;
	
	//VDB���
	hkArray<hkProcessContext*>	m_Contexts;
	hkpPhysicsContext*			m_Context;
	hkVisualDebugger*			m_Vdb;

	//ģ�����
	bool						m_bPause;
	//�Ƿ�����Debugger
	bool						m_bEnableVDB;
};


#endif