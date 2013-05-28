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
					bool LoadTerrain			(String^ path);
					void OnResize				(int width, int height);
					bool ChangeView				(int id);
					bool SelectObject			(int id);
					void GetSelected			(String^% name, int% id, float% rx, float% ry, float% rz, float% sx, float% sy, float% sz);
					void GetCameras				(Dictionary<String^, int>^ outCameras);
					void SetRotation			(float x, float y, float z);
					void SetScale				(float x, float y, float z);
					bool SetName				(String^ name);
					void SetFlyMode				(bool flymode);
					void GetSpeed				(float% speed);
					void SetSpeed				(float speed);
					bool LoadLevel				(String^ path, Dictionary<String^, int>^ mesh, Dictionary<String^, int>^ lights, Dictionary<String^, int>^ pickups);
					bool SaveLevel				(String^ path, String^ levelName);

					void OnKeyEvent(int key, bool released, bool ctrl, bool shift, bool alt);
					void OnMouseBtnEvent(int key, bool released, bool ctrl, bool shift, bool alt);
					void OnMouseMoveEvent(int cx, int cy, int rx, int ry, bool ctrl, bool shift, bool alt);
					void OnMouseScrollEvent(int delta, bool ctrl, bool shift, bool alt);
			};
		}
	}
}

#endif