/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef	__MATHDEF_H__
#define	__MATHDEF_H__



#include <cmath>

#include <Common\Base\hkBase.h>
#include <Common\Base\Math\hkMath.h>

#pragma comment(lib, "hkBase.lib")

class hkVector4;
class hkQuaternion;
class hkMatrix3;
class hkRotation;
class hkTransform;
class hkQsTransform;


namespace Billiards
{
	typedef	hkReal				Real;

	namespace detail
	{
		class Vector3
		{
		public:
			Real x, y, z;

		// hkVector4 adaption
		public:
			Vector3(const hkVector4& v)
				: x(v(0)), y(v(1)), z(v(2))
			{
			};

			operator hkVector4 () const
			{
				return hkVector4(x, y, z);
			};

		public:
			Vector3()
			{
			}

			Vector3( const Real fX, const Real fY, const Real fZ )
				: x( fX ), y( fY ), z( fZ )
			{
			}

			explicit Vector3( const Real afCoordinate[3] )
				: x( afCoordinate[0] ),
				y( afCoordinate[1] ),
				z( afCoordinate[2] )
			{
			}

			explicit Vector3( const int afCoordinate[3] )
			{
				x = (Real)afCoordinate[0];
				y = (Real)afCoordinate[1];
				z = (Real)afCoordinate[2];
			}

			explicit Vector3( Real* const r )
				: x( r[0] ), y( r[1] ), z( r[2] )
			{
			}

			explicit Vector3( const Real scaler )
				: x( scaler )
				, y( scaler )
				, z( scaler )
			{
			}


			Real operator [] ( const size_t i ) const
			{
				assert( i < 3 );

				return *(&x+i);
			}

			Real& operator [] ( const size_t i )
			{
				assert( i < 3 );

				return *(&x+i);
			}

			Real* ptr()
			{
				return &x;
			}

			const Real* ptr() const
			{
				return &x;
			}

			Vector3& operator = ( const Vector3& rkVector )
			{
				x = rkVector.x;
				y = rkVector.y;
				z = rkVector.z;

				return *this;
			}

			bool operator == ( const Vector3& rkVector ) const
			{
				return ( x == rkVector.x && y == rkVector.y && z == rkVector.z );
			}

			bool operator != ( const Vector3& rkVector ) const
			{
				return ( x != rkVector.x || y != rkVector.y || z != rkVector.z );
			}

			// arithmetic operations
			Vector3 operator + ( const Vector3& rkVector ) const
			{
				return Vector3(
					x + rkVector.x,
					y + rkVector.y,
					z + rkVector.z);
			}

			Vector3 operator - ( const Vector3& rkVector ) const
			{
				return Vector3(
					x - rkVector.x,
					y - rkVector.y,
					z - rkVector.z);
			}

			Vector3 operator * ( const Real fScalar ) const
			{
				return Vector3(
					x * fScalar,
					y * fScalar,
					z * fScalar);
			}

			Vector3 operator * ( const Vector3& rhs) const
			{
				return Vector3(
					x * rhs.x,
					y * rhs.y,
					z * rhs.z);
			}

			Vector3 operator / ( const Real fScalar ) const
			{
				assert( fScalar != 0.0 );

				Real fInv = 1.0f / fScalar;

				return Vector3(
					x * fInv,
					y * fInv,
					z * fInv);
			}

			Vector3 operator / ( const Vector3& rhs) const
			{
				return Vector3(
					x / rhs.x,
					y / rhs.y,
					z / rhs.z);
			}

			const Vector3& operator + () const
			{
				return *this;
			}

			Vector3 operator - () const
			{
				return Vector3(-x, -y, -z);
			}

			// overloaded operators to help Vector3
			friend Vector3 operator * ( const Real fScalar, const Vector3& rkVector )
			{
				return Vector3(
					fScalar * rkVector.x,
					fScalar * rkVector.y,
					fScalar * rkVector.z);
			}

			friend Vector3 operator / ( const Real fScalar, const Vector3& rkVector )
			{
				return Vector3(
					fScalar / rkVector.x,
					fScalar / rkVector.y,
					fScalar / rkVector.z);
			}

			friend Vector3 operator + (const Vector3& lhs, const Real rhs)
			{
				return Vector3(
					lhs.x + rhs,
					lhs.y + rhs,
					lhs.z + rhs);
			}

			friend Vector3 operator + (const Real lhs, const Vector3& rhs)
			{
				return Vector3(
					lhs + rhs.x,
					lhs + rhs.y,
					lhs + rhs.z);
			}

			friend Vector3 operator - (const Vector3& lhs, const Real rhs)
			{
				return Vector3(
					lhs.x - rhs,
					lhs.y - rhs,
					lhs.z - rhs);
			}

			friend Vector3 operator - (const Real lhs, const Vector3& rhs)
			{
				return Vector3(
					lhs - rhs.x,
					lhs - rhs.y,
					lhs - rhs.z);
			}

			// arithmetic updates
			Vector3& operator += ( const Vector3& rkVector )
			{
				x += rkVector.x;
				y += rkVector.y;
				z += rkVector.z;

				return *this;
			}

			Vector3& operator += ( const Real fScalar )
			{
				x += fScalar;
				y += fScalar;
				z += fScalar;
				return *this;
			}

			Vector3& operator -= ( const Vector3& rkVector )
			{
				x -= rkVector.x;
				y -= rkVector.y;
				z -= rkVector.z;

				return *this;
			}

			Vector3& operator -= ( const Real fScalar )
			{
				x -= fScalar;
				y -= fScalar;
				z -= fScalar;
				return *this;
			}

			Vector3& operator *= ( const Real fScalar )
			{
				x *= fScalar;
				y *= fScalar;
				z *= fScalar;
				return *this;
			}

			Vector3& operator *= ( const Vector3& rkVector )
			{
				x *= rkVector.x;
				y *= rkVector.y;
				z *= rkVector.z;

				return *this;
			}

			Vector3& operator /= ( const Real fScalar )
			{
				assert( fScalar != 0.0 );

				Real fInv = 1.0f / fScalar;

				x *= fInv;
				y *= fInv;
				z *= fInv;

				return *this;
			}

			Vector3& operator /= ( const Vector3& rkVector )
			{
				x /= rkVector.x;
				y /= rkVector.y;
				z /= rkVector.z;

				return *this;
			}

			Real length () const
			{
				return std::sqrt( x * x + y * y + z * z );
			}

			Real squaredLength () const
			{
				return x * x + y * y + z * z;
			}

			Real normalise()
			{
				Real fLength = std::sqrt( x * x + y * y + z * z );

				// Will also work for zero-sized vectors, but will change nothing
				if ( fLength > 1e-08 )
				{
					Real fInvLength = 1.0f / fLength;
					x *= fInvLength;
					y *= fInvLength;
					z *= fInvLength;
				}

				return fLength;
			}

			Vector3 normalisedCopy() const
			{
				Vector3 ret = *this;
				ret.normalise();
				return ret;
			}


			// special points
			static const Vector3 ZERO;
			static const Vector3 UNIT_X;
			static const Vector3 UNIT_Y;
			static const Vector3 UNIT_Z;
			static const Vector3 NEGATIVE_UNIT_X;
			static const Vector3 NEGATIVE_UNIT_Y;
			static const Vector3 NEGATIVE_UNIT_Z;
			static const Vector3 UNIT_SCALE;


			inline friend std::ostream& operator << (std::ostream& os, const Vector3& v)
			{
				return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
			}
		};
	}


	//typedef	hkVector4		Vector;
	typedef	detail::Vector3		Vector;
	typedef	hkQuaternion		Quaternion;


	inline std::ostream& operator << (std::ostream& os, const hkVector4& v)
	{
		return os << "(" << v(0) << ", " << v(1) << ", " << v(2) << ", " << v(3) << ")";
	}
}



#endif	// !defined(__MATHDEF_H__)
