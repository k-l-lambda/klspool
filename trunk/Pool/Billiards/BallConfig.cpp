/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "StableHeaders.h"

#include "BallConfig.h"

#include <fstream>

#include <boost\archive\xml_iarchive.hpp>
#include <boost\archive\xml_oarchive.hpp>


namespace Billiards
{
	BallConfigSet BallConfigSet::load(std::istream& is)
	{
		BallConfigSet set;

		boost::archive::xml_iarchive ia(is);
		ia >> boost::serialization::make_nvp("BallConfigSet", boost::ref(set).get());

		return set;
	}

	BallConfigSet BallConfigSet::loadFile(const std::string& filename)
	{
		std::ifstream file(filename.data());
		if(!file.is_open())
			throw std::runtime_error(("cannot open file \"" + filename + "\"").data());

		return load(file);
	}

	void BallConfigSet::save(std::ostream& os, const BallConfigSet& set)
	{
		boost::archive::xml_oarchive oa(os);
		oa << boost::serialization::make_nvp("BallConfigSet", boost::ref(set).get());
	}

	void BallConfigSet::saveFile(const std::string& filename, const BallConfigSet& set)
	{
		std::ofstream file(filename.data(), std::ios_base::out | std::ios_base::trunc);
		if(!file.is_open())
			throw std::runtime_error(("cannot open file \"" + filename + "\"").data());

		return save(file, set);
	}
}
