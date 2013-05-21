#include "FlyEngine_Core.h"


#include "..\Core\D3DShell.h"
#include "..\Core\AudioClass.h"

void FLYCALL FlyEngine_Core::Audio_Initialize()
{
	AudioClass::self()->intitialize();
	AudioClass::self()->loadSound();
}
void FLYCALL FlyEngine_Core::Audio_Shutdown()
{
	AudioClass::self()->shutdown();
}
void FLYCALL FlyEngine_Core::Audio_PlaySound(FlyEngineSounds sound)
{
	AudioClass::self()->playSound(sound);
}
