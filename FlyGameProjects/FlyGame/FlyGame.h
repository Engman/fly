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
		friend class IFlySystemState;
		struct _DATA_;
		_DATA_ *_pData;


	public:
		FlyGame();
		virtual~FlyGame();


		bool Initiate(FlyGameSystemState state = Level);


		FlyEngine* GetCoreInstance();


		void Run();
		void Update();
		void Render();


		static FlyGame* self();
		static void Destroy();
};

#endif