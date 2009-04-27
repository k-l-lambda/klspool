/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef __VISUALOBJECT_H__
#define __VISUALOBJECT_H__



#include "Billiards.h"

#include <boost\tuple\tuple.hpp>
#include <boost\function.hpp>
#include <boost\shared_ptr.hpp>


namespace Billiards
{
	struct VisualObjectParameters
	{
		typedef	std::map<unsigned int, std::string>		MaterialNameMap_t;
		typedef	boost::tuple<float, float, float>		Scale_t;

		std::string				Name;
		std::string				MeshName;
		MaterialNameMap_t		MaterialNameMap;
		Scale_t					Scale;
	};


	struct IVisualObject
	{
		virtual void	updateTransform(const Vector& position, const Quaternion& orientation)	= 0;
		virtual void	setVisible(bool visible)	= 0;

	protected:
		~IVisualObject()	{};
	};
}



#endif	// !defined(__VISUALOBJECT_H__)
