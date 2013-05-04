#include "FlyEditCLIWrapper.h"
#include <sstream>

namespace System { namespace Windows { namespace Interop {



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
				//return this->flyEngine->Shutdown();
				return S_OK;
			}

			HRESULT FlyEditCLIWrapper::ProcessFrame()
			{
				this->flyEngine->Frame();
				return 1;
			}

			void FlyEditCLIWrapper::LoadResources(array<String^>^ resourcePath, Dictionary<String^, int>^ loadedObjects)
			{
				if(resourcePath->Length == 0)
					return;
				
				std::vector<std::wstring> path((int)resourcePath->Length);
				for(int i = 0; i<resourcePath->Length; i++)
				{
					path[i] = (wchar_t*)Marshal::StringToHGlobalUni(resourcePath[i]).ToPointer();
				}
				
				std::map<std::wstring, int> loaded;
				this->flyEngine->LoadResource(path, loaded);
				

				std::map<std::wstring, int>::const_iterator i = loaded.begin();
				for (i; i != loaded.end(); i++)
				{
					String^ key = gcnew String(i->first.c_str());
					loadedObjects->Add(key, i->second);
				}
			}

			void FlyEditCLIWrapper::OnResize	(int width, int height)
			{
				
				this->flyEngine->ResizeWindow(width, height);
				//return 1;
			}
			

			void FlyEditCLIWrapper::OnKeyDown			(int key)
			{
				this->flyEngine->OnKeyDown(key);
			}
			void FlyEditCLIWrapper::OnKeyUp				(int key)
			{
				this->flyEngine->OnKeyDown(key);
			}
			void FlyEditCLIWrapper::OnMouseClick		(bool left, int x, int y)
			{
				this->flyEngine->OnMouseClick(left, x, y);
			}
			void FlyEditCLIWrapper::OnMouseMove			(int x, int y)
			{
				this->flyEngine->OnMouseMove(x, y);
			}
}}}