#pragma once
#ifndef AUDIOCLASS_H
#define AUDIOCLASS_H

#pragma comment(lib, "../../Dependencies/Lib/FMod/fmodex_vc.lib")

#include <FMod/fmod.hpp>
#include <vector>

class AudioClass
{
private:
	static bool instanceFlag;
	static AudioClass* single;

	FMOD::System * fmodSystem;
	FMOD_RESULT result;
	FMOD::Channel * channel;

	
	FMOD::Sound* sound1;
	FMOD::Sound* soundTrack;
	int maxChannels;

	AudioClass(void);

public:
	
	~AudioClass(void);

	static AudioClass* self();

	void intitialize();
	void loadSound();
	void uppdateSounds();
	void playSound1();
	void playSoundTrack();


};
#endif
