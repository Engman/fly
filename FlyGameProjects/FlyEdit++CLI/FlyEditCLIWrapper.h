#ifndef FLY_EDIT_CLI_WAPPER_H
#define FLY_EDIT_CLI_WAPPER_H

//#pragma comment(lib, "FlyLevelEditx64D.lib")

//forwarddeklarationen måste ligga innan alla "using" !!! annars tolkas
//det av visual studio som att GameEngineWrapper-klassen ligger i ett namespace
//#include "..\FlyGame\EngineCLIWrapper\EngineWrapper.h"
#include "..\FlyLevelEdit\FlyLevelEditor.h"
#include <Windows.h>
#include <msclr\marshal.h>


#include <stdlib.h>
#include <stdio.h>


using namespace msclr::interop;
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
					FlyLevelEditor* flyEngine;

				public:
					FlyEditCLIWrapper			();
					~FlyEditCLIWrapper			();

					bool Init					(IntPtr hWnd, int width, int height);
					bool Shutdown				();
					bool ProcessFrame			();
					bool LoadResources			(array<String^>^ resourcePath, Dictionary<String^, int>^ loadedObjects);
					void OnResize				(int width, int height);
					bool ChangeView				(int id);
					bool SelectObject			(int id);
					void GetSelected			(String^ name, int^ id);
					void GetCameras				(Dictionary<String^, int>^ outCameras);
			};
		}
	}
}

#endif