/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef __VISUALOBJECT_H__
#define __VISUALOBJECT_H__



#include "Billiards.h"

#include <boost\function.hpp>
#include <boost\shared_ptr.hpp>


namespace Billiards
{
	struct VisualObjectParameters
	{
		typedef	std::map<unsigned int, std::string>		MaterialNameMap_t;

		std::string			Name;
		std::string			MeshName;
		MaterialNameMap_t	MaterialNameMap;
	};


	struct IVisualObject
	{
		virtual void	setTransform(const Vector& position, const Quaternion& orientation)	= 0;
	};
}



#endif	// !defined(__VISUALOBJECT_H__)
