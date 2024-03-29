/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#include "OpenALSystem.h"

#include <al.h>
#include <alc.h>
#include <alut.h>

#pragma comment(lib, "OpenAL32.lib")
#pragma comment(lib, "alut.lib")

OpenALSystem::OpenALSystem()
{
	m_buffers = 0;
	m_sources = 0;

	m_playingSources = 0;
	m_loadedBuffer = 0;

	alutInit(0, 0);
	alGetError();
}

OpenALSystem::~OpenALSystem()
{
	if(m_sources)
	{
		//alDeleteSources(m_numSources, m_sources);
		delete [] m_sources;
	}

	if(m_buffers)
	{
		//alDeleteBuffers(m_numBuffers, m_buffers);
		delete [] m_buffers;
	}

	//alutExit();
}

bool OpenALSystem::loadWavFile(const std::string& fileName)
{
	ALenum format;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	ALboolean loop;

	ALbyte* filename = new(_NORMAL_BLOCK, __FILE__, __LINE__) ALbyte[fileName.size() + 1];
	strcpy(filename,fileName.c_str());
	alutLoadWAVFile(filename, &format, &data, &size, &freq, &loop);

	alBufferData(m_buffers[m_loadedBuffer], format, data, size, freq);

	alutUnloadWAV(format, data, size, freq);

	delete [] filename;

	// assign buffers to sources.
	for(int i = m_sourcesPerBuf * m_loadedBuffer; 
		i < m_sourcesPerBuf *(m_loadedBuffer + 1);++i)
		alSourcei (m_sources[i], AL_BUFFER, m_buffers[m_loadedBuffer]);
	m_loadedBuffer++;

	if (alGetError() != AL_NO_ERROR)
		return false;
	else
		return true;
}

bool OpenALSystem::init(int numBuffers, int sourcesPerBuf)
{
	// init buffers and sources
	m_numBuffers = numBuffers;
	m_numSources = sourcesPerBuf * numBuffers;
	m_sourcesPerBuf = sourcesPerBuf;

	m_playingSources = new int[numBuffers];
	for(int i = 0; i<numBuffers; ++i)
		m_playingSources[i] = 0;

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

void OpenALSystem::playSound(int numOfBuffer, const ALfloat* sourcePos, const ALfloat* listenerPos, float gain)
{
	if(numOfBuffer >= m_loadedBuffer)
		return;
	int sourceNum = m_playingSources[numOfBuffer] + numOfBuffer * m_sourcesPerBuf;
	alSourcef (m_sources[sourceNum], AL_GAIN,		gain);
	alSourcefv(m_sources[sourceNum], AL_POSITION, sourcePos);

	alListenerfv(AL_POSITION, listenerPos);

	alSourcePlay(m_sources[sourceNum]);

	m_playingSources[numOfBuffer]++;

	if(m_playingSources[numOfBuffer] >= m_sourcesPerBuf)
		m_playingSources[numOfBuffer] = 0;
}
