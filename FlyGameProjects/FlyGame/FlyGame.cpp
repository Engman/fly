#include "FlyGame.h"

#ifndef FLY_CORE_DLL
#define FLY_CORE_DLL
#endif

#include "..\FlyGameEngine\FlyEngine\FlyEngine.h"
#include "States\IFlySystemState.h"
#include "States\FlyState_Level.h"
#include "States\FlyState_Menu.h"
#include "..\FlyGameEngine\Util\SmartPtrs.h"


#if defined(_DEBUG) || defined(DEBUG)
#include <fcntl.h>
#include <io.h>
#include <iostream>
void SetStdOutToNewConsole()
{
    // allocate a console for this app
    AllocConsole();

    // redirect unbuffered STDOUT to the console
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    int fileDescriptor = _open_osfhandle((intptr_t)consoleHandle, _O_TEXT);
    FILE *fp = _fdopen( fileDescriptor, "w" );
    *stdout = *fp;
    setvbuf( stdout, NULL, _IONBF, 0 );
 
    // give the console window a nicer title
	wchar_t str[256];
	wsprintf(str, L"Debug Output");
    SetConsoleTitle(str);

    // give the console window a bigger buffer size
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if ( GetConsoleScreenBufferInfo(consoleHandle, &csbi) )
    {
        COORD bufferSize;
        bufferSize.X = csbi.dwSize.X;
        bufferSize.Y = 50;
        SetConsoleScreenBufferSize(consoleHandle, bufferSize);
    }
}
#endif



struct FlyGame::_DATA_
{
	SmartPtrStd<IFlySystemState> level;
	SmartPtrStd<IFlySystemState> mainMenu;
	FlyEngine* fly;
	IFlySystemState* state;
	lua_State* luaState;
};



FlyGame::FlyGame()
{
	this->_pData = new _DATA_();
	this->_pData->fly = 0;
	this->_pData->state = 0;

}
FlyGame::~FlyGame()
{
	lua_close(this->_pData->luaState);
	this->_pData->luaState = NULL;

	if(this->_pData->fly)					this->_pData->fly->Core_Shutdown();
	this->_pData->fly = NULL;
	
	if(this->_pData->level.IsValid())		this->_pData->level->Release();
	if(this->_pData->mainMenu.IsValid())	this->_pData->mainMenu->Release();
	this->_pData->state = NULL;


	delete this->_pData;
	
}
bool FlyGame::Initiate(FlyGameSystemState state)		  
{
#if defined(_DEBUG) || defined(DEBUG)
	SetStdOutToNewConsole();
	std::cout << "Starting debug session!" << std::endl;
#endif
	//We should load an .ini file to determinate initialization values

	FLY_ENGINE_INIT_DESC cd;
	cd.winWidth			= 1200;
	cd.winHeight		= 600;
	cd.fullscreen		= false;

	switch (state)
	{
		case Level:
			this->_pData->level = new FlyState_Level();
			this->_pData->state = this->_pData->level;
		break;

		case Menu:
			this->_pData->mainMenu = new FlyState_Menu();
			this->_pData->state = this->_pData->mainMenu;
		break;
	}

	this->_pData->luaState = luaL_newstate();
	luaL_openlibs(this->_pData->luaState);

	this->_pData->fly = FlyEngineCreate();
	if(!this->_pData->fly->Core_Initialize(cd))
		return false;

#if defined(_DEBUG) || defined(DEBUG)
	time_t start = clock();
	if(!this->_pData->state->Initiate(this))
		return false;
	std::cout << "State loaded on " << clock()-start << "ms\n";
#else
	if(!this->_pData->state->Initiate(this))
		return false;
#endif
	return true;
}
void FlyGame::Run()							  
{
	if(!this->_pData->fly)
	{
		MessageBox(0, L"Initiate not called!", L"Error!", 0);
		return;
	}

	if(!this->_pData->state)
		return;

#if defined(_DEBUG) || defined(DEBUG)
	cout << "Starting game..\n";
#endif
	MSG msg;
	while (this->_pData->state)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{ 
			if (msg.message == WM_QUIT)
			{
				break;
			}
			DispatchMessage(&msg);
		}
		else
		{
			this->_pData->state->Frame();
		}
	}
}



FlyEngine* FlyGame::GetCoreInstance() const
{
	return this->_pData->fly;
}

lua_State* FlyGame::GetLuaState() const
{
	return this->_pData->luaState;
}