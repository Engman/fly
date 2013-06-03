#include "FlyGame.h"
#include "GameObjects\Cutscene.h"
#include "..\FlyGameEngine\Util\MutexHandler.h"

#ifndef FLY_CORE_DLL
#define FLY_CORE_DLL
#endif

#include "..\FlyGameEngine\FlyEngine\FlyEngine.h"
#include "States\IFlySystemState.h"
#include "States\FlyState_Level.h"
#include "States\FlyState_Menu.h"
#include "..\FlyGameEngine\Util\SmartPtrs.h"

inline bool fileExists (const std::wstring& name) {
    wifstream f(name.c_str());
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }   
}


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
	IFlySystemState* curState;
	IFlySystemState* prevState;
	lua_State* luaState;
	bool stateChanged;
	HANDLE loadingThread;
	FlyGameSystemState enumState;
	SaveFile savedData; 
	wstring levelPath;
	bool idle;

	
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
	this->_pData->curState = 0;
	this->_pData->levelPath = L"";
	this->_pData->stateChanged = false;
	this->_pData->enumState = Menu;
	this->_pData->idle = false;
}
FlyGame::~FlyGame()
{
	lua_close(this->_pData->luaState);
	this->_pData->luaState = NULL;

	if(this->_pData->fly)					this->_pData->fly->Core_Shutdown();
	this->_pData->fly = NULL;
	
	if(this->_pData->level.IsValid())		this->_pData->level->Release();
	if(this->_pData->mainMenu.IsValid())	this->_pData->mainMenu->Release();
	this->_pData->curState = NULL;


	delete this->_pData;
	
}
bool FlyGame::Initiate()		  
{

	SetStdOutToNewConsole();

	//We should load an .ini file to determinate initialization values

	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	long horizontal = desktop.right;
	long vertical = desktop.bottom;

	FLY_ENGINE_INIT_DESC cd;
	cd.winWidth			= 800;
	cd.winHeight		= 600;
	cd.fullscreen		= false;
	cd.showSplash		= false;
	cd.windowName		= L"Sky Travler";
	cd.vSync			= true;
	cd.multisampling	= true;
	cd.winPosX			= 50;
	cd.winPosY			= 50;
	


	this->_pData->mainMenu = new FlyState_Menu();
	this->_pData->curState = this->_pData->mainMenu;

	this->_pData->luaState = luaL_newstate();
	luaL_openlibs(this->_pData->luaState);

	this->_pData->fly = FlyEngineCreate();
	if(!this->_pData->fly->Core_Initialize(cd))
		return false;

	if(!this->_pData->fly->Input_Initialize())
		return false;

	this->_pData->fly->Audio_Initialize();

	loadSaveFile(L"..\\Resources\\Levels\\saveFile.fgs");


	this->_pData->loadingThread	= CreateThread(NULL , 4*255, FlyGame::playCutscene, (void*)this->_pData->fly, CREATE_SUSPENDED, NULL);
	ResumeThread(this->_pData->loadingThread);
	/** Fix resource importers to handle multiple loads, to actualy win some time */
	

#if defined(_DEBUG) || defined(DEBUG)
	time_t start = clock();
	if(!this->_pData->curState->Initiate(this))
		return false;
	std::cout << "State loaded on " << clock()-start << "ms\n";
#else
	if(!this->_pData->curState->Initiate(this))
	{
		TerminateThread(this->_pData->loadingThread, 0);
		return false;
	}
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

	if(!this->_pData->curState)
		return;


	MSG msg;
	
	while (this->_pData->curState)
	{
		if (!this->_pData->idle)
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
				this->_pData->curState->Frame();

				if(this->_pData->stateChanged)
					handleStateChange();

			}
		}
	}
}

void FlyGame::setIdle(bool idle)
{
	this->_pData->idle = idle;
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
	this->_pData->prevState = this->_pData->curState;
	this->_pData->curState = _state;
	this->_pData->stateChanged = true;
}
void FlyGame::setState(FlyGameSystemState _state)
{
	this->_pData->prevState = this->_pData->curState;
	switch (_state)
	{
		case Level_1:
		case Level_2:
		case Level_3:
			if(!this->_pData->level.IsValid())
				this->_pData->level = new FlyState_Level();
			this->_pData->curState = this->_pData->level;
		break;

		case Menu:
			if(!this->_pData->mainMenu.IsValid())
				this->_pData->mainMenu = new FlyState_Menu();
			this->_pData->curState = this->_pData->mainMenu;
		break;
	}
	this->_pData->enumState = _state;
	this->_pData->stateChanged = true;
}
void FlyGame::handleStateChange()
{
	bool doCut = false;
	this->_pData->stateChanged = false;

	if(this->_pData->curState)
	{
		switch (this->_pData->enumState)
		{
			case Level_1:
				currentCut = FlyCutsceneType_Level1;
				doCut = true;
			break;

			case Level_2:
				currentCut = FlyCutsceneType_Level2;
				doCut = true;
			break;

			case Level_3:
				currentCut = FlyCutsceneType_Level3;
				doCut = true;
			break;
		}
		
		if(doCut)
		{
			this->_pData->loadingThread	= CreateThread(NULL , 4*255, FlyGame::playCutscene, (void*)this->_pData->fly, CREATE_SUSPENDED, NULL);
			ResumeThread(this->_pData->loadingThread);

			if(!this->_pData->curState->Initiate(this))
				this->_pData->curState = 0;

			WaitForSingleObject(this->_pData->loadingThread, INFINITE);
			TerminateThread(this->_pData->loadingThread, 0);
			//this->_pData->idle = true;
		}
		else
		{
			this->_pData->level->Release();
			this->_pData->level.Destroy();
			this->_pData->fly->Audio_PlayMenuSound(FlySound_MenuSoundTrack);
		}
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

std::vector<bool> FlyGame::getLvlSavedData()
{
	return this->_pData->savedData.levels[_pData->enumState].cargoTaken; 
}
void FlyGame::setLvlSaveData(int cargoTaken)
{

	this->_pData->savedData.levels[_pData->enumState].cargoTaken[cargoTaken] = true; 
	this->_pData->savedData.cargoCount++; 

	bool taken = true; 
	int nr = 0; 
	while(taken && nr<CARGO_COUNT)
	{
		if(!this->_pData->savedData.levels[_pData->enumState].cargoTaken[nr])
			taken = false; 
		nr ++; 
	}
	if(taken)
		this->_pData->savedData.levels[_pData->enumState].lvlCompleted = true; 

	nr = 0; 
	bool completed = true; 
	//while(completed && nr<CARGO_COUNT)
	//{
	//	if(!this->_pData->savedData.levels[_pData->enumState].lvlCompleted)
	//		completed = false; 
	//	nr++; 
	//}
	//if(completed)
	//{
	//	for(int i = 0; i< CARGO_COUNT; i++)
	//	{
	//		for(int k = 0; k<CARGO_COUNT; k ++)
	//			this->_pData->savedData.levels[i].cargoTaken.at(k) = false; 
	//		this->_pData->savedData.levels[i].lvlCompleted = false; 
	//	}
	//	// play outro cutscene
	//}


	WriteSaveFile(_pData->savedData.path, _pData->savedData); 
}
bool FlyGame::isLvlCompleted()
{
	return this->_pData->savedData.levels[_pData->enumState].lvlCompleted; 
}
void FlyGame::loadSaveFile(const wchar_t* fileName)
{
	readSaveFile(fileName, _pData->savedData); 
	 _pData->savedData.path = fileName; 
}

bool FlyGame::readSaveFile(const wchar_t* fileName, SaveFile & savedData)
{
	if(!fileExists(fileName))
	{
		std::vector<bool> cargo = std::vector<bool>(CARGO_COUNT);
		for (int i = 0; i < CARGO_COUNT; i++)
			cargo[i] = 0;
		savedData.cargoCount = 0;
		for (int i = 0; i < CARGO_COUNT; i++)
		{
			std::wstring lv = L"lvl";
			wchar_t buff[4];
			_itow_s(i, buff, 10);
			lv.append(buff);

			savedData.levels[i].cargoTaken = cargo;
			savedData.levels[i].lvlCompleted = 0;
			savedData.levels[i].name = lv;
		}
	}
	wifstream file(fileName);
	if(!file.is_open())	
		return false;

	wstring name; 
	std::vector<bool> cargo; 
	cargo.resize(CARGO_COUNT);
	bool completed; 
	savedData.cargoCount = 0; 
	for(int i = 0; i<CARGO_COUNT; i++)
	{

		file >> name; 

		for(int k = 0; k<CARGO_COUNT; k++)
		{
			bool t = 0;
			file >> t;
			cargo[k] = t;
			if(cargo[k] == 1)
				savedData.cargoCount++; 
		}
		file >> completed;

		savedData.levels[i].name = name; 
		savedData.levels[i].cargoTaken = cargo;
		savedData.levels[i].lvlCompleted = completed;
	}

	return true; 
}

bool FlyGame::WriteSaveFile(const wchar_t* fileName, SaveFile & savedData)
{
	wofstream file(fileName);
	if(!file.is_open())	
		return false;

	wstring save; 

	for(int i =0; i<CARGO_COUNT; i++)
	{
		file << savedData.levels[i].name; 
		file << L" "; 
		file << savedData.levels[i].cargoTaken[0]; 
		file << L" ";
		file << savedData.levels[i].cargoTaken[1]; 
		file << L" ";
		file << savedData.levels[i].cargoTaken[2]; 
		file << L" ";
		file << savedData.levels[i].lvlCompleted; 
		file << L" ";
		file << L"\n"; 
	}

	return true;
}

int FlyGame::getCargoCount()
{
	return this->_pData->savedData.cargoCount;
}