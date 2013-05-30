#include "AudioClass.h"

#if defined (_DEBUG) || defined (DEBUG)
#include <iostream>
#define C_PRINT(STR)\
		std::cout << STR
#else
#define C_PRINT
#endif

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
}

void AudioClass::intitialize()
{
	maxChannels = 40;
	result = System_Create(&fmodSystem);
	
	result = fmodSystem->init(maxChannels, FMOD_INIT_3D_RIGHTHANDED,0);
	result = fmodSystem->set3DSettings(1.0f, 1.0f, 1.0f);

	channel.resize(10);
}
bool AudioClass::loadSound(FlySoundState state, std::vector<const char*> path)
{
	switch(state)
	{
		case FlySound_Menu:
			if(!loadMenuSound(path))
				return false; 
			break;

		case FlySound_Level:
			if(!loadLevelSound(path))
				return false; 
			break;
	}
	return true; 
}

bool AudioClass::loadMenuSound(std::vector<const char*> path)
{
	if(path.size()<SOUNDMENU_COUNT)
	{
		MessageBox(0,L"Too few sounds for level", L"Error!", 0);
		return false; 
	}

	unLoadSounds(); 
	sounds.resize(SOUNDMENU_COUNT);

	//stream the big sound files 
	if ( !FmodErrorCheck( fmodSystem->createStream(path[FlySound_MenuSoundTrack], FMOD_DEFAULT, 0 ,& sounds[FlySound_MenuSoundTrack]) ) )
		return false;
	sounds[FlySound_MenuSoundTrack]->setMode(FMOD_LOOP_NORMAL);

	if ( !FmodErrorCheck( fmodSystem->createSound(path[FlySound_Hover], FMOD_DEFAULT, 0, & sounds[FlySound_Hover]) ) )
		return false;
	sounds[FlySound_Hover]->setMode(FMOD_LOOP_NORMAL);

	if ( !FmodErrorCheck( fmodSystem->createSound(path[FlySound_Click], FMOD_DEFAULT, 0, & sounds[FlySound_Click]) ) )
		return false;
	sounds[FlySound_Click]->setMode(FMOD_LOOP_NORMAL);

	return true; 
}
bool AudioClass::loadLevelSound(std::vector<const char*> path)
{
	if(path.size()<SOUNDLEVEL_COUNT)
	{
		MessageBox(0,L"Too few sounds for level", L"Error!", 0);
		return false; 
	}

	unLoadSounds();
	sounds.resize(SOUNDLEVEL_COUNT);

	//stream the big sound files 
	if ( !FmodErrorCheck( fmodSystem->createStream(path[FlySound_LevelSoundTrack], FMOD_DEFAULT, 0 ,& sounds[FlySound_LevelSoundTrack]) ) )
		return false;
	sounds[FlySound_LevelSoundTrack]->setMode(FMOD_LOOP_NORMAL);
	

	//------Collision sound--------//
	if ( !FmodErrorCheck( fmodSystem->createSound(path[FlySound_Collision], FMOD_DEFAULT, 0, & sounds[FlySound_Collision]) ) )
		return false;
	sounds[FlySound_Collision]->setMode(FMOD_LOOP_OFF);

	
	//----EnergyPickup sound-------//
	if ( !FmodErrorCheck( fmodSystem->createSound(path[FlySound_EnergyPickup], FMOD_DEFAULT, 0, & sounds[FlySound_EnergyPickup]) ) )
		return false;
	sounds[FlySound_EnergyPickup]->setMode(FMOD_LOOP_OFF);

	
	//----CargoPickup sound-------//
	if ( !FmodErrorCheck( fmodSystem->createSound(path[FlySound_CargoPickup], FMOD_DEFAULT, 0, & sounds[FlySound_CargoPickup]) ) )
		return false;
	sounds[FlySound_CargoPickup]->setMode(FMOD_LOOP_OFF);

	//------LowEnergy sound-------//
	if ( !FmodErrorCheck( fmodSystem->createSound(path[FlySound_LowEnergy], FMOD_DEFAULT, 0, & sounds[FlySound_LowEnergy]) ) )
		return false;
	sounds[FlySound_LowEnergy]->setMode(FMOD_LOOP_OFF);

	//------NoEnergy sound-------//
	if ( !FmodErrorCheck( fmodSystem->createSound(path[FlySound_NoEnergy], FMOD_DEFAULT, 0, & sounds[FlySound_NoEnergy]) ) )
		return false;
	sounds[FlySound_NoEnergy]->setMode(FMOD_LOOP_OFF);

	//--------Thrust sound-------//
	if ( !FmodErrorCheck( fmodSystem->createSound(path[FlySound_Thrust], FMOD_DEFAULT, 0, & sounds[FlySound_Thrust]) ) )
		return false;
	sounds[FlySound_Thrust]->setMode(FMOD_LOOP_OFF);
	
	//----------Wind sound-------//
	if ( !FmodErrorCheck( fmodSystem->createSound(path[FlySound_Wind], FMOD_DEFAULT, 0, & sounds[FlySound_Wind]) ) )
		return false;
	if ( !FmodErrorCheck( sounds[FlySound_Wind]->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR) ) )
		return false;
	sounds[FlySound_Wind]->setMode(FMOD_LOOP_NORMAL);

	return true;
}
void AudioClass::unLoadSounds()
{
	for(unsigned int i=0; i<sounds.size(); i++)
	{
		sounds[i]->release(); 
	}
	sounds.shrink_to_fit();
}

void AudioClass::shutdown()
{
	unLoadSounds();
	fmodSystem->close(); 
	fmodSystem->release();
}
void AudioClass::uppdateSounds(playerSoundData soundData)
{
	result = fmodSystem->update();
	//playerVel; 
	float speed = soundData.vel; 
	if(speed>1)
	{
		speed = 1;
	}
	result = channel[FlySound_Wind]->setVolume(speed);
	//FMOD_VECTOR pos = {listenerPos.x, listenerPos.y, listenerPos.z};
	//fmodSystem->set3DListenerAttributes(0, pos, )
}
void AudioClass::playLevelSound(FlyLevelSounds sound)
{	
	if(this->sounds.size() < SOUNDLEVEL_COUNT)
		return;

	bool playing; 
	switch(sound)
	{
		
		case FlySound_LevelSoundTrack:
			
			channel[FlySound_LevelSoundTrack]->isPlaying(&playing);
			if(!playing)
			{
				result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sounds[FlySound_LevelSoundTrack], false,&channel[FlySound_LevelSoundTrack]);
				result = channel[FlySound_LevelSoundTrack]->setVolume(0.4f);
			}
		break;
		case FlySound_Collision:
			channel[FlySound_Collision]->isPlaying(&playing);
			if(!playing)
			{
				result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sounds[FlySound_Collision], false,&channel[FlySound_Collision]);
				result = channel[FlySound_Collision]->setVolume(0.8f);
			}
			
		break;
		case FlySound_EnergyPickup:
			channel[FlySound_EnergyPickup]->isPlaying(&playing);
			if(!playing)
			{
				result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sounds[FlySound_EnergyPickup], false,&channel[FlySound_EnergyPickup]);
				result = channel[FlySound_EnergyPickup]->setVolume(0.8f);
			}		
			break;
		case FlySound_CargoPickup:
			result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sounds[FlySound_CargoPickup], false,&channel[FlySound_CargoPickup]);
			result = channel[FlySound_CargoPickup]->setVolume(0.8f);
		break;
		case FlySound_LowEnergy:
			channel[FlySound_LowEnergy]->isPlaying(&playing);
			if(!playing)
			{
				result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sounds[FlySound_LowEnergy], false,&channel[FlySound_LowEnergy]);
				result = channel[FlySound_LowEnergy]->setVolume(0.8f);
			}	
		break;
		case FlySound_NoEnergy:
			channel[FlySound_NoEnergy]->isPlaying(&playing);
			if(!playing)
			{
				result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sounds[FlySound_NoEnergy], false,&channel[FlySound_NoEnergy]);
				result = channel[FlySound_NoEnergy]->setVolume(0.8f);
			}	
			break;
		case FlySound_Thrust:
			channel[FlySound_Thrust]->isPlaying(&playing);
			if(!playing)
			{
				result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sounds[FlySound_Thrust], false,&channel[FlySound_Thrust]);
				result = channel[FlySound_Thrust]->setVolume(0.35f);
			}
			
		break;
		case FlySound_Wind:
			channel[FlySound_Wind]->isPlaying(&playing);
			if(!playing)
			{
				result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sounds[FlySound_Wind], false,&channel[FlySound_Wind]);
				result = channel[FlySound_Wind]->setVolume(0.3f);
			}	
		break;
	}

}
void AudioClass::playMenuSound(FlyMenuSounds sound)
{	
	if(this->channel.size() < SOUNDLEVEL_COUNT)
		return;

	bool playing; 
	switch(sound)
	{

	case FlySound_MenuSoundTrack:

		channel[FlySound_MenuSoundTrack]->isPlaying(&playing);
		if(!playing)
		{
			result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sounds[FlySound_MenuSoundTrack], false,&channel[FlySound_MenuSoundTrack]);
			result = channel[FlySound_MenuSoundTrack]->setVolume(0.2f);
		}
		break;
	case FlySound_Hover:
		channel[FlySound_Hover]->isPlaying(&playing);
		if(!playing)
		{
			result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sounds[FlySound_Hover], false,&channel[FlySound_Hover]);
			result = channel[FlySound_Hover]->setVolume(0.8f);
		}

		break;
	case FlySound_Click:
		channel[FlySound_Click]->isPlaying(&playing);
		if(!playing)
		{
			result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sounds[FlySound_Click], false,&channel[FlySound_Click]);
			result = channel[FlySound_Click]->setVolume(0.8f);
		}		
		break;
	}
}

void AudioClass::pauseAllSound()
{


}
void AudioClass::toggleSoundTrack()
{
	bool paused;
	channel[FlySound_LevelSoundTrack]->getPaused(&paused);
	channel[FlySound_LevelSoundTrack]->setPaused(!paused);

	channel[FlySound_Wind]->getPaused(&paused);
	channel[FlySound_Wind]->setPaused(!paused);
}

bool AudioClass::FmodErrorCheck(FMOD_RESULT result)	// this is an error handling function
{						// for FMOD errors
	if (result != FMOD_OK)
	{
		C_PRINT("Sound failed");
		return false;
	}
	return true;
}