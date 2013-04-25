#include "Core\Application.h"



#ifdef DLL_USAGE

#else
	int WINAPI WinMain( HINSTANCE hInst, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
	{

	#if defined(DEBUG) || defined(_DEBUG)
		FIND_MEMORY_LEAKS;
	#endif

		Application app;

		if(!app.Initialize(hInst, 800, 600))
			return cmdShow;

		app.Run();

		app.Shutdown();

		return cmdShow;

	}

#endif