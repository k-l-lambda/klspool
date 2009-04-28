/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef __POOLAUDIO_H__
#define __POOLAUDIO_H__



#include "OpenALSystem.h"
#include <Ogre.h>

#ifdef	_DEBUG
#	pragma comment(lib, "OgreMain_d.lib")
#else
#	pragma comment(lib, "OgreMain.lib")
#endif

#include <string>
#include <memory>


class AUDIOSYSTEM_API PoolAudio
{
public:
	static PoolAudio& instance();

	bool init(int numSounds = 3, int numSources = 3);

	void setListenerPosition(const Ogre::Vector3& pos);

	bool loadWavFile(const std::string& fileName);

	void playSound(int soundNum, const Ogre::Vector3& source = Ogre::Vector3::ZERO);

private:
	static std::auto_ptr<PoolAudio> m_poolAudioPtr;

	OpenALSystem m_OALSystem;

	Ogre::Vector3 m_ListenerPos;
};



#endif	// !defined(__POOLAUDIO_H__)
