
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

bool PoolAudio::loadWavFile(std::string fileName)
{
	return m_OALSystem.loadWavFile(fileName);
}

void PoolAudio::playSound(int soundNum, const Ogre::Vector3 &source)
{
	ALfloat sourcePos[3] = {source.x, source.y, source.z};
	ALfloat litenerPos[3] = {m_CamPos.x, m_CamPos.y, m_CamPos.z};

	m_OALSystem.playSound(soundNum, sourcePos, litenerPos);
}

void PoolAudio::setCameraPosition(const Ogre::Vector3& pos)
{
	m_CamPos = pos;
}