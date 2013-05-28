#pragma once
#ifndef AUDIOCLASS_H
#define AUDIOCLASS_H

#pragma comment(lib, "../../Dependencies/Lib/FMod/fmodex_vc.lib")

#include <FMod/fmod.hpp>
#include <vector>
#include "stdafx.h"
#include <D3DX11\D3D11.h>
#include <D3DX11\D3DX11.h>
#include "..\Util\misc.h"
#include "..\Util\SmartPtrs.h"

enum FlySoundState
{
	FlySound_Menu, 
	FlySound_Level,
};

enum FlyMenuSounds
{	
	FlySound_MenuSoundTrack,
	FlySound_Hover,
	FlySound_Click,

	SOUNDMENU_COUNT
};

enum FlyLevelSounds
{	
	FlySound_LevelSoundTrack,

	FlySound_Collision,
	FlySound_EnergyPickup, 
	FlySound_CargoPickup, 
	FlySound_LowEnergy, 
	FlySound_NoEnergy, 
	FlySound_Thrust, 

	
	FlySound_Wind,

	SOUNDLEVEL_COUNT
};



class AudioClass
{
public:
struct	playerSoundData
{
	D3DXVECTOR3 pos; 
	float vel; 
};

private:
	static bool instanceFlag;
	static AudioClass* single;
	FMOD::System * fmodSystem;
	FMOD_RESULT result;

	std::vector<FMOD::Channel*> channel;
	std::vector<FMOD::Sound*> sounds;

	int maxChannels;
	AudioClass(void);

	FMOD_VECTOR listenerPos; 
	void unLoadSounds();
	bool loadMenuSound(std::vector<const char*> path);
	bool loadLevelSound(std::vector<const char*> path);
	bool FmodErrorCheck(FMOD_RESULT result);

public:
	
	~AudioClass(void);

	static AudioClass* self();

	void intitialize();
	void shutdown();
	bool loadSound(FlySoundState state, std::vector<const char*> path);


	void uppdateSounds(playerSoundData soundData);
	void playLevelSound(FlyLevelSounds sound);
	void playMenuSound(FlyMenuSounds sound);
	void toggleSoundTrack();
	void pauseAllSound();

};
#endif
