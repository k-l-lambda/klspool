/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "PoolAudio.h"

#include <al.h>
#include <alc.h>
#include <alut.h>


//std::auto_ptr<PoolAudio> PoolAudio::m_poolAudioPtr;

PoolAudio::PoolAudio()
{
}

PoolAudio::~PoolAudio()
{
}

PoolAudio& PoolAudio::instance()
{
	/*if(!m_poolAudioPtr.get())
		m_poolAudioPtr = std::auto_ptr <PoolAudio> ( new PoolAudio() );

	return *(m_poolAudioPtr.get());*/
	static PoolAudio s_Instance;

	return s_Instance;
}

bool PoolAudio::init(int numSounds, int numSources)
{
	return m_OALSystem.init(numSounds, numSources);
}

bool PoolAudio::loadWavFile(const std::string& fileName)
{
	return m_OALSystem.loadWavFile(fileName);
}

void PoolAudio::playSound(int soundNum, const float source[3], float gain)
{
	m_OALSystem.playSound(soundNum, source, m_ListenerPos, gain);
}

void PoolAudio::setListenerPosition(const float pos[3])
{
	m_ListenerPos[0] = pos[0];
	m_ListenerPos[1] = pos[1];
	m_ListenerPos[2] = pos[2];
}
