/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef __OPENALSYSTEM_H__
#define __OPENALSYSTEM_H__



#include <map>
#include <vector>
#include <string>

#include <al.h>
#include <alc.h>
#include <alut.h>

#pragma comment(lib, "OpenAL32.lib")
#pragma comment(lib, "alut.lib")

#ifdef	_AUDIOSYSTEMDLL
#	define	AUDIOSYSTEM_API	__declspec(dllexport)
#else
#	define	AUDIOSYSTEM_API	__declspec(dllimport)
#endif	// defined(_AUDIOSYSTEMDLL)

class OpenALSystem
{
public:
	OpenALSystem();
	~OpenALSystem();

	bool init(int numBuffers = 3, int numSources = 3);

	bool loadWavFile(const std::string& fileName);
	void playSound(int numOfBuffer, ALfloat* sourcePos, ALfloat* listenerPos);

private:
    ALuint* m_buffers;
	ALuint* m_sources;
	int m_numBuffers;
	int m_numSources;

	int m_playingSource;
	int m_loadedBuffer;
};



#endif	// !defined(__OPENALSYSTEM_H__)
