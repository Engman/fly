#include "Core\Application.h"

//#define TEST



int WINAPI WinMain( HINSTANCE hInst, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
{

#if defined(DEBUG) || defined(_DEBUG)
	FIND_MEMORY_LEAKS;
#endif


#ifdef TEST

	SmartPtrArr<int> myarr;
	myarr = new int[10];
	for (int i = 0; i < 10; i++)
	{
		myarr[i] = i;
	}
	char tex[10];
	for (int i = 0; i < 10; i++)
	{
		char t[1];
		itoa(i, t, 10);
		tex[i] = t[0];
	}

#else
	Application app;

	if(!app.Initialize(hInst))
		return cmdShow;

	app.Run();

	app.Shutdown();

	return cmdShow;
#endif
}
