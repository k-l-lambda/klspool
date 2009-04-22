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


// Havok declarations
class hkpPhysicsContext;
class hkpRigidBody;
class hkpWorld;
class hkJobThreadPool;
class hkJobQueue;
class hkPoolMemory;
class hkThreadMemory;
class hkVisualDebugger;
template <typename T> class hkArray;


#ifdef	_BILLIARDSDLL
#	define	BILLIARDS_API	__declspec(dllexport)
#else
#	define	BILLIARDS_API	__declspec(dllimport)
#endif	// defined(_BILLIARDSDLL)


#include "MathDef.h"


namespace Billiards
{
	class Ball;
	class Game;
	class HavokSystem;
}



#endif	// !defined(__BILLIARDS_H__)
