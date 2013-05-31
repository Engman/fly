#ifndef FLY_GAME_H
#define FLY_GAME_H


#if defined(_DEBUG) || defined(DEBUG)
#pragma comment(lib, "FlyGameEngineDx86.lib")
#else
#pragma comment(lib, "FlyGameEnginex86.lib")
#endif
#pragma comment(lib, "lua52.lib")

#include <Windows.h>
extern "C"
{
	#include <Lua/lua.h>
	#include <Lua/lualib.h>
	#include <Lua/lauxlib.h>
}

#include "..\FlyGameEngine\Util\misc.h"


class FlyEngine;
class IFlySystemState;

enum FlyGameSystemState
{
	Level_1,
	Level_2,
	Level_3,
	Menu,
	Editor,
};

static const int CARGO_COUNT = 3;

struct LevelData
{
	std::wstring name;
	std::vector<int> cargoTaken;
	bool lvlCompleted; 
};
struct SaveFile
{
	const wchar_t* path; 
	int cargoCount; 
	LevelData levels [CARGO_COUNT]; 
};
class FlyGame
{
	public:
		
		

	private:
		struct _DATA_;
		_DATA_ *_pData;
		friend class FlyState_Menu;
		friend class FlyState_Level;

		void setLevelPath(wchar_t* level);
		void setState(IFlySystemState* state);
		void setState(FlyGameSystemState state);
		void handleStateChange();
		FlyGameSystemState getCurrState() const;
		std::vector<int> getLvlSavedData(); 
		void setLvlSaveData(int cargoTaken);
		bool isLvlCompleted();
		int getCargoCount(); 
		void loadSaveFile(const wchar_t* fileName); 
		static DWORD WINAPI FlyGame::playCutscene(LPVOID lpParameter);
		bool readSaveFile(const wchar_t* fileName, SaveFile & savedData);
		bool WriteSaveFile(const wchar_t* fileName, SaveFile & savedData); 


	public:
		FlyGame();
		virtual~FlyGame();


		bool Initiate();
		


		FlyEngine* GetCoreInstance() const;
		lua_State* GetLuaState() const;

		void Run();
		void Update();
		void Render();
		const wchar_t* getLevel();

};

#endif