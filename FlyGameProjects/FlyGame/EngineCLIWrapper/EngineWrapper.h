#ifndef __ENGINE_WRAPPER_H__
#define __ENGINE_WRAPPER_H__



#include "..\Core\stdafx.h"

#ifdef DLL_USAGE

	//Engine class
	class Application;

	class DLL_EXPORT EngineWrapper
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

			void Pick(int x, int y);
	};


	#endif

#endif
