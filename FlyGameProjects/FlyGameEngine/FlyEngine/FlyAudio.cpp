#include "FlyEngine_Core.h"


#include "..\Core\D3DShell.h"
#include "..\Core\AudioClass.h"

void FLYCALL FlyEngine_Core::Audio_Initialize()
{
	AudioClass::self()->intitialize();
	
}
void FLYCALL FlyEngine_Core::Audio_LoadMenuSound()
{
	vector<const char*> paths;
	paths.push_back("../../WorkingDir/Resources/Sound/jaguar.wav");	//soundTrack
	paths.push_back("../../WorkingDir/Resources/Sound/swish.wav");	//hover
	paths.push_back("../../WorkingDir/Resources/Sound/swish.wav");	//click

	AudioClass::self()->loadSound(FlySound_Menu, paths);
}
void FLYCALL FlyEngine_Core::Audio_LoadLevelSound(const char* soundTrack)
{
	vector<const char*> paths;
	paths.push_back(soundTrack);
	paths.push_back("../Resources/Sound/jaguar.wav");	//collision
	paths.push_back("../Resources/Sound/jaguar.wav");	//energyPickup
	paths.push_back("../Resources/Sound/jaguar.wav");	//cargoPickup
	paths.push_back("../Resources/Sound/jaguar.wav");	//lowEnergy
	paths.push_back("../Resources/Sound/jaguar.wav");	//noEnergy
	paths.push_back("../Resources/Sound/jaguar.wav");	//thrust
	paths.push_back("../Resources/Sound/swish.wav");	//wind
	

	AudioClass::self()->loadSound(FlySound_Level, paths);
}

void FLYCALL FlyEngine_Core::Audio_Shutdown()
{
	AudioClass::self()->shutdown();
}
void FLYCALL FlyEngine_Core::Audio_PlaySound(FlyLevelSounds sound)
{
	AudioClass::self()->playLevelSound(sound);
}
void FLYCALL FlyEngine_Core::Audio_PlayMenuSound(FlyMenuSounds sound)
{
	AudioClass::self()->playMenuSound(sound);
}

void FLYCALL FlyEngine_Core::Audio_ToggleSoundTrack()
{
	AudioClass::self()->toggleSoundTrack(); 
}
void FLYCALL FlyEngine_Core::Audio_Update(D3DXVECTOR3 pos, float speed)
{
	AudioClass::playerSoundData data; 
	data.pos = pos; 
	data.vel = speed; 
	AudioClass::self()->uppdateSounds(data); 
}