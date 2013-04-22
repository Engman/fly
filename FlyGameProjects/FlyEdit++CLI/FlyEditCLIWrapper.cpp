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
				//m_GameEngine = new GameEngineWrapper();
			}

			FlyEditCLIWrapper::~FlyEditCLIWrapper()
			{
				//SAFE_DELETE(m_GameEngine);
			}

			HRESULT FlyEditCLIWrapper::Init(IntPtr hWnd, int width, int height)
			{
				HWND hwnd = (HWND)(void*)hWnd;
				//return m_GameEngine->Init(hwnd, width, height);

				return S_OK;
			}

			HRESULT FlyEditCLIWrapper::Shutdown()
			{
				//SAFE_DELETE(m_GameEngine);

				return S_OK;
			}

			HRESULT FlyEditCLIWrapper::ProcessFrame()
			{
				//return m_GameEngine->ProcessFrame();
				return S_OK;
			}

			HRESULT FlyEditCLIWrapper::OnResize(int width, int height)
			{
				//return m_GameEngine->OnResize(width, height);
				return S_OK;
			}

			String^ FlyEditCLIWrapper::ProcessText(String^ msg)
			{
				char* lpText = 0;
				String^ returnText;

				//Konvert String^ -> char*
				try
				{
					lpText = (char*)Marshal::StringToHGlobalAnsi(msg).ToPointer();

					//returnText = gcnew String(m_GameEngine->ProcessText(lpText));
				}
				finally
				{
					Marshal::FreeHGlobal((IntPtr) const_cast<char*>(lpText)); // Free memory
				}

				return returnText;
			}

		}
	}
}