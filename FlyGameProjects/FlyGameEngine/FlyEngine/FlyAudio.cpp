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
	paths.push_back("../../WorkingDir/Resources/Sound/level_background.mp3");	//soundTrack
	paths.push_back("../../WorkingDir/Resources/Sound/button_hover.wav");	//hover
	paths.push_back("../../WorkingDir/Resources/Sound/button_click.wav");	//click

	AudioClass::self()->loadSound(FlySound_Menu, paths);
}
void FLYCALL FlyEngine_Core::Audio_LoadLevelSound(const char* soundTrack)
{
	vector<const char*> paths;
	paths.push_back("../../WorkingDir/Resources/Sound/level_background.mp3");	//soundTrack
	paths.push_back("../../WorkingDir/Resources/Sound/button_hover.wav");	//hover
	paths.push_back("../../WorkingDir/Resources/Sound/button_click.wav");	//click
	paths.push_back(soundTrack);
	paths.push_back("../Resources/Sound/collision_grinding.wav");	//collision
	paths.push_back("../Resources/Sound/energy_pickup.wav");	//energyPickup
	paths.push_back("../Resources/Sound/cargo_pickup.wav");	//cargoPickup
	paths.push_back("../Resources/Sound/player_near_death.wav");	//lowEnergy
	paths.push_back("../Resources/Sound/player_death.wav");	//noEnergy
	paths.push_back("../Resources/Sound/thrusters.wav");	//thrust
	paths.push_back("../Resources/Sound/wind.wav");	//wind
	

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
void FLYCALL FlyEngine_Core::Audio_Update()
{
	AudioClass::self()->uppdateSounds(); 
}