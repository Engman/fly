#include "FlyEditCLIWrapper.h"
#include <sstream>
#include <map>

namespace System { namespace Windows { namespace Interop {



			FlyEditCLIWrapper::FlyEditCLIWrapper()
			{
				this->flyEngine = FlyEngineCreate();
				this->entities = new std::vector<Entity*>();
			}
			
			FlyEditCLIWrapper::~FlyEditCLIWrapper()
			{
				delete this->flyEngine;
				this->flyEngine = NULL;

				for (int i = 0; i < (int)this->entities->size(); i++)
				{
					delete (*this->entities)[i];
				}
				delete this->entities;
			}

			bool FlyEditCLIWrapper::Init(IntPtr _hWnd, int width, int height)
			{
				HWND hwnd = (HWND)(void*)_hWnd;
				FLY_ENGINE_INIT_DESC desc;
				desc.parent = hwnd;
				desc.winWidth = width;
				desc.winHeight = height;
				desc.windowName = L"Fly Editor";
				if( FAILED (this->flyEngine->Core_Initialize(desc) ) )
					return false;


				return true;
			}

			HRESULT FlyEditCLIWrapper::Shutdown()
			{
				this->flyEngine->Core_Shutdown();
				return S_OK;
			}

			HRESULT FlyEditCLIWrapper::ProcessFrame()
			{
				this->flyEngine->Gfx_BeginDeferredScene();

				this->flyEngine->Gfx_EndDeferredScene();
				return 1;
			}

			void FlyEditCLIWrapper::LoadResources(array<String^>^ resourcePath, Dictionary<String^, int>^ loadedObjects)
			{
				if(resourcePath->Length == 0)
					return;
				
				std::vector<const wchar_t*> path((int)resourcePath->Length);
				for(int i = 0; i<resourcePath->Length; i++)
				{
					path[i] = (wchar_t*)Marshal::StringToHGlobalUni(resourcePath[i]).ToPointer();
				}
				
				std::vector<SmartPtrStd<FlyMesh>> loaded;
				this->flyEngine->Geometry_Load(path, loaded);
	
			}

			void FlyEditCLIWrapper::OnResize	(int width, int height)
			{
				
				this->flyEngine->Gfx_Resize(width, height);
			}
}}}