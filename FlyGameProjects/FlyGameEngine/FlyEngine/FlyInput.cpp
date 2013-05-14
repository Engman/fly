#include "FlyEngine_Core.h"


#include "..\Core\Input.h"
#include "..\Core\WindowShell.h"



bool FlyEngine_Core::Input_Initialize()
{
	/*Input::GLARE_INPUT_INIT_DESC desc;
	desc.deviceFlag = Input::Flags::NOLEGACY;
	desc.deviceType = Input::Flags::keyboard;
	desc.target = WindowShell::self()->getHWND();
	if(!Input::self()->Register(desc))
		return false;

	desc.deviceType = Input::Flags::mouse;
	desc.deviceFlag = Input::Flags::DAFAULT;
	if(!Input::self()->Register(desc))
		return false;*/

	Input::self()->Initialize(WindowShell::self()->getHINSTANCE(), WindowShell::self()->getHWND(), (int)D3DShell::self()->getWidth(), (int)D3DShell::self()->getHeight());

	return true;
}

bool FlyEngine_Core::Input_Shutdown()
{
	/*if(!Input::self()->Unregister(Input::Flags::keyboard))
		return false;
	if(!Input::self()->Unregister(Input::Flags::mouse))
		return false;*/

	return true;
}

void FlyEngine_Core::Input_Activate()
{
	/*Input::self()->ActivateCallback();*/
}

void FlyEngine_Core::Input_Deactivate()
{
	/*Input::self()->DeactivateCallback();*/
}