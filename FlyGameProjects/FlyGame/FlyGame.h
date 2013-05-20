#ifndef FLY_GAME_H
#define FLY_GAME_H


#pragma comment(lib, "FlyGameEngineDx86.lib")

#include <Windows.h>


class FlyEngine;

enum FlyGameSystemState
{
	Level,
	Menu,
	Editor,
};


class FlyGame
{
	public:
		
	private:
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