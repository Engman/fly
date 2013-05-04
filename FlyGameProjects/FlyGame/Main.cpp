#include <Windows.h>

#include "FlyGame.h"


int WINAPI WinMain( HINSTANCE hInst, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
{
	FlyGame fg;

	fg.Initiate();
	fg.Run();
	
	return cmdShow;

}
