#ifndef ENGINE_EDITOR_WRAPPER_H
#define ENGINE_EDITOR_WRAPPER_H


#include "..\Core\stdafx.h"
#include <string>

	#ifdef DLL_USAGE

	class FlyEngine;


	class DLL_EXPORT EngineEditorWrapper
	{
		private:
			FlyEngine *flyEngine;

		public:
			EngineEditorWrapper();
			~EngineEditorWrapper();

			HRESULT Init(HWND hWnd, int width, int height);
			HRESULT Shutdown();

			int LoadResource(std::wstring path);

			HRESULT Update();
			HRESULT Render();

			HRESULT OnResize	(int width, int height);
			HRESULT OnMouseDown	(int x, int y);
			HRESULT OnMouseUp	(int x, int y);
	};


	#endif

#endif
