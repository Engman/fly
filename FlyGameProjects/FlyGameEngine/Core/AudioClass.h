#pragma once
#ifndef AUDIOCLASS_H
#define AUDIOCLASS_H

#pragma comment(lib, "../../Dependencies/Lib/FMod/fmodex_vc.lib")

#include <FMod/fmod.hpp>
#include <vector>
#include "stdafx.h"

enum FlyEngineSounds
{
	FlySound_Wings,
	FlySound_Collision,
	FlySound_LevelMusic,
	FlySound_Wind,

	SOUND_COUNT
};

class AudioClass
{
private:
	static bool instanceFlag;
	static AudioClass* single;

	FMOD::System * fmodSystem;
	FMOD_RESULT result;
	FMOD::Channel * channel;

	
	std::vector<FMOD::Sound*> sounds;
	FMOD::Sound* soundTrack;
	int maxChannels;

	 float t;
	 FMOD_VECTOR lastpos;
	FMOD_VECTOR forward       ;
	FMOD_VECTOR up             ;
	FMOD_VECTOR vel; 
	AudioClass(void);

	//void D3d_to_fmodVec(D3DXVECTOR3 d3dVec3, FMOD_VECTOR &out);
public:
	
	~AudioClass(void);

	static AudioClass* self();

	void intitialize();
	void shutdown();
	void loadSound();
	void uppdateSounds();
	void playSound(FlyEngineSounds sound);
	void playSoundTrack();


};
#endif
