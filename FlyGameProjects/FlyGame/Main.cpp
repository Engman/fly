#include <Windows.h>

#include "FlyGame.h"


int WINAPI WinMain( HINSTANCE hInst, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
{
	FlyGame fg;

	fg.Initiate(Level);
	fg.Run();
	
	//fg.Destroy();
	return cmdShow;

}
