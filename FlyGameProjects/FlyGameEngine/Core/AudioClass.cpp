#include "AudioClass.h"


bool AudioClass::instanceFlag = false;
AudioClass* AudioClass::single = NULL;

AudioClass* AudioClass::self()
{
	if(!instanceFlag)
	{
		single = new AudioClass();
		instanceFlag = true;
		return single;
	}
	else
	{
		return single;
	}
}
AudioClass::AudioClass(void)
{
}


AudioClass::~AudioClass(void)
{
}

void AudioClass::intitialize()
{
	maxChannels = 2; // max two sound playing 
	result = System_Create(&fmodSystem);
	
	result = fmodSystem->init(maxChannels, FMOD_INIT_NORMAL,0);
	
}
void AudioClass::loadSound()
{
	//create sound for small sound effects
	result = fmodSystem->createSound("../../WorkingDir/Resources/Sound/jaguar.wav", FMOD_DEFAULT, 0, & sound1);
	sound1->setMode(FMOD_LOOP_NORMAL);

	////stream the big sound files 
	//result = fmodSystem->createStream("soundtrac.wma", FMOD_DEFAULT, 0 ,& soundTrack);
	//soundTrack->setMode(FMOD_LOOP_NORMAL);
}
void AudioClass::uppdateSounds()
{
	result = fmodSystem->update();
}
void AudioClass::playSound1()
{
	result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sound1, false,&channel);
	result = channel->setVolume(0.8f);
}
void AudioClass::playSoundTrack()
{
	result = fmodSystem->playSound(FMOD_CHANNEL_FREE, soundTrack, false, &channel);
}