#include "FlyEngine_Core.h"
#include "..\Core\AudioClass.h"



void FLYCALL FlyEngine_Core::Audio_PlaySound(const wchar_t* path)
{
	AudioClass::self()->playSound1();
}