#include "FlyEngine_Core.h"


#include "..\Core\Input.h"
#include "..\Core\WindowShell.h"



bool FlyEngine_Core::Input_Initialize()
{
	if( FAILED (Input::self()->Initialize(WindowShell::self()->getHINSTANCE(), WindowShell::self()->getHWND(), (int)D3DShell::self()->getWidth(), (int)D3DShell::self()->getHeight() ) ) )
		return false;

	return true;
}

bool FlyEngine_Core::Input_Shutdown()
{
	return true;
}

void FlyEngine_Core::Input_Activate()
{

}

void FlyEngine_Core::Input_Deactivate()
{

}