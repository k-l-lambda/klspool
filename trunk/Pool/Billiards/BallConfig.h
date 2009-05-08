/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef	__BALLCONFIG_H__
#define	__BALLCONFIG_H__



#include "Billiards.h"

#pragma warning(push)
#pragma warning(disable: 4267)
#include <boost\serialization\vector.hpp>
#include <boost\serialization\set.hpp>
#include <boost\serialization\map.hpp>
#pragma warning(pop)


namespace Billiards
{
	struct BallConfig
	{
		struct Less
			: public std::binary_function<const BallConfig&, const BallConfig&, bool>
		{
			bool operator () (const BallConfig& lhs, const BallConfig& rhs) const
			{
				return std::less<std::string>()(lhs.Name, rhs.Name);
			};
		};

		std::string		Name;
		Real			Redius;
		Real			Mass;
		std::string		Material;


		template<typename Archive>
		void serialize(Archive& ar, const unsigned int /*version*/)
		{
			ar & BOOST_SERIALIZATION_NVP(Name);
			ar & BOOST_SERIALIZATION_NVP(Redius);
			ar & BOOST_SERIALIZATION_NVP(Mass);
			ar & BOOST_SERIALIZATION_NVP(Material);
		};
	};


	struct BILLIARDS_API	BallConfigSet
		: public std::set<BallConfig, BallConfig::Less>
#pragma warning(suppress: 4251)	// 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
#pragma warning(suppress: 4275)	// non ¨C DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
	{
		typedef	std::set<BallConfig, BallConfig::Less>	Set;

		std::string		NameSpace;


		static BallConfigSet	load(std::istream& is);
		static BallConfigSet	loadFile(const std::string& filename);
		static void				save(std::ostream& os, const BallConfigSet& set);
		static void				saveFile(const std::string& filename, const BallConfigSet& set);

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int /*version*/)
		{
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Set);
			ar & BOOST_SERIALIZATION_NVP(NameSpace);
		};
	};
}


BOOST_CLASS_IMPLEMENTATION(Billiards::BallConfig, boost::serialization::object_serializable)
BOOST_CLASS_IMPLEMENTATION(Billiards::BallConfigSet, boost::serialization::object_serializable)
BOOST_CLASS_IMPLEMENTATION(Billiards::BallConfigSet::Set, boost::serialization::object_serializable)



#endif	// !defined(__BALLCONFIG_H__)
