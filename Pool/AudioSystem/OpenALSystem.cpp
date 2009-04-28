/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "OpenALSystem.h"

OpenALSystem::OpenALSystem()
{
	m_buffers = 0;
	m_sources = 0;

	m_playingSource = 0;
	m_loadedBuffer = 0;

	alutInit(0, 0);
	alGetError();
}

OpenALSystem::~OpenALSystem()
{
	//alDeleteBuffers(m_numBuffers, m_buffers);
	//alDeleteSources(m_numSources, m_sources);
	alutExit();

	if(m_sources)
		delete [] m_sources;
	if(m_buffers)
		delete [] m_buffers;
}

bool OpenALSystem::loadWavFile(const std::string& fileName)
{
	ALenum format;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	ALboolean loop;

	ALbyte* filename = new ALbyte[fileName.size()];
	strcpy(filename,fileName.c_str());
	alutLoadWAVFile(filename, &format, &data, &size, &freq, &loop);

	alBufferData(m_buffers[m_loadedBuffer], format, data, size, freq);
	m_loadedBuffer++;

	alutUnloadWAV(format, data, size, freq);

	if (alGetError() != AL_NO_ERROR)
		return false;
	else
		return true;
}

bool OpenALSystem::init(int numBuffers, int numSources)
{
	// init buffers and sources
	m_numBuffers = numBuffers;
	m_numSources = numSources;

	m_buffers = new ALuint[m_numBuffers];
	alGenBuffers(m_numBuffers, m_buffers);

	if (alGetError() != AL_NO_ERROR)
		return false;

	m_sources = new ALuint[m_numSources];
	alGenSources(m_numSources, m_sources);

	if (alGetError() != AL_NO_ERROR)
		return false;

	//setup every source
	ALfloat sourcePos[] = { 0.0, 0.0, 0.0 };
	ALfloat sourceVel[] = { 0.0, 0.0, 0.0 };

	for(int i = 0; i < m_numSources; ++i)
	{
		alSourcei (m_sources[i], AL_BUFFER,		m_buffers[1]);
		alSourcef (m_sources[i], AL_PITCH,		1.0f);
		alSourcef (m_sources[i], AL_GAIN,		18.0f);
		alSourcefv(m_sources[i], AL_POSITION,	sourcePos);
		alSourcefv(m_sources[i], AL_VELOCITY,	sourceVel);
		alSourcei (m_sources[i], AL_LOOPING,	0);

		if (alGetError() != AL_NO_ERROR)
			return false;
	}

	// setup litener values
	ALfloat listenerPos[] = { 0.0, 0.0, 0.0 };
	ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
	ALfloat listenerDir[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

	alListenerfv(AL_POSITION,    listenerPos);
	alListenerfv(AL_VELOCITY,    listenerVel);
	alListenerfv(AL_ORIENTATION, listenerDir);

	if (alGetError() != AL_NO_ERROR)
		return false;
	else
		return true;
}

void OpenALSystem::playSound(int numOfBuffer, ALfloat* sourcePos, ALfloat* listenerPos)
{
	if(numOfBuffer >= m_loadedBuffer)
		return;

	alSourcei (m_sources[m_playingSource], AL_BUFFER, m_buffers[numOfBuffer]);
	alSourcefv(m_sources[m_playingSource], AL_POSITION, sourcePos);

	alListenerfv(AL_POSITION, listenerPos);

	alSourcePlay(m_sources[m_playingSource]);

	m_playingSource++;
	if(m_playingSource>=m_numSources)
		m_playingSource = 0;
}
