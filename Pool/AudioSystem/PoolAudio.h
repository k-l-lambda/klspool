/*
**	This source file is part of Pool.
**
**	Copyright (c) 2009	K.L.<xxxk.l.xxx@gmail.com>, Lazy<yinxiaozhou@gmail.com>
**	This program is free software without any warranty.
*/

#ifndef __POOLAUDIO_H__
#define __POOLAUDIO_H__



#include "OpenALSystem.h"

#include <string>
#include <memory>


class AUDIOSYSTEM_API PoolAudio
{
private:
	PoolAudio();
	~PoolAudio();

public:
	static PoolAudio& instance();

	bool init(int numSounds = 3, int numSources = 3);

	void setListenerPosition(const float pos[3]);

	bool loadWavFile(const std::string& fileName);

	void playSound(int soundNum, const float source[3], float gain = 18.0f);

private:
	//static std::auto_ptr<PoolAudio> m_poolAudioPtr;

#pragma warning(suppress: 4251)	// 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
	OpenALSystem m_OALSystem;

	float m_ListenerPos[3];
};



#endif	// !defined(__POOLAUDIO_H__)
