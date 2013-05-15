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
			void FlyEditCLIWrapper::GetSelected(String^% name, int% id, float% rx, float% ry, float% rz, float% sx, float% sy, float% sz)
			{
				std::wstring cpp_name = L"";
				int cpp_id = -1;
				float cpp_rx = 0.0f;
				float cpp_ry = 0.0f;
				float cpp_rz = 0.0f;
				float cpp_sx = 0.0f;
				float cpp_sy = 0.0f;
				float cpp_sz = 0.0f;

				if(this->flyEngine->GetSelected(cpp_name, cpp_id, cpp_rx, cpp_ry, cpp_rz, cpp_sx, cpp_sy, cpp_sz))
				{
					name = marshal_as<String^>(cpp_name.c_str()); 
					id = cpp_id;
					rx = cpp_rx;
					ry = cpp_ry;
					rz = cpp_rz;
					sx = cpp_sx;
					sy = cpp_sy;
					sz = cpp_sz;
				}
			}
			void FlyEditCLIWrapper::SetRotation			(float x, float y, float z)
			{
				this->flyEngine->SetRotation(x, y, z);
			}
			void FlyEditCLIWrapper::SetScale			(float x, float y, float z)
			{
				if(x == 0 || y == 0 || z == 0)
					return;

				this->flyEngine->SetScale(x, y, z);
			}
			bool FlyEditCLIWrapper::SetName				(String^ name)
			{
				const wchar_t* n = (wchar_t*)Marshal::StringToHGlobalUni(name).ToPointer();
				return this->flyEngine->SetName(n);
			}

			void FlyEditCLIWrapper::OnKeyEvent(int key, bool released, bool ctrl, bool shift, bool alt)
			{
				this->flyEngine->OnKeyEvent(key, released, ctrl, shift, alt);
			}
			void FlyEditCLIWrapper::OnMouseBtnEvent(int key, bool released, bool ctrl, bool shift, bool alt)
			{
				this->flyEngine->OnMouseBtnEvent(key, released, ctrl, shift, alt);
			}
			void FlyEditCLIWrapper::OnMouseMoveEvent(int cx, int cy, int rx, int ry, bool ctrl, bool shift, bool alt)
			{
				this->flyEngine->OnMouseMoveEvent(cx, cy, rx, ry, ctrl, shift, alt);
			}
			void FlyEditCLIWrapper::OnMouseScrollEvent(int delta, bool ctrl, bool shift, bool alt)
			{
				this->flyEngine->OnMouseScrollEvent(delta, ctrl, shift, alt);
			}
			void FlyEditCLIWrapper::SetFlyMode(bool flymode)
			{
				this->flyEngine->SetFlyMode(flymode);
			}
			void FlyEditCLIWrapper::GetSpeed(float% speed)
			{
				float _s = 0.0f;
				this->flyEngine->GetSpeed(_s);
				speed = _s;
			}
			void FlyEditCLIWrapper::SetSpeed(float speed)
			{
				this->flyEngine->SetSpeed(speed);
			}
			bool FlyEditCLIWrapper::LoadLevel(String^ path, Dictionary<String^, int>^ mesh, Dictionary<String^, int>^ lights, Dictionary<String^, int>^ pickups)
			{
				std::map<std::wstring, int> _m;
				std::map<std::wstring, int> _l;
				std::map<std::wstring, int> _p;

				if(!this->flyEngine->LoadLevel((wchar_t*)Marshal::StringToHGlobalUni(path).ToPointer(), &_m, &_l, &_p))
					return false;
				MessageBox::Show(gcnew Window(), "1");
				//Mesh
				std::map<std::wstring, int>::iterator _im = _m.begin();
				for (_im; _im != _m.end(); _im++)
					mesh->Add(gcnew String(_im->first.c_str()), _im->second);
				MessageBox::Show(gcnew Window(), "2");
				//Lights
				std::map<std::wstring, int>::iterator _il = _l.begin();
				for (_il; _il != _l.end(); _il++)
					lights->Add(gcnew String(_il->first.c_str()), _il->second);
				MessageBox::Show(gcnew Window(), "3");
				//Pickups
				std::map<std::wstring, int>::iterator _ip = _p.begin();
				for (_ip; _ip != _p.end(); _ip++)
					pickups->Add(gcnew String(_ip->first.c_str()), _ip->second);
				MessageBox::Show(gcnew Window(), "4");
				return true;
			}
			bool FlyEditCLIWrapper::SaveLevel(String^ path, String^ levelName)
			{
				return this->flyEngine->SaveLevel((wchar_t*)Marshal::StringToHGlobalUni(path).ToPointer(), (wchar_t*)Marshal::StringToHGlobalUni(levelName).ToPointer());
			}
}}}