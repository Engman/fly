#include "FlyEngine_Core.h"

#include "..\Core\WindowShell.h"
#include "..\Core\Input.h"
#include "..\Core\Render\GBufferShader.h"
#include "..\Core\Render\ColorShader.h"
#include "..\Core\Render\LightShader.h"
#include "..\Util\Camera.h"



SmartPtrStd<FlyEngine> pFlyEngine;

static LRESULT CALLBACK FlyEngineCoreWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


BOOL APIENTRY DllMain(HANDLE, DWORD, LPVOID)
{
    return TRUE;
}


FlyEngine* FLYCALL FlyEngineCreate()
{
	FlyEngine_Core::_Interface();

	return pFlyEngine;
}



void FlyEngine_Core::_Interface()
{
	if(!pFlyEngine.IsValid())
		pFlyEngine = new FlyEngine_Core();

}



FlyEngine_Core::FlyEngine_Core()
{
	D3DShell::self();
	WindowShell::self();
	//Input::self();

	this->gbufferShader			= new GBufferShader();
	this->colorShader			= new ColorShader();
	this->lightShader			= new LightShader();
	this->matrixBuffer			= new BaseBuffer();
	this->fsq					= new FullScreenQuad();
	this->defaultCam			= new Camera();
	this->activeCamera			= NULL;
	this->deferredRenderFunc	= 0;
	this->deferredUpdateFunc	= 0;
	this->forwardRenderFunc		= 0;
	this->forwardUpdateFunc		= 0;
	this->splash				= 0;
	this->orthographicCamera	= 0;
}



bool FLYCALL FlyEngine_Core::Core_Run()
{
	if(!WindowShell::self()->getHWND())
		return false;

	if(this->splash)
		this->_ShowSplash();
	
	MSG	msg;
	while (true)
	{

		if(!WindowShell::self()->getParent())
		{
			if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{ 
				if (msg.message == WM_QUIT)	
					break;
				DispatchMessage(&msg);
				continue;
			}
		}

		
		//Deferred rendering pass
		if(this->deferredUpdateFunc) this->deferredUpdateFunc();
		if(this->deferredRenderFunc) 
		{
			Gfx_BeginDeferredScene();
			this->deferredRenderFunc();
			Gfx_EndDeferredScene();
		}

		//Forward rendering pass
		if(this->forwardUpdateFunc) this->forwardUpdateFunc();
		if(this->forwardRenderFunc) 
		{
			Gfx_BeginForwardScene();
			this->forwardRenderFunc();
			Gfx_EndForwardScene();
		}


	}
	return true;
}

bool FLYCALL FlyEngine_Core::Core_Initialize(FLY_ENGINE_INIT_DESC& desc)
{
	this->deferredRenderFunc	= desc.deferredRenderFunc;
	this->deferredUpdateFunc	= desc.deferredUpdateFunc;
	this->forwardRenderFunc		= desc.forwardRenderFunc;
	this->forwardUpdateFunc		= desc.forwardUpdateFunc;
	this->splash				= desc.showSplash;

	if(!this->_InitWin(desc))			return false;
	if(!this->_InitGfx(desc))			return false;
	if(!this->_InitGBuffers())			return false;
	if(!this->_InitColorShader())		return false;
	if(!this->_InitMatrixBuffer())		return false;

	if(!this->fsq->Initialize(D3DShell::self()->getDevice(), this->colorShader))
		return false;

	this->_InitCam();

	return true;
}

void FLYCALL FlyEngine_Core::Core_Shutdown()
{
	D3DShell::self()->destroy();
	WindowShell::self()->destroy();
	Input::self()->ReleaseInput();

	this->gbufferShader.Destroy();			
	this->colorShader.Destroy();			
	this->lightShader.Destroy();			
	this->matrixBuffer.Destroy();			
	this->fsq.Destroy();
	this->defaultCam.Destroy();

	pFlyEngine.Destroy();
}

bool FLYCALL FlyEngine_Core::Core_Message()
{
	MSG	msg;

	if(!WindowShell::self()->getParent())
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
			if (msg.message == WM_QUIT)
			{
				PostQuitMessage(0);
				return false;
			}
			DispatchMessage(&msg);
			return true;
		}
	}

	return true;
}

LRESULT CALLBACK FlyEngineCoreWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int cc = 0;
	switch (message)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		
		case WM_INPUT:
			//Input::self()->proccessRawDeviceData(lParam);
		break;

		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			PostMessage(WindowShell::self()->getParent(), message, wParam, lParam);
		break;

		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			PostMessage(WindowShell::self()->getParent(), message, wParam, lParam);
		break;

		case WM_MOUSEMOVE:
			PostMessage(WindowShell::self()->getParent(), message, wParam, lParam);
		break;
		case 0x020A: // WM_MOUSEWHEEL, GET_WHEEL_DELTA_WPARAM(wparam);
		break;
	}
	
	return DefWindowProc(hWnd, message, wParam, lParam);
}



//################################//
//########### LOCAL DATA #########//
//####################################################################//
bool FlyEngine_Core::_InitWin(FLY_ENGINE_INIT_DESC& desc)
{
	WindowShell::INIT_DESC_WINDOW descWindow;
	if(desc.handle)
		descWindow.hInstance		= desc.handle;
	else
		descWindow.hInstance		= GetModuleHandle(0);

	descWindow.windowName			= desc.windowName;
	descWindow.windowClassName		= L"FlyEngineWindowClass";
	descWindow.parent				= desc.parent;
	descWindow.windowPosition		= Point2D(desc.winPosX, desc.winPosY);
	descWindow.windowProcCallback	= FlyEngineCoreWndProc;
	descWindow.windowSize			= Point2D(desc.winWidth, desc.winHeight);

	if(!WindowShell::self()->createWin(descWindow))
		return false;

	return true;
}
void FlyEngine_Core::_ShowSplash()
{
	Texture2D texture;
	if(!texture.loadTexture(D3DShell::self()->getDevice(), L"..\\Resources\\Textures\\flyEngineSplash.png"))
		return;

	this->Gfx_BeginForwardScene();
	IShader::DRAW_DATA d;
	this->colorShader->getShader()->Render();
	D3DShell::self()->getDeviceContext()->PSSetShaderResources(0, 1, &(*texture.getTextures())[0]);
	this->Gfx_EndForwardScene();

	this->splash = false;
}
//####################################################################//
