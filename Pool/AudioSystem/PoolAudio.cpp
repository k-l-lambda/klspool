/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "PoolAudio.h"


std::auto_ptr<PoolAudio> PoolAudio::m_poolAudioPtr;

PoolAudio& PoolAudio::instance()
{
	if(!m_poolAudioPtr.get())
		m_poolAudioPtr = std::auto_ptr <PoolAudio> ( new PoolAudio() );

	return *(m_poolAudioPtr.get());
}

bool PoolAudio::init(int numSounds, int numSources)
{
	return m_OALSystem.init(numSounds, numSources);
}

bool PoolAudio::loadWavFile(const std::string& fileName)
{
	return m_OALSystem.loadWavFile(fileName);
}

void PoolAudio::playSound(int soundNum, const Ogre::Vector3 &source)
{
	ALfloat sourcePos[3] = {source.x, source.y, source.z};
	ALfloat listenerPos[3] = {m_ListenerPos.x, m_ListenerPos.y, m_ListenerPos.z};

	m_OALSystem.playSound(soundNum, sourcePos, listenerPos);
}

void PoolAudio::setListenerPosition(const Ogre::Vector3& pos)
{
	m_ListenerPos = pos;
}