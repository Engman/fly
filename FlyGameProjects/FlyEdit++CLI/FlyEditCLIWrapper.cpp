#include "FlyEditCLIWrapper.h"
#include <map>



namespace System { namespace Windows { namespace Interop {


			FlyEditCLIWrapper::FlyEditCLIWrapper()
			{
				this->flyEngine = GetEditorInstance();
			}
			
			FlyEditCLIWrapper::~FlyEditCLIWrapper()
			{
				delete this->flyEngine;
				this->flyEngine = NULL;
			}

			bool FlyEditCLIWrapper::Init(IntPtr _hWnd, int width, int height)
			{
				HWND hwnd = (HWND)(void*)_hWnd;

				if( FAILED (this->flyEngine->Initiate(hwnd, width, height) ) )
					return false;

				return true;
			}

			bool FlyEditCLIWrapper::Shutdown()
			{
				this->flyEngine->Terminate();
				return true;
			}

			bool FlyEditCLIWrapper::ProcessFrame()
			{
				if(this->flyEngine)
					this->flyEngine->Frame();
				else
					return false;

				return true;
			}
			
			bool FlyEditCLIWrapper::LoadResources(array<String^>^ resourcePath, Dictionary<String^, int>^ loadedObjects)
			{
				if(resourcePath->Length == 0)
					return false;
				
				std::vector<const wchar_t*> path((int)resourcePath->Length);
				for(int i = 0; i<resourcePath->Length; i++)
				{
					path[i] = (wchar_t*)Marshal::StringToHGlobalUni(resourcePath[i]).ToPointer();
				}
				std::map<std::wstring, int> loaded;
				if( !this->flyEngine->LoadData(path, &loaded) )
					return false;
				
				std::map<std::wstring, int>::iterator it = loaded.begin();
				for (it; it != loaded.end(); it++)
				{
					loadedObjects->Add(gcnew String(it->first.c_str()), it->second);
				}

				return true;
			}

			void FlyEditCLIWrapper::OnResize	(int width, int height)
			{
				this->flyEngine->Resize(width, height);
			}

			void FlyEditCLIWrapper::ChangeView	(Cameras view)
			{
				switch (view)
				{
					case System::Windows::Interop::Cameras::FirstPerson:
						this->flyEngine->SetCamera(FlyLevelEditor::CAM_DEFAULT);
					break;

					case System::Windows::Interop::Cameras::Top:
						this->flyEngine->SetCamera(FlyLevelEditor::CAM_TOP);
					break;
				}
			}

			
}}}