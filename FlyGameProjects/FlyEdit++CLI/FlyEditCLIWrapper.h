#ifndef FLY_EDIT_CLI_WAPPER_H
#define FLY_EDIT_CLI_WAPPER_H

#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

//forwarddeklarationen måste ligga innan alla "using" !!! annars tolkas
//det av visual studio som att GameEngineWrapper-klassen ligger i ett namespace
//#include "..\FlyGame\EngineCLIWrapper\EngineWrapper.h"
#include "..\FlyGame\EngineCLIWrapper\EngineEditorWrapper.h"


using namespace System;
using namespace System::Windows;
using namespace System::Windows::Interop;
using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;


namespace System 
{ 
	namespace Windows 
	{ 
		namespace Interop
		{
			public ref class FlyEditCLIWrapper
			{
				protected:
					EngineEditorWrapper* flyEngine;

				public:
					FlyEditCLIWrapper			();
					~FlyEditCLIWrapper			();

					bool Init					(IntPtr hWnd, int width, int height);
					HRESULT Shutdown			();
					
					HRESULT ProcessFrame		();

					void LoadResources			(array<String^>^ resourcePath, Dictionary<String^, int>^ loadedObjects);

					void OnResize				(int width, int height);
					
					void OnKeyDown				(int key);
					void OnKeyUp				(int key);
					void OnMouseClick			(bool left, int x, int y);
					void OnMouseMove			(int x, int y);
			};
		}
	}
}

#endif