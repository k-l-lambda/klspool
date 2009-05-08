/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef __AUDIOSOCKET_H__
#define __AUDIOSOCKET_H__



#include "Billiards.h"

#include <boost\signal.hpp>
#include <boost\thread\mutex.hpp>


namespace Billiards
{
	class BILLIARDS_API	AudioSocket
	{
	private:
		AudioSocket()	{};
		~AudioSocket()	{};

	public:
		static AudioSocket&	instance()		{static AudioSocket instance; return instance;};

	public:
		typedef	boost::signal<void (const std::string& name, const Vector position, Real volume)>	PlaySoundSignal;

#pragma warning(suppress: 4251)	// 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
		PlaySoundSignal	playSound;

		static boost::mutex&	getPlaySoundMutex()	{static boost::mutex m; return m;};
	};
}



#endif	// !defined(__AUDIOSOCKET_H__)
