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
				this->flyEngine = new EngineWrapper();
			}

			FlyEditCLIWrapper::~FlyEditCLIWrapper()
			{
				delete this->flyEngine;
				this->flyEngine = NULL;
			}

			HRESULT FlyEditCLIWrapper::Init(IntPtr _hWnd, int width, int height)
			{
				HWND hwnd = (HWND)(void*)_hWnd;
				if( FAILED (this->flyEngine->Init(hwnd, width, height) ) )
					return E_FAIL;


				return S_OK;
			}

			HRESULT FlyEditCLIWrapper::Shutdown()
			{
				return this->flyEngine->Shutdown();
				return S_OK;
			}

			HRESULT FlyEditCLIWrapper::ProcessFrame()
			{
				return this->flyEngine->ProcessFrame();
				return S_OK;
			}

			HRESULT FlyEditCLIWrapper::OnResize(int width, int height)
			{
				
				return this->flyEngine->OnResize(width, height);
				return S_OK;
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

			void FlyEditCLIWrapper::Pick(int x, int y)
			{
				this->flyEngine->Pick(x, y);
			}

		}
	}
}