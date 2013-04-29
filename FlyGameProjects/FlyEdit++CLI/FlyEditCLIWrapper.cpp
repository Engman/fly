#include "FlyEditCLIWrapper.h"
#include <sstream>

namespace System 
{ 
	namespace Windows 
	{ 
		namespace Interop
		{
			FlyEditCLIWrapper::FlyEditCLIWrapper()
			{
				this->flyEngine = new EngineEditorWrapper();
			}

			FlyEditCLIWrapper::~FlyEditCLIWrapper()
			{
				delete this->flyEngine;
				this->flyEngine = NULL;
			}

			bool FlyEditCLIWrapper::Init(IntPtr _hWnd, int width, int height)
			{
				HWND hwnd = (HWND)(void*)_hWnd;
				if( FAILED (this->flyEngine->Init(hwnd, width, height) ) )
					return false;


				return true;
			}

			HRESULT FlyEditCLIWrapper::Shutdown()
			{
				return this->flyEngine->Shutdown();
				return 1;
			}

			HRESULT FlyEditCLIWrapper::ProcessFrame()
			{
				this->flyEngine->Update();
				this->flyEngine->Render();
				return 1;
			}

			int FlyEditCLIWrapper::LoadResources(String^ path)
			{
				wchar_t *msg = (wchar_t*)Marshal::StringToHGlobalUni(path).ToPointer();
				return this->flyEngine->LoadResource(msg);
			}

			HRESULT FlyEditCLIWrapper::OnResize(int width, int height)
			{
				
				return this->flyEngine->OnResize(width, height);
				return 1;
			}
			HRESULT FlyEditCLIWrapper::OnMouseDown(int x, int y)
			{
				return this->flyEngine->OnMouseDown(x, y);
			}




			String^ FlyEditCLIWrapper::ProcessText(String^ msg)
			{
				wchar_t* lpText = 0;
				String^ returnText;

				//Konvert String^ -> char*
				try
				{
					lpText = (wchar_t*)Marshal::StringToHGlobalUni(msg).ToPointer();

					//returnText = gcnew String(m_GameEngine->ProcessText(lpText));
				}
				finally
				{
					Marshal::FreeHGlobal((IntPtr) const_cast<wchar_t*>(lpText)); // Free memory
				}

				return returnText;
			}


		}
	}
}