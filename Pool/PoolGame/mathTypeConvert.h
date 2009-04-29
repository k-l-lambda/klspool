/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef	__MATHTYPECONVERT_H__
#define	__MATHTYPECONVERT_H__



#include <Ogre.h>

#include "../Billiards/MathDef.h"

#ifdef	_DEBUG
#	pragma comment(lib, "OgreMain_d.lib")
#else
#	pragma comment(lib, "OgreMain.lib")
#endif


/*inline Ogre::Vector3 bld2Ogre(const Billiards::Vector& vec)
{
	return Ogre::Vector3(vec(0),vec(1),vec(2));
}*/

inline const Ogre::Vector3& bld2Ogre(const Billiards::Vector& vec)
{
	return reinterpret_cast<const Ogre::Vector3&>(vec);
}

inline Ogre::Quaternion bld2Ogre(const Billiards::Quaternion& quat)
{	
	Billiards::Vector imgpart = quat.getImag();
	Billiards::Real realpart = quat.getReal();

	//return Ogre::Quaternion(realpart, imgpart(0), imgpart(1), imgpart(2));
	return Ogre::Quaternion(realpart, imgpart.x, imgpart.y, imgpart.z);
}

/*inline Billiards::Vector ogre2Bld(const Ogre::Vector3& vec)
{
	return Billiards::Vector(vec.x, vec.y, vec.z);
}*/

inline const Billiards::Vector& ogre2Bld(const Ogre::Vector3& vec)
{
	return reinterpret_cast<const Billiards::Vector&>(vec);
}

inline Billiards::Quaternion ogre2Bld(const Ogre::Quaternion& quat)
{
	return Billiards::Quaternion(quat.x, quat.y, quat.z, quat.w);
}



#endif	// !defined(__MATHTYPECONVERT_H__)
