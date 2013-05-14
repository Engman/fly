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
				if( !this->flyEngine->Entity_LoadData(path, &loaded) )
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

			bool FlyEditCLIWrapper::ChangeView	(int id)
			{
				return this->flyEngine->SetCamera(id);
			}
			void FlyEditCLIWrapper::GetCameras	(Dictionary<String^, int>^ outCameras)
			{
				std::map<std::wstring, int> loaded;
				this->flyEngine->GetCameras(&loaded);

				std::map<std::wstring, int>::iterator it = loaded.begin();
				for (it; it != loaded.end(); it++)
				{
					outCameras->Add(gcnew String(it->first.c_str()), it->second);
				}
			}
			bool FlyEditCLIWrapper::SelectObject			(int id)
			{
				return this->flyEngine->Entity_Select(id);
			}
			void FlyEditCLIWrapper::GetSelected(String^ name, int^ id)
			{
				std::wstring cpp_name = L"";
				int cpp_id = -1;
				this->flyEngine->GetSelected(cpp_name, cpp_id);
				
				name = gcnew String(cpp_name.c_str());
				id = gcnew int(cpp_id);

				//wchar_t buff[5];
				//wchar_t* te;
				//te = _itow(cpp_id, buff, 10);
				//MessageBox::Show(gcnew String(te));
			}
}}}