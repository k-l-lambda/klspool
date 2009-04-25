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

		//����hkpWorld
		bool createHavokWorld(Real worldsize);
		//��ʼ��VDB
		bool InitVDB();
		//������������
		void createPhysicsScene();

		//step simulation
		void simulate(Real elapsedTime);

		void setup();

		//Physics
		hkpWorld*					m_World;

	private:
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
#pragma warning(suppress: 4251)	// 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
		hkArray<hkProcessContext*>	m_Contexts;
		hkpPhysicsContext*			m_Context;
		hkVisualDebugger*			m_Vdb;

		//ģ�����
		bool						m_bPause;
		//�Ƿ�����Debugger
		bool						m_bEnableVDB;
	};
}



#endif	// !defined(__HAVOKSYSTEM_H__)