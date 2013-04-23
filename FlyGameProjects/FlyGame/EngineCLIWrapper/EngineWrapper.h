#ifndef ENGINE_WRAPPER_H
#define ENGINE_WRAPPER_H


#include "..\Core\stdafx.h"

//Engine class
class Application;

class DLL_USAGE EngineWrapper
{
	private:
		Application* flyGameEngine;

	public:
		EngineWrapper();
		~EngineWrapper();

		HRESULT Init(HWND hWnd, int width, int height);
		HRESULT Shutdown();

		HRESULT ProcessFrame();
		HRESULT OnResize(int width, int height);
};


#endif

