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
					FlyEditCLIWrapper();
					~FlyEditCLIWrapper();

					bool Init(IntPtr hWnd, int width, int height);
					HRESULT Shutdown();
					
					HRESULT ProcessFrame();

					int LoadResources(String^ path);

					HRESULT OnResize(int width, int height);
					HRESULT OnMouseDown(int x, int y);

					String^ ProcessText(String^ text);
			};
		}
	}
}

#endif