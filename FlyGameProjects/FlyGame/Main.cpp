#include <Windows.h>

#include "FlyGame.h"
#include "..\FlyGameEngine\Core\stdafx.h"
#include "..\FlyGameEngine\Util\GID.h"


int WINAPI WinMain( HINSTANCE hInst, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
{
	FIND_MEMORY_LEAKS;

	FlyGame *fg = new FlyGame();

	if(!fg->Initiate())
	{
		MessageBox(0, L"Fatal error!", L"Error!", 0);
		delete fg;
		return cmdShow;
	}
	fg->Run();

	
	delete fg;

#if defined(_DEBUG) || defined(DEBUG)
	std::vector<int> mem = GID::getAllocList();
#endif

	return cmdShow;

}
