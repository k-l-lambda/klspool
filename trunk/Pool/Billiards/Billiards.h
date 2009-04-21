/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef	__BILLIARDS_H__
#define	__BILLIARDS_H__



#ifdef	_MSC_VER
#	pragma warning(push)
#	pragma warning(disable: 4710)	// 'function' : function not inlined
#	pragma warning(disable: 4711)	// function 'function' selected for inline expansion
#endif

#ifdef	_MSC_VER
#	pragma warning(push)
#	pragma warning(disable: 4820)	// 'bytes' bytes padding added after construct 'member_name'
#	include <wchar.h>
#	pragma warning(pop)
#endif

#include <vector>
#if	_MSC_VER == 1400	// tolerant warnings
#	pragma warning(push)
#	pragma warning(disable: 4510)	// 'class' : default constructor could not be generated
#	pragma warning(disable: 4571)	// Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
#	pragma warning(disable: 4610)	// object 'class' can never be instantiated - user-defined constructor required
#	pragma warning(disable: 4820)	// 'bytes' bytes padding added after construct 'member_name'
#endif	// _MSC_VER == 1400
#include <list>
#if	_MSC_VER == 1400
#	pragma warning(pop)
#endif	// _MSC_VER == 1400
#include <set>
#include <map>
#include <stack>


// Havok library

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

#pragma comment(lib, "hkBase.lib")
#pragma comment(lib, "hkSerialize.lib")
#pragma comment(lib, "hkSceneData.lib")
#pragma comment(lib, "hkInternal.lib")
#pragma comment(lib, "hkGeometryUtilities.lib")
#pragma comment(lib, "hkVisualize.lib")
#pragma comment(lib, "hkCompat.lib")
#pragma comment(lib, "hkpCollide.lib")
#pragma comment(lib, "hkpConstraintSolver.lib")
#pragma comment(lib, "hkpDynamics.lib")
#pragma comment(lib, "hkpInternal.lib")
#pragma comment(lib, "hkpUtilities.lib")
#pragma comment(lib, "hkpVehicle.lib")


#ifdef	_BILLIARDSDLL
#	define	BILLIARDS_API	__declspec(dllexport)
#else
#	define	BILLIARDS_API	__declspec(dllimport)
#endif	// defined(_BILLIARDSDLL)


namespace Billiards
{
	class Ball;
	class BldGame;
	class HavokSystem;
}



#endif	// !defined(__BILLIARDS_H__)
