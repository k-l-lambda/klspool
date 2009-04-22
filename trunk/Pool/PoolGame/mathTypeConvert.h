#include <Ogre.h>
#include <Common\Base\Math\hkMath.h>

inline Ogre::Vector3 hkVector4ToOgre(const hkVector4& vec)
{
	Ogre::Vector3 q(vec(0),vec(1),vec(2));

	return q;
}

inline Ogre::Quaternion hkQuatToOgre(const hkQuaternion& quat)
{	
	hkVector4 imgpart = quat.getImag();
	hkReal realpart = quat.getReal();

	Ogre::Quaternion q(realpart,imgpart(0),imgpart(1),imgpart(2));

	return q;
}

inline hkVector4 OgreTohkVector4(const Ogre::Vector3& vec)
{
	hkVector4 v(vec.x, vec.y, vec.z);

	return v;
}

inline hkQuaternion OgreTohkQuaternion(const Ogre::Quaternion& quat)
{
	hkQuaternion q(quat.x, quat.y, quat.z, quat.w);

	return q;
}