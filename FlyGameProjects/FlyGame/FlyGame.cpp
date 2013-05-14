#include "FlyGame.h"

#ifndef FLY_CORE_DLL
#define FLY_CORE_DLL
#endif

#include "..\FlyGameEngine\FlyEngine\FlyEngine.h"
#include "States\IFlySystemState.h"
#include "States\FlyState_Editor.h"
#include "States\FlyState_Level.h"
#include "States\FlyState_Menu.h"
#include "..\FlyGameEngine\Util\SmartPtrs.h"


static FlyGame* FlyGameInstance = NULL;

struct FlyGame::_DATA_
{
	SmartPtrStd<IFlySystemState> editor;
	SmartPtrStd<IFlySystemState> level;
	SmartPtrStd<IFlySystemState> menu;
	FlyEngine* fly;
	IFlySystemState* state;
};



FlyGame::FlyGame()
{
	this->_pData = new _DATA_();
	this->_pData->fly = 0;
	this->_pData->state = 0;

}
FlyGame::~FlyGame()
{
	this->_pData->fly->Core_Shutdown();
	this->_pData->fly = NULL;
	this->_pData->level->Release();
	delete this->_pData;
}

bool FlyGame::Initiate(FlyGameSystemState state)		  
{
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
			this->_pData->menu = new FlyState_Menu();
			this->_pData->state = this->_pData->menu;
		break;

		case Editor:
			this->_pData->editor = new FlyState_Editor();
			this->_pData->state = this->_pData->editor;
		break;
	}

	
	this->_pData->fly = FlyEngineCreate();
	if(!this->_pData->fly->Core_Initialize(cd))
		return false;
	this->_pData->state->Initiate(this);
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

	while (this->_pData->state)
	{
		if(!this->_pData->fly->Core_Message())
		{
			break;
		}

		this->_pData->state->Frame();
	}
}
void FlyGame::Update()						  
{

}
void FlyGame::Render()						  
{

}



FlyGame* FlyGame::self()					  
{
	if(!FlyGameInstance)
		FlyGameInstance = new FlyGame();

	return FlyGameInstance;
}
void FlyGame::Destroy()						  
{
	FlyGameInstance->_pData->fly->Core_Shutdown();
	FlyGameInstance->_pData->fly = NULL;
	delete FlyGameInstance;
	FlyGameInstance = NULL;
}


FlyEngine* FlyGame::GetCoreInstance()
{
	return this->_pData->fly;
}