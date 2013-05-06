#ifndef FLY_EDIT_CLI_WAPPER_H
#define FLY_EDIT_CLI_WAPPER_H



//forwarddeklarationen måste ligga innan alla "using" !!! annars tolkas
//det av visual studio som att GameEngineWrapper-klassen ligger i ett namespace
//#include "..\FlyGame\EngineCLIWrapper\EngineWrapper.h"
#include "..\FlyGameEngine\FlyEngine\FlyEngine.h"

#include <stdlib.h>
#include <stdio.h>

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
					FlyEngine* flyEngine;
					std::vector<Entity*> *entities;

				public:
					FlyEditCLIWrapper			();
					~FlyEditCLIWrapper			();

					bool Init					(IntPtr hWnd, int width, int height);
					HRESULT Shutdown			();
					
					HRESULT ProcessFrame		();

					void LoadResources			(array<String^>^ resourcePath, Dictionary<String^, int>^ loadedObjects);

					void OnResize				(int width, int height);
			};
		}
	}
}

#endif