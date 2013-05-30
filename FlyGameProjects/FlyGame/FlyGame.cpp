#include "FlyGame.h"
#include "GameObjects\Cutscene.h"

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

#else
void SetStdOutToNewConsole(){}
#endif



struct FlyGame::_DATA_
{
	SmartPtrStd<IFlySystemState> level;
	SmartPtrStd<IFlySystemState> mainMenu;
	FlyEngine* fly;
	IFlySystemState* state;
	lua_State* luaState;
	bool stateChanged;
	HANDLE loadingThread;
	FlyGameSystemState enumState;

	wstring levelPath;

	
};
FlyCutsceneType currentCut = FlyCutsceneType_Intro;


DWORD WINAPI FlyGame::playCutscene(LPVOID lpParameter)
{
	if(!FlyCutscene::RunCutscene(currentCut, (FlyEngine*)lpParameter))
		return E_FAIL;

	return 1;
}

FlyGame::FlyGame()
{
	this->_pData = new _DATA_();
	this->_pData->fly = 0;
	this->_pData->state = 0;
	this->_pData->levelPath = L"";
	this->_pData->stateChanged = false;
	this->_pData->enumState = Menu;
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
bool FlyGame::Initiate()		  
{

	SetStdOutToNewConsole();

	//We should load an .ini file to determinate initialization values

	FLY_ENGINE_INIT_DESC cd;
	cd.winWidth			= 1200;
	cd.winHeight		= 600;
	cd.fullscreen		= false;


	this->_pData->mainMenu = new FlyState_Menu();
	this->_pData->state = this->_pData->mainMenu;


	this->_pData->luaState = luaL_newstate();
	luaL_openlibs(this->_pData->luaState);

	this->_pData->fly = FlyEngineCreate();
	if(!this->_pData->fly->Core_Initialize(cd))
		return false;

	if(!this->_pData->fly->Input_Initialize())
		return false;

	this->_pData->fly->Audio_Initialize();

	//SECURITY_ATTRIBUTES attr;
	//attr.nLength = sizeof(SECURITY_ATTRIBUTES);
	//attr.lpSecurityDescriptor = THREAD_SUSPEND_RESUME;

	this->_pData->loadingThread	= CreateThread(NULL , 4*255, FlyGame::playCutscene, (void*)this->_pData->fly, CREATE_SUSPENDED, NULL);
	DWORD test = ResumeThread(this->_pData->loadingThread);
	/** Fix resource importers to handle multiple loads, to actualy win some time */
	

#if defined(_DEBUG) || defined(DEBUG)
	time_t start = clock();
	if(!this->_pData->state->Initiate(this))
		return false;
	std::cout << "State loaded on " << clock()-start << "ms\n";
#else
	if(!this->_pData->state->Initiate(this))
		return false;
#endif

	WaitForSingleObject(this->_pData->loadingThread, INFINITE);
	TerminateThread(this->_pData->loadingThread, 0);

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

			if(this->_pData->stateChanged)
				handleStateChange();

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


void FlyGame::setLevelPath(wchar_t* level)
{
	this->_pData->levelPath = level;
}
void FlyGame::setState(IFlySystemState* _state)
{
	this->_pData->state = _state;
	this->_pData->stateChanged = true;
}
void FlyGame::setState(FlyGameSystemState _state)
{
	switch (_state)
	{
		case Level_1:
		case Level_2:
		case Level_3:
			this->_pData->level = new FlyState_Level();
			this->_pData->state = this->_pData->level;
		break;

		case Menu:
			this->_pData->mainMenu = new FlyState_Menu();
			this->_pData->state = this->_pData->mainMenu;
		break;
	}
	this->_pData->enumState = _state;
	this->_pData->stateChanged = true;
}
void FlyGame::handleStateChange()
{
	this->_pData->stateChanged = false;

	if(this->_pData->state)
	{
		switch (this->_pData->enumState)
		{
			case Level_1:
				currentCut = FlyCutsceneType_Level1;
			break;

			case Level_2:
				currentCut = FlyCutsceneType_Level2;
			break;

			case Level_3:
				currentCut = FlyCutsceneType_Level3;
			break;
		}
		
		this->_pData->loadingThread	= CreateThread(NULL , 4*255, FlyGame::playCutscene, (void*)this->_pData->fly, CREATE_SUSPENDED, NULL);
		ResumeThread(this->_pData->loadingThread);

		if(!this->_pData->state->Initiate(this))
			this->_pData->state = 0;

		WaitForSingleObject(this->_pData->loadingThread, INFINITE);
		TerminateThread(this->_pData->loadingThread, 0);
	}
}
FlyGameSystemState FlyGame::getCurrState() const
{
	return this->_pData->enumState;
}

const wchar_t* FlyGame::getLevel()
{
	return this->_pData->levelPath.c_str();
}