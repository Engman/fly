#include "AudioClass.h"

const int   INTERFACE_UPDATETIME = 50;      // 50ms update for interface
const float DISTANCEFACTOR = 1.0f;          // Units per meter.  I.e feet would = 3.28.  centimeters would = 100.
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
	for(int i=0; i<SOUND_COUNT; i++)
	{
		sounds[i]->release(); 
	}
}

void AudioClass::intitialize()
{
	maxChannels = 4; // max two sound playing 
	result = System_Create(&fmodSystem);
	
	result = fmodSystem->init(maxChannels, FMOD_INIT_NORMAL,0);
	sounds.resize(SOUND_COUNT);

	t = 0;
	lastpos.x	= 0.0f;
	lastpos.y	= 0.0f;
	lastpos.z	= 0.0f; 
	forward.x	= 0.0f; 
	forward.y	= 0.0f; 
	forward.z	= 1.0f; 
	up.x		= 0.0f; 
	up.y		= 1.0f; 
	up.z		= 0.0f; 
	vel;
}
void AudioClass::loadSound()
{
	//create sound for small sound effects
	result = fmodSystem->createSound("../../WorkingDir/Resources/Sound/jaguar.wav", FMOD_DEFAULT, 0, & sounds[FlySound_Wings]);
	sounds[FlySound_Wings]->setMode(FMOD_LOOP_OFF);


	result = fmodSystem->createSound("../../WorkingDir/Resources/Sound/swish.wav", FMOD_DEFAULT, 0, & sounds[FlySound_Collision]);
	sounds[FlySound_Collision]->setMode(FMOD_LOOP_NORMAL);

	result = fmodSystem->createSound("../../WorkingDir/Resources/Sound/1978-039 Kansas - Dust In The Wind.mp3", FMOD_DEFAULT, 0, & sounds[FlySound_LevelMusic]);
	sounds[FlySound_LevelMusic]->setMode(FMOD_LOOP_NORMAL);

	result = fmodSystem->createSound("../../WorkingDir/Resources/Sound/swish.wav", FMOD_3D, 0, & sounds[FlySound_Wind]);
	result = sounds[FlySound_Wind]->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
	sounds[FlySound_Wind]->setMode(FMOD_LOOP_NORMAL);

	////stream the big sound files 
	//result = fmodSystem->createStream("soundtrac.wma", FMOD_DEFAULT, 0 ,& soundTrack);
	//soundTrack->setMode(FMOD_LOOP_NORMAL);
}
void AudioClass::shutdown()
{
	for(size_t i=0; i<this->sounds.size(); i++)
	{
		sounds[i]->release(); 
	}
}
void AudioClass::uppdateSounds()
{
	result = fmodSystem->update();
}
void AudioClass::playSound(FlyEngineSounds sound)
{	
		FMOD_VECTOR pos = { 15.0f * DISTANCEFACTOR, 0.0f, 0.0f };
		FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

		FMOD_VECTOR      listenerpos  = { 0.0f, 0.0f, -1.0f * DISTANCEFACTOR };
		listenerpos.x = (float)sin(t * 0.05f) * 33.0f * DISTANCEFACTOR; // left right pingpong
		

		// ********* NOTE ******* READ NEXT COMMENT!!!!!
		// vel = how far we moved last FRAME (m/f), then time compensate it to SECONDS (m/s).
		vel.x = (listenerpos.x - lastpos.x) * (1000 / INTERFACE_UPDATETIME);
		vel.y = (listenerpos.y - lastpos.y) * (1000 / INTERFACE_UPDATETIME);
		vel.z = (listenerpos.z - lastpos.z) * (1000 / INTERFACE_UPDATETIME);

		// store pos for next time
		lastpos = listenerpos;

		result = fmodSystem->set3DListenerAttributes(0, &listenerpos, &vel, &forward, &up);

		 t += (30 * (1.0f / (float)INTERFACE_UPDATETIME));

	switch( sound)
	{
		case FlySound_Wings:
			result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sounds[FlySound_Wings], false,&channel);
			result = channel->setVolume(0.8f);
		break;

		case FlySound_Collision:
			result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sounds[FlySound_Collision], false,&channel);
			result = channel->setVolume(0.8f);
		break;
		case FlySound_LevelMusic:
			result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sounds[FlySound_LevelMusic], false,&channel);
			result = channel->setVolume(0.8f);
		break;
		case FlySound_Wind:
			result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sounds[FlySound_Wind], false,&channel);
			result = channel->setVolume(0.8f);
		break;
	}
	
}
void AudioClass::playSoundTrack()
{
	result = fmodSystem->playSound(FMOD_CHANNEL_FREE, soundTrack, false, &channel);
}