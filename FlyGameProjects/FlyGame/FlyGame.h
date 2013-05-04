#ifndef FLY_GAME_H
#define FLY_GAME_H


#pragma comment(lib, "FlyGameEngineDx86.lib")

#include <Windows.h>

#ifdef FLYDLL
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif


class FlyEngine;

enum FlyGameSystemState
{
	Level,
	Menu,
	Editor,
};


class DLLEXPORT FlyGame
{
	public:
		
	private:
		struct _DATA_;
		_DATA_ *_pData;


	public:
		FlyGame();
		virtual~FlyGame();


		bool Initiate(FlyGameSystemState state = Menu);


		FlyEngine* GetCoreInstance();


		void Run();
		void Update();
		void Render();


		static FlyGame* self();
		static void Destroy();
};

#endif

