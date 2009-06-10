/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef	__BILLIARDS_H__
#define	__BILLIARDS_H__



// compiling options
#define	ENABLE_LUASYMBOLS_EXPORT


#ifdef	_MSC_VER
#	pragma warning(push)
#	pragma warning(disable: 4710)	// 'function' : function not inlined
#	pragma warning(disable: 4711)	// function 'function' selected for inline expansion
#endif

/*#ifdef	_MSC_VER
#	pragma warning(push)
#	pragma warning(disable: 4820)	// 'bytes' bytes padding added after construct 'member_name'
#	include <wchar.h>
#	pragma warning(pop)
#endif*/

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
#include <fstream>


#ifdef _MSC_VER
#	pragma warning(push, 1)
#endif
#include <boost\shared_ptr.hpp>
#include <boost\function.hpp>
#ifdef _MSC_VER
#	pragma warning(pop)
#endif

#ifdef	_MSC_VER
#	pragma warning(pop)
#endif


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

typedef	boost::shared_ptr<hkpRigidBody>		hkpRigidBodyPtr;


#ifdef	_BILLIARDSDLL
#	define	BILLIARDS_API	__declspec(dllexport)
#else
#	define	BILLIARDS_API	__declspec(dllimport)
#endif	// defined(_BILLIARDSDLL)


#include "MathDef.h"


#ifdef	ENABLE_LUASYMBOLS_EXPORT
struct lua_State;
#endif	// defined(ENABLE_LUASYMBOLS_EXPORT)


namespace Billiards
{
	class Ball;
	class Game;
	class HavokSystem;
	class HolePhantom;

	struct BallConfig;
	struct BallConfigSet;
	struct GameLayout;
	struct IVisualObject;
	struct VisualObjectParameters;

	typedef	boost::shared_ptr<const Ball>											ConstBallPtr;
	typedef	boost::shared_ptr<Ball>													BallPtr;
	typedef	boost::shared_ptr<IVisualObject>										VisualObjectPtr;
	typedef	boost::shared_ptr<HolePhantom>                                          HolePhantomPtr;
	typedef	boost::function<VisualObjectPtr (const VisualObjectParameters&)>		VisualObjectCreationFunctor;


#ifdef	ENABLE_LUASYMBOLS_EXPORT
	extern "C" BILLIARDS_API	int registerLuaSymbols(lua_State* L);
#endif	// defined(ENABLE_LUASYMBOLS_EXPORT)
}



#endif	// !defined(__BILLIARDS_H__)
