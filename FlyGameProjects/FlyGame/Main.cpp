#include <Windows.h>

#include "FlyGame.h"
#include "..\FlyGameEngine\Core\stdafx.h"

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
{
	FIND_MEMORY_LEAKS;

	FlyGame fg;


	if(!fg.Initiate(Level))
		MessageBox(0,L"Failed to initialize Fly Game", L"Error!", 0);


	fg.Run();
	
	//fg.Destroy();
	return cmdShow;

}
